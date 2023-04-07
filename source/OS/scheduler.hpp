// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Graham Riches

#pragma once

/********************************** Includes *******************************************/
#include "scheduler_impl.hpp"
#include "thread.hpp"
#include <type_traits>
#include <utility>

namespace os
{

/**
 * \brief Singleton accessor for the scheduler
 */
class scheduler : public scheduler_impl {
  public:
    /**
     * \brief Singleton accessor for the scheduler
     * 
     * \retval scheduler& Reference to the scheduler
     */
    static scheduler& get();

    /**
     * \brief Run method for the scheduler     
     */
    static void update();

    /**
     * \brief Register a new thread with the scheduler     
     */
    static void register_new_thread(thread* thread);

    /**
     * \brief Sleep the calling thead for a number of ticks
     * 
     * \param ticks Number of ticks to sleep
     */
    static void sleep(uint32_t ticks);

    /**
     * \brief Get the active task control block
     * 
     * \retval task_control_block* Currently active task block
     */
    static task_control_block* get_active_task_control_block();

    /**
     * \brief Get the pending task control block
     * 
     * \retval task_control_block* Next task control block 
     */
    static task_control_block* get_pending_task_control_block();

    /**
     * \brief Suspends the calling thread indefinitely
     */
    static void suspend_calling_thread();

    /**
     * \brief get the elapsed system tick time
     * 
     * \retval uint32_t elapsed ticks
     */
    static uint32_t get_elapsed_ticks();

    /**
     * \brief update the system clock
     * 
     * \param ticks number of elapsed ticks since last update
     */
    static void update_system_ticks(uint32_t ticks);

    /**
     * \brief Lock the scheduler to perform atomic operations without interrupting
     */
    static void lock();

    /**
     * \brief Unlock the scheduler
     */
    static void unlock();

  private:
    /**
     * \brief Construct the os scheduler as a singleton instance
     */
    scheduler();

    bool m_locked;
};

namespace this_thread
{

/**
 * \brief Helper function to make thread sleeps more inline with std::this_thread::sleep_for
 * 
 * @tparam Duration Integral constant
 * \param duration_msec Duration of the sleep in milliseconds
 */
template <typename Duration>
static inline void sleep_for_msec(Duration&& duration_msec) {
    static_assert(std::is_convertible_v<uint32_t, Duration>, "Sleep interval must be convertible to integral constant");
    os::scheduler::sleep(static_cast<uint32_t>(std::forward<Duration>(duration_msec)));
}
}  // namespace this_thread

};  // namespace os