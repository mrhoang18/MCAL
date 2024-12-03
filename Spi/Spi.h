/**********************************************************
 * @file Spi.h
 * @brief AUTOSAR SPI Driver Header File
 * @details This file contains the necessary definitions for
 *          the SPI driver according to the AUTOSAR standard.
 * @version 1.0
 * @date 2024-11-01
 * @author Tong Xuan Hoang
 **********************************************************/

#ifndef SPI_H
#define SPI_H

#include "Std_Types.h"      /**< Type definitions for standard types used across AUTOSAR modules */
#include "stm32f10x.h"      /**< Header from the Standard Peripheral Library for STM32F103C8T6 */
#include "stm32f10x_spi.h"  /**< SPI header from the Standard Peripheral Library for STM32F103C8T6 */

#ifdef __cplusplus
extern "C"{
#endif

/*==============================================================================
 *                              MACROS                                         *
 ==============================================================================*/

/**********************************************************
 * @brief SPI Module ID Configuration
 * @details This section defines macros related to the identification 
 *          of the SPI module, including the vendor ID, module ID, 
 *          and instance ID. These identifiers are used for module 
 *          configuration and integration in the AUTOSAR environment.
 *          - SPI_VENDOR_ID: Defines the company/vendor ID for the AUTOSAR 
 *            implementation. Example: Tong Xuan Hoang's ID is 1810.
 *          - SPI_MODULE_ID: Specifies the unique ID for the SPI module.
 *          - SPI_INSTANCE_ID: Specifies the instance ID for the SPI module.
 **********************************************************/
#define SPI_VENDOR_ID (1810U)
#define SPI_MODULE_ID (83U)
#define SPI_INSTANCE_ID (0U)

/**********************************************************
 * @brief SPI Software Version Configuration
 * @details This section defines the versioning information for the SPI 
 *          module. The version is represented by major, minor, and patch 
 *          numbers to indicate the software's release level and changes.
 *          - SPI_SW_MAJOR_VERSION: Major version of the SPI module.
 *          - SPI_SW_MINOR_VERSION: Minor version of the SPI module.
 *          - SPI_SW_PATCH_VERSION: Patch version of the SPI module.
 **********************************************************/
#define SPI_SW_MAJOR_VERSION (1U)
#define SPI_SW_MINOR_VERSION (0U)
#define SPI_SW_PATCH_VERSION (0U)

/**********************************************************
 * @brief SPI Error Codes
 * @details This section defines the error codes used in the SPI module 
 *          to indicate specific error conditions during operations. 
 *          - SPI_E_PARAM_INVALID_CHANNEL_ID: Error code for invalid 
 *            channel ID.
 *          - SPI_E_PARAM_INVALID_SEQUENCE: Error code for invalid 
 *            sequence parameter.
 *          - SPI_E_PARAM_INVALID_JOB: Error code for invalid job parameter.
 *          - SPI_E_PARAM_POINTER: Error code for invalid pointer.
 **********************************************************/
#define SPI_E_PARAM_INVALID_CHANNEL_ID (0x0A)
#define SPI_E_PARAM_INVALID_SEQUENCE (0x15)
#define SPI_E_PARAM_INVALID_JOB (0x16)
#define SPI_E_PARAM_POINTER (0x20)

/**********************************************************
 * @brief SPI Channel Configuration
 * @details This section defines macros related to the configuration 
 *          of the SPI channels. These macros specify the maximum number 
 *          of channels and the individual channel identifiers used in the 
 *          SPI module configuration. 
 *          - SPI_MAX_CHANNEL: Defines the maximum number of SPI channels 
 *            available in the system. In this case, there are 2 SPI channels.
 *          - SPI_CHANNEL_1: Identifies SPI channel 1 (SPI1).
 *          - SPI_CHANNEL_2: Identifies SPI channel 2 (SPI2).
 **********************************************************/
#define SPI_MAX_CHANNEL 2
#define SPI_CHANNEL_1 0
#define SPI_CHANNEL_2 1

/**********************************************************
 * @brief SPI Sequence Configuration
 * @details This section defines macros related to the configuration 
 *          of SPI sequences. These macros specify the maximum number 
 *          of sequences and the identifiers for each sequence used 
 *          in the SPI module.
 *          - SPI_MAX_SEQUENCE: Defines the maximum number of SPI sequences 
 *            available in the system. In this case, there are 2 SPI sequences.
 *          - SPI_SEQ_1: Identifies SPI Sequence 1.
 *          - SPI_SEQ_2: Identifies SPI Sequence 2.
 **********************************************************/
#define SPI_MAX_SEQUENCE 2
#define SPI_SEQ_1 0
#define SPI_SEQ_2 1

/**********************************************************
 * @brief SPI Job Configuration
 * @details This section defines macros related to the configuration 
 *          of SPI jobs. These macros specify the maximum number of jobs 
 *          and the identifiers for each job used in the SPI module.
 *          - SPI_MAX_JOB: Defines the maximum number of SPI jobs 
 *            available in the system. In this case, there are 2 SPI jobs.
 *          - SPI_JOB_1: Identifies SPI Job 1.
 *          - SPI_JOB_2: Identifies SPI Job 2.
 **********************************************************/
#define SPI_MAX_JOB 2
#define SPI_JOB_1 0
#define SPI_JOB_2 1

/*==============================================================================
 *                          TYPE DEFINITIONS                                   *
 ==============================================================================*/

/**********************************************************
 * @typedef Spi_BaudRateType
 * @brief Defines supported baud rates for SPI.
 * @details This enumeration defines the available baud rate prescaler values 
 *          for configuring the SPI communication speed. 
 *          The prescaler values allow the APB (Advanced Peripheral Bus) clock 
 *          to be divided by factors ranging from 2 to 256.
 **********************************************************/
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

/**********************************************************
 * @typedef Spi_DataSizeType
 * @brief Data size type for SPI transmission.
 * @details This enumeration defines the available data sizes for SPI transmission.  
 *          The data size can be selected for SPI communication based on the 
 *          requirements of the application. The available options are:
 *          - SPI_DATASIZE_8BIT: Data size of 8 bits (1 byte).
 *          - SPI_DATASIZE_16BIT: Data size of 16 bits (2 bytes).
 **********************************************************/
typedef enum
{
    SPI_DATASIZE_8BIT = SPI_DataSize_8b,
    SPI_DATASIZE_16BIT = SPI_DataSize_16b
} Spi_DataSizeType;

/**********************************************************
 * @typedef Spi_ClockPolarityType
 * @brief Clock polarity type for SPI.
 * @details This enumeration defines the clock polarity, that determines 
 *          how the clock signal behaves when there is no data being transmitted. 
 *          The available options are:
 *          - SPI_CPOL_LOW: The clock is idle low (CPOL = 0), meaning the clock line 
 *          is at a low level when no data is being transmitted.
 *          - SPI_CPOL_HIGH: The clock is idle high (CPOL = 1), meaning the clock line 
 *          is at a high level when no data is being transmitted.
 **********************************************************/
typedef enum
{
    SPI_CPOL_LOW = SPI_CPOL_Low,
    SPI_CPOL_HIGH = SPI_CPOL_High
} Spi_ClockPolarityType;

/**********************************************************
 * @typedef Spi_ClockPhaseType
 * @brief Clock phase type for SPI.
 * @details This enumeration defines the clock phase, that determines 
 *          whether data is captured on the first or second clock edge. 
 *          The available options are:
 *          - SPI_CPHA_1EDGE: Data is captured on the first clock edge (CPHA = 0).
 *          - SPI_CPHA_2EDGE: Data is captured on the second clock edge (CPHA = 1).
 **********************************************************/
typedef enum
{
    SPI_CPHA_1EDGE = SPI_CPHA_1Edge,
    SPI_CPHA_2EDGE = SPI_CPHA_2Edge
} Spi_ClockPhaseType;

/**********************************************************
 * @typedef Spi_ModeType
 * @brief SPI mode type.
 * @details This enumeration defines the operational mode for SPI communication.
 *          The mode determines the role of the SPI device in the communication:
 *          - SPI_MODE_MASTER: The SPI interface operates in **Master** mode.
 *          - SPI_MODE_SLAVE: The SPI interface operates in **Slave** mode.
 **********************************************************/
typedef enum
{
    SPI_MODE_MASTER = SPI_Mode_Master,
    SPI_MODE_SLAVE = SPI_Mode_Slave
} Spi_ModeType;

/**********************************************************
 * @typedef Spi_NSSManagementType
 * @brief NSS (Slave Select) management type for SPI.
 * @details This enumeration defines how the **NSS (Slave Select)** line is managed 
 *          The available management options are:
 *          - SPI_NSS_SOFT: The NSS line is managed by **software**.
 *          - SPI_NSS_HARD: The NSS line is managed by **hardware**.
 **********************************************************/
typedef enum
{
    SPI_NSS_SOFT = SPI_NSS_Soft,
    SPI_NSS_HARD = SPI_NSS_Hard
} Spi_NSSManagementType;

/**********************************************************
 * @typedef Spi_DirectionType
 * @brief Specifies the data transfer direction mode for SPI.
 * @details This enumeration defines the modes for SPI data transfer.
 *          - SPI_DIRECTION_2LINES_FULLDUPLEX: Full-duplex mode, where both 
 *            transmission and reception occur simultaneously using two data lines.
 *          - SPI_DIRECTION_2LINES_RXONLY: Receive-only mode, where data is 
 *            only received, and the transmit line is not used.
 *          - SPI_DIRECTION_1LINE_RX: Single-line receive mode, where data is 
 *            received on a single data line.
 *          - SPI_DIRECTION_1LINE_TX: Single-line transmit mode, where data is 
 *            transmitted on a single data line.
 **********************************************************/
typedef enum
{
    SPI_DIRECTION_2LINES_FULLDUPLEX = SPI_Direction_2Lines_FullDuplex,
    SPI_DIRECTION_2LINES_RXONLY = SPI_Direction_2Lines_RxOnly,
    SPI_DIRECTION_1LINE_RX = SPI_Direction_1Line_Rx,
    SPI_DIRECTION_1LINE_TX = SPI_Direction_1Line_Tx
} Spi_DirectionType;

/**********************************************************
 * @typedef Spi_StatusType
 * @brief Defines a range of specific status values for the SPI Handler/Driver.
 * @details This enumeration represents the possible states of the SPI Handler/Driver.
 *          - SPI_UNINIT: The SPI Handler/Driver is not initialized or not usable.
 *          - SPI_IDLE: The SPI Handler/Driver is not currently transmitting any Job.
 *          - SPI_BUSY: The SPI Handler/Driver is performing a SPI Job (transmit).
 **********************************************************/
typedef enum
{
    SPI_UNINIT = 0x00, 
    SPI_IDLE = 0x01,
    SPI_BUSY = 0x02
} Spi_StatusType;

/**********************************************************
 * @typedef Spi_JobResultType
 * @brief Defines a range of specific job status values for the SPI Handler/Driver.
 * 
 * @details This enumeration indicates the result of the last job transmission 
 *          or the current status of a job if it is in progress.
 *          - SPI_JOB_OK: The last transmission of the Job has been finished successfully.
 *          - SPI_JOB_PENDING: The SPI Handler/Driver is performing a SPI Job. 
 *          - SPI_JOB_FAILED: The last transmission of the Job has failed.
 *          - SPI_JOB_QUEUED: An asynchronous transmit Job has been accepted, 
 *            but transmission has not started yet.
 **********************************************************/
typedef enum
{
    SPI_JOB_OK = 0x00,
    SPI_JOB_PENDING = 0x01,
    SPI_JOB_FAILED = 0x02,
    SPI_JOB_QUEUED = 0x03
} Spi_JobResultType;

/**********************************************************
 * @typedef Spi_SeqResultType
 * @brief Defines a range of specific sequence status values for the SPI Handler/Driver.
 * @details This enumeration represents the status of the last sequence transmission, 
 *          or indicates if a sequence is currently in progress or has been canceled.
 *        - SPI_SEQ_OK: The last transmission of the Sequence has been finished successfully.
 *        - SPI_SEQ_PENDING: The SPI Handler/Driver is performing a SPI Sequence. 
 *        - SPI_SEQ_FAILED: The last transmission of the Sequence has failed.
 *        - SPI_SEQ_CANCELED: The last transmission of the Sequence has been canceled by the user.
 **********************************************************/
typedef enum
{
    SPI_SEQ_OK = 0x00,
    SPI_SEQ_PENDING = 0x01,
    SPI_SEQ_FAILED = 0x02,
    SPI_SEQ_CANCELED = 0x03
} Spi_SeqResultType;

/**********************************************************
 * @typedef Spi_DataBufferType
 * @brief Type of application data buffer elements.
 * @details This type defines the data type for elements of the application data buffer
 **********************************************************/
typedef uint8 Spi_DataBufferType;

/**********************************************************
 * @typedef Spi_NumberOfDataType
 * @brief Type for defining the number of data elements to send and/or receive by Channel.
 * @details This type specifies the number of data elements (e.g., bytes or words) 
 *          to be sent or received over a given SPI channel.
 **********************************************************/
typedef uint16 Spi_NumberOfDataType;

/**********************************************************
 * @typedef Spi_ChannelType
 * @brief Specifies the identification (ID) for a Channel.
 * @details This type defines the channel ID used in SPI communication. 
 *          The ID is typically mapped to a physical SPI channel 
 *          on the microcontroller (e.g., SPI1, SPI2).
 **********************************************************/
typedef uint8 Spi_ChannelType;

/**********************************************************
 * @typedef Spi_JobType
 * @brief Specifies the identification (ID) for a Job.
 * @details This type defines the job ID used in SPI communication.
 **********************************************************/
typedef uint16 Spi_JobType;

/**********************************************************
 * @typedef Spi_SequenceType
 * @brief Specifies the identification (ID) for a sequence of jobs.
 * @details This type defines the sequence ID used in SPI communication.
 **********************************************************/
typedef uint8 Spi_SequenceType;

/**********************************************************
 * @typedef Spi_HWUnitType
 * @brief Specifies the identification (ID) for a SPI Hardware microcontroller peripheral (unit).
 * @details This type defines the hardware unit ID for the SPI peripheral on 
 *          the microcontroller.
 **********************************************************/
typedef uint8 Spi_HWUnitType;

/**********************************************************
 * @typedef Spi_AsyncModeType
 * @brief Specifies the asynchronous mechanism mode for SPI buses handled asynchronously.
 * @details This enumeration defines the two possible modes for managing asynchronous
 *          SPI communication, either by polling or by interrupt.
 *          - SPI_POLLING_MODE: The asynchronous mechanism is managed by polling.
 *            Interrupts related to SPI buses handled asynchronously are disabled.
 *          - SPI_INTERRUPT_MODE: The asynchronous mechanism is managed by interrupt.
 *            Interrupts related to SPI buses handled asynchronously are enabled.
 **********************************************************/
typedef enum
{
    SPI_POLLING_MODE = 0x00,
    SPI_INTERRUPT_MODE = 0x01
} Spi_AsyncModeType;

/**********************************************************
 * @typedef Spi_ConfigType
 * @brief This type of the external data structure shall contain the initialization data.
 * @details This structure defines the configuration settings for the SPI Handler/Driver. It includes
 *          mandatory parameters such as the SPI channel, job, and sequence, as well as optional 
 *          parameters for various SPI settings like baud rate, clock polarity, phase, and others.
 *          This structure is used to initialize the SPI module with the appropriate configuration. 
 *          Madatory parameters:
 *          - Spi_Channel: Specifies the SPI channel to be used.
 *          - Spi_Job: Specifies the SPI job to be executed.
 *          - Spi_Sequence: Specifies the SPI sequence for job execution.
 *          Optional parameters:
 *          - Spi_BaudRate: The baud rate for SPI communication. Default is 1 MHz.
 *          - Spi_ClockPolarityType CPOL: The clock polarity (CPOL). Default is 0 (low).
 *          - Spi_ClockPhaseType CPHA: The clock phase (CPHA). Default is 0.
 *          - Spi_ModeType Mode: The SPI mode (Master/Slave). Default is Master.
 *          - Spi_NSSManagementType NSS: Defines NSS (Slave Select) management mode. Default is software.
 *          - Spi_DataSizeType DataSize: Defines the data size for SPI. Default is 8-bit.
 *          - Spi_DirectionType Direction: Defines the data transfer direction. Default is 2-line
 *            full-duplex mode.
 **********************************************************/
typedef struct
{
    Spi_ChannelType Channel;   
    Spi_JobType Job;           
    Spi_SequenceType Sequence; 
    Spi_BaudRateType BaudRate;   
    Spi_ClockPolarityType CPOL;  
    Spi_ClockPhaseType CPHA;     
    Spi_ModeType Mode;           
    Spi_NSSManagementType NSS;   
    Spi_DataSizeType DataSize;
    Spi_DirectionType Direction;
} Spi_ConfigType;

/*==============================================================================
 *                           FUNCTION PROTOTYPES                               *
 ==============================================================================*/

Std_ReturnType StartSpiTransmission(Spi_SequenceType Sequence); //

/**
 * @brief Initializes the SPI driver with specified settings.
 * @param ConfigPtr: Pointer to SPI configuration (Spi_ConfigType).
 *                   If NULL, function exits without action.
 * @return None
 */
void Spi_Init(const Spi_ConfigType *ConfigPtr);

/**
 * @brief  De-initialize all SPI drivers (SPI1 and SPI2) and release resources.
 * @retval Std_ReturnType
 *         - E_OK: Successfully de-initialized all SPI channels
 *         - E_NOT_OK: De-initialization failed
 */
Std_ReturnType Spi_DeInit(void);

/**
 * @brief  Writes a single 8-bit data to the SPI hardware register for transmission.
 * @param  Channel: Specifies the SPI channel (e.g., SPI_CHANNEL_1 or SPI_CHANNEL_2).
 * @param  DataBufferPtr: Pointer to the data buffer containing a single 8-bit data to be transmitted.
 *                        If NULL, no data will be written, and the function returns E_NOT_OK.
 * @retval Std_ReturnType
 *         - E_OK: Data successfully written to SPI hardware.
 *         - E_NOT_OK: Write operation failed (invalid channel or NULL data buffer).
 */
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const Spi_DataBufferType *DataBufferPtr);

