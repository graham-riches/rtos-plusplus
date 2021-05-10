/**
 * \file semaphore_tests.cpp
 * \author Graham Riches (graham.riches@live.com)
 * \brief unit tests for the OS semaphore mechanism
 * \version 0.1
 * \date 2021-05-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/********************************** Includes *******************************************/
#include "gtest/gtest.h"
#include "thread_impl.h"
#include "system_clock_impl.h"
#include "scheduler_impl.h"
#include "semaphore_impl.h"
#include "common.h"
#include <iostream>
#include <memory>


/*********************************** Consts ********************************************/
constexpr uint16_t thread_stack_size = 512;
constexpr uint8_t thread_count = 3;

/************************************ Local Variables ********************************************/
static bool pending_irq;

/************************************ Local Functions ********************************************/
/**
 * \brief test setting a fake pending IRQ in a c-style callback so that the API to the PendSV handler
 *        is somewhat tested
*/
static void set_pending_irq(){
    pending_irq = true;
}

/**
 * \brief function to check if an interrupt is already pending
 * \return returns true if a request is already pending
*/
static bool is_pending_irq(){
    return pending_irq;
}

/************************************ Test Fixtures ********************************************/
/**
* \brief test fixture class for testing semaphore mechanism
*/
class semaphore_tests : public ::testing::Test {

using counting_semaphore = os::counting_semaphore<uint8_t>;

protected:
    static void thread_task(void *arguments){ PARAMETER_NOT_USED(arguments); };    

    void SetUp(void) override {
        //!< setup the scheduler and system clock
        internal_thread = create_thread(reinterpret_cast<os::thread::task_pointer>(&thread_task), nullptr, 0xFFFF, internal_stack, thread_stack_size);
        scheduler = std::make_unique<os::scheduler_impl>(&clock, thread_count, set_pending_irq, is_pending_irq);
        scheduler->set_internal_task(internal_thread.get());
        pending_irq = false;
        clock.start();

        //!< register two threads with the scheduler

        //!< TODO: you are here
        /*
            - create simple threading object that takes a semaphore pointer, and has an internal thread + stack etc
            - add functions to acquire and release the semaphore
            - create those objects here and use in tests.
        */


        internal_thread = create_thread(reinterpret_cast<os::thread::task_pointer>(&thread_task), nullptr, 1, thread_one_stack, thread_stack_size);
        internal_thread = create_thread(reinterpret_cast<os::thread::task_pointer>(&thread_task), nullptr, 2, thread_two_stack, thread_stack_size);

        //!< setup the semaphore
        semaphore = std::make_unique<counting_semaphore>(scheduler.get(), 1, 3);
    }

public:
    uint32_t internal_stack[thread_stack_size] = {0};
    uint32_t thread_one_stack[thread_stack_size] = {0};
    uint32_t thead_two_stack[thread_stack_size] = {0};
    std::unique_ptr<os::thread> internal_thread;
    std::unique_ptr<os::thread> task_thread_one;
    std::unique_ptr<os::thread> task_thread_two;
    std::unique_ptr<os::scheduler_impl> scheduler;
    std::unique_ptr<counting_semaphore> semaphore;
    os::system_clock_impl clock;
        
    std::unique_ptr<os::thread> create_thread(os::thread::task_pointer task_ptr, void *args, uint32_t thread_id, uint32_t *stack_ptr, uint32_t stack_size) {
        return std::make_unique<os::thread>(task_ptr, args, thread_id, stack_ptr, stack_size);
    }
};

/************************************ Unit Tests ********************************************/

TEST_F(semaphore_tests, test_wait_on_semaphore_with_resource_available_does_not_suspend) {
    
}