/******************************************************************************
 *  @file    ComStack_Types.h
 *  @brief   Defines common types and constants for AUTOSAR Communication Stack.
 *
 *  @details This file contains basic type definitions (e.g., PduIdType).
 *
 *  @version 1.0
 *  @date    2024-10-10
 *
 *  @section Author
 *           Tong Xuan Hoang
 ******************************************************************************/

#ifndef COMSTACK_TYPES_H
#define COMSTACK_TYPES_H

/*==================================================================================================
 *  INCLUDES
 *==================================================================================================*/

/* Include standard types like uint8, uint16, etc., if required */
#include "Std_Types.h"

/*==================================================================================================
 *  MACROS
 *==================================================================================================*/

/* Define macros here if required, for example, version information */

/*==================================================================================================
 *  TYPE DEFINITIONS
 *==================================================================================================*/

/**
 * @brief Identifier type for PDUs (Protocol Data Units).
 *
 * This type is used to identify a PDU uniquely within the context of communication
 * stack modules (e.g., CAN, LIN, FlexRay). It is typically used for Tx/Rx PDUs.
 */
typedef uint16 PduIdType;

/**
 * @typedef Can_IdType
 * @brief Represents the identifier of a CAN L-PDU.
 * 
 * This type is used to specify the identifier for CAN messages. The most significant bits
 * of the identifier define the frame type:
 * - `00`: Standard CAN ID for classic CAN messages.
 * - `01`: CAN FD frame with Standard CAN ID.
 * - `10`: CAN message with Extended CAN ID.
 * - `11`: CAN FD frame with Extended CAN ID.
 * 
 * Range:
 * - Standard 32-bit: 0x00000000 to 0x1FFFFFFF.
 * - Extended 32-bit: 0x00000000 to 0xFFFFFFFF.
 *
 */
typedef uint32 Can_IdType;

/**
 * @typedef Can_HwHandleType
 * @brief Represents hardware object handles for a CAN hardware unit.
 * 
 * This type is used to identify hardware objects within the CAN driver.
 * - Standard range: 0x00 - 0xFF (for up to 255 hardware objects).
 * - Extended range: 0x0000 - 0xFFFF (for more than 255 hardware objects).
 * 
 */
typedef uint16 Can_HwHandleType;

/**
 * @struct Can_HwType
 * @brief Defines a structure to represent CAN hardware object information.
 * 
 * This structure provides the Hardware Object Handle, corresponding CAN Controller, 
 * and CAN ID, which are used in CAN L-PDUs.
 * 
 * Elements:
 * - CanId: Standard/Extended CAN ID of CAN L-PDU.
 * - Hoh: Hardware object handle ID, linked to the hardware object range.
 * - ControllerId: Identifies the corresponding CAN Controller.
 * 
 */
typedef struct 
{
    Can_IdType CanId;          /**< Standard/Extended CAN ID of CAN L-PDU */
    Can_HwHandleType Hoh;      /**< ID of the corresponding Hardware Object Range */
    uint8 ControllerId;        /**< CAN Controller ID provided by CanIf */
} Can_HwType;

/**
 * @enum Can_ErrorStateType
 * @brief Defines the error states of a CAN controller.
 * 
 * This enumeration is used to represent the current error state of a CAN controller
 * as defined by the AUTOSAR standard.
 * 
 * Enumerators:
 * - `CAN_ERRORSTATE_ACTIVE`: The CAN controller takes full part in communication.
 * - `CAN_ERRORSTATE_PASSIVE`: The CAN controller takes part in communication 
 *   but does not send active error frames.
 * - `CAN_ERRORSTATE_BUSOFF`: The CAN controller does not participate in communication.
 * 
 */
typedef enum 
{
    CAN_ERRORSTATE_ACTIVE,  /**< Fully operational state. */
    CAN_ERRORSTATE_PASSIVE, /**< Limited operational state. */
    CAN_ERRORSTATE_BUSOFF   /**< Controller is disconnected from communication. */
} Can_ErrorStateType;

/**
 * @enum Can_ControllerStateType
 * @brief Represents the states of a CAN controller.
 * 
 * This enumeration defines the possible states of a CAN controller used in various 
 * ControllerMode functions, such as initialization, operation, and power management.
 * 
 * Enumerators:
 * - `CAN_CS_UNINIT`: The CAN controller is uninitialized.
 * - `CAN_CS_STARTED`: The CAN controller is started and operational.
 * - `CAN_CS_STOPPED`: The CAN controller is stopped and not participating in communication.
 * - `CAN_CS_SLEEP`: The CAN controller is in sleep mode.
 * 
 */
