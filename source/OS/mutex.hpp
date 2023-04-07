// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

#pragma once

#include "device_port.hpp"
#include "interrupt_lock_guard.hpp"
#include "ring_buffer.hpp"
#include "scheduler.hpp"
#include "scheduler_impl.hpp"
#include "task_control_block.hpp"
#include <algorithm>
#include <cstdint>
#include <type_traits>

namespace os
{

class mutex {
  public:
    /**
     * \brief Create a new mutex     
     * \todo: constexpr?
     */
    mutex()
        : m_scheduler(&scheduler::get())
        , m_locked(false) { }

    // Mutex is not copyable
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    // Mutex can be moved
    mutex(mutex&&) = default;
    mutex& operator=(mutex&&) = default;

    // Destroys the mutex, undefined behavior if any thread still owns the lock
    ~mutex() = default;

    /**
     * \brief Attempt to acquire the lock
     */
    void lock() {
        DISABLE_INTERRUPTS();
        if ( !m_locked ) {
            m_locked = true;
            ENABLE_INTERRUPTS();            
        } else {
            // Suspend the calling thread on the mutex
            m_suspended_threads.push_back(m_scheduler->get_active_tcb_ptr());
            ENABLE_INTERRUPTS();
            m_scheduler->suspend_thread();
            // Thread has now worken up and can now take the lock
            DISABLE_INTERRUPTS();
            m_locked = true;
            ENABLE_INTERRUPTS();            
        }
    }

    /**
     * \brief Try to lock the mutex and immediately fail if it is already locked
     * 
     * \return bool True if successfully locked
     */
    bool try_lock() {
        os::interrupt_guard guard;
        if ( !m_locked ) {
            m_locked = true;
            return true;
        }
        return false;
    }

    /**
     * \brief Unlock the mutex and wake up any threads pending on the lock     
     */
    void unlock() {
        os::interrupt_guard guard;
        m_locked = false;        
        if ( auto pending = m_suspended_threads.pop_back() ) {
            auto* tcb = pending.value();
            tcb->thread_ptr->set_status(thread::status::pending);
        }
    }

  private:
    scheduler_impl* m_scheduler;
    bool m_locked;
    ring_buffer<task_control_block*, MAX_THREAD_COUNT> m_suspended_threads;
};

};  // namespace os
