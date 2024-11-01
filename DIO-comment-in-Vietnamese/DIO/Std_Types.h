/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Std_Types.h
 *       Module:  Standard Types
 *
 *  Description:  Provides standard types for AUTOSAR modules.
 *
 *********************************************************************************************************************/

#ifndef STD_TYPES_H
#define STD_TYPES_H

#define STD_TYPES_SW_MAJOR_VERSION (4U)
#define STD_TYPES_SW_MINOR_VERSION (3U)
#define STD_TYPES_SW_PATCH_VERSION (1U)

#define STD_HIGH (1U) // M?c tín hi?u cao
#define STD_LOW (0U)  // M?c tín hi?u th?p

/* Standard AUTOSAR data types */
typedef unsigned char uint8;   /*           0 .. 255             */
typedef signed char sint8;     /*        -128 .. +127            */
typedef unsigned short uint16; /*           0 .. 65535           */
typedef signed short sint16;   /*      -32768 .. +32767          */
typedef unsigned long uint32;  /*           0 .. 4294967295      */
typedef signed long sint32;    /* -2147483648 .. +2147483647     */

/* Standard Return Type */
typedef uint8 Std_ReturnType;
#define E_OK ((Std_ReturnType)0x00U)
#define E_NOT_OK ((Std_ReturnType)0x01U)

/* Boolean data type */
typedef uint8 boolean;
#ifndef TRUE
#define TRUE (1U)
#endif
#ifndef FALSE
#define FALSE (0U)
#endif

/* Version Information structure */
typedef struct
{
    uint16 vendorID;
    uint16 moduleID;
    uint8 sw_major_version;
    uint8 sw_minor_version;
    uint8 sw_patch_version;
} Std_VersionInfoType;

#endif /* STD_TYPES_H */
