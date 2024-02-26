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

    std::cout << "\n\033[33mEnd of test\033[0m" << std::endl;
    
    return 0;
}