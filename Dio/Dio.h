/**********************************************************
 * @file Dio.h
 * @brief AUTOSAR DIO Driver Header File
 * @details This file contains the necessary definitions for
 *          the DIO driver according to the AUTOSAR standard.
 * @version 1.0
 * @date 2024-12-03
 * @author Tong Xuan Hoang
 **********************************************************/

#ifndef DIO_H
#define DIO_H

#ifdef __cplusplus
extern "C"{
#endif

#include "Std_types.h"          /**< Type definitions for standard types used across AUTOSAR modules */
#include "stm32f10x_gpio.h"     /**< GPIO header from the Standard Peripheral Library for STM32F103C8T6 */

/*==============================================================================
 *                              MACROS                                         *
 ==============================================================================*/

/**********************************************************
 * @brief DIO Module ID Configuration
 * 
 *        This section defines macros related to the identification 
 *        of the DIO module, including the vendor ID, module ID, 
 *        and instance ID. These identifiers are used for module 
 *        configuration and integration in the AUTOSAR environment.
 * 
 *        - DIO_VENDOR_ID: Defines the company/vendor ID for the AUTOSAR 
 *          implementation. Example: Tong Xuan Hoang's ID is 1810.
 *        - DIO_MODULE_ID: Specifies the unique ID for the DIO module.
 *        - DIO_INSTANCE_ID: Specifies the instance ID for the DIO module.
 **********************************************************/
#define DIO_VENDOR_ID    (1810U)             
#define DIO_MODULE_ID    (202U)              
#define DIO_INSTANCE_ID  (0U)               

/**********************************************************
 * @brief DIO Module Software Version
 * 
 *        This section defines the versioning information for the DIO 
 *        module. The version is represented by major, minor, and patch 
 *        numbers to indicate the software's release level and changes.
 * 
 *        - DIO_SW_MAJOR_VERSION: Major version of the DIO module.
 *        - DIO_SW_MINOR_VERSION: Minor version of the DIO module.
 *        - DIO_SW_PATCH_VERSION: Patch version of the DIO module.
 **********************************************************/
#define DIO_SW_MAJOR_VERSION           (1U)
#define DIO_SW_MINOR_VERSION           (0U)
#define DIO_SW_PATCH_VERSION           (0U)

/**********************************************************
 * @brief DIO Error Codes
 * 
 *        This section defines the error codes used in the DIO module 
 *        to indicate specific error conditions during operations.
 * 
 *        - DIO_E_PARAM_INVALID_CHANNEL_ID: Error code for invalid 
 *          channel ID.
 *        - DIO_E_PARAM_INVALID_PORT_ID: Error code for invalid port ID.
 *        - DIO_E_PARAM_INVALID_GROUP: Error code for invalid group parameter.
 *        - DIO_E_PARAM_POINTER: Error code for invalid pointer.
 **********************************************************/
#define DIO_E_PARAM_INVALID_CHANNEL_ID 	(0x0A)
#define DIO_E_PARAM_INVALID_PORT_ID 	(0x14)
#define DIO_E_PARAM_INVALID_GROUP 		(0x1F)
#define DIO_E_PARAM_POINTER 			(0x20)

/*==============================================================================
 *                          TYPE DEFINITIONS                                   *
 ==============================================================================*/

/**********************************************************
 * @typedef Dio_ChannelType
 * @brief Type definition for Dio_ChannelType, representing a channel ID 
 *        used in Digital I/O (DIO) operations in AUTOSAR.
 * 
 * @details This type specifies digital input/output channels on the STM32F103C8T6 
 *          microcontroller. The values correspond to GPIO pin numbers as follows:
 *          
 *          Dio_ChannelType channelId = x;  // GPIO Pin x (e.g., PAx, PBx)
 *          
 *          Where x ranges from 0 to 15, representing the GPIO pins PA0-PA15 or PB0-PB15.
 **********************************************************/
typedef uint8 Dio_ChannelType;

/**********************************************************
 * @typedef Dio_PortType
 * @brief Type definition for Dio_PortType, representing a port ID 
 *        used in Digital I/O (DIO) operations in AUTOSAR.
 * 
 * @details This type specifies digital input/output ports on the STM32F103C8T6 
 *          microcontroller. The values correspond to the GPIO port numbers as follows:
 * 
 *          Dio_PortType portId = x;  // GPIO Port x (e.g., 0 for Port A, 1 for Port B)
 * 
 *          Where x is a number representing the GPIO port, such as 0 for Port A, 
 *          1 for Port B, etc.
 **********************************************************/
typedef uint16 Dio_PortType;

/**********************************************************
 * @typedef Dio_ChannelGroupType
 * @brief Type definition for Dio_ChannelGroupType, representing a group of 
 *        channels in Digital I/O (DIO) operations in AUTOSAR.
 * 
 * @details This type defines a channel group, which includes:
 *          - A mask that specifies the positions of the channels in the group.
 *          - An offset indicating the position of the group from the least significant bit (LSB).
 *          - The port ID (as a number) to which the channel group belongs.
 * 
 *          Example: A group of channels with mask 0x0000000F, starting at LSB, 
 *          and assigned to Port A (Port ID = 0):
 *          Dio_ChannelGroupType group = {0x0000000F, 0, 0};  
 *          // mask = 0x0000000F (positions of channels in the group), offset = 0 
 *          // (starting from LSB), port = 0 (Port A)
 **********************************************************/
typedef struct
{
    uint32 mask;        
    uint8 offset;       
    Dio_PortType port;
} Dio_ChannelGroupType;

/**********************************************************
 * @typedef Dio_LevelType
 * @brief Type definition for Dio_LevelType, representing the 
 *        digital level (high or low) used in Digital I/O (DIO) 
 *        operations in AUTOSAR.
 * 
 * @details This type defines the possible levels for digital 
 *          input/output channels. The values typically correspond 
 *          to high (1) and low (0) logic levels.
 * 
 *          Dio_LevelType level = STD_HIGH;  // Represents a high logic level (1)
 *          Dio_LevelType level = STD_LOW;   // Represents a low logic level (0)
 * 
 *          Where:
 *          STD_HIGH = 1,  // Represents the high level (1)
 *          STD_LOW  = 0   // Represents the low level (0)
 **********************************************************/
typedef uint8 Dio_LevelType;

/**********************************************************
 * @typedef Dio_PortLevelType
 * @brief Type definition for Dio_PortLevelType, representing 
 *        the digital level (high or low) for a group of digital I/O 
 *        ports in AUTOSAR.
 * 
 * @details This type defines the possible levels for multiple digital 
 *          input/output channels (ports). The values correspond to 
 *          the logic levels (high or low) across all pins within a 
 *          port, typically represented as a 16-bit value.
 * 
 *          Dio_PortLevelType portLevel = 0b1111111111111111;  // All pins in the port at high level (1)
 *          Dio_PortLevelType portLevel = 0b0000000000000000;  // All pins in the port at low level (0)
 * 
 *          Where each bit in the 16-bit value represents the level 
 *          of a corresponding pin in the port (1 for high, 0 for low).
 **********************************************************/
typedef uint16 Dio_PortLevelType;

/*==============================================================================
 *                           FUNCTION PROTOTYPES                               *
 ==============================================================================*/

/**********************************************************
 * @brief Reads the digital level (high or low) of a specified DIO channel.
 * @param ChannelId The ID of the DIO channel to be read.
 * @return Dio_LevelType The digital level of the specified channel.
 **********************************************************/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

/**********************************************************
 * @brief Writes a digital level (high or low) to a specified DIO channel.
 * @param ChannelId The ID of the DIO channel to be written to.
 * @param Level The digital level to be written to the channel.
 * @return void This function does not return a value.
 **********************************************************/
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

/**********************************************************
 * @brief Reads the digital levels of all channels of a specified DIO port.
 * @param PortId The ID of the DIO port to be read.
 * @return Dio_PortLevelType The levels of all channels in the specified port.
 **********************************************************/
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);

