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
                throw std::invalid_argument("\033[91mFailed to allocate buffer: size cannot be zero.\033[0m");
                
            _size = sz;
            _buf = new T[_size]; // Create a C-style array of type `T` and size `_size` on the heap and return the pointer to it
            _pHead = _buf; // For a new buffer set to the beginning
            _pTail = _buf; // For a new buffer set to the beginning
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
                throw std::out_of_range("\033[91mFailed to insert element: buffer is full.\033[0m");
            
            // Loop the pointer, if needed
            if (_pTail == _buf + _size)
                _pTail = _buf;

            // Buffer is not full -- can write
            *_pTail = value;
        
            // Move the write pointer
            _pTail++;
            
            // Any push makes the buffer non-empty
            if (isEmpty())
                _empty = false;   
        }

        /**
         * @brief Get the oldest element.
         * 
         * @return T& 
         */
        T& front()
        {
            // Buffer is empty -- no front element
            if (isEmpty())
                throw std::out_of_range("\033[91mNo front element: buffer is empty.\033[0m");
                
            // Loop the pointer, if needed
            if (_pHead == _buf + _size)
                _pHead = _buf;
            
            return *_pHead;
        }
        
        /**
         * @brief Get the oldest element (const overload).
         * 
         * @return const T& 
         */
        const T& front() const
        {
            // Buffer is empty -- no front element
            if (isEmpty())
                throw std::out_of_range("\033[91mNo front element: buffer is empty.\033[0m");

            // Loop the pointer, if needed
            if (_pHead == _buf + _size)
                _pHead = _buf;
            
            return *_pHead;
        }

        /**
         * @brief Get the newest element.
         * 
         * @return T& 
         */
        T& back()
        {
            // Buffer is empty -- no back element
            if (isEmpty())
                throw std::out_of_range("\033[91mNo back element: buffer is empty.\033[0m");
            
            // Loop the pointer, if needed
            if (_pTail == _buf)
                return *(_buf + _size - 1);
            
            return *(_pTail - 1);
        }

        /**
         * @brief Get the newest element (const overload).
         * 
         * @return const T& 
         */
        const T& back() const
        {
            // Buffer is empty -- no back element
            if (isEmpty())
                throw std::out_of_range("\033[91mNo back element: buffer is empty.\033[0m");

            // Loop the pointer, if needed
            if (_pTail == _buf)
                return *(_buf + _size - 1);
            
            return *(_pTail - 1);
        }

    private:
        size_t _size; // Size of the buffer in elements
        T* _buf; // Pointer to the first element of the buffer (beginning)
        T* _pHead; // Read pointer to the oldest item in the queue which will be retrieved next when the `pop()` method is called
        T* _pTail; // Write pointer to the position where the next element will be added at the next call to the `push()` method
        bool _empty; // Empty buffer flag
};

#endif // RINGBUF_HPP
