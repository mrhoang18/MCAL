/******************************************************************************
 *  @file    Dio.h
 *  @brief   Interface for Digital Input/Output (DIO) functions.
 *
 *  @details This header file provides the function prototypes and necessary 
 *           definitions for the Digital Input/Output (DIO) module, allowing 
 *           configuration and control of digital pins as specified by the 
 *           AUTOSAR standard.
 *  
 *  @version 1.0
 *  @date    2024-10-10
 *
 *  @section Author
 *           Tong Xuan Hoang
 ******************************************************************************/

#ifndef DIO_H
#define DIO_H

#include "Std_types.h"
#include "stm32f10x_gpio.h"

/* Id for the company in the AUTOSAR
 * For example Tong Xuan Hoang's ID = 1810 */
#define DIO_VENDOR_ID    (1810U)

/* Dio Module Id */
#define DIO_MODULE_ID    (202U)

/* Dio Instance Id */
#define DIO_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define DIO_SW_MAJOR_VERSION           (1U)
#define DIO_SW_MINOR_VERSION           (0U)
#define DIO_SW_PATCH_VERSION           (0U)

#define DIO_E_PARAM_INVALID_CHANNEL_ID 	(0x0A)
#define DIO_E_PARAM_INVALID_PORT_ID 	(0x14)
#define DIO_E_PARAM_INVALID_GROUP 		(0x1F)
#define DIO_E_PARAM_POINTER 			(0x20)

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/* Type definition for Dio_ChannelType used by the DIO APIs */
typedef uint8 Dio_ChannelType;

/* Type definition for Dio_PortType used by the DIO APIs */
typedef uint16 Dio_PortType;

/* Structure for Dio_ChannelGroup */
typedef struct
{
	/* Mask which defines the positions of the channel group */
    uint32 mask;
	/* Position of the group from LSB */
    uint8 offset;
	/* This shall be the port ID which the Channel group is defined. */
    Dio_PortType port;
} Dio_ChannelGroupType;

/* Type definition for Dio_ChannelType used by the DIO APIs */
typedef uint8 Dio_LevelType;

/* Type definition for Dio_PortLevelType used by the DIO APIs */
typedef uint16 Dio_PortLevelType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/* Function for DIO read Channel API */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

/* Function for DIO write Channel API */
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

/* Function for DIO read Port API */
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);

/* Function for DIO write Port API */
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);

/* Function for DIO read Channel Group API */
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr);

/* Function for DIO write Channel Group API */
void Dio_WriteChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr, Dio_PortLevelType Level);

/* Function for DIO Get Version Info API */
void Dio_GetVersionInfo(Std_VersionInfoType *versioninfo);

/* Function for DIO flip channel API */
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);

#endif // DIO_H
