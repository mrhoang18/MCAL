/******************************************************************************
 *  @file    Std_Types.h
 *  @brief   
 *
 *  @details 
 *  
 *  @version 1.0
 *  @date    2024-10-10
 *
 *  @section Author
 *           Tong Xuan Hoang
 ******************************************************************************/
#ifndef STD_TYPES_H
#define STD_TYPES_H

/* Id for the company in the AUTOSAR
 * For example TONG XUAN HOANG's ID = 1810 */
#define STD_TYPES_VENDOR_ID				(1810U)

/*
 * Module Version 1.0.0
 */
#define STD_TYPES_SW_MAJOR_VERSION		(1U)
#define STD_TYPES_SW_MINOR_VERSION		(0U)
#define STD_TYPES_SW_PATCH_VERSION		(0U)

/* Standard AUTOSAR data types */
typedef unsigned char uint8;   /*           0 .. 255             */
typedef signed char sint8;     /*        -128 .. +127            */
typedef unsigned short uint16; /*           0 .. 65535           */
typedef signed short sint16;   /*      -32768 .. +32767          */
typedef unsigned long uint32;  /*           0 .. 4294967295      */
typedef signed long sint32;    /* -2147483648 .. +2147483647     */

/**
* The NULL define with a void pointer to zero definition.
*/
#define NULL ((void*)0)
	
/*
 *  Describes the standard Return Type Definitions used in the project
 */
typedef uint8 Std_ReturnType;


/* Boolean data type */
typedef uint8 boolean;
#ifndef TRUE
#define TRUE (1U)
#endif
#ifndef FALSE
#define FALSE (0U)
#endif

/*
 * Structure for the Version of the module.
 * This is requested by calling <Module name>_GetVersionInfo()
 */
typedef struct
{
    uint16 vendorID;
    uint16 moduleID;
    uint8 sw_major_version;
    uint8 sw_minor_version;
    uint8 sw_patch_version;
} Std_VersionInfoType;

#define STD_HIGH	(0x01U)						/* Standard HIGH */
#define STD_LOW		(0x00U)						/* Standard LOW  */

#define E_OK 		((Std_ReturnType)0x00U)		/* Function Return OK     */
#define E_NOT_OK 	((Std_ReturnType)0x01U)		/* Function Return NOT OK */


#endif /* STD_TYPES_H */
