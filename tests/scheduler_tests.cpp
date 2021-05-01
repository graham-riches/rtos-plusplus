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
#include "system_clock.h"
#include "scheduler.h"
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
* \brief test fixture class for testing the scheduler and thread registry components of the OS
*/
class SchedulerTests : public ::testing::Test {
protected:
    static void thread_task(void *arguments){};    

    void SetUp(void) override {
        internal_thread = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 0xFFFF, internal_stack, thread_stack_size);
        scheduler = std::make_unique<OS::Scheduler>(clock, thread_count, set_pending_irq, is_pending_irq);
        scheduler->set_internal_task(internal_thread.get());
        pending_irq = false;        
    }

public:
    uint32_t internal_stack[thread_stack_size] = {0};
    std::unique_ptr<OS::Thread> internal_thread;
    std::unique_ptr<OS::Scheduler> scheduler;
    OS::SystemClock clock;
        
    std::unique_ptr<OS::Thread> create_thread(OS::task_ptr_t task_ptr, void *args, uint8_t thread_id, uint32_t *stack_ptr, uint32_t stack_size) {
        return std::make_unique<OS::Thread>(task_ptr, args, thread_id, stack_ptr, stack_size);
    }
};


/**
 * \brief extentension of the main scheduler test class with some pre-registered threads for some more
 *        complex tests to reduce some boilerplate.
*/
class SchedulerTestsWithPreRegisteredThreads : public SchedulerTests {
protected:
    void SetUp(void) override {
        scheduler = std::make_unique<OS::Scheduler>(clock, thread_count, set_pending_irq, is_pending_irq);
        internal_thread = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 0xFFFF, internal_stack, thread_stack_size);
        thread_one = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 1, stack_one, thread_stack_size);
        thread_two = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 2, stack_two, thread_stack_size);   
        scheduler->set_internal_task(internal_thread.get());
        scheduler->register_thread(thread_one.get());
        scheduler->register_thread(thread_two.get());        
        pending_irq = false;
    }

public:
    uint32_t stack_one[thread_stack_size] = {0};
    uint32_t stack_two[thread_stack_size] = {0};    
    std::unique_ptr<OS::Thread> thread_one;
    std::unique_ptr<OS::Thread> thread_two;
};


/************************************ Tests ********************************************/
TEST_F(SchedulerTests, test_initialization) {
    ASSERT_EQ(0, scheduler->get_thread_count());
    OS::Scheduler::TaskControlBlock* tcb = scheduler->get_active_tcb_ptr();
    ASSERT_EQ(nullptr, tcb->active_stack_pointer);
    ASSERT_EQ(nullptr, tcb->next);
    ASSERT_EQ(nullptr, tcb->thread);
}

TEST_F(SchedulerTests, test_registering_thread) {
    uint32_t stack[thread_stack_size] = {0};    
    std::unique_ptr<OS::Thread> thread = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 1, stack, thread_stack_size);
    ASSERT_TRUE(scheduler->register_thread(thread.get()));
    ASSERT_EQ(1, scheduler->get_thread_count());
    OS::Scheduler::TaskControlBlock* tcb = scheduler->get_active_tcb_ptr();
    ASSERT_EQ(tcb->thread, thread.get());
    ASSERT_EQ(nullptr, tcb->next);
}

TEST_F(SchedulerTests, test_registering_multiple_threads){    
    uint32_t stack[thread_stack_size] = {0};    
    std::unique_ptr<OS::Thread> thread_one = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 1, stack, thread_stack_size);
    std::unique_ptr<OS::Thread> thread_two = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 2, stack, thread_stack_size);
    scheduler->register_thread(thread_one.get());
    ASSERT_TRUE(scheduler->register_thread(thread_two.get()));
    ASSERT_EQ(2, scheduler->get_thread_count());
    OS::Scheduler::TaskControlBlock* tcb = scheduler->get_active_tcb_ptr();
    OS::Scheduler::TaskControlBlock* tcb_next = tcb->next;
    ASSERT_EQ(tcb_next->thread, thread_two.get());
    ASSERT_EQ(tcb, tcb_next->next);  //!< linked list is circular
    ASSERT_EQ(2, tcb_next->thread->get_id());
}

TEST_F(SchedulerTests, test_filling_tcb_buffer) {
    /* create one thread and just register it multiple times */
    uint32_t stack[thread_stack_size] = {0};    
    std::unique_ptr<OS::Thread> thread = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 1, stack, thread_stack_size);
    scheduler->register_thread(thread.get());
    scheduler->register_thread(thread.get());
    ASSERT_TRUE(scheduler->register_thread(thread.get()));
    ASSERT_EQ(3, scheduler->get_thread_count());
    OS::Scheduler::TaskControlBlock* tcb = scheduler->get_active_tcb_ptr();
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

