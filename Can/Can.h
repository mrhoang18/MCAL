/******************************************************************************
 *  @file    Can.h
 *  @brief   Header file for CAN driver in AUTOSAR communication stack.
 *
 *  @details This file contains the default initialization configuration and
 *           function prototypes for the CAN driver module.
 *
 *           This file is hardware-independent.
 *
 *  @version 1.0
 *  @date    2024-10-10
 *
 *  @section Author
 *           Tong Xuan Hoang
 ******************************************************************************/

#ifndef CAN_H
#define CAN_H

#include "Can_GeneralTypes.h"
#include "ComStack_Types.h"
#include "stm32f10x.h"
#include "stm32f10x_can.h"
#include "stm32f10x_gpio.h"  

/**
 * @file Can_Cfg.h
 * @brief CAN configuration header file for STM32F103C8T6.
 * 
 * This file contains the configuration settings for the CAN controller
 * as required by AUTOSAR standards.
 */

/* Pre-compile time parameter settings */
#define CAN_DEV_ERROR_DETECT        STD_ON  /**< Enable Development Error Detection */
#define CAN_VERSION_INFO_API        STD_OFF /**< Disable version info API */
#define CAN_MAX_CONTROLLERS         1       /**< Number of CAN controllers supported */

/**
 * @struct Can_ConfigType
 * @brief Configuration structure for CAN driver with nested structures.
 * 
 * This structure contains two sub-structures:
 * - `Can_HardwareConfig`: Configurations for the CAN hardware (e.g., timing, mode).
 * - `Can_GPIOConfig`: Configurations for the GPIO pins used by CAN.
 */
typedef struct 
{
    /**
     * @struct Can_HardwareConfig
     * @brief Sub-structure for CAN hardware configuration.
     * 
     * This structure contains the core settings for CAN hardware, including
     * baudrate prescaler, mode, and timing segments.
     */
    struct 
	{
        uint16_t CAN_Prescaler;     /**< Baudrate prescaler (1 to 1024) */
        uint8_t CAN_Mode;           /**< CAN mode (Normal, Loopback, Silent, etc.) */
        uint8_t CAN_SJW;            /**< Synchronization Jump Width (1 to 4 TQ) */
        uint8_t CAN_BS1;            /**< Bit Segment 1 (1 to 16 TQ) */
        uint8_t CAN_BS2;            /**< Bit Segment 2 (1 to 8 TQ) */
        FunctionalState CAN_TTCM;   /**< Time Triggered Communication Mode (ENABLE/DISABLE) */
        FunctionalState CAN_ABOM;   /**< Automatic Bus-Off Management (ENABLE/DISABLE) */
        FunctionalState CAN_AWUM;   /**< Automatic Wake-Up Mode (ENABLE/DISABLE) */
        FunctionalState CAN_NART;   /**< No Automatic Retransmission (ENABLE/DISABLE) */
        FunctionalState CAN_RFLM;   /**< Receive FIFO Locked Mode (ENABLE/DISABLE) */
        FunctionalState CAN_TXFP;   /**< Transmit FIFO Priority (ENABLE/DISABLE) */
    } Can_HardwareConfig;

    /**
     * @struct Can_GPIOConfig
     * @brief Sub-structure for GPIO configuration.
     * 
     * This structure defines the GPIO pins and their configurations for CAN_TX
     * and CAN_RX.
     */
    struct 
	{
        uint16_t GPIO_Pin_RX;       /**< GPIO pin for CAN_RX */
        uint16_t GPIO_Pin_TX;       /**< GPIO pin for CAN_TX */
        GPIOMode_TypeDef GPIO_Mode; /**< GPIO mode */
        GPIOSpeed_TypeDef GPIO_Speed; /**< GPIO speed */
    } Can_GPIOConfig;

} Can_ConfigType;

/**
 * @var Can_ConfigData
 * @brief Example configuration instance for CAN driver.
 * 
 * This variable contains a predefined configuration for CAN hardware and GPIO
 * settings.
 */
extern Can_ConfigType Can_ConfigData;

/*==================================================================================================
 *  FUNCTION DECLARATIONS
 *==================================================================================================*/

/**
 * @brief  Initializes the CAN controller with the specified configuration.
 * @param  Config: Pointer to the configuration structure for CAN initialization.
 * @retval None
 */
