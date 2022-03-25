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
#include "thread_impl.hpp"


namespace os
{

/**
 * \brief singleton accessor for the scheduler
 */
class scheduler : public scheduler_impl {
  public:
    /**
     * \brief singleton accessor for the scheduler
     * 
     * \retval scheduler& reference to the scheduler
     */
    static scheduler& get();

    /**
     * \brief run method for the scheduler     
     */
    static void update();

    /**
     * \brief register a new thread with the scheduler     
     */
    static void register_new_thread(thread* thread);

    /**
     * \brief sleep the calling thead for a number of ticks
     * 
     * \param ticks 
     */
    static void sleep(uint32_t ticks);

    /**
     * \brief Get the active task control block
     * 
     * \retval TaskControlBlock* currently active task block
     */
    static TaskControlBlock* get_active_task_control_block();

    /**
     * \brief Get the pending task control block
     * 
     * \retval TaskControlBlock* next task control block 
     */
    static TaskControlBlock* get_pending_task_control_block();

    /**
     * \brief lock the scheduler to perform atomic operations without interrupting
     */
    static void lock();

    /**
     * \brief unlock the scheduler
     */
    static void unlock();


  private:
    /**
     * \brief Construct the os scheduler as a singleton instance
     */
    scheduler();

    bool locked;
};

};  // namespace os