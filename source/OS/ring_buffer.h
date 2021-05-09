/*! \file ring_buffer.h
*
*  \brief ring_buffer module functions and variables declarations.
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include <memory>
#include <optional>

/*********************************** Consts ********************************************/

/************************************ Types ********************************************/
/**
 * \brief template class for a re-usable ring buffer
 * 
 * \tparam T parameter type
 */
template <typename T>
class RingBuffer {
  private:
    std::unique_ptr<T[]> buffer;
    const size_t max_size;
    size_t head = 0;
    size_t tail = 0;
    bool full = false;

  public:
    explicit RingBuffer(size_t size)
        : buffer(std::make_unique<T[]>(size))
        , max_size(size) { }

    //!< delete copies and moves and default construction
    RingBuffer() = delete;
    RingBuffer(const RingBuffer& other) = delete;
    RingBuffer(RingBuffer&& other) = delete;
    RingBuffer& operator = (const RingBuffer& other) = delete;
    RingBuffer& operator = (RingBuffer&& other) = delete;

    /**
     * \brief put data onto the ring buffer
     * 
     * \param data to put into the ring buffer
     * \return True if the put was successful
     */
    bool push(T data) {
        /* return if the buffer is full */
        if ( this->full ) {            
            return false;
        }
        this->buffer[this->head] = data;
        this->head = (this->head + 1) % this->max_size;
        this->full = (this->head == this->tail);
        return true;
    }

    /**
     * \brief get data off the ring buffer
     * 
     * \retval T 
     */
    std::optional<T> pop(void) {
        if ( this->is_empty() ) {
            return std::optional<T>();
        }

        auto value = this->buffer[tail];
        this->tail = (this->tail + 1) % this->max_size;
        this->full = false;
        return value;
    }

    /**
     * \brief check if the buffer is empty 
     * 
     * \retval true/false       
     */
    bool is_empty(void) {
        return (!this->full && (this->head == this->tail));
    }

    /**
     * \brief check if the buffer is full
     * 
     * \retval true/false
     */
    bool is_full(void) {
        return this->full;
    }

    /**
     * \brief flush data out of the ring buffer       
     */
    void flush(void) {
        while ( this->pop().has_value() ) {}        
    }
};
