/******************************************************************************
 *  @file    Can.c
 *  @brief   Header file for CAN driver in AUTOSAR communication stack.
 *
 *  @details This file contains the default initialization configuration and
 *           function prototypes for the CAN driver module.
 *
 *           This file is hardware-independent.
 *
 *  @version 1.0
 *  @date    2024-10-10
 *
 *  @section Author
 *           Tong Xuan Hoang
 ******************************************************************************/

#include "Can.h"
#include "stm32f10x_can.h"

/**
 * @var Can_ConfigData
 * @brief Example configuration instance for CAN driver.
 * 
 * This variable contains a predefined configuration for CAN hardware and GPIO
 * settings.
 */
Can_ConfigType Can_ConfigData = 
{
    .Can_HardwareConfig = 
	{
        .CAN_Prescaler = 18,                     /**< Set baudrate to 500 kbps */
        .CAN_Mode = CAN_Mode_Normal,             /**< Normal communication mode */
        .CAN_SJW = CAN_SJW_1tq,                  /**< Synchronization Jump Width */
        .CAN_BS1 = CAN_BS1_6tq,                  /**< Bit Segment 1 */
        .CAN_BS2 = CAN_BS2_7tq,                  /**< Bit Segment 2 */
        .CAN_TTCM = DISABLE,                     /**< Time Triggered Communication Mode */
        .CAN_ABOM = ENABLE,                      /**< Automatic Bus-Off Management */
        .CAN_AWUM = ENABLE,                      /**< Automatic Wake-Up Mode */
        .CAN_NART = DISABLE,                     /**< No Automatic Retransmission */
        .CAN_RFLM = DISABLE,                     /**< Receive FIFO Locked Mode */
        .CAN_TXFP = ENABLE                       /**< Transmit FIFO Priority */
    },
    .Can_GPIOConfig = 
	{
        .GPIO_Pin_RX = GPIO_Pin_11,              /**< CAN_RX pin (PA11) */
        .GPIO_Pin_TX = GPIO_Pin_12,              /**< CAN_TX pin (PA12) */
        .GPIO_Mode = GPIO_Mode_AF_PP,            /**< Alternate function push-pull */
        .GPIO_Speed = GPIO_Speed_50MHz           /**< GPIO speed */
    }
};

/**
 * @brief Initializes the CAN driver with the specified configuration.
 *
 * @param[in] Config Pointer to the CAN driver configuration structure.
 * 
 * @details This function initializes the CAN peripheral and its associated GPIO
 *          pins based on the provided configuration.
 */
void Can_Init(const Can_ConfigType* Config) 
{
    /* Validate input parameter */
    if (Config == NULL) 
	{
        return; // Handle error
    }

    /* Enable clocks for CAN and GPIO */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Configure GPIO pins for CAN_RX and CAN_TX */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = Config->Can_GPIOConfig.GPIO_Pin_RX | Config->Can_GPIOConfig.GPIO_Pin_TX;
    GPIO_InitStruct.GPIO_Mode = Config->Can_GPIOConfig.GPIO_Mode;
    GPIO_InitStruct.GPIO_Speed = Config->Can_GPIOConfig.GPIO_Speed;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Reset CAN peripheral */
    CAN_DeInit(CAN1);

    /* Initialize CAN peripheral */
    CAN_InitTypeDef CAN_InitStruct;
    CAN_InitStruct.CAN_Prescaler = Config->Can_HardwareConfig.CAN_Prescaler;
    CAN_InitStruct.CAN_Mode = Config->Can_HardwareConfig.CAN_Mode;
    CAN_InitStruct.CAN_SJW = Config->Can_HardwareConfig.CAN_SJW;
    CAN_InitStruct.CAN_BS1 = Config->Can_HardwareConfig.CAN_BS1;
    CAN_InitStruct.CAN_BS2 = Config->Can_HardwareConfig.CAN_BS2;
    CAN_InitStruct.CAN_TTCM = Config->Can_HardwareConfig.CAN_TTCM;
    CAN_InitStruct.CAN_ABOM = Config->Can_HardwareConfig.CAN_ABOM;
    CAN_InitStruct.CAN_AWUM = Config->Can_HardwareConfig.CAN_AWUM;
    CAN_InitStruct.CAN_NART = Config->Can_HardwareConfig.CAN_NART;
    CAN_InitStruct.CAN_RFLM = Config->Can_HardwareConfig.CAN_RFLM;
    CAN_InitStruct.CAN_TXFP = Config->Can_HardwareConfig.CAN_TXFP;

    if (CAN_Init(CAN1, &CAN_InitStruct) == CAN_InitStatus_Failed) 
	{
        /* Initialization failed: Handle the error */
        return;
    }

    /* Configure CAN filters (default configuration) */
    CAN_FilterInitTypeDef CAN_FilterInitStruct;
    CAN_FilterInitStruct.CAN_FilterNumber = 0;
    CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStruct.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStruct.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStruct);
}

