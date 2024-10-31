/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Dio.h
 *       Module:  DIO
 *
 *  Description:  Header file for AUTOSAR DIO Driver.
 *
 *********************************************************************************************************************/

#ifndef DIO_H
#define DIO_H

#include "Std_types.h"
#include "stm32f10x_gpio.h"

/* Id for the company in the AUTOSAR
 * For example TONG XUAN HOANG's ID = 1810 */
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

#define DIO_E_PARAM_INVALID_CHANNEL_ID (0x0A)
#define DIO_E_PARAM_INVALID_PORT_ID (0x14)
#define DIO_E_PARAM_INVALID_GROUP (0x1F)
#define DIO_E_PARAM_POINTER (0x20)

typedef uint8 Dio_ChannelType;
typedef uint16 Dio_PortType;

typedef struct
{
    uint32 mask;
    uint8 offset;
    Dio_PortType port;
} Dio_ChannelGroupType;

typedef uint8 Dio_LevelType;
typedef uint16 Dio_PortLevelType;

// Page 26
Dio_LevelType Dio_ReadChannel(
    Dio_ChannelType ChannelId);

// Page 26
void Dio_WriteChannel(
    Dio_ChannelType ChannelId,
    Dio_LevelType Level);

// Page 27
Dio_PortLevelType Dio_ReadPort(
    Dio_PortType PortId);

// Page 28
void Dio_WritePort(
    Dio_PortType PortId,
    Dio_PortLevelType Level);

// Page 28 
Dio_PortLevelType Dio_ReadChannelGroup(
    const Dio_ChannelGroupType *ChannelGroupIdPtr);

// Page 29 
void Dio_WriteChannelGroup(
    const Dio_ChannelGroupType *ChannelGroupIdPtr,
    Dio_PortLevelType Level);

// Page 30
void Dio_GetVersionInfo(
    Std_VersionInfoType *versioninfo);

// Page 31
Dio_LevelType Dio_FlipChannel(
    Dio_ChannelType ChannelId);

#endif // DIO_H
