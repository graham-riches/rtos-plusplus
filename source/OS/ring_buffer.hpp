/*! \file ring_buffer.hpp
*
*  \brief ring_buffer container type that supports iterators 
*
*
*  \author Graham Riches
*/

#pragma once

/********************************** Includes *******************************************/
#include <optional>
#include <type_traits>

/************************************ Types ********************************************/
namespace detail
{
/**
 * \brief Regular forward iterator type
 * 
 * @tparam It Type of the iterator
 * @tparam MaxCapacity max size of the buffer
 * @tparam is_const flag for const iterator
 */
template <typename It, std::size_t MaxCapacity, bool is_const>
struct iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = std::conditional_t<is_const, It const, It>;
    using pointer = value_type*;
    using reference = value_type&;

    iterator(pointer ptr, pointer buffer)
        : m_ptr(ptr)
        , m_buff(buffer)
        , m_end(buffer + MaxCapacity - 1)
        , m_start(ptr) { }

    reference operator*() const {
        return *m_ptr;
    }
    pointer operator->() {
        return m_ptr;
    }

    iterator& operator++() {
        if ( m_ptr == m_end ) {
            m_ptr = m_buff;
        } else {
            m_ptr++;
        } 
        if ( m_ptr == m_start ) {
            m_ptr = nullptr;
        }
        return *this;
    }

    iterator operator++(It) {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    iterator& operator--() {
        m_ptr--;
        if ( m_ptr < m_buff ) {
            m_ptr = m_end;
        }
        if ( m_ptr == m_start ) {
            m_ptr = nullptr;
        }
        return *this;
    }

    iterator operator--(It) {
        auto temp = *this;
        --(*this);
        return temp;
    }

    bool operator!=(const iterator& other) {
        return m_ptr != other.m_ptr;
    }

  private:
    pointer m_ptr;
    pointer m_buff;
    pointer m_end;
    pointer m_start;
};

/**
 * \brief Reverse iterator type
 * 
 * @tparam It Type of the iterator
 * @tparam MaxCapacity max size of the buffer
 * @tparam is_const flag for const iterator
 */
template <typename It, std::size_t MaxCapacity, bool is_const>
struct reverse_iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = std::conditional_t<is_const, It const, It>;
    using pointer = value_type*;
    using reference = value_type&;

    reverse_iterator(pointer ptr, pointer buffer)
        : m_ptr(ptr)
        , m_buff(buffer)
        , m_end(buffer + MaxCapacity - 1)
        , m_start(ptr) { }

    reference operator*() const {
        return *m_ptr;
    }
    pointer operator->() {
        return m_ptr;
    }

    reverse_iterator& operator++() {
        m_ptr--;
        if ( m_ptr < m_buff ) {
            m_ptr = m_end;
        }
        if ( m_ptr == m_start ) {
            m_ptr = nullptr;
        }
        return *this;
    }

    reverse_iterator operator++(It) {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    reverse_iterator& operator--() {
        m_ptr++;
        if ( m_ptr >= m_end ) {
            m_ptr = m_buff;
        }
        if ( m_ptr == m_start ) {
            m_ptr = nullptr;
        }
        return *this;
    }

    reverse_iterator operator--(It) {
        auto temp = *this;
        --(*this);
        return temp;
    }

    bool operator!=(const reverse_iterator& other) {
        return m_ptr != other.m_ptr;
    }

  private:
    pointer m_ptr;
    pointer m_buff;
    pointer m_end;
    pointer m_start;
};

};  // namespace detail

/**
 * \brief template cla  ss for a STL-like re-usable ring buffer with no dynamic allocation
 * 
 * \tparam T parameter type
 */
template <typename T, std::size_t MaxCapacity>
class ring_buffer {
    static_assert((MaxCapacity > 0), "ring_buffer size must be non-zero");

  public:
    //!< Constructors

    /**
     * \brief Construct a new Ring Buffer object
     * 
     */
    explicit ring_buffer()
        : m_buffer()
        , m_head(0)
        , m_tail(0) { }

    //!< Delete copies and moves
    ring_buffer(const ring_buffer& other) = delete;
    ring_buffer(ring_buffer&& other) = delete;
    ring_buffer& operator=(const ring_buffer& other) = delete;
    ring_buffer& operator=(ring_buffer&& other) = delete;

    //!< Public Members
    /**
     * \brief Push a value to the back of the buffer
     * 
     * @tparam ValueType Type of the buffer templated to overload on lvalues and rvalues
     * \param data Universal reference to the data to put into the buffer
     */
    template <typename ValueType>
    void push_back(ValueType&& data) {
        static_assert(std::is_same_v<std::remove_reference_t<ValueType>, T>, "Invalid type for ring_buffer");
        if ( full() ) {
            increment(m_tail);
        } else {
            ++m_size;
        }
        m_buffer[m_head] = data;
        increment(m_head);
    }

