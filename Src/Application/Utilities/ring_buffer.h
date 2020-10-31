/*! \file ring_buffer.h
*
*  \brief ring_buffer module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

/********************************** Includes *******************************************/
#include "common.h"
#include <memory>

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief template class for a re-usable ring buffer
 * 
 * \tparam T parameter type
 */
template <typename T>
class RingBuffer
{
   private:
   std::unique_ptr<T[]> buffer;
   size_t max_size;
   size_t head = 0;
   size_t tail = 0;
   bool full = false;
   

   public:
      RingBuffer( size_t size )
         : buffer( std::make_unique<T[]>( size ) )
         , max_size( size )
      { }

      /**
       * \brief initialize the container
       * 
       * \param size size of the container
       */
      void initialize( size_t size )
      {
         this->max_size = size;
         this->buffer = std::make_unique<T[]>( size );
      }

      /**
       * \brief put data onto the ring buffer
       * 
       * \param data to put into the ring buffer
       */
      void put( T data )
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
       * \brief get data off the ring buffer
       * 
       * \retval T 
       */
      T get( void )
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
       * \brief check if the buffer is empty 
       * 
       * \retval true/false       
       */
      bool is_empty( void )
      { 
         return ( !this->full && ( this->head == this->tail ) ); 
      }

      /**
       * \brief check if the buffer is full
       * 
       * \retval true/false
       */
      bool is_full( void )
      { 
         return this->full;
      }
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/****************************** Functions Prototype ************************************/

#endif /* __RING_BUFFER_H__ */
