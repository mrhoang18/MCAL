/**********************************************************
 * @file Spi.c
 * @brief AUTOSAR SPI Driver Source File
 * @details This file contains the function definitions for
 *          the SPI driver according to the AUTOSAR standard.
 * @version 1.0
 * @date 2024-11-01
 * @author Tong Xuan Hoang
 **********************************************************/

#include "Spi.h"
#include "Spi_Cfg.h"

/**
 * @brief  Array to store the status of each SPI channel.
 * @details This array holds the current status for each SPI channel,
 *          allowing independent tracking of each channel's state.
 *          The initial state of each channel is set to SPI_UNINIT,
 *          indicating that neither channel is initialized at startup.
 *          - SPI_STATUS[0]: Represents the status of SPI channel 1.
 *          - SPI_STATUS[1]: Represents the status of SPI channel 2.
 */
static Spi_StatusType Spi_Status[SPI_MAX_CHANNEL] = {SPI_UNINIT, SPI_UNINIT};

/**
 * @brief Array to store the status of each SPI sequence.
 * @details This array maintains the current status of each SPI sequence,
 *          enabling the software to monitor the progress and result of each
 *          sequence independently. Each sequence represents a group of jobs
 *          that are executed together in a specific order.
 *
 *          The initial state of each sequence is set to `SPI_SEQUENCE_PENDING`,
 *          meaning that all sequences are awaiting execution at startup.
 *
 *          Each element in the array corresponds to a specific sequence:
 *          - Spi_SequenceStatus[0]: Represents the status of Sequence 0.
 *          - Spi_SequenceStatus[1]: Represents the status of Sequence 1.
 *          - Spi_SequenceStatus[2]: Represents the status of Sequence 2.
 *
 *          The status of each sequence will be updated based on the completion
 *          of its jobs, reflecting whether the sequence has completed, is pending,
 *          failed, or was cancelled.
 */
static Spi_SeqResultType Spi_SequenceStatus[SPI_MAX_SEQUENCE] = {SPI_SEQ_PENDING, SPI_SEQ_PENDING};

/**
 * @brief Array to store the status of each SPI job.
 * @details This array holds the current status of each SPI job in the system,
 *          allowing independent tracking and management of each job's state.
 *          The initial state of each job is set to `SPI_JOB_PENDING`,
 *          indicating that all jobs are waiting to be processed at startup.
 *
 *          Each element in the array corresponds to a specific job:
 *          - Spi_JobStatus[0]: Represents the status of Job 0.
 *          - Spi_JobStatus[1]: Represents the status of Job 1.
 *
 *          The status of each job will be updated during SPI operations,
 *          reflecting whether the job has completed successfully, is pending,
 *          failed, or was cancelled.
 */
static Spi_JobResultType Spi_JobStatus[SPI_MAX_JOB] = {SPI_JOB_PENDING, SPI_JOB_PENDING};

/**
 * @brief Initializes the SPI driver with specified settings.
 * @param ConfigPtr: Pointer to SPI configuration (Spi_ConfigType).
 * @return None
 * @note Configures GPIO pins for:
 *       - SPI1: SCK (PA5), MISO (PA6), MOSI (PA7), optional NSS (PA4).
 *       - SPI2: SCK (PB13), MISO (PB14), MOSI (PB15), optional NSS (PB12).
 */
