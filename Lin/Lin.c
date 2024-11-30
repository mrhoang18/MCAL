/**********************************************************
 * @file Lin.c
 * @brief AUTOSAR LIN Driver Source File
 * @details This file contains the function definitions for
 *          the LIN driver according to the AUTOSAR standard.
 * @version 1.0
 * @date 2024-11-01
 * @author Tong Xuan Hoang
 **********************************************************/

#include "Lin.h"
#include "Lin_Cfg.h"

/**********************************************************
 * @brief Initialize the LIN module.
 * @param Config Pointer to the LIN configuration structure.
 **********************************************************/
void Lin_Init(const Lin_ConfigType *Config)
{
    // Check if the configuration is valid
    if (Config == NULL)
    {
        return; // Return if the configuration is invalid
    }

    // Enable clock for GPIO ports and UART used for LIN
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    // Configure Tx (PA9) and Rx (PA10) pins for UART used for LIN
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // Tx pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // Rx pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure USART for LIN communication
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = Config->Lin_BaudRate; // Baud rate from the configuration
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    // Enable USART
    USART_Cmd(USART1, ENABLE);

    // Enable LIN mode
    USART_LINCmd(USART1, ENABLE);

    // Enable interrupt if required
    if (Config->Lin_IRQn != 0)
    {
        NVIC_EnableIRQ(Config->Lin_IRQn); // Enable interrupt for LIN
    }
}

/**********************************************************
 * @brief Check for wake-up event on the LIN channel.
 * @param Channel The LIN channel to check.
 * @return `E_OK` if a wake-up event is detected, `E_NOT_OK` if not detected.
 * @details This function checks the status of the LIN channel to determine if a wake-up event has occurred.
 **********************************************************/
Std_ReturnType Lin_CheckWakeup(uint8 Channel)
{
    // Check if the Channel is within a valid range
    if (Channel >= MAX_LIN_CHANNELS)
    {
        return E_NOT_OK; // Return if the Channel is invalid
    }

    // Read the status from the hardware register or LIN module to check for a wake-up event
    if (USART1->SR & USART_SR_WU)
    { // Assume USART_SR_WU is the wake-up flag in the USART status register
        // Clear the wake-up flag after checking
        USART1->SR &= ~USART_SR_WU;
        return E_OK; // Wake-up event detected
    }

    return E_NOT_OK; // No wake-up event detected
}

/**********************************************************
 * @brief Retrieve the version information of the LIN driver.
 * @param versioninfo Pointer to the structure where version information will be stored.
 **********************************************************/
void Lin_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
    if (versioninfo != NULL)
    {
        versioninfo->vendorID = LIN_VENDOR_ID;
        versioninfo->moduleID = LIN_MODULE_ID;
        versioninfo->sw_major_version = LIN_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = LIN_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = LIN_SW_PATCH_VERSION;
    }
}

/**********************************************************
 * @brief Calculate the checksum value for a LIN frame.
 * @param data Pointer to the data array for which the checksum will be calculated.
 * @param length The length of the data array.
 * @return The calculated checksum value.
 * @details This function calculates the checksum according to the LIN protocol for frames.
 **********************************************************/
static uint8 LIN_CalculateChecksum(const uint8 *data, uint8 length)
{
    uint16 checksum = 0;

    // Add all the data bytes
    for (uint8 i = 0; i < length; i++)
    {
        checksum += data[i];

        // If there is a carry bit, add it to the checksum
        if (checksum > 0xFF)
        {
            checksum = (checksum & 0xFF) + 1;
        }
    }

    // Return the checksum's complement value
    return (uint8)(~checksum);
}

/**********************************************************
 * @brief Send a LIN frame.
 * @param Channel The LIN channel to send the frame.
 * @param PduInfoPtr Pointer to the PDU containing the information to send.
 * @return `E_OK` if successful, `E_NOT_OK` if failed.
 **********************************************************/
