/******************************************************************************
 *  @file    Spi.h
 *  @brief   Interface for Serial Peripheral Interface (SPI) functions.
 *
 *  @details This header file provides the function prototypes and necessary
 *           definitions for the Serial Peripheral Interface (SPI) module, allowing
 *           configuration and control of SPI communication as specified by the
 *           AUTOSAR standard.
 *
 *  @version 1.0
 *  @date    2024-10-10
 *
 *  @section Author
 *           Tong Xuan Hoang
 ******************************************************************************/

#ifndef SPI_H
#define SPI_H

#include "Std_Types.h"

/* Id for the company in the AUTOSAR
 * For example, Tong Xuan Hoang's ID = 1810 */
#define SPI_VENDOR_ID (1810U)

/* SPI Module Id */
#define SPI_MODULE_ID (83U) /* AUTOSAR-defined ID for SPI module */

/* SPI Instance Id */
#define SPI_INSTANCE_ID (0U)

/*
 * Module Version 1.0.0
 */
#define SPI_SW_MAJOR_VERSION (1U)
#define SPI_SW_MINOR_VERSION (0U)
#define SPI_SW_PATCH_VERSION (0U)

/* Error Codes for SPI module */
#define SPI_E_PARAM_INVALID_CHANNEL_ID (0x0A)
#define SPI_E_PARAM_INVALID_SEQUENCE (0x15)
#define SPI_E_PARAM_INVALID_JOB (0x16)
#define SPI_E_PARAM_POINTER (0x20)

#define SPI_MAX_CHANNEL 2
#define SPI_CHANNEL_1 0 /**< Channel SPI1 */
#define SPI_CHANNEL_2 1 /**< Channel SPI2 */

#define SPI_MAX_SEQUENCE 2
#define SPI_SEQ_1 0 /**< Sequence 1 */
#define SPI_SEQ_2 1 /**< Sequence 2 */

#define SPI_MAX_JOB 2
#define SPI_JOB_1 0 /**< Job 1 */
#define SPI_JOB_2 1 /**< Job 2 */

/**
 * @brief Defines supported baud rates for SPI.
 * @details Specifies the baud rate prescaler values,
 *          allowing the APB clock to be divided by factors from 2 to 256.
 */
typedef enum
{
    SPI_BAUDRATEPRESCALER_2 = SPI_BaudRatePrescaler_2,
    SPI_BAUDRATEPRESCALER_4 = SPI_BaudRatePrescaler_4,
    SPI_BAUDRATEPRESCALER_8 = SPI_BaudRatePrescaler_8,
    SPI_BAUDRATEPRESCALER_16 = SPI_BaudRatePrescaler_16,
    SPI_BAUDRATEPRESCALER_32 = SPI_BaudRatePrescaler_32,
    SPI_BAUDRATEPRESCALER_64 = SPI_BaudRatePrescaler_64,
    SPI_BAUDRATEPRESCALER_128 = SPI_BaudRatePrescaler_128,
    SPI_BAUDRATEPRESCALER_256 = SPI_BaudRatePrescaler_256
} Spi_BaudRateType;

/**
 * @brief Data size type for SPI transmission.
 * @details Specifies the size of the data to be transmitted, either 8-bit or 16-bit.
 */
typedef enum
{
    SPI_DATASIZE_8BIT = SPI_DataSize_8b,
    SPI_DATASIZE_16BIT = SPI_DataSize_16b
} Spi_DataSizeType;

/**
 * @brief Clock polarity type for SPI.
 * @details Specifies the idle state of the clock line (CPOL), with options for low or high polarity.
 */
typedef enum
{
    SPI_CPOL_LOW = SPI_CPOL_Low,
    SPI_CPOL_HIGH = SPI_CPOL_High
} Spi_ClockPolarityType;

/**
 * @brief Clock phase type for SPI.
 * @details Specifies which clock edge (CPHA) is used to capture data, either the first or second edge.
 */
typedef enum
{
    SPI_CPHA_1EDGE = SPI_CPHA_1Edge,
    SPI_CPHA_2EDGE = SPI_CPHA_2Edge
} Spi_ClockPhaseType;

/**
 * @brief SPI mode type.
 * @details Specifies whether the SPI operates in Master or Slave mode.
 */
typedef enum
{
    SPI_MODE_MASTER = SPI_Mode_Master,
    SPI_MODE_SLAVE = SPI_Mode_Slave
} Spi_ModeType;

/**
 * @brief NSS (Slave Select) management type for SPI.
 * @details Specifies whether NSS is managed by software or hardware.
 */
typedef enum
{
    SPI_NSS_SOFT = SPI_NSS_Soft,
    SPI_NSS_HARD = SPI_NSS_Hard
} Spi_NSSManagementType;

/**
 * @brief Specifies the data transfer direction mode for SPI.
 * @details Defines the available modes for SPI data transfer,
 *          including full-duplex, receive-only, and transmit-only modes.
 */
typedef enum
{
    SPI_DIRECTION_2LINES_FULLDUPLEX = SPI_Direction_2Lines_FullDuplex,
    SPI_DIRECTION_2LINES_RXONLY = SPI_Direction_2Lines_RxOnly,
    SPI_DIRECTION_1LINE_RX = SPI_Direction_1Line_Rx,
    SPI_DIRECTION_1LINE_TX = SPI_Direction_1Line_Tx
} Spi_DirectionType;

