/******************************************************************************
 *  @file    Std_Types.h
 *  @brief   Standard type definitions for AUTOSAR modules.
 *  @details This header file provides the standard data types, macros, and 
 *           constants required by AUTOSAR-compliant modules.
 *  @version 1.0
 *  @date    2024-10-10
 *  @section Author
 *           Tong Xuan Hoang
 ******************************************************************************/

#ifndef STD_TYPES_H
#define STD_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Id for the company in the AUTOSAR
 * For example TONG XUAN HOANG's ID = 1810 */
#define STD_TYPES_VENDOR_ID				(1810U) 
/**
 * @brief  Vendor ID for the AUTOSAR-compliant module.
 *         This ID uniquely identifies the vendor of the module.
 *         It is used to ensure compatibility and traceability of the software.
 * @note   This value is specific to the vendor and must be unique.
 */

#define STD_TYPES_SW_MAJOR_VERSION		(1U)
#define STD_TYPES_SW_MINOR_VERSION		(0U)
#define STD_TYPES_SW_PATCH_VERSION		(0U)
/**
 * @brief  Versioning for the AUTOSAR module.
 *         Defines the major, minor, and patch version numbers of the software module.
 * @note   This information is crucial for software versioning and backward compatibility.
 */

#define NULL ((void*)0)
/**
 * @brief  NULL pointer definition.
 *         Represents a pointer that does not point to any valid memory address.
 *         Used to indicate that a pointer is uninitialized or invalid.
 * @note   This is often used as a sentinel value in C.
 */

#define STD_HIGH	(0x01U)						
/**
 * @brief  Standard HIGH value.
 *         Used to indicate a high-level logic (typically 1).
 *         Often used in control systems to represent an active or enabled state.
 */

#define STD_LOW		(0x00U)						
/**
 * @brief  Standard LOW value.
 *         Used to indicate a low-level logic (typically 0).
 *         Often used in control systems to represent an inactive or disabled state.
 */

#define E_OK 		((Std_ReturnType)0x00U)		
/**
 * @brief  Return code for successful operation.
 *         Indicates that a function executed successfully without errors.
 * @note   This is commonly used in error handling to signify successful completion.
 */

#define E_NOT_OK 	((Std_ReturnType)0x01U)		
/**
 * @brief  Return code for failed operation.
 *         Indicates that a function encountered an error or failure during execution.
 * @note   This is commonly used in error handling to signify failure or an exception.
 */

#ifdef __cplusplus
}
#endif

#endif /* STD_TYPES_H */
