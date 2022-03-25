/**
 * \file semaphore_impl.hpp
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
#include "ring_buffer.hpp"
#include "scheduler_impl.hpp"


namespace os {

/********************************** Types *******************************************/

/**
 * \brief semaphore type that is implemented with an arbitrary integral type T
 * 
 * \tparam T integral semaphore type
 */
template <typename T>
class counting_semaphore {
    public:
        static_assert(std::is_integral<T>::type, "Semaphore must be created with an integral type");
        
        /**
         * \brief Construct a new counting semaphore object
         * 
         * \param scheduler_ptr pointer to a scheduler
         * \param initial_count initial count for the resource
         * \param max_pending_thread_count max pending thread depth (for sizing the suspended threads queue)
         * \todo the size of this can be optimized to be less than the max thread count because the thread cannot
         *       suspend on itself so this can save a bit of memory in the future
         */
        counting_semaphore(scheduler_impl* scheduler_ptr, T initial_count, uint8_t max_pending_thread_count)
        : count(initial_count)
        , max_pending_threads(max_pending_thread_count)
        , pending_threads(max_pending_thread_count)
        , scheduler_ptr(scheduler_ptr) {}

        /**
         * \brief try to wait on the semaphore. This will suspend the calling thread if the resource is not available
         */
        void wait() {
            auto tcb = scheduler::get_active_task_control_block();            
            if ( count <= 0 ){                
                pending_threads.push(tcb);
                scheduler_ptr->suspend_thread();
            }
            count--;
        }

        /**
         * \brief signal the semaphore and wake up the next thread that is pending
         */
        void signal() {             
            //!< wake up the first pending thread
            if (!pending_threads.is_empty()) {
                auto maybe_task = pending_threads.pop();
                if (maybe_task.has_value()) {
                    auto tcb = maybe_task.value();
                    tcb->thread_ptr->set_status(thread::status::pending);
                }
            }
            
            //!< increment the semaphore
            count++;
        }

        ~counting_semaphore(){}


    protected:
    T count;
    const uint8_t max_pending_threads;
    ring_buffer<scheduler::TaskControlBlock> pending_threads;
    scheduler_impl* scheduler_ptr;
};

};