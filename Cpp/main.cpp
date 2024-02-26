#include <iostream>
#include "ringbuf.hpp"

int main()
{
    std::cout << std::endl << "Test the RingBuffer Class Template\n";
    
    std::cout << "\n1. Buffer of integers\n";
    
    std::cout << "\n1.1. Instantiate a buffer with zero size\n";
    try
    {
        RingBuffer<int> rb1(0);
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    
    size_t sz = 5;
    std::cout << "\n1.2. Instantiate a buffer of size " << sz << '\n';
    RingBuffer<int> rb2(sz);
    std::cout << "Buffer size: " << rb2.getSize() << '\n';
    std::cout << "Empty? " << std::boolalpha << rb2.isEmpty() << '\n';
    std::cout << "Inserted count: " << rb2.getCount() << " element(s)\n";
    std::cout << "Can insert more: " << rb2.getFree() << " element(s)\n";
    std::cout << "Full? " << std::boolalpha << rb2.isFull() << '\n';

    std::cout << "\n1.3. Write (push) " << rb2.getSize() + 1 << " element(s) to the buffer\n";
    for (size_t i = 0; i < rb2.getSize() + 1; ++i)
    {
        try
        {
            std::cout << "Trying to insert an element with value: " << i << '\n';
            rb2.push(i);
            std::cout << "Inserted count: " << rb2.getCount() << " element(s)\n";
            std::cout << "Can insert more: " << rb2.getFree() << " element(s)\n";
            std::cout << "Full? " << std::boolalpha << rb2.isFull() << '\n';
            std::cout << "The newest value: " << rb2.back() << '\n';
            std::cout << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << '\n';
        }
    }

    std::cout << "\n1.4. Read (pop) " << rb2.getSize() + 1 << " element(s) from the buffer\n";
    for (size_t i = 0; i < rb2.getSize() + 1; ++i)
    {
        try
        {
            std::cout << "The oldest value: " << rb2.front() << '\n';
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
    
    std::cout << "\n2. Buffer of chars\n";
    sz = 3;
    std::cout << "\n2.1. Instantiate a buffer of size " << sz << " and push three chars to it: 'A', 'B', 'C'\n";
    RingBuffer<char> rb3(sz);
    rb3.push('A');
    rb3.push('B');
    rb3.push('C');
    std::cout << "Buffer size: " << rb3.getSize() << '\n';
    std::cout << "Empty? " << std::boolalpha << rb3.isEmpty() << '\n';
    std::cout << "Inserted count: " << rb3.getCount() << " element(s)\n";
    std::cout << "Can insert more: " << rb3.getFree() << " element(s)\n";
    std::cout << "Full? " << std::boolalpha << rb3.isFull() << '\n';

    std::cout << "\n2.2. Read (pop) and print out the content of the buffer\n";
    while (!rb3.isEmpty())
        std::cout << rb3.pop();
    std::cout << std::endl;

    std::cout << "\n3. Test the copy constructor\n";
    

    RingBuffer<int> rb4(10);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    std::cout << "Empty? " << std::boolalpha << rb4.isEmpty() << '\n';
    std::cout << "Inserted count: " << rb4.getCount() << " element(s)\n";
    std::cout << "Can insert more: " << rb4.getFree() << " element(s)\n";
    std::cout << "Full? " << std::boolalpha << rb4.isFull() << '\n';
    rb4.pop();
    rb4.pop();
    rb4.pop();
    rb4.pop();
    rb4.pop();
    std::cout << "Empty? " << std::boolalpha << rb4.isEmpty() << '\n';
    std::cout << "Inserted count: " << rb4.getCount() << " element(s)\n";
    std::cout << "Can insert more: " << rb4.getFree() << " element(s)\n";
    std::cout << "Full? " << std::boolalpha << rb4.isFull() << '\n';
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.pop();
    rb4.pop();
    rb4.pop();
    rb4.pop();
    rb4.pop();
    rb4.pop();
    rb4.pop();
    rb4.pop();
    rb4.push(3);
    rb4.push(3);
    rb4.pop();
    rb4.pop();
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    rb4.push(3);
    std::cout << "Empty? " << std::boolalpha << rb4.isEmpty() << '\n';
    std::cout << "Inserted count: " << rb4.getCount() << " element(s)\n";
    std::cout << "Can insert more: " << rb4.getFree() << " element(s)\n";
    std::cout << "Full? " << std::boolalpha << rb4.isFull() << '\n';



    std::cout << "\nEnd of test" << std::endl;
    
    return 0;
}