/**
 * @brief De-initializes the CAN peripheral and releases resources.
 * 
 * @details This function disables the CAN peripheral, clears its configurations,
 *          and resets GPIO pins to their default state to save power. It is used
 *          when the CAN peripheral is no longer needed.
 */
void Can_DeInit(void) 
{
    /* Reset all registers of CAN1 to their default state */
    CAN_DeInit(CAN1);

    /* Disable all CAN-related interrupts if enabled */
    CAN_ITConfig(CAN1, CAN_IT_FMP0 | CAN_IT_TME | CAN_IT_ERR, DISABLE); 

    /* Turn off clocks for CAN1 and GPIOA to save power */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, DISABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE);

    /* Reset GPIO pins for CAN TX (PA12) and RX (PA11) to default mode */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;           /**< Set to Analog mode to save power */
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief  Configures the baud rate for the specified CAN controller.
 * @param  Controller: The CAN controller to configure (0 for CAN1, 1 for CAN2).
 * @param  BaudRateConfigID: The desired baud rate configuration ID (e.g., 125, 250, 500, 1000).
 * @retval Std_ReturnType: E_OK if the baud rate is successfully configured, E_NOT_OK if an error occurs.
 */
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID) 
	{
    CAN_TypeDef* CANx = NULL;  /**< Pointer to CAN controller (CAN1 or CAN2) */

    /* Select the appropriate CAN controller based on the 'Controller' parameter */
    if (Controller == 0) 
	{
        CANx = CAN1;  /**< Assign CAN1 to CANx (0 -> CAN1) */
    } 
	else if (Controller == 1) 
	{
        CANx = CAN2;  /**< Assign CAN2 to CANx (1 -> CAN2) */
    } 
	else 
	{
        return E_NOT_OK;  /**< Invalid controller, return error */
    }

    /* Disable the CAN controller before configuring */
    CANx->MCR |= CAN_MCR_INRQ;  /**< Request initialization mode for the selected CAN controller */
    
    /* Wait until the CAN controller enters initialization mode */
    while ((CANx->MSR & CAN_MSR_INAK) == 0);  /**< Wait until CAN enters initialization mode */

    /* Configure the CAN controller based on BaudRateConfigID */
    CAN_InitTypeDef CAN_InitStructure;
    CAN_StructInit(&CAN_InitStructure);  /**< Set default CAN configuration */

    switch (BaudRateConfigID) 
	{
        case 125:  /* 125 kbps */
            CAN_InitStructure.CAN_Prescaler = 48; /**< Set prescaler for 125 kbps */
            CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
            CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
            break;

        case 250:  /* 250 kbps */
            CAN_InitStructure.CAN_Prescaler = 24; /**< Set prescaler for 250 kbps */
            CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
            CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
            break;

        case 500:  /* 500 kbps */
            CAN_InitStructure.CAN_Prescaler = 12; /**< Set prescaler for 500 kbps */
            CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
            CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
            break;

        case 1000:  /* 1 Mbps */
            CAN_InitStructure.CAN_Prescaler = 6;  /**< Set prescaler for 1 Mbps */
            CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
            CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
            break;

        default:
            return E_NOT_OK;  /**< Unsupported BaudRateConfigID */
    }

    /* Initialize the selected CAN controller with the new configuration */
    if (CAN_Init(CANx, &CAN_InitStructure) != SUCCESS) 
	{
        return E_NOT_OK;  /**< Return error if initialization fails */
    }

    /* Enable the CAN controller after configuration */
    CANx->MCR &= ~CAN_MCR_INRQ;  /**< Clear the initialization request to exit init mode */
    
    /* Wait until the CAN controller exits initialization mode */
    while ((CANx->MSR & CAN_MSR_INAK) != 0);  /**< Wait until CAN exits initialization mode */

    return E_OK;  /**< Return success if CAN is successfully configured */
}