typedef enum 
{
    CAN_CS_UNINIT = 0x00,  /**< Controller is uninitialized. */
    CAN_CS_STARTED = 0x01, /**< Controller is started and operational. */
    CAN_CS_STOPPED = 0x02, /**< Controller is stopped, not active in communication. */
    CAN_CS_SLEEP = 0x03    /**< Controller is in sleep mode. */
} Can_ControllerStateType;

/**
 * @enum Can_ErrorType
 * @brief Defines the types of errors that can occur in a CAN network.
 * 
 * This enumeration represents a superset of CAN error types, which provide detailed 
 * information about potential hardware and protocol issues encountered during 
 * communication.
 * 
 * Enumerators:
 * - `CAN_ERROR_BIT_MONITORING`: A '0' was transmitted and a '1' was read back.
 * - `CAN_ERROR_BIT2_MONITORING`: A '1' was transmitted and a '0' was read back.
 * - `CAN_ERROR_BIT`: A general CAN bit error when specific monitoring errors can't be distinguished.
 * - `CAN_ERROR_CHECK_ACK_FAILED`: Acknowledgement check failed.
 * - `CAN_ERROR_ACK_DELIMITER_CHECK_FAILED`: Acknowledgement delimiter check failed.
 * - `CAN_ERROR_ARBITRATION_LOST`: Sender lost arbitration.
 * - `CAN_ERROR_OVERLOAD`: Overload frame due to a full receive buffer or transmitter buffer.
 * - `CAN_ERROR_CHECK_FORM_FAILED`: Frame format check failed.
 * - `CAN_ERROR_CHECK_STUFFING_FAILED`: Stuffing bits check failed.
 * - `CAN_ERROR_CHECK_CRC_FAILED`: CRC field check failed.
 * - `CAN_ERROR_BUS_LOCK`: Bus lock due to dominant level in CAN bus.
 * 
 * Note:
 * - This enumeration is specific to CAN HW and not all hardware will support the full set of error types.
 * 
 * Available via: Can_GeneralTypes.h
 */
typedef enum 
{
    CAN_ERROR_BIT_MONITORING = 0x01,            /**< '0' transmitted but '1' read back. */
    CAN_ERROR_BIT2_MONITORING = 0x02,           /**< '1' transmitted but '0' read back. */
    CAN_ERROR_BIT = 0x03,                       /**< General CAN bit error. */
    CAN_ERROR_CHECK_ACK_FAILED = 0x04,          /**< Acknowledgement check failed. */
    CAN_ERROR_ACK_DELIMITER_CHECK_FAILED = 0x05,/**< Acknowledgement delimiter failed. */
    CAN_ERROR_ARBITRATION_LOST = 0x06,          /**< Arbitration lost by the sender. */
    CAN_ERROR_OVERLOAD = 0x07,                  /**< Overload frame error. */
    CAN_ERROR_CHECK_FORM_FAILED = 0x08,         /**< Frame format check failed. */
    CAN_ERROR_CHECK_STUFFING_FAILED = 0x09,     /**< Stuffing bits check failed. */
    CAN_ERROR_CHECK_CRC_FAILED = 0x0A,          /**< CRC field check failed. */
    CAN_ERROR_BUS_LOCK = 0x0B                   /**< Bus lock due to dominant signal. */
} Can_ErrorType;

/**
 * @struct Can_TimeStampType
 * @brief Represents a time stamp based on relative time.
 * 
 * This structure is used to express time stamps with both seconds and nanoseconds 
 * precision. It is typically used in CAN-related operations to provide precise timing 
 * information.
 * 
 * Members:
 * - `seconds`: The seconds part of the time stamp. 
 *              Range: 0 to 4,294,967,295 seconds (~136 years).
 * - `nanoseconds`: The nanoseconds part of the time stamp. 
 *                  Range: 0 to 999,999,999 nanoseconds.
 * 
 * Available via: Can_GeneralTypes.h
 */
typedef struct 
{
    uint32 seconds;      /**< Seconds part of the time stamp. */
    uint32 nanoseconds;  /**< Nanoseconds part of the time stamp. */
} Can_TimeStampType;

/*==================================================================================================
 *  CONSTANTS
 *==================================================================================================*/

/**
 * @def CAN_BUSY
 * @brief Extended return value for Std_ReturnType in CAN driver.
 * 
 * This value is used specifically in the CAN module, typically for the `Can_Write` API.
 * It indicates that the transmit request could not be processed because no transmit
 * object was available at the time of the request.
 * 
 * Value: 0x02
 * 
 */
#define CAN_BUSY 0x02

/*==================================================================================================
 *  FUNCTION DECLARATIONS
 *==================================================================================================*/



/*==================================================================================================
 *  END OF FILE
 *==================================================================================================*/

#endif /* COMSTACK_TYPES_H */