Std_ReturnType Lin_SendFrame(uint8 Channel, const Lin_PduType *PduInfoPtr)
{
    // Check the validity of the input parameters
    if (PduInfoPtr == NULL)
    {
        return E_NOT_OK;
    }

    // Start sending the LIN frame by transmitting the Break field
    USART_SendBreak(USART1); // Send the Break field via UART

    // Wait for the Break field transmission to complete
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;

    // Send the Sync Field
    USART_SendData(USART1, 0x55); // Sync byte with fixed value 0x55
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;

    // Calculate and send the ID field
    uint8 id_with_parity = PduInfoPtr->Pid | LIN_CalculateParity(PduInfoPtr->Pid);
    USART_SendData(USART1, id_with_parity);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;

    // Send the Data Field
    for (uint8 i = 0; i < PduInfoPtr->Dl; i++)
    {
        USART_SendData(USART1, PduInfoPtr->SduPtr[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
            ;
    }

    // Calculate and send the Checksum field
    uint8 checksum = LIN_CalculateChecksum(PduInfoPtr->SduPtr, PduInfoPtr->Dl);
    USART_SendData(USART1, checksum);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;

    return E_OK; // Return `E_OK` if the transmission completes successfully
}

/**********************************************************
 * @brief Put the LIN channel into sleep mode.
 * @param Channel The LIN channel to put into sleep mode.
 * @return `E_OK` if the sleep command is accepted, `E_NOT_OK` if an error occurs.
 * @details This function sends the "go-to-sleep" command on the specified LIN channel
 *          and puts the channel into sleep mode to save power.
 **********************************************************/
Std_ReturnType Lin_GoToSleep(uint8 Channel)
{
    // Check the validity of the Channel
    if (Channel >= MAX_LIN_CHANNELS)
    {
        return E_NOT_OK; // Invalid Channel
    }

    // Send the "go-to-sleep" signal by transmitting the Break field and sending the sleep ID frame
    USART_SendBreak(USART1); // Transmit Break field to signal sleep

    // Wait for the transmission to complete
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;

    USART_SendData(USART1, LIN_GO_TO_SLEEP); // Transmit frame with sleep ID

    // Wait for the transmission to complete
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;

    // Set the LIN channel state to sleep mode
    LinChannelState[Channel] = LIN_CH_SLEEP;

    return E_OK; // Sleep command executed successfully
}

/**********************************************************
 * @brief Put the LIN channel into sleep mode and activate wake-up detection.
 * @param Channel The LIN channel to process.
 * @return `E_OK` if the command is accepted, `E_NOT_OK` if an error occurs.
 * @details This function sends the "go-to-sleep" command on the specified LIN channel and
 *          puts the channel into sleep mode to save power. While in sleep mode,
 *          the system will activate monitoring to detect wake-up signals.
 **********************************************************/
Std_ReturnType Lin_GoToSleepInternal(uint8 Channel)
{
    // Check if the Channel is valid
    if (Channel >= MAX_LIN_CHANNELS)
    {
        return E_NOT_OK; // Return error if channel is invalid
    }

    // Send the "go-to-sleep" signal by transmitting the Break field and sending the sleep ID
    USART_SendBreak(USART1); // Transmit Break field to signal sleep mode

    // Wait for the transmission to complete
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;

    // Update the LIN channel state to sleep mode
    LinChannelState[Channel] = LIN_CH_SLEEP;

    // Activate wake-up detection if necessary
    if (LinChannelConfig[Channel].LinChannelWakeupSupport == ENABLE)
    {
        // Enable wake-up detection mode, which could be through interrupts or bus monitoring
    }

    return E_OK; // Return `E_OK` if the process is successful
}

/**********************************************************
 * @brief Send a wake-up pulse and set the channel state to LIN_CH_OPERATIONAL.
 * @param Channel The LIN channel to send the wake-up pulse.
 * @return `E_OK` if successful, `E_NOT_OK` if failed.
 * @details Sends a wake-up signal to wake up LIN nodes from sleep mode.
 **********************************************************/
Std_ReturnType Lin_Wakeup(uint8 Channel)
{
    // Check if the Channel is valid
    if (Channel >= MAX_LIN_CHANNELS)
    {
        return E_NOT_OK; // Return error if Channel is invalid
    }

    // Check the channel state; it must be LIN_CH_SLEEP to continue
    if (LinChannelState[Channel] != LIN_CH_SLEEP)
    {
        return E_NOT_OK; // Return error if the channel is not in sleep state
    }

    // Send a wake-up signal by transmitting a dominant bit
    USART_SendData(USART1, 0x80); // Transmit byte with dominant bit 0b10000000

    // Wait for the transmission to complete
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;

    // Update the channel state to LIN_CH_OPERATIONAL
    LinChannelState[Channel] = LIN_CH_OPERATIONAL;

    return E_OK; // Return `E_OK` if successful
}

/**********************************************************
 * @brief Get the current status of the LIN channel.
 * @param Channel The LIN channel to check.
 * @param Lin_SduPtr A pointer to a pointer that will contain the current SDU.
 * @return The current status of the LIN channel.
 * @details This function checks the status of the LIN channel and returns its current operational state.
 **********************************************************/
Lin_StatusType Lin_GetStatus(uint8 Channel, const uint8 **Lin_SduPtr)
{
    // Check the validity of the input pointer
    if (Lin_SduPtr == NULL)
    {
        return LIN_NOT_OK; // Return error if pointer is invalid
    }

    // Check if the Channel is within a valid range
    if (Channel >= MAX_LIN_CHANNELS)
    {
        return LIN_NOT_OK; // Return error if Channel is invalid
    }

    // Retrieve the current status from hardware or a status variable
    Lin_StatusType currentStatus = LinChannelState[Channel];

    // If the status is LIN_RX_OK or LIN_TX_OK, update Lin_SduPtr
    if (currentStatus == LIN_RX_OK || currentStatus == LIN_TX_OK)
    {
        *Lin_SduPtr = LinChannelData[Channel]; // LinChannelData is the data region containing the SDU
    }
    else
    {
        *Lin_SduPtr = NULL; // If no data, set the SDU pointer to NULL
    }

    return currentStatus; // Return the current status of the LIN channel
}
