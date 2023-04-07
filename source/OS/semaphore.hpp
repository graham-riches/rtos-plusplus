// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

#pragma once

#include "device_port.hpp"
#include "interrupt_lock_guard.hpp"
#include "ring_buffer.hpp"
#include "scheduler.hpp"
#include "task_control_block.hpp"
#include <cstdint>
#include <type_traits>
#include <algorithm>

namespace os
{

template <typename std::ptrdiff_t LeastMaxValue>
class counting_semaphore {
  public:
    /**
     * \brief Create a new counting semaphore with an initial count
     * \param desired Desired initial resource count
     */
    constexpr explicit counting_semaphore(std::ptrdiff_t desired)
        : m_scheduler(&scheduler::get())
        , m_count(desired) { }

    // Default destruction
    ~counting_semaphore() = default;

    // Semaphore is non-copyable, and non-assignable
    counting_semaphore(const counting_semaphore&) = delete;
    counting_semaphore& operator=(counting_semaphore&) = delete;

    // Semaphore can be moved
    counting_semaphore(counting_semaphore&&) = default;
    counting_semaphore& operator=(counting_semaphore&&) = default;

    /**
     * \brief Atomically increments the internal counter by the value of update (default 1)
     * \note Any suspended threads waiting on the counter will be scheduled to wake up in the order that they were suspended
     *       (FIFO).
     * \todo When considering thread priority, think about how to pop a thread by highest priority to awake (min heap)
     * 
     * \param update Amount to increment the internal count
     */
    void release(std::ptrdiff_t update = 1) {
        os::interrupt_guard guard;
        m_count = m_count + update;
        m_count = std::clamp(m_count, static_cast<uint32_t>(0), static_cast<uint32_t>(LeastMaxValue));
        auto pending = m_suspended_threads.pop_back();
        if ( pending.has_value() ) {
            auto* tcb = pending.value();
            tcb->thread_ptr->set_status(thread::status::pending);
        }
    }

    /**
     * \brief Atomically decrements the counter by one or suspends the calling thread until it can     
     */
    void acquire() {
        DISABLE_INTERRUPTS();
        if ( m_count > 0 ) {
            //!< TODO: look into figuring this out with exclusive access instructions
            m_count--;
            ENABLE_INTERRUPTS();            
        } else {
            // Add the calling thread to the list of threads waiting on this semaphore to be woken up
            // when the resource becomes available
            auto *tcb = m_scheduler->get_active_tcb_ptr();
            m_suspended_threads.push_back(tcb);
            ENABLE_INTERRUPTS();
            m_scheduler->suspend_thread();
            DISABLE_INTERRUPTS();
            // Thread has now woken up, so decrement the counter and return
            m_count--;
            ENABLE_INTERRUPTS();            
        }
    }

    /**
     * \brief Attempt to acquire the semaphore. If the resource is not availble, fail right away
     * 
     * \return bool True if the resource was acquired
     */
    [[nodiscard]] bool try_acquire() {
        os::interrupt_guard guard;
        if ( m_count > 0 ) {
            m_count--;
            return true;
        } else {
            return false;
        }
    }

    /**
     * \brief Try to acquire the semaphore for a set period of time in milliseconds. If not acquired
     *        immediately, the calling thread is slept for up rel_time_ms
     * 
     * \param rel_time_ms Time to try to acquire for in ms
     * \return bool True if acquired
     */
    [[nodiscard]] bool try_acquire_for(uint32_t rel_time_ms) {
        DISABLE_INTERRUPTS();
        if ( m_count > 0 ) {
            m_count--;
            ENABLE_INTERRUPTS();
            return true;
        } else {
            m_suspended_threads.push_back(m_scheduler->get_active_tcb_ptr());

            // Wait in a loop for up to the total requested time while trying to acquire the resource
            auto start_tick = m_scheduler->get_elapsed_ticks();
            uint32_t elapsed_ticks = 0;
            while ( elapsed_ticks <= rel_time_ms ) {
                // Re-enable interrupts to allow a context switch
                ENABLE_INTERRUPTS();
                m_scheduler->sleep_thread(rel_time_ms - elapsed_ticks);

                // Returned from sleep here via context switch. Either by elapsed time expiring, or from another thread releasing
                // the resource
                DISABLE_INTERRUPTS();
                elapsed_ticks = m_scheduler->get_elapsed_ticks() - start_tick;
                // Check again if we can acquire the resource
                if ( try_acquire() ) {
                    ENABLE_INTERRUPTS();
                    return true;
                }
            }
            ENABLE_INTERRUPTS();
            return false;
        }
    }

    /**
     * \brief Get the maximum possible value of the semaphore
     * 
     * \return constexpr std::ptrdiff_t 
     */
    constexpr std::ptrdiff_t max() {
        return LeastMaxValue;
    }

  private:
    scheduler_impl* m_scheduler;
    ring_buffer<task_control_block*, MAX_THREAD_COUNT> m_suspended_threads;
    uint32_t m_count;
};

// Alias a binary sempahore as a specialization of counting_semaphore
using binary_semaphore = counting_semaphore<1>;


};  // namespace os