/**
 * @brief Defines a range of specific status values for the SPI Handler/Driver.
 * @details This enumeration represents the possible states of the SPI Handler/Driver,
 *          indicating whether it is initialized, idle, or currently busy with a transmission.
 */
typedef enum
{
    SPI_UNINIT = 0x00, /**< The SPI Handler/Driver is not initialized or not usable. */
    SPI_IDLE = 0x01,   /**< The SPI Handler/Driver is not currently transmitting any Job. */
    SPI_BUSY = 0x02    /**< The SPI Handler/Driver is performing a SPI Job (transmit). */
} Spi_StatusType;

/**
 * @brief Defines a range of specific job status values for the SPI Handler/Driver.
 * @details This enumeration indicates the result of the last job transmission or the current
 *          status of a job if it is in progress.
 */
typedef enum
{
    SPI_JOB_OK = 0x00,      /**< The last transmission of the Job has been finished successfully. */
    SPI_JOB_PENDING = 0x01, /**< The SPI Handler/Driver is performing a SPI Job. Equivalent to SPI_BUSY status. */
    SPI_JOB_FAILED = 0x02,  /**< The last transmission of the Job has failed. */
    SPI_JOB_QUEUED = 0x03   /**< An asynchronous transmit Job has been accepted, but transmission has not started yet. */
} Spi_JobResultType;

/**
 * @brief Defines a range of specific sequence status values for the SPI Handler/Driver.
 * @details This enumeration represents the status of the last sequence transmission, or indicates
 *          if a sequence is currently in progress or has been canceled.
 */
typedef enum
{
    SPI_SEQ_OK = 0x00,      /**< The last transmission of the Sequence has been finished successfully. */
    SPI_SEQ_PENDING = 0x01, /**< The SPI Handler/Driver is performing a SPI Sequence. Equivalent to SPI_BUSY status. */
    SPI_SEQ_FAILED = 0x02,  /**< The last transmission of the Sequence has failed. */
    SPI_SEQ_CANCELED = 0x03 /**< The last transmission of the Sequence has been canceled by the user. */
} Spi_SeqResultType;

/**
 * @brief Type of application data buffer elements.
 */
typedef uint8 Spi_DataBufferType;

/**
 * @brief Type for defining the number of data elements to send and/or receive by Channel.
 */
typedef uint16 Spi_NumberOfDataType;

/**
 * @brief Specifies the identification (ID) for a Channel.
 */
typedef uint8 Spi_ChannelType;

/**
 * @brief Specifies the identification (ID) for a Job.
 */
typedef uint16 Spi_JobType;

/**
 * @brief Specifies the identification (ID) for a sequence of jobs.
 */
typedef uint8 Spi_SequenceType;

/**
 * @brief Specifies the identification (ID) for a SPI Hardware microcontroller peripheral (unit).
 */
typedef uint8 Spi_HWUnitType;

/**
 * @brief Specifies the asynchronous mechanism mode for SPI buses handled asynchronously.
 * @details This enumeration defines the two possible modes for managing asynchronous
 *          SPI communication, either by polling or by interrupt.
 */
typedef enum
{
    SPI_POLLING_MODE = 0x00,  /**< The asynchronous mechanism is managed by polling.
                                    Interrupts related to SPI buses handled asynchronously are disabled. */
    SPI_INTERRUPT_MODE = 0x01 /**< The asynchronous mechanism is managed by interrupt.
                                    Interrupts related to SPI buses handled asynchronously are enabled. */
} Spi_AsyncModeType;

/*
 * This type of the external data structure shall contain the initialization data for the SPI Handler/Driver.
 */
typedef struct
{
    Spi_ChannelType Channel;   /**< SPI channel (mandatory) */
    Spi_JobType Job;           /**< SPI Job (mandatory) */
    Spi_SequenceType Sequence; /**< SPI Sequence (mandatory) */

    Spi_BaudRateType BaudRate;   /**< Baud rate (optional, default 1 MHz) */
    Spi_ClockPolarityType CPOL;  /**< Clock polarity (optional, default CPOL = 0) */
    Spi_ClockPhaseType CPHA;     /**< Clock phase (optional, default CPHA = 0) */
    Spi_ModeType Mode;           /**< SPI mode (optional, default Master) */
    Spi_NSSManagementType NSS;   /**< NSS management (optional, default software) */
    Spi_DataSizeType DataSize;   /**< Data size (optional, default 8-bit) */
    Spi_DirectionType Direction; /**< Direction (optional, default 2-line full-duplex mode) */
} Spi_ConfigType;

Std_ReturnType StartSpiTransmission(Spi_SequenceType Sequence); //

void Spi_Init(const Spi_ConfigType *ConfigPtr);

Std_ReturnType Spi_DeInit(void);

Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const Spi_DataBufferType *DataBufferPtr);

Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence);

Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType *DataBufferPointer);

Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel, const Spi_DataBufferType *SrcDataBufferPtr, //
                           Spi_DataBufferType *DesDataBufferPtr, Spi_NumberOfDataType Length);

Spi_StatusType Spi_GetStatus(void);

Spi_JobResultType Spi_GetJobResult(Spi_JobType Job);

Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence);

void Spi_GetVersionInfo(Std_VersionInfoType *versioninfo);

Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence);

Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit); //

void Spi_Cancel(Spi_SequenceType Sequence); //

Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode); //

void Spi_MainFunction_Handling(void); //

// void (*Spi_SeqEndNotification)(void);	//

#endif /* SPI_H */
