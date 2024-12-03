/******************************************************************************
 *  @file    Can_GeneralTypes.h
 *  @brief   Defines general types for the CAN module.
 *  @details This file contains the type definitions and constants used by the 
 *           CAN driver and related modules.
 *  @version 1.0
 *  @date    2024-10-10
 *  @section Author
 *           Tong Xuan Hoang
 ******************************************************************************/

#ifndef CAN_GENERAL_TYPES_H
#define CAN_GENERAL_TYPES_H

#include "Std_Types.h"
#include "ComStack_Types.h"

#ifdef __cplusplus
extern "C"{
#endif

/*==============================================================================
 *                          TYPE DEFINITIONS                                   *
 ==============================================================================*/

/**
 * @brief Identifier for a CAN L-PDU.
 *
 * Represents the Identifier of an L-PDU. The two most significant bits specify the frame type:
 * - `00`: Standard CAN ID
 * - `01`: CAN FD frame with Standard CAN ID
 * - `10`: CAN message with Extended CAN ID
 * - `11`: CAN FD frame with Extended CAN ID
 *
 * Range:
 * - Standard CAN ID: 0x00000000 - 0x400007FF
 * - Extended CAN ID: 0x00000000 - 0xDFFFFFFF
 */
typedef uint32 Can_IdType;

/**
 * @brief Structure defining a CAN L-PDU (Protocol Data Unit).
 *
 * This structure unites:
 * - swPduHandle: Software PDU handle.
 * - length: Data length (DLC).
 * - id: CAN identifier.
 * - sdu: Pointer to payload data.
 */
typedef struct
{
    PduIdType swPduHandle;  /**< Identifier for the PDU in the software. */
    uint8 length;           /**< Length of the CAN message payload (0-8 bytes, or up to 64 bytes for CAN FD). */
    Can_IdType id;          /**< CAN identifier (Standard or Extended). */
    uint8* sdu;             /**< Pointer to the payload data. */
} Can_PduType;

#ifdef __cplusplus
}
#endif

#endif /* CAN_GENERAL_TYPES_H */