/**
 * @brief  Changes the operating mode of the specified CAN controller.
 * @param  Controller: The CAN controller to change mode (0 for CAN1, 1 for CAN2).
 * @param  Transition: The desired transition (mode) to set for the controller.
 * @retval E_OK if the mode was successfully changed, E_NOT_OK if an error occurred.
 */
Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */
    Std_ReturnType status = E_OK; /* Initialize the return status */

    /* Select the appropriate CAN controller based on the 'Controller' parameter */
    if (Controller == 0)
    {                
        CANx = CAN1; /**< Assign CAN1 to CANx (0 -> CAN1) */
    }
    else if (Controller == 1)
    {                
        CANx = CAN2; /**< Assign CAN2 to CANx (1 -> CAN2) */
    }
    else
    {
        return E_NOT_OK; /* Invalid controller, return error */
    }

    /* Switch based on the requested controller mode transition */
    switch (Transition)
    {
        case CAN_CS_STARTED: /**< Normal mode */
            /* Enter Initialization Mode */
            CANx->MCR |= CAN_MCR_INRQ; /**< Request Initialization mode */
            
            /* Wait until CAN is in Initialization Mode */
            while ((CANx->MSR & CAN_MSR_INAK) == 0); /**< Wait for INAK flag */
            
            /* Exit Initialization Mode to start CAN operation */
            CANx->MCR &= ~CAN_MCR_INRQ; /**< Clear INRQ bit to leave initialization mode */
            
            /* Wait until CAN leaves Initialization Mode */
            while ((CANx->MSR & CAN_MSR_INAK) != 0); /**< Wait for INAK flag to be cleared */
            break;

        case CAN_CS_SLEEP: /**< Sleep mode */
            /* Request Sleep Mode */
            CANx->MCR |= CAN_MCR_SLEEP; /**< Set the SLEEP bit to enter Sleep mode */
            
            /* Wait for CAN to enter Sleep mode */
            while ((CANx->MSR & CAN_MSR_SLAK) == 0); /**< Wait for SLAK flag to be set */
            break;

        case CAN_CS_STOPPED: /**< Stop mode */
            /* Enter Initialization Mode first */
            CANx->MCR |= CAN_MCR_INRQ; /**< Request Initialization mode */
            
            /* Wait until CAN is in Initialization Mode */
            while ((CANx->MSR & CAN_MSR_INAK) == 0); /**< Wait for INAK flag */
            
            /* Request Stop Mode */
            CANx->MCR |= CAN_MCR_SLEEP; /**< Set the SLEEP bit to stop CAN operation */
            
            /* Wait for CAN to enter Stop Mode */
            while ((CANx->MSR & CAN_MSR_SLAK) == 0); /**< Wait for SLAK flag */
            break;

        case CAN_CS_UNINIT: /**< Uninitialized state */
            /* Disable the CAN controller */
            CANx->MCR |= CAN_MCR_INRQ; /**< Request Initialization mode */
            
            /* Wait until CAN is in Initialization Mode */
            while ((CANx->MSR & CAN_MSR_INAK) == 0); /**< Wait for INAK flag */
            
            /* Reset the CAN controller to uninitialized state */
            CANx->MCR |= CAN_MCR_RESET; /**< Reset CAN controller */
            
            /* Wait for CAN to reset */
            while ((CANx->MSR & CAN_MSR_INAK) != 0); /**< Wait for INAK flag to be cleared */
            break;

        default:
            status = E_NOT_OK; /**< Invalid transition state */
            break;
    }

    return status; /**< Return the operation status (E_OK or E_NOT_OK) */
}