/**
 * @brief  Initiates an asynchronous transmission for the specified SPI Sequence.
 * @param  Sequence: The ID of the SPI Sequence to be transmitted.
 * @retval Std_ReturnType
 *         - E_OK: Transmission successfully started.
 *         - E_NOT_OK: Transmission failed (e.g., invalid Sequence or SPI busy).
 */
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence);

/**
 * @brief  Reads data from the internal buffer of a specified SPI channel.
 * @param  Channel: Specifies the SPI channel to read data from.
 * @param  DataBufferPointer: Pointer to the buffer where the read data will be stored.
 *                            Must not be NULL.
 * @retval Std_ReturnType
 *         - E_OK: Data successfully read from internal buffer.
 *         - E_NOT_OK: Read operation failed (invalid channel or NULL data buffer).
 */
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType *DataBufferPointer);

Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel, const Spi_DataBufferType *SrcDataBufferPtr, //
                           Spi_DataBufferType *DesDataBufferPtr, Spi_NumberOfDataType Length);

/**
 * @brief  Retrieves the current overall status of the SPI module.
 * @details This function checks the status of each SPI channel and returns a
 *          general status for the entire SPI module. If any channel is busy,
 *          it returns SPI_BUSY. If all channels are uninitialized, it returns
 *          SPI_UNINIT. If all channels are idle, it returns SPI_IDLE.
 * @retval Spi_StatusType
 *         - SPI_UNINIT: If all SPI channels are uninitialized.
 *         - SPI_IDLE: If all SPI channels are initialized and currently idle.
 *         - SPI_BUSY: If at least one SPI channel is busy.
 */