void Spi_Init(const Spi_ConfigType *ConfigPtr)
{
    /* If NULL, function exits without action */
    if (ConfigPtr == NULL)
    {
        return;
    }

    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_TypeDef *SPIx;

    /* Determine which SPI peripheral and GPIO settings to use based on the channel */
    if (ConfigPtr->Channel == SPI_CHANNEL_1)
    {
        /* Update status of channel SPI1 */ 
        Spi_Status[SPI_CHANNEL_1] = SPI_IDLE;

        SPIx = SPI1;

        /* Enable clocks for SPI1 and GPIOA (SPI1 uses GPIOA pins) */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

        /* Configure GPIO for SPI1, SCK (PA5), MISO (PA6), MOSI (PA7) */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* Configure NSS (optional, PA4) */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
        GPIO_InitStructure.GPIO_Mode = (ConfigPtr->NSS == SPI_NSS_SOFT) ? GPIO_Mode_Out_PP : GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    else if (ConfigPtr->Channel == SPI_CHANNEL_2)
    {
        /* Update status of channel SPI1 */
        Spi_Status[SPI_CHANNEL_2] = SPI_IDLE;

        SPIx = SPI2;

        /* Enable clocks for SPI2 and GPIOB (SPI2 uses GPIOB pins) */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

        /* Configure GPIO for SPI2, SCK (PB13), MISO (PB14), MOSI (PB15) */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* Configure NSS (optional, PB12) */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
        GPIO_InitStructure.GPIO_Mode = (ConfigPtr->NSS == SPI_NSS_SOFT) ? GPIO_Mode_Out_PP : GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    else
    {
        /* Invalid channel: handle error or return if necessary */
        return;
    }

    /* Configure SPI settings based on ConfigPtr */
    SPI_InitStructure.SPI_BaudRatePrescaler = ConfigPtr->BaudRate;
    SPI_InitStructure.SPI_CPOL = ConfigPtr->CPOL;
    SPI_InitStructure.SPI_CPHA = ConfigPtr->CPHA;
    SPI_InitStructure.SPI_Mode = ConfigPtr->Mode;
    SPI_InitStructure.SPI_NSS = ConfigPtr->NSS;
    SPI_InitStructure.SPI_DataSize = ConfigPtr->DataSize;
    SPI_InitStructure.SPI_Direction = ConfigPtr->Direction;

    /* Initialize SPI with the specified settings */
    SPI_Init(SPIx, &SPI_InitStructure);

    /* Enable the SPI peripheral */ 
    SPI_Cmd(SPIx, ENABLE);
}

/**
 * @brief  De-initialize all SPI drivers (SPI1 and SPI2) and release resources.
 * @retval Std_ReturnType
 *         - E_OK: Successfully de-initialized all SPI channels
 *         - E_NOT_OK: De-initialization failed
 */
Std_ReturnType Spi_DeInit(void)
{
    /* Update status of channel SPI1, SPI2 */
    Spi_Status[SPI_CHANNEL_1] = SPI_UNINIT;
    Spi_Status[SPI_CHANNEL_2] = SPI_UNINIT;

    GPIO_InitTypeDef GPIO_InitStructure;

    /* Disable both SPI peripherals */
    SPI_Cmd(SPI1, DISABLE);
    SPI_Cmd(SPI2, DISABLE);

    /* Reset SPI1 and SPI2 registers by disabling their peripheral clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, DISABLE);

    /* De-initialize GPIO pins for SPI1 (PA4, PA5, PA6, PA7) */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   /**< Default state after reset */
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* De-initialize GPIO pins for SPI2 (PB12, PB13, PB14, PB15) */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   /**< Default state after reset */
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Check if both SPI peripherals have been disabled */
    if ((SPI1->CR1 & SPI_CR1_SPE) == 0 && (SPI2->CR1 & SPI_CR1_SPE) == 0)
    {
        return E_OK; /**< De-initialization successful for both SPI1 and SPI2 */
    }
    else
    {
        return E_NOT_OK; /**< De-initialization failed for at least one SPI channel */
    }
}

/**
 * @brief  Writes a single 8-bit data to the SPI hardware register for transmission.
 * @param  Channel: Specifies the SPI channel (e.g., SPI_CHANNEL_1 or SPI_CHANNEL_2).
 * @param  DataBufferPtr: Pointer to the data buffer containing a single 8-bit data to be transmitted.
 *                        If NULL, no data will be written, and the function returns E_NOT_OK.
 * @retval Std_ReturnType
 *         - E_OK: Data successfully written to SPI hardware.
 *         - E_NOT_OK: Write operation failed (invalid channel or NULL data buffer).
 */
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const Spi_DataBufferType *DataBufferPtr)
{
    /* Check if the data buffer pointer is NULL */
    if (DataBufferPtr == NULL)
    {
        return E_NOT_OK; /**< Return error if no data buffer is provided */
    }

    /* Determine the SPI peripheral based on the channel */
    SPI_TypeDef *SPIx;
    if (Channel == SPI_CHANNEL_1)
    {
        SPIx = SPI1;
    }
    else if (Channel == SPI_CHANNEL_2)
    {
        SPIx = SPI2;
    }
    else
    {
        return E_NOT_OK; /**< Invalid channel */
    }

    /* Wait until the transmit buffer is empty */
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

    /* Write the single 8-bit data from DataBufferPtr to the SPI Data Register */
    SPI_I2S_SendData(SPIx, *DataBufferPtr);

    /* Wait until the transmission is complete */
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET);

    return E_OK;
}