/**
 * @brief  Disables interrupts for the specified CAN controller and clears interrupt flags.
 * @param  Controller: The CAN controller to disable interrupts (0 for CAN1, 1 for CAN2).
 * @retval None
 */
void Can_DisableControllerInterrupts(uint8 Controller)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */

    /* Select the appropriate CAN controller based on the 'Controller' parameter */
    if (Controller == 0)
    {                
        CANx = CAN1; /**< Assign CAN1 to CANx (0 -> CAN1) */
    }
    else if (Controller == 1)
    {                
        CANx = CAN2; /**< Assign CAN2 to CANx (1 -> CAN2) */
    }
    else
    {
        return; /* Invalid controller, do nothing or handle error */
    }

    /* Enable interrupts for the selected CAN controller */
    
    /* FIFO message pending interrupts */
    CAN_ITConfig(CANx, CAN_IT_FMP0, DISABLE); /**< FIFO 0 message pending interrupt */
    CAN_ITConfig(CANx, CAN_IT_FMP1, DISABLE); /**< FIFO 1 message pending interrupt */

    /* Transmit mailbox empty interrupt */
    CAN_ITConfig(CANx, CAN_IT_TME, DISABLE); /**< Transmit mailbox empty interrupt */

    /* Error interrupts */
    CAN_ITConfig(CANx, CAN_IT_ERR, DISABLE); /**< Error interrupt */
    CAN_ITConfig(CANx, CAN_IT_BOF, DISABLE); /**< Bus-off interrupt */
    CAN_ITConfig(CANx, CAN_IT_LEC, DISABLE); /**< Last error code interrupt */

    /* FIFO full interrupts */
    CAN_ITConfig(CANx, CAN_IT_FF0, DISABLE); /**< FIFO 0 full interrupt */
    CAN_ITConfig(CANx, CAN_IT_FF1, DISABLE); /**< FIFO 1 full interrupt */

    /* FIFO overrun interrupts */
    CAN_ITConfig(CANx, CAN_IT_FOV0, DISABLE); /**< FIFO 0 overrun interrupt */
    CAN_ITConfig(CANx, CAN_IT_FOV1, DISABLE); /**< FIFO 1 overrun interrupt */

    /* Bus-off flags */
    CAN_ITConfig(CANx, CAN_IT_BOF, DISABLE); /**< Bus-off flag */

    /* Error warning and error passive flags */
    CAN_ITConfig(CANx, CAN_IT_EWG, DISABLE); /**< Error warning flag */
    CAN_ITConfig(CANx, CAN_IT_EPV, DISABLE); /**< Error passive flag */

    /* Wake-up and sleep interrupts */
    CAN_ITConfig(CANx, CAN_IT_WKU, DISABLE); /**< Wake-up interrupt */
    CAN_ITConfig(CANx, CAN_IT_SLK, DISABLE); /**< Sleep interrupt */

    /* Clear the pending interrupt flags */
    
    /* FIFO message pending flags */
    CAN_ClearITPendingBit(CANx, CAN_IT_FMP0); /**< Clear FIFO 0 message pending interrupt flag */
    CAN_ClearITPendingBit(CANx, CAN_IT_FMP1); /**< Clear FIFO 1 message pending interrupt flag */

    /* Transmit mailbox empty interrupt flag */
    CAN_ClearITPendingBit(CANx, CAN_IT_TME); /**< Clear Transmit mailbox empty interrupt flag */

    /* Error flags */
    CAN_ClearITPendingBit(CANx, CAN_IT_ERR); /**< Clear Error interrupt flag */
    CAN_ClearITPendingBit(CANx, CAN_IT_BOF); /**< Clear Bus-off interrupt flag */
    CAN_ClearITPendingBit(CANx, CAN_IT_LEC); /**< Clear Last error code interrupt flag */

    /* FIFO full flags */
    CAN_ClearITPendingBit(CANx, CAN_IT_FF0); /**< Clear FIFO 0 full interrupt flag */
    CAN_ClearITPendingBit(CANx, CAN_IT_FF1); /**< Clear FIFO 1 full interrupt flag */

    /* FIFO overrun flags */
    CAN_ClearITPendingBit(CANx, CAN_IT_FOV0); /**< Clear FIFO 0 overrun interrupt flag */
    CAN_ClearITPendingBit(CANx, CAN_IT_FOV1); /**< Clear FIFO 1 overrun interrupt flag */

    /* Bus-off flags */
    CAN_ClearITPendingBit(CANx, CAN_IT_BOF); /**< Clear Bus-off flag */

    /* Error warning and error passive flags */
    CAN_ClearITPendingBit(CANx, CAN_IT_EWG); /**< Clear Error warning flag */
    CAN_ClearITPendingBit(CANx, CAN_IT_EPV); /**< Clear Error passive flag */

    /* Wake-up and sleep flags */
    CAN_ClearITPendingBit(CANx, CAN_IT_WKU); /**< Clear Wake-up interrupt flag */
    CAN_ClearITPendingBit(CANx, CAN_IT_SLK); /**< Clear Sleep interrupt flag */
}

