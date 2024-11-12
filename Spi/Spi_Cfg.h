/******************************************************************************
 *  @file    Spi_Cfg.h
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
#ifndef SPI_CFG_H
#define SPI_CFG_H

#include "Std_Types.h"
#include "Spi.h"

// #define MAX_CHANNELS_PER_JOB 2   // Each Job has 1 Channel (limited for example)
// #define MAX_JOBS_PER_SEQUENCE 2  // Sequence contains 2 Jobs
// #define SPI_SEQUENCE_MAX 1

//// Define constants for Job IDs
// #define SPI_JOB_TRANSMIT 1
// #define SPI_JOB_RECEIVE 2

uint8_t transmitDataBuffer_1[1] = {0xA5};
uint8_t transmitDataBuffer_2[1] = {0x10};

// Job Configuration Structure
typedef struct
{
    Spi_ChannelType Channel;
    uint8_t *DataBuffer;
} Spi_JobConfigType;

// Sequence Configuration Structure
typedef struct
{
    Spi_JobType Jobs[SPI_MAX_JOB]; /**< List of Jobs in the Sequence */
    uint8_t JobCount;              /**< Number of Jobs in the Sequence */
} Spi_SequenceConfigType;

// Job configuration table (array of job configurations)
Spi_JobConfigType Spi_Jobs[] = {
    {SPI_CHANNEL_1, transmitDataBuffer_1}, // Job 0 on Channel 1 with transmit buffer
    {SPI_CHANNEL_2, transmitDataBuffer_2}  // Job 1 on Channel 2 with receive buffer
};

// Sequence configuration table (array of sequence configurations)
const Spi_SequenceConfigType Spi_Sequences[SPI_MAX_SEQUENCE] = {
    {.Jobs = {0, 1}, .JobCount = 2} // Sequence 1 with 2 jobs (transmit and receive)
};

#endif /* SPI_CFG_H */
