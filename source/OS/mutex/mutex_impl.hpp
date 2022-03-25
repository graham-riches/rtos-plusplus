/**
 * \file mutex_impl.hpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief internal OS implementation of a mutex
 * \version 0.1
 * \date 2021-05-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "ring_buffer.hpp"
#include "scheduler_impl.hpp"


namespace os
{


class mutex_impl {
    public:
    Mutex(Scheduler& scheduler)
    : locked(false)
    , waiting_on_lock(scheduler.g)
    {}


    void lock();

    void try_lock()

    void unlock();

    //!< disable moves and copies
    Mutex(const Mutex& other) = delete;
    Mutex(Mutex&& other) = delete;
    Mutex& operator = (const Mutex& other) = delete;
    Mutex& operator = (Mutex&& other) = delete;
    
    private:

    bool locked;
    ring_buffer<Scheduler::TaskControlBlock> waiting_on_lock;
};


};