void Can_Init(const Can_ConfigType* Config);

/**
 * @brief  De-initializes the CAN controller and resets all CAN peripheral registers.
 * @retval None
 */
void Can_DeInit(void);

/**
 * @brief  Initializes the CAN controller with specified baud rate configuration.
 * @param  Controller: The CAN controller to configure (0 for CAN1, 1 for CAN2).
 * @param  BaudRateConfigID: Baud rate configuration identifier.
 * @retval Std_ReturnType: E_OK if the baud rate is set successfully, E_NOT_OK otherwise.
 */
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID);

/**
 * @brief  Sets the operating mode of the CAN controller (e.g., Normal, Sleep, or Stop mode).
 * @param  Controller: The CAN controller to configure (0 for CAN1, 1 for CAN2).
 * @param  Transition: The desired controller state transition (e.g., CAN_CS_SLEEP, CAN_CS_ACTIVE).
 * @retval Std_ReturnType: E_OK if the transition is successful, E_NOT_OK otherwise.
 */
Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition);

/**
 * @brief  Disables interrupts for the specified CAN controller and clears interrupt flags.
 * @param  Controller: The CAN controller to disable interrupts (0 for CAN1, 1 for CAN2).
 * @retval None
 */
void Can_DisableControllerInterrupts(uint8 Controller);

/**
 * @brief  Enables interrupts for the specified CAN controller.
 * @param  Controller: The CAN controller to enable interrupts (0 for CAN1, 1 for CAN2).
 * @retval None
 */
void Can_EnableControllerInterrupts(uint8 Controller);

/**
 * @brief  Checks if the specified CAN controller has been woken up from Sleep mode.
 * @param  Controller: The CAN controller to check wake-up status (0 for CAN1, 1 for CAN2).
 * @retval Std_ReturnType: E_OK if the CAN controller has woken up from Sleep mode, E_NOT_OK if the controller is still in Sleep mode.
 */
Std_ReturnType Can_CheckWakeup(uint8 Controller);

/**
 * @brief  Retrieves the error state of the specified CAN controller.
 * @param  ControllerId: The CAN controller ID (0 for CAN1, 1 for CAN2).
 * @param  ErrorStatePtr: Pointer to store the current error state of the CAN controller.
 * @retval Std_ReturnType: 
 *         - E_OK if the error state is successfully retrieved.
 *         - E_NOT_OK if an error occurred while retrieving the error state.
 */
Std_ReturnType Can_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType *ErrorStatePtr);

/**
 * @brief  Retrieves the current operational mode of the specified CAN controller.
 * @param  Controller: The CAN controller to get the mode (0 for CAN1, 1 for CAN2).
 * @param  ControllerModePtr: Pointer to store the current mode of the CAN controller.
 * @retval Std_ReturnType: 
 *         - E_OK if the mode is successfully retrieved.
 *         - E_NOT_OK if an error occurred while retrieving the mode.
 */
Std_ReturnType Can_GetControllerMode(uint8 Controller, Can_ControllerStateType *ControllerModePtr);

/**
 * @brief  Retrieves the receive error counter value for the specified CAN controller.
 * @param  ControllerId: The CAN controller ID (0 for CAN1, 1 for CAN2).
 * @param  RxErrorCounterPtr: Pointer to store the receive error counter value of the CAN controller.
 * @retval Std_ReturnType: 
 *         - E_OK if the receive error counter value is successfully retrieved.
 *         - E_NOT_OK if an error occurred while retrieving the counter value.
 */
Std_ReturnType Can_GetControllerRxErrorCounter(uint8 ControllerId, uint8 *RxErrorCounterPtr);

/**
 * @brief  Retrieves the transmit error counter value for the specified CAN controller.
 * @param  ControllerId: The CAN controller ID (0 for CAN1, 1 for CAN2).
 * @param  TxErrorCounterPtr: Pointer to store the transmit error counter value of the CAN controller.
 * @retval Std_ReturnType: 
 *         - E_OK if the transmit error counter value is successfully retrieved.
 *         - E_NOT_OK if an error occurred while retrieving the counter value.
 */
Std_ReturnType Can_GetControllerTxErrorCounter(uint8 ControllerId, uint8 *TxErrorCounterPtr);


#endif /* CAN_CFG_H */
