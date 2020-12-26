/*! \file queue.h
*
*  \brief linked list templated queue
*
*
*  \author Graham Riches
*/

#ifndef __QUEUE_H__
#define __QUEUE_H__

/********************************** Includes *******************************************/
#include <memory>

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief template struct for a queue entry
 * 
 * \tparam T 
 */
template <typename T>
struct QueueEntry {
    T data;
    QueueEntry* next;
    QueueEntry* previous;
};

/**
 * \brief template class for a queue
 * 
 * \tparam T parameter type
 */
template <typename T>
class Queue {
  private:
    std::unique_ptr<QueueEntry<T>[]> buffer;  //!< container to store all possible entries
    QueueEntry<T>* head;                      //!< pointer to first item in the queue
    QueueEntry<T>* tail;                      //!< pointer to the tail
    const size_t max_size;
    size_t items;
    bool full = false;

  public:
    explicit Queue(size_t size)
        : buffer(std::make_unique<T[]>(size)) {
        this->items = 0;
        this->head = this->buffer[0];
        this->tail = this->head;
    }

    /**
       * \brief add onto the queue
       * 
       * \param data the data to put on the queue
       */
    void insert(T data) {
        QueueEntry<T>* previous = this->tail;
        this->tail->data = data;
        this->tail = this->tail->next;
        this->tail->previous = previous;
    }

    /**
    * \brief check if there is data on the queue
    * 
    * \retval true 
    * \retval false 
    */
    bool is_empty(void) {
        bool is_empty = (this->head == this->tail) && !this->full;
        return is_empty;
    }
};

/*********************************** Macros ********************************************/

/******************************* Global Variables **************************************/

/****************************** Functions Prototype ************************************/

#endif /* __QUEUE_H__ */