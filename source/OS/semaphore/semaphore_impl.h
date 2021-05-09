/**
 * \file semaphore_impl.h
 * \author Graham Riches (graham.riches@live.com)
 * \brief contains a basic templated semaphore primative that is used to build OS
 *        synchronization primatives. 
 * \version 0.1
 * \date 2021-05-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include <type_traits>
#include "ring_buffer.h"
#include "scheduler_impl.h"


namespace os {

/********************************** Types *******************************************/

/**
 * \brief semaphore type that is implemented with an arbitrary integral type T
 * 
 * \tparam T integral semaphore type
 */
template <typename T>
class Sempahore {
    public:
        static_assert(std::is_integral<T>::type, "Sempahore must be created with an integral type");
        
        Sempahore(T initial_count, uint8_t max_pending_thread_count)
        : count(initial_count)
        , max_pending_threads(max_pending_thread_count) {}

        /**
         * \brief 
         */
        void wait() {

        }

        /**
         * \brief 
         */
        void put() {
            
        }


        ~Sempahore(){}


    protected:
    T count;
    const uint8_t max_pending_threads;
    RingBuffer<scheduler_impl::TaskControlBlock> pending_threads;
};

};