/**********************************************************
 * @brief Writes a digital level (high or low) to all channels of a specified DIO port.
 * @param PortId The ID of the DIO port to be written to.
 * @param Level The digital level to be written to all channels in the port.
 * @return void This function does not return a value.
 **********************************************************/
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);

/**********************************************************
 * @brief Reads the digital levels (high or low) of all channels in a specified DIO channel group.
 * @param ChannelGroupIdPtr Pointer to the channel group structure (Dio_ChannelGroupType) 
 *                          that contains the mask, offset, and port ID for the group.
 * @return Dio_PortLevelType The levels of all channels in the specified group.
 **********************************************************/
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr);

/**********************************************************
 * @brief Writes a digital level (high or low) to all channels in a specified DIO channel group.
 * @param ChannelGroupIdPtr Pointer to the channel group structure (Dio_ChannelGroupType) 
 *                          that contains the mask, offset, and port ID for the group.
 * @param Level The digital level to be written to all channels in the group.
 * @return void This function does not return a value.
 **********************************************************/
void Dio_WriteChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr, Dio_PortLevelType Level);

/**********************************************************
 * @brief Retrieves the version information of the DIO module.
 * @param versioninfo Pointer to a **Std_VersionInfoType** structure that will 
 *                    be filled with the version information of the DIO module. 
 *                    The structure will contain the major, minor, and patch version.
 * @return void This function does not return a value. The version info is stored 
 *              in the provided structure.
 **********************************************************/
void Dio_GetVersionInfo(Std_VersionInfoType *versioninfo);

/**********************************************************
 * @brief Flips the digital level of a specified DIO channel.
 *        If the current level is **STD_HIGH (1)**, it will change to **STD_LOW (0)**. 
 *        If the current level is **STD_LOW (0)**, it will change to **STD_HIGH (1)**.
 * @param ChannelId The ID of the DIO channel whose level is to be toggled.  
 * @return Dio_LevelType The new level of the specified channel after flipping.
 **********************************************************/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);

#ifdef __cplusplus
}
#endif

#endif // DIO_H