Spi_StatusType Spi_GetStatus(void);

/**
 * @brief Returns the current status of the specified SPI job.
 *
 * @param Job  ID of the SPI job to query.
 * @return Spi_JobResultType  Current job status:
 *         - SPI_JOB_OK: Job completed successfully.
 *         - SPI_JOB_PENDING: Job is still pending.
 *         - SPI_JOB_FAILED: Job failed or ID is invalid.
 *         - SPI_JOB_CANCELLED: Job was cancelled.
 */
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job);

/**
 * @brief Returns the current status of the specified SPI sequence.
 *
 * @param Sequence  ID of the SPI sequence to query.
 * @return Spi_SeqResultType  Current sequence status:
 *         - SPI_SEQUENCE_OK: Sequence completed successfully.
 *         - SPI_SEQUENCE_PENDING: Sequence is still pending.
 *         - SPI_SEQUENCE_FAILED: Sequence failed or ID is invalid.
 *         - SPI_SEQUENCE_CANCELLED: Sequence was cancelled.
 */
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence);

/**
 * @brief  Retrieves the version information of the SPI driver.
 * @param  versioninfo: Pointer to the structure where the version information will be stored.
 *                      Must not be NULL.
 * @retval None
 */
void Spi_GetVersionInfo(Std_VersionInfoType *versioninfo);

/**
 * @brief Performs a synchronous SPI data transmission for the specified sequence.
 * @details This function initiates data transmission over SPI in a blocking manner.
 *          It waits for each job in the sequence to complete before moving to the next job,
 *          effectively holding the CPU until all jobs are finished. The function returns
 *          only after all jobs in the sequence have been processed.
 *
 * @param Sequence  The ID of the SPI sequence to transmit.
 * @return Std_ReturnType
 *         - E_OK: Transmission completed successfully.
 *         - E_NOT_OK: Transmission failed due to initialization issues, invalid ID, or an error during transmission.
 */
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence);

Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit); //

void Spi_Cancel(Spi_SequenceType Sequence); //

Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode); //

void Spi_MainFunction_Handling(void); //

#ifdef __cplusplus
}
#endif

#endif /* SPI_H */