/**
 * @brief  Enables interrupts for the specified CAN controller.
 * @param  Controller: The CAN controller to enable interrupts (0 for CAN1, 1 for CAN2).
 * @retval None
 */
void Can_EnableControllerInterrupts(uint8 Controller)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */

    /* Select the appropriate CAN controller based on the 'Controller' parameter */
    if (Controller == 0)
    {                /* CAN1 */
        CANx = CAN1; /* Assign CAN1 to CANx */
    }
    else if (Controller == 1)
    {                /* CAN2 */
        CANx = CAN2; /* Assign CAN2 to CANx */
    }
    else
    {
        return; /* Invalid controller, do nothing or handle error */
    }

    /* Enable interrupts for the selected CAN controller */
    CAN_ITConfig(CANx, CAN_IT_TME, ENABLE); /**< Transmit mailbox empty interrupt */

    /* FIFO message pending interrupts */
    CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE); /**< FIFO 0 message pending interrupt */
    CAN_ITConfig(CANx, CAN_IT_FMP1, ENABLE); /**< FIFO 1 message pending interrupt */

    /* FIFO full interrupts */
    CAN_ITConfig(CANx, CAN_IT_FF0, ENABLE); /**< FIFO 0 full interrupt */
    CAN_ITConfig(CANx, CAN_IT_FF1, ENABLE); /**< FIFO 1 full interrupt */

    /* FIFO overrun interrupts */
    CAN_ITConfig(CANx, CAN_IT_FOV0, ENABLE); /**< FIFO 0 overrun interrupt */
    CAN_ITConfig(CANx, CAN_IT_FOV1, ENABLE); /**< FIFO 1 overrun interrupt */

    /* Operating mode interrupts */
    CAN_ITConfig(CANx, CAN_IT_WKU, ENABLE); /**< Wake-up interrupt */
    CAN_ITConfig(CANx, CAN_IT_SLK, ENABLE); /**< Sleep acknowledge interrupt */

    /* Error interrupts */
    CAN_ITConfig(CANx, CAN_IT_EWG, ENABLE); /**< Error warning interrupt */
    CAN_ITConfig(CANx, CAN_IT_EPV, ENABLE); /**< Error passive interrupt */
    CAN_ITConfig(CANx, CAN_IT_BOF, ENABLE); /**< Bus-off interrupt */
    CAN_ITConfig(CANx, CAN_IT_LEC, ENABLE); /**< Last error code interrupt */
    CAN_ITConfig(CANx, CAN_IT_ERR, ENABLE); /**< General error interrupt */
}