/**
 * @brief  Initiates an asynchronous transmission for the specified SPI Sequence.
 * @param  Sequence: The ID of the SPI Sequence to be transmitted.
 * @retval Std_ReturnType
 *         - E_OK: Transmission successfully started.
 *         - E_NOT_OK: Transmission failed (e.g., invalid Sequence or SPI busy).
 * @note   This function begins an asynchronous transmission without waiting
 *         for the transmission to complete. The status of the transmission
 *         should be checked later to confirm its success.
 */
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence)
{
    if (Spi_Status[0] == SPI_UNINIT && Spi_Status[1] == SPI_UNINIT)
    {
        return E_NOT_OK;
    }

    if (Sequence >= SPI_MAX_CHANNEL)
    {
        return E_NOT_OK;
    }

    Spi_SequenceStatus[Sequence] = SPI_SEQ_PENDING;

    const Spi_SequenceConfigType *SequenceConfig = &Spi_Sequences[Sequence];

    for (uint8_t jobIndex = 0; jobIndex < SequenceConfig->JobCount; jobIndex++)
    {
        Spi_JobType currentJob = SequenceConfig->Jobs[jobIndex];
        const Spi_JobConfigType *JobConfig = &Spi_Jobs[currentJob];
        Spi_ChannelType channel = JobConfig->Channel;

        Spi_JobStatus[currentJob] = SPI_JOB_PENDING;

        if (channel == SPI_CHANNEL_1)
        {
            /* Transmit data on SPI1 */ 
            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

            SPI_I2S_SendData(SPI1, *(JobConfig->DataBuffer));

            /* If transmition is successfull */ 
            if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != RESET)
            {
                Spi_JobStatus[currentJob] = SPI_JOB_OK;
            }
            /* If transmition is fail */  
            else
            {
                Spi_JobStatus[currentJob] = SPI_JOB_FAILED;
                Spi_SequenceStatus[Sequence] = SPI_SEQ_FAILED;

                return E_NOT_OK;
            }
        }
        else if (channel == SPI_CHANNEL_2)
        {
            /* Transmit data on SPI2 */ 
            while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

            SPI_I2S_SendData(SPI2, *(JobConfig->DataBuffer));

            /* If transmition is successfull */
            if (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != RESET)
            {
                Spi_JobStatus[currentJob] = SPI_JOB_OK;
            }
            /* If transmition is fail */
            else
            {
                Spi_JobStatus[currentJob] = SPI_JOB_FAILED;
                Spi_SequenceStatus[Sequence] = SPI_SEQ_FAILED;
                return E_NOT_OK;
            }
        }
        /* If channel is not available */
        else
        {
            Spi_JobStatus[currentJob] = SPI_JOB_FAILED;
            Spi_SequenceStatus[Sequence] = SPI_SEQ_FAILED;
            return E_NOT_OK;
        }
    }

    Spi_SequenceStatus[Sequence] = SPI_SEQ_OK;
    return E_OK;
}

/**
 * @brief  Reads data from the internal buffer of a specified SPI channel.
 * @param  Channel: Specifies the SPI channel to read data from.
 * @param  DataBufferPointer: Pointer to the buffer where the read data will be stored.
 *                            Must not be NULL.
 * @retval Std_ReturnType
 *         - E_OK: Data successfully read from internal buffer.
 *         - E_NOT_OK: Read operation failed (invalid channel or NULL data buffer).
 */
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType *DataBufferPointer)
{
    /* Check if the data buffer pointer is NULL */
    if (DataBufferPointer == NULL)
    {
        return E_NOT_OK; /**< Return error if no data buffer is provided */
    }

    /* Determine the SPI peripheral based on the channel */
    SPI_TypeDef *SPIx;
    if (Channel == SPI_CHANNEL_1)
    {
        SPIx = SPI1;
    }
    else if (Channel == SPI_CHANNEL_2)
    {
        SPIx = SPI2;
    }
    else
    {
        return E_NOT_OK; /**< Invalid channel */
    }

    /* Wait until data is ready to be received */
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

    /* Read the received data from the SPI Data Register */
    *DataBufferPointer = (Spi_DataBufferType)SPI_I2S_ReceiveData(SPIx);

    return E_OK;
}

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
Spi_StatusType Spi_GetStatus(void)
{
    /* Check if any channel is busy */
    for (int i = 0; i < SPI_MAX_CHANNEL; i++)
    {
        if (Spi_Status[i] == SPI_BUSY)
        {
            return SPI_BUSY; /**< Return SPI_BUSY if any channel is busy */
        }
    }

    /* Check if all channels are uninitialized */
    for (int i = 0; i < SPI_MAX_CHANNEL; i++)
    {
        if (Spi_Status[i] != SPI_UNINIT)
        {
            return SPI_IDLE; /**< Return SPI_IDLE if at least one channel is initialized */
        }
    }

    /* If all channels are uninitialized, return SPI_UNINIT */
    return SPI_UNINIT;
}

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
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job)
{
    if (Job >= SPI_MAX_JOB)
    {
        return SPI_JOB_FAILED;
    }
    return Spi_JobStatus[Job];
}

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
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence)
{
    if (Sequence >= SPI_MAX_SEQUENCE)
    {
        return SPI_SEQ_FAILED;
    }
    return Spi_SequenceStatus[Sequence];
}

