#include <iostream>
#include "ringbuf.hpp"

int main()
{
    std::cout << "\n\033[33mRingBuffer Class Template test\033[0m\n";
    
    std::cout << "\n\033[33mTest on a buffer of integers\033[0m\n";

    std::cout << "\n\033[33mStage 1. Definition of class fields\n&\033[0m";
    std::cout << "\n\033[33mStage 2. Getter method determines the buffer fullness\033[0m\n";
    
    size_t sz = 0;
    std::cout << "\n1. Instantiate a buffer of size " << sz << '\n';
    try
    {
        RingBuffer<int> rb1(sz);
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    
    sz = 5;
    std::cout << "\n2. Instantiate a buffer of size " << sz << '\n';
    RingBuffer<int> rb2(sz);
    std::cout << "Buffer size: " << rb2.getSize() << '\n';
    std::cout << "Empty? " << std::boolalpha << rb2.isEmpty() << '\n';
    std::cout << "Inserted count: " << rb2.getCount() << " element(s)\n";
    std::cout << "Can insert more: " << rb2.getFree() << " element(s)\n";
    std::cout << "Full? " << std::boolalpha << rb2.isFull() << '\n';

    std::cout << "\n\033[33mStage 3. The `push()` method\n&\033[0m";
    std::cout << "\n\033[33mStage 4. The `front()` and `back()` methods\033[0m\n\n";
    
    try
    {
        std::cout << "Current oldest value: " << rb2.front() << '\n';
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    
    try
    {
        std::cout << "Current newest value: " << rb2.back() << '\n';
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }

    std::cout << "\n1. Write (push) " << rb2.getSize() + 1 << " element(s) to the buffer\n";
    for (size_t i = 0; i < rb2.getSize() + 1; ++i)
    {
        try
        {
            std::cout << "Trying to insert an element with value: " << i << '\n';
            rb2.push(i);
            std::cout << "Inserted count: " << rb2.getCount() << " element(s)\n";
            std::cout << "Can insert more: " << rb2.getFree() << " element(s)\n";
            std::cout << "Full? " << std::boolalpha << rb2.isFull() << '\n';
            std::cout << "Current oldest value: " << rb2.front() << '\n';
            std::cout << "Current newest value: " << rb2.back() << '\n';
            std::cout << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << '\n';
        }
    }

    std::cout << "\n\033[33mStage 5. The `pop()` method\033[0m\n";
    std::cout << "\n1. Read (pop) " << rb2.getSize() + 1 << " element(s) from the buffer\n";
    for (size_t i = 0; i < rb2.getSize() + 1; ++i)
    {
        try
        {
            std::cout << "Trying to read: ";
            std::cout << rb2.pop() << '\n';
            std::cout << "Inserted count: " << rb2.getCount() << " element(s)\n";
            std::cout << "Can insert more: " << rb2.getFree() << " element(s)\n";
            std::cout << "Empty? " << std::boolalpha << rb2.isEmpty() << '\n';
            std::cout << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << '\n';
        }   
    }

    std::cout << "\n\033[33mEnd of test\033[0m" << std::endl;
    
    return 0;
}