TEST_F(SchedulerTests, test_thread_sleep_adds_to_tcb_ticks) {
    uint32_t stack[thread_stack_size] = {0};    
    std::unique_ptr<OS::Thread> thread = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 1, stack, thread_stack_size);
    scheduler->register_thread(thread.get());
    scheduler->sleep_thread(100);
    auto maybe_tcb = scheduler->get_task_by_id(1);
    ASSERT_TRUE(maybe_tcb);
    auto tcb = maybe_tcb.value();
    ASSERT_EQ(100, tcb->suspended_ticks_remaining);
}

TEST_F(SchedulerTests, test_sleeping_all_threads_sets_internal_thread_active) {
    uint32_t stack[thread_stack_size] = {0};    
    std::unique_ptr<OS::Thread> thread = create_thread(reinterpret_cast<OS::task_ptr_t>(&thread_task), nullptr, 1, stack, thread_stack_size);
    scheduler->register_thread(thread.get());
    scheduler->sleep_thread(1);
    auto tcb = scheduler->get_active_tcb_ptr();
    ASSERT_EQ(internal_thread.get(), tcb->thread);
}

TEST_F(SchedulerTestsWithPreRegisteredThreads, test_update_from_clock_triggers_context_switch) {    
    thread_two->set_status(OS::Thread::Status::suspended);
    clock.update(1);
    scheduler->run();
    ASSERT_TRUE(pending_irq);
    ASSERT_EQ(OS::Thread::Status::pending, thread_two->get_status());
}

TEST_F(SchedulerTestsWithPreRegisteredThreads, test_thread_sleep_wakes_up) {
    scheduler->sleep_thread(1);
    clock.update(1);
    scheduler->run();
    ASSERT_TRUE(pending_irq);
    ASSERT_EQ(OS::Thread::Status::pending, thread_one->get_status());
}

TEST_F(SchedulerTestsWithPreRegisteredThreads, test_multiple_threads_asleep_wakeup_at_the_same_time) {
    scheduler->sleep_thread(1);
    /* TODO: task pointer really needs to not be accessible to prevent shenanigans like the following */
    auto tcb = scheduler->get_active_tcb_ptr(); 
    tcb = tcb->next;
    tcb->suspended_ticks_remaining = 1;
    tcb->thread->set_status(OS::Thread::Status::suspended);

    clock.update(1);
    scheduler->run();
    ASSERT_TRUE(pending_irq);
    ASSERT_EQ(OS::Thread::Status::pending, thread_one->get_status());

    //!< reset the pending flag, and run the scheduler again. The second thread should now trigger
    pending_irq = false;
    clock.update(1);
    scheduler->run();
    ASSERT_TRUE(pending_irq);
    ASSERT_EQ(OS::Thread::Status::pending, thread_two->get_status());
}

TEST_F(SchedulerTestsWithPreRegisteredThreads, test_scheduler_doesnt_clobber_pending_request) {
    /* sleep thread one with one tick remaining */
    scheduler->sleep_thread(1);    

    /* manually sleep thread two as well */
    auto tcb = scheduler->get_active_tcb_ptr(); 
    tcb = tcb->next;
    tcb->suspended_ticks_remaining = 1;
    tcb->thread->set_status(OS::Thread::Status::suspended);

    /* run an update, which should pick up the first thread */
    clock.update(1);
    scheduler->run();    
    ASSERT_TRUE(pending_irq);
    ASSERT_EQ(OS::Thread::Status::pending, thread_one->get_status());
    ASSERT_EQ(OS::Thread::Status::active, thread_two->get_status());

    //!< reset the pending flag, and run the scheduler again. The second thread should now trigger
    pending_irq = false;
    clock.update(1);
    scheduler->run();
    ASSERT_TRUE(pending_irq);
    ASSERT_EQ(OS::Thread::Status::pending, thread_two->get_status());
}

TEST_F(SchedulerTestsWithPreRegisteredThreads, test_handle_clock_rollover_with_suspended_thread) {
    clock.update(0xFFFFFFFF); //!< set the clock to rollover
    scheduler->run();
    scheduler->sleep_thread(1);
    clock.update(1);
    scheduler->run();
    ASSERT_TRUE(pending_irq);
    ASSERT_EQ(OS::Thread::Status::pending, thread_one->get_status());
}
