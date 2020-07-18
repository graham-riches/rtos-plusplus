/*! \file utilities.c
*
*  \brief contains useful utilities that are shared by different submodules throughout the project.
*
*
*  \author Graham Riches
*  \copyright Copyright (c) ATTAbotics Inc. 2020
*/


/********************************** Includes *******************************************/
#include "utilities.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
 * \brief add data to a ring buffer
 * \param buffer pointer to the buffer handler
 * \param data pointer to the data buffer to add
 * \param size how much data to add
 * \retval UTILITIES_ringBufferStatus_t enum type
 */
UTILITIES_ringBufferStatus_t UTILITIES_ringBufferAdd( UTILITIES_ringBuffer_t *buffer, uint8_t *data, uint16_t size )
{
    UTILITIES_ringBufferStatus_t status = UTILITIES_RINGBUFFER_OK;

    if ( (buffer == NULL) || (data == NULL) || (size == 0) )
    {
        return UTILITIES_RINGBUFFER_NULL;
    }

    /* check if there is space in the buffer */
    if ( UTILITIES_ringBufferGetFreeSpace( buffer ) < size )
    {
        return UTILITIES_RINGBUFFER_FULL;
    }

    /* add the data */
    if ( (buffer->head + size) <= buffer->size )
    {
        /* simply add to the head if there is room */
        memcpy( &buffer->data[buffer->head], data, size );
        buffer->head += size;
    }
    else
    {
        /* handle wrap around */
        memcpy( &buffer->data[buffer->head], data, buffer->size - buffer->head );
        memcpy( &buffer->data[0], &data[buffer->size - buffer->head], size - (buffer->size - buffer->head) );
        buffer->head = size - (buffer->size - buffer->head);
    }

    return status;
}


/**
 * \brief get data from a ring buffer
 * \param buffer pointer to the ring buffer handler
 * \param data pointer to the receivers data buffer
 * \param size how much data to get
 * \retval UTILITIES_ringBufferStatus_t enum type
 */
UTILITIES_ringBufferStatus_t UTILITIES_ringBufferGet( UTILITIES_ringBuffer_t *buffer, uint8_t *data, uint16_t size )
{
    UTILITIES_ringBufferStatus_t status = UTILITIES_RINGBUFFER_OK;
    uint16_t dataAvailable = 0;

    if ( (buffer == NULL) || (data == NULL) || (size == 0) )
    {
        return UTILITIES_RINGBUFFER_NULL;
    }

    /* check that we aren't requesting more data than the buffer has available */
    dataAvailable = buffer->size - UTILITIES_ringBufferGetFreeSpace( buffer );

    if ( size > dataAvailable )
    {
        return UTILITIES_RINGBUFFER_EMPTY;
    }

    /* copy the data into the receivers buffer */
    if ( (buffer->head > buffer->tail) || (size <= (buffer->size - buffer->tail)) )
    {
        memcpy( data, &buffer->data[buffer->tail], size );
        buffer->tail += size;
    }
    else
    {
        /* handle wrap around */
        memcpy( data, &buffer->data[buffer->tail], buffer->size - buffer->tail );
        memcpy( &data[buffer->size - buffer->tail], &buffer->data[0], size - (buffer->size - buffer->tail));
        buffer->tail = size - (buffer->size - buffer->tail);
    }
    return status;
}


/**
 * \brief get how much free space there is in a buffer
 * \param buffer pointer to the ring buffer handler
 * \retval number of bytes available
 */
uint16_t UTILITIES_ringBufferGetFreeSpace( UTILITIES_ringBuffer_t *buffer )
{
    uint16_t freeSize = 0;
    if ( buffer == NULL )
    {
        return freeSize;
    }

    freeSize = buffer->size - UTILITIES_ringBufferGetUsedSpace( buffer ) -1 ;
    return freeSize;
}


/**
 * \brief get how much space is used in a buffer
 * \param buffer the pointer to the ring buffer
 * \retval number of bytes used
 */
uint16_t UTILITIES_ringBufferGetUsedSpace( UTILITIES_ringBuffer_t *buffer )
{
    uint16_t usedSize = 0;
    if ( buffer == NULL )
    {
        return 0;
    }

    if ( buffer->head > buffer->tail )
    {
        usedSize = buffer->head - buffer->tail;
    }
    else
    {
        /* handle wrap around */
        usedSize = buffer->size - buffer->tail  + buffer->head;
    }
    return usedSize;
}
