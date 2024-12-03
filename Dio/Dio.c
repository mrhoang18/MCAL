/**********************************************************
 * @file Dio.c
 * @brief AUTOSAR DIO Driver Source File
 * @details This file contains the function definitions for
 *          the DIO driver according to the AUTOSAR standard.
 * @version 1.0
 * @date 2024-11-01
 * @author Tong Xuan Hoang
 **********************************************************/

#include "Dio.h"
#include "stm32f10x_gpio.h"

/***********************************************************
 * @brief  Reads the signal level (HIGH or LOW) of a specified GPIO pin.
 * @param  ChannelId: The unique identifier for the GPIO pin, which encodes
 *                    both the port and pin number. Each set of 16 ChannelIds
 *                    maps to a specific port:
 *                    - ChannelId 0-15: GPIOA pins 0-15
 *                    - ChannelId 16-31: GPIOB pins 0-15
 *                    - ChannelId 32-47: GPIOC pins 0-15
 * @retval Dio_LevelType: The current level of the specified GPIO pin.
 *                    - STD_HIGH: Pin is at a high signal level.
 *                    - STD_LOW: Pin is at a low signal level.
 * @note   If an invalid port number is provided, the function will return STD_LOW by default.
 ***********************************************************/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId)
{
    GPIO_TypeDef *GPIOx;
    uint16 GPIO_Pin;
    Dio_LevelType level = STD_LOW;

    /* Extract port and pin from ChannelId */
    uint8 port = ChannelId / 16; // Determine port number
    uint8 pin = ChannelId % 16;  // Determine pin within port

    /* Determine GPIOx based on port number */
    switch (port)
    {
    case 0:
        GPIOx = GPIOA;
        break;
    case 1:
        GPIOx = GPIOB;
        break;
    case 2:
        GPIOx = GPIOC;
        break;
    default:
        return STD_LOW; // Return LOW if port is invalid
    }

    /* Convert pin to SPL format */
    GPIO_Pin = (1 << pin); // Shift to match SPL's GPIO_Pin_x format

    /* Read the pin level */
    if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == Bit_SET)
    {
        level = STD_HIGH;
    }
    else
    {
        level = STD_LOW;
    }

    return level;
}

/***********************************************************
 * @brief  Sets the specified GPIO pin to a signal level (HIGH or LOW).
 * @param  ChannelId: Unique identifier for the GPIO pin, which includes the port
 *                    and pin number:
 *                    - ChannelId 0-15: Maps to GPIOA pins 0-15.
 *                    - ChannelId 16-31: Maps to GPIOB pins 0-15.
 *                    - ChannelId 32-47: Maps to GPIOC pins 0-15.
 * @param  Level: Signal level to set on the GPIO pin.
 *                - STD_HIGH: Sets the pin to a high level.
 *                - STD_LOW: Sets the pin to a low level.
 * @retval None
 *
 * @note   If an invalid port is specified in ChannelId, the function exits without
 *         modifying any pin state.
 ***********************************************************/
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;

    /* Convert ChannelId into port and pin */
    uint8 port = ChannelId / 16; // Calculate GPIO port from ChannelId
    uint8 pin = ChannelId % 16;  // Calculate pin within the port

    /* Determine GPIOx and GPIO_Pin based on port and pin */
    switch (port)
    {
    case 0:
        GPIOx = GPIOA;
        break;
    case 1:
        GPIOx = GPIOB;
        break;
    case 2:
        GPIOx = GPIOC;
        break;
    default:
        return; // Exit if port is invalid
    }

    /* Convert pin to SPL format */
    GPIO_Pin = (1 << pin);

    /* Write the signal level to the pin */
    if (Level == STD_HIGH)
    {
        GPIO_SetBits(GPIOx, GPIO_Pin); // Set pin to high level
    }
    else
    {
        GPIO_ResetBits(GPIOx, GPIO_Pin); // Set pin to low level
    }
}

