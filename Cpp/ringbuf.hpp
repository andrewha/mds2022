/**
 * @file ringbuf.hpp
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief 
 * @version 0.1
 * @date 2024-02-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef RINGBUF_HPP
#define RINGBUF_HPP

//#include <stdexcept>

/**
 * @brief RingBuffer Class Template.
 * 
 * @tparam T Type
 */
template<typename T>
class RingBuffer {

    // TODO: provide a definition of the class template here.
    
    public:

        /**
         * @brief Initialized constructor of a new Ring Buffer object.
         * 
         * @param sz Size of the buffer in elements
         */
        RingBuffer(size_t sz)
        {
            if (!sz)
                throw std::invalid_argument("Failed to allocate buffer: size cannot be zero.");
                
            _size = sz;
            _buf = new T[_size]; // Create a C-style array of type `T` and size `_size` on the heap and return the pointer to it
            _pHead = _buf; // For a new buffer, set to the beginning
            _pTail = _buf; // For a new buffer, set to the beginning
            _empty = true; // New buffer is always empty; don't confuse with copied buffers -- this constructor is not for copies
        }

        /**
         * @brief Destructor of the Ring Buffer object.
         * 
         */
        ~RingBuffer()
        {
            delete [] _buf;
        }

    /**
     * @brief Get the size of the buffer in elements.
     * 
     * @return size_t
     */
    size_t getSize() const
    { 
        return _size;
    }

    /**
     * @brief Get the number of items added to the buffer.
     * 
     * @return size_t
     */
    size_t getCount() const
    { 
        // Buffer is empty
        if (_pTail == _pHead && isEmpty())
            return 0;
        
        // Buffer is full
        if (_pTail == _pHead && !isEmpty())
            return getSize();

        // Correct pointers
        if (_pTail > _pHead)
            return _pTail - _pHead;

        // Inverted pointers
        // _pTail < _pHead
        return _size - (_pHead - _pTail);
    }

    /**
     * @brief Get the number of elements that can be added to the buffer before it is full.
     * 
     * @return size_t
     */
    size_t getFree() const
    { 
        return getSize() - getCount();
    }

    /**
     * @brief Checks if there are no elements in the buffer.
     * 
     * @return bool
     */
    bool isEmpty() const
    { 
        return _empty;
    }

    /**
     * @brief Checks if the buffer is full.
     * 
     * @return bool
     */
    bool isFull() const
    { 
        return getCount() == getSize();
    }

    /**
     * @brief Insert (push) an element.
     * 
     * @param value 
     */
    void push(const T& value)
    {
        // Buffer is full -- cannot write anymore
        if (isFull())
            throw std::out_of_range("Failed to insert element: buffer is full.");
        
        // Loop the pointer, if needed
        if (_pTail == _buf + _size)
            _pTail = _buf;

        // Buffer is not full -- can write
        *_pTail = value;
    
        // Move the write pointer
        _pTail++;
        
        // Any push makes the buffer non-empty
        _empty = false;   
    }

    /**
     * @brief Get the oldest element.
     * 
     * @return T& 
     */
    T& front()
    {
        return *_pHead;
    }
    
    /**
     * @brief Get the oldest element (const overload).
     * 
     * @return const T& 
     */
    const T& front() const
    {
        return *_pHead;
    }

    /**
     * @brief Get the newest element.
     * 
     * @return T& 
     */
    T& back()
    {
        if (_pTail == _buf)
            return *_pTail;
        else
            return *(_pTail - 1);
    }

    /**
     * @brief Get the newest element (const overload).
     * 
     * @return const T& 
     */
    const T& back() const
    {
        if (_pTail == _buf)
            return *_pTail;
        else
            return *(_pTail - 1);
    }

    /**
     * @brief Pop the oldest element from the buffer.
     * 
     * @return T& 
     */
    T& pop()
    {                
        // Buffer is empty -- cannot read anymore
        if (isEmpty())
            throw std::out_of_range("Failed to read element: buffer is empty.");

        // Loop the pointer, if needed
        if (_pHead == _buf + _size)
            _pHead = _buf;

        // Buffer is not empty -- can read
        T& value = *_pHead;

        // Move the read pointer
        _pHead++;

        // Check if all elements have been read
        if (_pHead == _pTail)
            _empty = true;
        
        return value;
    }

    private:
        T* _buf; // Pointer to the first element of the buffer (beginning)
        size_t _size; // Size of the buffer in elements
        T* _pTail; // Write pointer to the position where the next element will be added at the next call to the `push()` method
        T* _pHead; // Read pointer to the oldest item in the queue which will be retrieved next when the `pop()` method is called
        bool _empty; // Empty buffer flag
};

#endif // RINGBUF_HPP