    /**
     * \brief Push a value to the front of the buffer
     * 
     * @tparam ValueType Type of the buffer templated to overload on lvalues and rvalues
     * \param data Universal reference to the data to put into the buffer
     */
    template <typename ValueType>
    void push_front(ValueType&& data) {
        static_assert(std::is_same_v<std::remove_reference_t<ValueType>, T>, "Invalid type for ring_buffer");
        decrement(m_tail);
        if ( full() ) {
            increment(m_head);
        } else {
            ++m_size;
        }
        m_buffer[m_tail] = data;                
    }

    /**
     * \brief Pop from the front of the buffer
     * 
     * \return std::optional<T> Returns an optional if popping from an empty buffer
     */
    std::optional<T> pop_front() {
        if ( empty() ) {
            return {};
        }
        auto temp = m_head;
        decrement(temp);
        auto value = m_buffer[temp];
        --m_size;
        return value;                
    }

    /**
     * \brief Pop from the back of the buffer
     * 
     * \return std::optional<T> Returns an empty optional if the buffer is empty, otherwise the item
     */
    std::optional<T> pop_back() {
        if ( empty() ) {
            return {};
        }

        auto value = m_buffer[m_tail];
        increment(m_tail);        
        --m_size;
        return value;
    }

    //!< Iterators
    // clang-format off
    using forward_iterator       = detail::iterator<T, MaxCapacity, false>;
    using const_forward_iterator = detail::iterator<T, MaxCapacity, true>;
    using reverse_iterator       = detail::reverse_iterator<T, MaxCapacity, false>;
    using const_reverse_iterator = detail::reverse_iterator<T, MaxCapacity, true>;
    // clang-format on

    /**
     * \brief Returns a forward_iterator to the first item in the buffer
     * 
     * \return forward_iterator 
     */
    forward_iterator begin() {
        return forward_iterator(&m_buffer[m_tail], m_buffer);
    }

    /**
     * \brief Returns a const_forward_iterator to the first item in the buffer
     * 
     * \return const_forward_iterator 
     */
    const_forward_iterator cbegin() {
        return const_forward_iterator(&m_buffer[m_tail], m_buffer);
    }

    /**
     * \brief Returns a forward_iterator to the last item in the collection
     * 
     * \return forward_iterator 
     */
    forward_iterator end() {
        if ( full() ) {
            return forward_iterator(nullptr, m_buffer);
        }
        auto temp = m_head;
        decrement(temp);
        return forward_iterator(&m_buffer[temp], m_buffer);
    }

    /**
     * \brief Returns a const iterator to the last item in the collection
     * 
     * \return const_forward_iterator 
     */
    const_forward_iterator cend() {
        if ( full() ) {
            return const_forward_iterator(nullptr, m_buffer);
        }
        auto temp = m_head;
        decrement(temp);
        return const_forward_iterator(&m_buffer[temp], m_buffer);
    }

    //!< Reverse Iterators
    /**
     * \brief Returns an initial reverse iterator
     * 
     * \return reverse_iterator 
     */
    reverse_iterator rbegin() {
        auto temp = m_head;
        decrement(temp);
        return reverse_iterator(&m_buffer[temp], m_buffer);
    }

    /**
     * \brief Returns an initial const reverse iterator
     * 
     * \return const_reverse_iterator 
     */
    const_reverse_iterator crbegin() {
        auto temp = m_head;
        decrement(temp);
        return const_reverse_iterator(&m_buffer[temp], m_buffer);
    }

    /**
     * \brief Returns a ending reverse iterator
     * 
     * \return iterator 
     */
    reverse_iterator rend() {
        if ( full() ) {
            return reverse_iterator(nullptr, m_buffer);
        }
        return reverse_iterator(&m_buffer[m_tail], m_buffer);
    }

    /**
     * \brief Returns an ending const reverse iterator
     * 
     * \return const_reverse_iterator 
     */
    const_reverse_iterator crend() {
        if ( full() ) {
            return const_reverse_iterator(nullptr, m_buffer);
        }
        return const_reverse_iterator(&m_buffer[m_tail], m_buffer);
    }

    /**
     * \brief Returns the current size of the buffer
     * 
     * \return std::size_t 
     */
    std::size_t size() const {
        return m_size;
    }

    /**
     * \brief Checks if the buffer is empty
     * 
     * \return bool True if empty
     */
    bool empty() const {
        return size() == 0;
    }

    /**
     * \brief Checks if the buffer is full
     * 
     * \return bool True if full
     */
    bool full() const {
        return size() == MaxCapacity;
    }

    /**
     * \brief Flushes all items from the buffer
     */
    void flush() {
        while ( pop_front().has_value() ) {
        }
    }

  private:
    /**
     * \brief Helper to increment an index
     * 
     * \param index The index to increment
     */
    inline void increment(std::size_t& index) {
        index++;
        if ( index >= MaxCapacity ) {
            index = 0;
        }
    }

    /**
     * \brief Helper to decrement an index
     * 
     * \param index The index to decrement
     */
    inline void decrement(std::size_t& index) {
        if ( index == 0 ) {
            index = MaxCapacity - 1;
        } else {
            index--;
        }
    }

    T m_buffer[MaxCapacity];
    std::size_t m_head = 0;
    std::size_t m_tail = 0;
    std::size_t m_size = 0;    
};