/**
 * @brief  Checks if the specified CAN controller has been woken up from Sleep mode.
 * @param  Controller: The CAN controller to check wake-up status (0 for CAN1, 1 for CAN2).
 * @retval E_OK if the CAN controller has woken up from Sleep mode, E_NOT_OK if the controller is still in Sleep mode.
 */
Std_ReturnType Can_CheckWakeup(uint8 Controller)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */
    Std_ReturnType status = E_NOT_OK; /* Initialize the return status to E_NOT_OK */

    /* Select the appropriate CAN controller based on the 'Controller' parameter */
    if (Controller == 0)
    {                
        CANx = CAN1; /**< Assign CAN1 to CANx (0 -> CAN1) */
    }
    else if (Controller == 1)
    {                
        CANx = CAN2; /**< Assign CAN2 to CANx (1 -> CAN2) */
    }
    else
    {
        return E_NOT_OK; /* Invalid controller, return error */
    }

    /* Check if the CAN controller is awake (SLAK bit in MSR should be cleared) */
    if ((CANx->MSR & CAN_MSR_SLAK) == 0) /**< SLAK = 0 means CAN is awake */
    {
        status = E_OK; /**< Return E_OK if the CAN controller is awake */
        CAN_ClearITPendingBit(CANx, CAN_IT_WKU); /**< Clear Wake-up interrupt flag */
		
		status = E_OK; /**< Return E_OK if the CAN controller is awake */
    }

    return status; /**< Return the status (E_OK if awake, E_NOT_OK if in sleep) */
}

/**
 * @brief  Retrieves the error state of the specified CAN controller.
 * @param  ControllerId: The CAN controller to check (0 for CAN1, 1 for CAN2).
 * @param  ErrorStatePtr: Pointer to a Can_ErrorStateType variable that will store the error state.
 * @retval Std_ReturnType: E_OK if the error state was retrieved successfully, E_NOT_OK otherwise.
 */
Std_ReturnType Can_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType *ErrorStatePtr)
{
    CAN_TypeDef *CANx = NULL; /**< Declare pointer for CAN controller */

    /* Check if the ErrorStatePtr is valid */
    if (ErrorStatePtr == NULL)
    {
        return E_NOT_OK; /**< Invalid pointer, return error */
    }

    /* Select the appropriate CAN controller based on the ControllerId */
    if (ControllerId == 0)
    {
        CANx = CAN1; /**< CAN1 selected */
    }
    else if (ControllerId == 1)
    {
        CANx = CAN2; /**< CAN2 selected */
    }
    else
    {
        return E_NOT_OK; /**< Invalid controller ID, return error */
    }

    /* Check the error flags in the CAN controller status register */
    if (CANx->ESR & CAN_ESR_BOFF)
    {
        *ErrorStatePtr = CAN_ERRORSTATE_BUSOFF; /**< Bus-off error state */
    }
    else if (CANx->ESR & CAN_ESR_EPVF)
    {
        *ErrorStatePtr = CAN_ERRORSTATE_PASSIVE; /**< Error passive state */
    }
    else if (CANx->ESR & CAN_ESR_EWGF)
    {
        *ErrorStatePtr = CAN_ERRORSTATE_ACTIVE; /**< Error warning state */
    }
    else
    {
        return E_NOT_OK; /**< Invalid, return error */
    }

    return E_OK; /**< Return success */
}

/**
 * @brief  Get the current mode of the specified CAN controller.
 * @param  Controller: The CAN controller ID (0 for CAN1, 1 for CAN2).
 * @param  ControllerModePtr: Pointer to store the current mode of the controller.
 * @retval E_OK if successful, E_NOT_OK if there is an error or invalid controller.
 */