/**
 * @brief  Retrieves the version information of the SPI driver.
 * @param  versioninfo: Pointer to the structure where the version information will be stored.
 *                      Must not be NULL.
 * @retval None
 */
void Spi_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
    /* Check if the versioninfo pointer is NULL */
    if (versioninfo == NULL)
    {
        return;
    }

    /* Populate the version information structure */
    versioninfo->vendorID = SPI_VENDOR_ID;
    versioninfo->moduleID = SPI_MODULE_ID;
    versioninfo->sw_major_version = SPI_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = SPI_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = SPI_SW_PATCH_VERSION;
}

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
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence)
{
    /* Check if SPI is initialized */
    if (Spi_Status[0] == SPI_UNINIT && Spi_Status[1] == SPI_UNINIT)
    {
        return E_NOT_OK;
    }

    /* Check if the Sequence ID is valid */
    if (Sequence >= SPI_MAX_CHANNEL)
    {
        return E_NOT_OK;
    }

    /* Set the sequence status to PENDING at the beginning */
    Spi_SequenceStatus[Sequence] = SPI_SEQ_PENDING;
    const Spi_SequenceConfigType *SequenceConfig = &Spi_Sequences[Sequence];

    /* Process each job in the sequence */
    for (uint8_t jobIndex = 0; jobIndex < SequenceConfig->JobCount; jobIndex++)
    {
        Spi_JobType currentJob = SequenceConfig->Jobs[jobIndex];
        const Spi_JobConfigType *JobConfig = &Spi_Jobs[currentJob];
        Spi_ChannelType channel = JobConfig->Channel;

        /* Set the job status to PENDING at the start of the job */
        Spi_JobStatus[currentJob] = SPI_JOB_PENDING;

        if (channel == SPI_CHANNEL_1)
        {
            /* Transmit data on SPI1 */
            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

            SPI_I2S_SendData(SPI1, *(JobConfig->DataBuffer));

            /* Check if transmission was successful */
            if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != RESET)
            {
                Spi_JobStatus[currentJob] = SPI_JOB_OK;
            }
            else
            {
                /* If transmission failed */
                Spi_JobStatus[currentJob] = SPI_JOB_FAILED;
                Spi_SequenceStatus[Sequence] = SPI_SEQ_FAILED;
                return E_NOT_OK;
            }
        }
        else if (channel == SPI_CHANNEL_2)
        {
            /* Transmit data on SPI2 */
            while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

            SPI_I2S_SendData(SPI2, *(JobConfig->DataBuffer));

            /* Check if transmission was successful */
            if (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != RESET)
            {
                Spi_JobStatus[currentJob] = SPI_JOB_OK;
            }
            else
            {
                /* If transmission failed */
                Spi_JobStatus[currentJob] = SPI_JOB_FAILED;
                Spi_SequenceStatus[Sequence] = SPI_SEQ_FAILED;
                return E_NOT_OK;
            }
        }
        else
        {
            /* Invalid channel */
            Spi_JobStatus[currentJob] = SPI_JOB_FAILED;
            Spi_SequenceStatus[Sequence] = SPI_SEQ_FAILED;
            return E_NOT_OK;
        }
    }

    /* If all jobs were processed successfully, set sequence status to OK */
    Spi_SequenceStatus[Sequence] = SPI_SEQ_OK;
    return E_OK;
}
