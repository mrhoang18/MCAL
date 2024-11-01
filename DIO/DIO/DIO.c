/******************************************************************************
 *  @file    Dio.c
 *  @brief   Definition of Digital Input/Output (DIO) control functions.
 *
 *  @details This file contains the implementation of DIO control functions, 
 *           responsible for configuring and controlling the digital pins of 
 *           the microcontroller. The DIO module enables reading and writing 
 *           of digital signals in compliance with the AUTOSAR standard.
 *  
 *  @version 1.0
 *  @date    2024-11-01
 *
 *  @section Author
 *           Tong Xuan Hoang
 ******************************************************************************/

#include "Dio.h"

// #if (DIO_DEV_ERROR_DETECT == STD_ON)
// #include "Det.h"
// #endif

#include "stm32f10x_gpio.h"

/**
  * @brief  Reads the current signal level (HIGH or LOW) from the specified GPIO pin.
  * @param  ChannelId: Specifies the unique ID of the GPIO pin, which contains information about the port and pin number.
  *                     - For example: ChannelId 0->15 corresponds to GPIOA pins 0->15,
  *                       ChannelId 16->31 corresponds to GPIOB pins 0->15, and so on.
  * @retval Dio_LevelType: Returns the signal level of the specified pin.
  *                - STD_HIGH: The pin is at a high level.
  *                - STD_LOW: The pin is at a low level.
  */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId) 
{
    GPIO_TypeDef* GPIOx;
    uint16 GPIO_Pin;
    Dio_LevelType level = STD_LOW;

    /* Extract port and pin from ChannelId */
    uint8 port = ChannelId / 16;     // Determine port number
    uint8 pin = ChannelId % 16;      // Determine pin within port

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
            return STD_LOW;  // Return LOW if port is invalid
    }

    /* Convert pin to SPL format */
    GPIO_Pin = (1 << pin);  // Shift to match SPL's GPIO_Pin_x format

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

/**
  * @brief  Writes a signal level (HIGH or LOW) to the specified GPIO pin.
  * @param  ChannelId: Specifies the unique ID of the GPIO pin, which includes information about the port and pin number.
  *                     - For example: ChannelId 0->15 corresponds to GPIOA pins 0->15,
  *                       ChannelId 16->31 corresponds to GPIOB pins 0->15, and so on.
  * @param  Level: Specifies the level to be written to the GPIO pin.
  *                - STD_HIGH: Sets the pin to a high level.
  *                - STD_LOW: Sets the pin to a low level.
  * @retval None
  */
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
        GPIO_SetBits(GPIOx, GPIO_Pin);   // Set pin to high level
    }
    else
    {
        GPIO_ResetBits(GPIOx, GPIO_Pin); // Set pin to low level
    }
}


/**
  * @brief  Reads the signal levels of all pins in the specified GPIO port.
  * @param  PortId: Specifies the unique ID of the GPIO port.
  *                     - 0: GPIOA
  *                     - 1: GPIOB
  *                     - 2: GPIOC
  *                     - and so on, depending on the MCU configuration.
  * @retval Dio_PortLevelType: Returns the level of all pins in the port.
  *                - Each bit in the returned value corresponds to the level of a specific pin.
  *                - Bit value '1' indicates HIGH, '0' indicates LOW.
  */
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId)
{
    GPIO_TypeDef* GPIOx;
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
            return 0;  // Return 0 if PortId is invalid
    }

    /* Read the level of all pins in the port */
    portLevel = (Dio_PortLevelType)GPIO_ReadInputData(GPIOx);

    return portLevel;
}

void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
    GPIO_TypeDef* GPIOx;

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



Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr)
{
    GPIO_TypeDef* GPIOx;
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
            return 0;  // Return 0 if the port is invalid
    }

    /* Read data from the GPIO port */
    uint16 portValue = GPIO_ReadInputData(GPIOx); // Read the full port value

    /* Apply mask and offset to retrieve the pin group value */
    groupLevel = (portValue & ChannelGroupIdPtr->mask) >> ChannelGroupIdPtr->offset;

    return groupLevel;
}

/**
  * @brief  Writes a specified level to a group of adjacent pins in a GPIO port.
  * @param  ChannelGroupIdPtr: Pointer to the Dio_ChannelGroupType structure, which contains:
  *                            - port: The GPIO port identifier (e.g., 0 for GPIOA, 1 for GPIOB).
  *                            - mask: Bitmask specifying the pins within the port that belong to the group.
  *                            - offset: The starting bit position of the pin group within the port.
  * @param  Level: The level to be written to the specified group of pins. Each bit in Level corresponds
  *                to a specific pin in the group, shifted according to the offset.
  * @retval None
  * 
  * @note   This function writes a level to a subset of contiguous pins within a port without affecting 
  *         the other pins outside of this group.
  */
void Dio_WriteChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr, Dio_PortLevelType Level)
{
    GPIO_TypeDef* GPIOx;

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
            return;  // Exit if port is invalid
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

#define NULL_PTR          ((void *)0)
	
void Dio_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
	if (versioninfo != NULL_PTR)
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

/**
  * @brief  Toggles (flips) the state of a specified DIO channel.
  *         If the channel is currently HIGH, it will be set to LOW, and vice versa.
  * @param  ChannelId: The unique identifier of the DIO channel to be toggled.
  * @retval Dio_LevelType: Returns the new state of the channel after toggling.
  *                         - STD_HIGH: The channel is now set to HIGH.
  *                         - STD_LOW: The channel is now set to LOW.
  * 
  * @note   This function reads the current state of the specified DIO channel, 
  *         flips its state, writes the new state back to the channel, and returns 
  *         the new state for further use.
  */
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
