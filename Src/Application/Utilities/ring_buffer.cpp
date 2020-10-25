/*! \file ring_buffer.c
*
*  \brief circular buffer API.
*
*
*  \author Graham Riches
*/


/********************************** Includes *******************************************/
#include "ring_buffer.h"

/*********************************** Consts ********************************************/


/************************************ Types ********************************************/


/*********************************** Macros ********************************************/


/******************************* Global Variables **************************************/


/******************************** Local Variables **************************************/


/****************************** Functions Prototype ************************************/


/****************************** Functions Definition ***********************************/
/**
 * \brief add data to a ring buffer
 * 
 * \tparam T template type
 * \param data data to add
 */
template <typename T>
void RingBuffer<T>::put( T data )
{
   /* return if the buffer is full */
   if ( this->full )
   {
      return;
   }

   this->buffer[this->head] = data;
   this->head = ( this->head + 1 ) % this-> max_size;
   this->full = ( this->head == this->tail );
}

/**
 * \brief get data from a ring buffer
 *  
 * \retval T data value
 */
template <typename T>
T RingBuffer<T>::get( void )
{
   if ( this->is_empty() )
   {
      return T();
   }

   auto value = this->buffer[tail];
   this->tail = ( this->tail + 1 ) % this->max_size;
   this->full = false;
   return value;
}


/**
 * \brief check if a ring buffer is empty
 * \retval true if empty
 */
template <typename T>
bool RingBuffer<T>::is_empty( void )
{
   return ( !this->full && ( this->head == this->tail ) );
}

/**
 * \brief check if a ring buffer is full
 * \retval true if full
 */
template <typename T>
bool RingBuffer<T>::is_full( void )
{
   return this->full;
}