/***********************************************************
 * @brief  Reads the signal levels of all pins in the specified GPIO port.
 *
 * @param  PortId: Unique identifier of the GPIO port.
 *                 - 0: GPIOA
 *                 - 1: GPIOB
 *                 - 2: GPIOC
 *                 - and so forth, depending on the MCU configuration.
 *
 * @retval Dio_PortLevelType: The combined signal levels of all pins in the specified port.
 *                 - Each bit in the returned value represents the level of a pin in the port:
 *                   - Bit '1': HIGH (pin at high level).
 *                   - Bit '0': LOW (pin at low level).
 *
 * @note   If an invalid PortId is specified, the function returns 0.
 ***********************************************************/
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId)
{
    GPIO_TypeDef *GPIOx;
    Dio_PortLevelType portLevel = 0;

    /* Determine GPIOx based on PortId */
    switch (PortId)
    {
    case 0:
        GPIOx = GPIOA;
        break;
    case 1:
        GPIOx = GPIOB;
        break;
    case 2:
        GPIOx = GPIOC;
        break;
    default:
        return 0; // Return 0 if PortId is invalid
    }

    /* Read the level of all pins in the port */
    portLevel = (Dio_PortLevelType)GPIO_ReadInputData(GPIOx);

    return portLevel;
}

/***********************************************************
 * @brief  Writes a specified signal level to all pins in the specified GPIO port.
 * @param  PortId: Unique identifier for the GPIO port.
 *                 - 0: GPIOA
 *                 - 1: GPIOB
 *                 - 2: GPIOC
 *                 - and so on, depending on the MCU configuration.
 * @param  Level: Signal level to be written to all pins in the port.
 *                - Each bit in `Level` represents the desired level for a specific pin in the port.
 *                  Bit '1' sets the pin to HIGH, and bit '0' sets it to LOW.
 * @retval None
 * @note   If `PortId` is invalid, the function exits without modifying any pin state.
 ***********************************************************/
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
    GPIO_TypeDef *GPIOx;

    /* Determine GPIOx based on PortId */
    switch (PortId)
    {
    case 0:
        GPIOx = GPIOA;
        break;
    case 1:
        GPIOx = GPIOB;
        break;
    case 2:
        GPIOx = GPIOC;
        break;
    default:
        return; // Exit if PortId is invalid
    }

    /* Write the Level to the GPIO port */
    GPIO_Write(GPIOx, Level);
}

/***********************************************************
 * @brief  Reads the signal level of a specific group of pins within a GPIO port.
 * @param  ChannelGroupIdPtr: Pointer to the `Dio_ChannelGroupType` structure that specifies
 *                             the port, mask, and offset for the pin group.
 *                             - `port`: The GPIO port number (e.g., 0 for GPIOA, 1 for GPIOB).
 *                             - `mask`: The bit mask defining the specific pins in the port.
 *                             - `offset`: The bit offset indicating the position of the group within the port.
 * @retval Dio_PortLevelType: The signal level of the specified pin group, after applying
 *                            the mask and shifting by the offset.
 * @note   If the port specified in `ChannelGroupIdPtr` is invalid, the function returns 0.
 ***********************************************************/
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr)
{
    GPIO_TypeDef *GPIOx;
    Dio_PortLevelType groupLevel = 0;

    /* Determine GPIOx based on ChannelGroupIdPtr->port */
    switch (ChannelGroupIdPtr->port)
    {
    case 0:
        GPIOx = GPIOA;
        break;
    case 1:
        GPIOx = GPIOB;
        break;
    case 2:
        GPIOx = GPIOC;
        break;
    // Add more cases if additional ports are needed
    default:
        return 0; // Return 0 if the port is invalid
    }

    /* Read data from the GPIO port */
    uint16 portValue = GPIO_ReadInputData(GPIOx); // Read the full port value

    /* Apply mask and offset to retrieve the pin group value */
    groupLevel = (portValue & ChannelGroupIdPtr->mask) >> ChannelGroupIdPtr->offset;

    return groupLevel;
}