Std_ReturnType Can_GetControllerMode(uint8 Controller, Can_ControllerStateType *ControllerModePtr)
{
    CAN_TypeDef *CANx = NULL; /**< Declare pointer for CAN controller */

    /* Check if the ControllerModePtr is valid */
    if (ControllerModePtr == NULL)
    {
        return E_NOT_OK; /**< Invalid pointer, return error */
    }

    /* Select the appropriate CAN controller based on the Controller parameter */
    if (Controller == 0)
    {
        CANx = CAN1; /**< CAN1 selected */
    }
    else if (Controller == 1)
    {
        CANx = CAN2; /**< CAN2 selected */
    }
    else
    {
        return E_NOT_OK; /**< Invalid controller ID, return error */
    }

    /* Check the current mode of the selected CAN controller */
    if (CANx->MCR & CAN_MCR_INRQ) /**< Initialization request flag */
    {
        *ControllerModePtr = CAN_CS_UNINIT; /**< Controller is uninitialized */
    }
    else if (CANx->MSR & CAN_MSR_SLAK) /**< Sleep acknowledge flag */
    {
        *ControllerModePtr = CAN_CS_SLEEP; /**< Controller is in sleep mode */
    }
    else if (CANx->MSR & CAN_MSR_TXM) /**< Transmit mode flag */
    {
        *ControllerModePtr = CAN_CS_STARTED; /**< Controller is operational */
    }
    else
    {
        *ControllerModePtr = CAN_CS_STOPPED; /**< Controller is stopped */
    }

    return E_OK; /**< Return success */
}

/**
 * @brief  Get the Receive Error Counter for the specified CAN controller.
 * @param  ControllerId: The CAN controller ID (0 for CAN1, 1 for CAN2).
 * @param  RxErrorCounterPtr: Pointer to store the Receive Error Counter value.
 * @retval E_OK if successful, E_NOT_OK if there is an error or invalid controller.
 */
Std_ReturnType Can_GetControllerRxErrorCounter(uint8 ControllerId, uint8 *RxErrorCounterPtr)
{
    CAN_TypeDef *CANx = NULL; /**< Declare pointer for CAN controller */

    /* Check if the RxErrorCounterPtr is valid */
    if (RxErrorCounterPtr == NULL)
    {
        return E_NOT_OK; /**< Invalid pointer, return error */
    }

    /* Select the appropriate CAN controller based on the ControllerId */
    if (ControllerId == 0)
    {
        CANx = CAN1; /**< CAN1 selected */
    }
    else if (ControllerId == 1)
    {
        CANx = CAN2; /**< CAN2 selected */
    }
    else
    {
        return E_NOT_OK; /**< Invalid controller ID, return error */
    }

    /* Read the receive error counter (REC) from ESR register */
    *RxErrorCounterPtr = (uint8)((CANx->ESR & CAN_ESR_REC) >> 24); /**< Mask and shift to get REC-Bits 31:24 */

    return E_OK; /**< Return success */
}

Std_ReturnType Can_GetControllerTxErrorCounter(uint8 ControllerId, uint8 *TxErrorCounterPtr)
{
    CAN_TypeDef *CANx = NULL; /**< Declare pointer for CAN controller */

    /* Check if the TxErrorCounterPtr is valid */
    if (TxErrorCounterPtr == NULL)
    {
        return E_NOT_OK; /**< Invalid pointer, return error */
    }

    /* Select the appropriate CAN controller based on the ControllerId */
    if (ControllerId == 0)
    {
        CANx = CAN1; /**< CAN1 selected */
    }
    else if (ControllerId == 1)
    {
        CANx = CAN2; /**< CAN2 selected */
    }
    else
    {
        return E_NOT_OK; /**< Invalid controller ID, return error */
    }

    /* Read the transmit error counter (TEC) from ESR register */
    *TxErrorCounterPtr = (uint8)((CANx->ESR & CAN_ESR_TEC) >> 16); /**< Mask and shift to get TEC-Bits 23:16 */

    return E_OK; /**< Return success */
}
