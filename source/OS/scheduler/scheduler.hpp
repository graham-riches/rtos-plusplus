/**
 * \file scheduler.hpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief singleton instance of the scheduler_impl class that handles all OS task scheduling
 * \version 0.1
 * \date 2021-05-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include "scheduler_impl.hpp"
#include "thread.hpp"

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

};  // namespace os