/***********************************************************
 * @brief  Writes a specified level to a group of adjacent pins in a GPIO port.
 * @param  ChannelGroupIdPtr: Pointer to the `Dio_ChannelGroupType` structure, which specifies:
 *                            - `port`: GPIO port identifier (e.g., 0 for GPIOA, 1 for GPIOB).
 *                            - `mask`: Bitmask defining the pins within the port that belong to the group.
 *                            - `offset`: Starting bit position of the pin group within the port.
 * @param  Level: The signal level to write to the specified group of pins. Each bit in `Level` corresponds
 *                to a specific pin in the group, aligned based on the offset.
 * @retval None
 * @note   This function updates only the specified group of contiguous pins within a port,
 *         leaving other pins in the port unaffected.
 ***********************************************************/
void Dio_WriteChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr, Dio_PortLevelType Level)
{
    GPIO_TypeDef *GPIOx;

    /* Determine GPIOx based on ChannelGroupIdPtr->port */
    switch (ChannelGroupIdPtr->port)
    {
    case 0:
        GPIOx = GPIOA;
        break;
    case 1:
        GPIOx = GPIOB;
        break;
    case 2:
        GPIOx = GPIOC;
        break;
    // Add additional cases if there are more ports
    default:
        return; // Exit if port is invalid
    }

    /* Read the current value of the GPIO port */
    uint32_t portValue = GPIO_ReadOutputData(GPIOx);

    /* Clear the bits of the specified group in the port */
    portValue &= ~(ChannelGroupIdPtr->mask); // Clear bits in the group using the mask

    /* Set the new level for the group of pins */
    portValue |= ((Level << ChannelGroupIdPtr->offset) & ChannelGroupIdPtr->mask); // Shift Level by offset and apply mask

    /* Write the updated value back to the GPIO port */
    GPIO_Write(GPIOx, (uint16_t)portValue);
}

/***********************************************************
 * @brief  Retrieves the version information of the DIO module.
 * @param  versioninfo: Pointer to the `Std_VersionInfoType` structure where version
 *                      information will be stored.
 *                      - `vendorID`: Vendor ID of the DIO module.
 *                      - `moduleID`: Module ID of the DIO module.
 *                      - `sw_major_version`: Major version number of the software.
 *                      - `sw_minor_version`: Minor version number of the software.
 *                      - `sw_patch_version`: Patch version number of the software.
 * @retval None
 * @note   If `versioninfo` is a null pointer, no action is performed. Ensure that
 *         `versioninfo` is valid before calling this function.
 ***********************************************************/
void Dio_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
    if (versioninfo != NULL)
    {
        /* Copy the vendor Id */
        versioninfo->vendorID = (uint16)DIO_VENDOR_ID;
        /* Copy the module Id */
        versioninfo->moduleID = (uint16)DIO_MODULE_ID;
        /* Copy Software Major Version */
        versioninfo->sw_major_version = (uint8)DIO_SW_MAJOR_VERSION;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version = (uint8)DIO_SW_MINOR_VERSION;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version = (uint8)DIO_SW_PATCH_VERSION;
    }
}

/***********************************************************
 * @brief  Toggles (flips) the current state of a specified DIO channel.
 *         If the channel is currently set to HIGH, it will be set to LOW, and vice versa.
 * @param  ChannelId: Unique identifier for the DIO channel to be toggled.
 * @retval Dio_LevelType: The new state of the channel after toggling.
 *                        - `STD_HIGH`: The channel is now set to HIGH.
 *                        - `STD_LOW`: The channel is now set to LOW.
 * @note   This function reads the current state of the specified DIO channel,
 *         flips the state, writes the new state back to the channel, and returns
 *         the new state for further use. This operation does not affect other channels.
 ***********************************************************/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)
{
    Dio_LevelType currentLevel;

    /* Read the current level of the channel */
    currentLevel = Dio_ReadChannel(ChannelId);

    /* Determine the new level by flipping the current level */
    Dio_LevelType newLevel = (currentLevel == STD_HIGH) ? STD_LOW : STD_HIGH;

    /* Write the new level to the channel */
    Dio_WriteChannel(ChannelId, newLevel);

    /* Return the new level */
    return newLevel;
}
