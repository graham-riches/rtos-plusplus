/*! \file events_tests.cpp
*
*  \brief Unit tests for the event flags group.
*
*
*  \author Graham Riches
*/

/********************************** Includes *******************************************/
#include "gtest\gtest.h"
#include "threading.h"
#include "scheduler.h"
#include <iostream>
#include <memory>


/*********************************** Consts ********************************************/
constexpr uint16_t thread_stack_size = 512;
constexpr uint8_t thread_count = 3;

/************************************ Test Fixtures ********************************************/
/**
* \brief test fixture class for testing the scheduler and thread registry components of the OS
*/
class SchedulerTests : public ::testing::Test {
protected:
    static void thread_task(void *arguments){};

    void SetUp(void) override {        
        scheduler = std::make_unique<OS::Scheduler>(thread_count);    
    }

public:
    std::unique_ptr<OS::Scheduler> scheduler;
        
    std::unique_ptr<OS::Thread> create_thread(OS::task_ptr_t task_ptr, void *args, uint8_t thread_id, uint32_t *stack_ptr, uint32_t stack_size) {
        return std::make_unique<OS::Thread>(task_ptr, args, 1, stack_ptr, stack_size);
    }
};

/************************************ Tests ********************************************/
TEST_F(SchedulerTests, test_initialization) {
    ASSERT_EQ(0, scheduler->get_thread_count());
    OS::TaskControlBlock* tcb = scheduler->get_active_tcb_ptr();
    ASSERT_EQ(nullptr, tcb->active_stack_pointer);
    ASSERT_EQ(nullptr, tcb->next);
    ASSERT_EQ(nullptr, tcb->thread);
}

TEST_F(SchedulerTests, test_registering_thread) {
    uint32_t stack[thread_stack_size] = {0};    
    std::unique_ptr<OS::Thread> thread = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 1, stack, thread_stack_size);
    ASSERT_TRUE(scheduler->register_thread(thread.get()));
    ASSERT_EQ(1, scheduler->get_thread_count());
    OS::TaskControlBlock* tcb = scheduler->get_active_tcb_ptr();
    ASSERT_EQ(tcb->thread, thread.get());
    ASSERT_EQ(nullptr, tcb->next);
}

TEST_F(SchedulerTests, test_registering_multiple_threads){
    /* create one thread and just register it multiple times */
    uint32_t stack[thread_stack_size] = {0};    
    std::unique_ptr<OS::Thread> thread = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 1, stack, thread_stack_size);
    scheduler->register_thread(thread.get());
    ASSERT_TRUE(scheduler->register_thread(thread.get()));
    ASSERT_EQ(2, scheduler->get_thread_count());
    OS::TaskControlBlock* tcb = scheduler->get_active_tcb_ptr();
    OS::TaskControlBlock* tcb_next = tcb->next;
    ASSERT_EQ(tcb_next->thread, thread.get());
    ASSERT_EQ(tcb, tcb_next->next);  //!< linked list is circular
}

TEST_F(SchedulerTests, test_filling_tcb_buffer) {
    /* create one thread and just register it multiple times */
    uint32_t stack[thread_stack_size] = {0};    
    std::unique_ptr<OS::Thread> thread = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 1, stack, thread_stack_size);
    scheduler->register_thread(thread.get());
    scheduler->register_thread(thread.get());
    ASSERT_TRUE(scheduler->register_thread(thread.get()));
    ASSERT_EQ(3, scheduler->get_thread_count());
    OS::TaskControlBlock* tcb = scheduler->get_active_tcb_ptr();
    ASSERT_TRUE(tcb->next->next != tcb);
}

TEST_F(SchedulerTests, test_overfilling_tcb_buffer_fails) {
    /* create one thread and just register it multiple times */
    uint32_t stack[thread_stack_size] = {0};    
    std::unique_ptr<OS::Thread> thread = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 1, stack, thread_stack_size);
    scheduler->register_thread(thread.get());
    scheduler->register_thread(thread.get());
    scheduler->register_thread(thread.get());
    ASSERT_FALSE(scheduler->register_thread(thread.get()));
}