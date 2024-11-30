/**********************************************************
 * @file Lin_Cfg.h
 * @brief The sole header file for the LIN driver.
 * @details This file contains declarations of structures, constants, and arrays required
 *          to configure and manage the state of LIN channels, along with
 *          version information and vendor/module IDs.
 * @version 1.0
 * @date 2024-11-01
 * @author Tong Xuan Hoang
 **********************************************************/

#ifndef LIN_CFG_H
#define LIN_CFG_H

#include "stm32f1xx.h" // Include hardware library if using STM32
#include "Lin_Types.h" // Include necessary data type definitions

/**********************************************************
 * @brief Defines the states of the LIN channel.
 **********************************************************/
#define LIN_CH_SLEEP 0         /**< @brief Sleep state of the LIN channel. */
#define LIN_CH_OPERATIONAL 1   /**< @brief Operational state of the LIN channel. */
#define LIN_CH_SLEEP_PENDING 2 /**< @brief Pending state for transitioning to sleep. */

/**********************************************************
 * @brief Defines the maximum number of LIN channels.
 * @details This value should be configured based on the actual system
 *          and the microcontroller's capabilities.
 **********************************************************/
#define MAX_LIN_CHANNELS 2 // This value can be adjusted depending on the number of LIN channels supported by the system

/**********************************************************
 * @brief Defines the Vendor ID.
 **********************************************************/
#define LIN_VENDOR_ID 123 /**< @brief Vendor ID for the LIN driver. */

/**********************************************************
 * @brief Defines the Module ID.
 **********************************************************/
#define LIN_MODULE_ID 456 /**< @brief Module ID for the LIN driver. */

/**********************************************************
 * @brief Defines the Software Version.
 **********************************************************/
#define LIN_SW_MAJOR_VERSION 1 /**< @brief Major version of the software. */
#define LIN_SW_MINOR_VERSION 0 /**< @brief Minor version of the software. */
#define LIN_SW_PATCH_VERSION 0 /**< @brief Patch version of the software. */

/**********************************************************
 * @enum Lin_StatusType
 * @brief Different states of the LIN channel.
 **********************************************************/
typedef enum
{
    LIN_CH_SLEEP,        /**< @brief Sleep state of the LIN channel. */
    LIN_CH_OPERATIONAL,  /**< @brief Operational state of the LIN channel. */
    LIN_CH_SLEEP_PENDING /**< @brief Pending state to transition to sleep. */
} Lin_StatusType;

/**********************************************************
 * @struct LinChannelConfigType
 * @brief Structure containing configuration information for a LIN channel.
 **********************************************************/
typedef struct
{
    uint32 Lin_BaudRate;                     /**< @brief Data transmission speed of the LIN channel. */
    FunctionalState LinChannelWakeupSupport; /**< @brief Support for wake-up detection (ENABLE/DISABLE). */
    uint8 Lin_ChannelID;                     /**< @brief ID of the LIN channel. */
    GPIO_TypeDef *Lin_Port;                  /**< @brief GPIO port of the LIN channel. */
    uint16 Lin_TxPin;                        /**< @brief Tx pin of the LIN channel. */
    uint16 Lin_RxPin;                        /**< @brief Rx pin of the LIN channel. */
} LinChannelConfigType;

/**********************************************************
 * @brief Array storing the states of LIN channels.
 * @details Each element in the array represents the state of a specific LIN channel.
 **********************************************************/
Lin_StatusType LinChannelState[MAX_LIN_CHANNELS] = {
    LIN_CH_SLEEP, // Initialize the initial state for each channel
    LIN_CH_SLEEP  // Initialize for the second channel, if required
};

/**********************************************************
 * @brief Array storing configurations for LIN channels.
 * @details Each element in the array contains configuration information for a specific LIN channel,
 *          including baud rate, wake-up support status, and associated GPIO pins.
 **********************************************************/
LinChannelConfigType LinChannelConfig[MAX_LIN_CHANNELS] = {
    {
        .Lin_BaudRate = 19200,             /**< @brief Baud rate for the LIN channel. */
        .LinChannelWakeupSupport = ENABLE, /**< @brief Wake-up support. */
        .Lin_ChannelID = 0,                /**< @brief ID of the LIN channel. */
        .Lin_Port = GPIOA,                 /**< @brief GPIO port used. */
        .Lin_TxPin = GPIO_Pin_9,           /**< @brief Tx pin of the channel. */
        .Lin_RxPin = GPIO_Pin_10           /**< @brief Rx pin of the channel. */
    },
    // Add configurations for additional channels if needed
};

#endif /* LIN_CFG_H */
