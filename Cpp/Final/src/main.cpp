/**
 * @file main.cpp
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief 
 * @version 0.1
 * @date 2024-02-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "record.hpp"
#include "register.hpp"
#include "printers.hpp"

int main()
{
    std::string user_input;
    char action;
    std::string filename;
    std::ifstream ifs;
    std::ofstream ofs;
    size_t line_count;
    
    // Linux: flush the output buffer to show not terminated lines immediately
    #ifdef __linux__
    std::cout.setf(std::ios_base::unitbuf); 
    #endif

    // Instantiate an empty register
    Register reg;

    // User interface loop
    while (true)
    {
        printMenu();
        
        std::getline(std::cin, user_input);
        action = user_input[0];
        //std::cout << "\nYou have chosen: " << action << std::endl;
        
        // User input action tree
        switch (action)
        {
            // (X) Exit
            case 'x':
            case 'X':
            {
                // Clean up
                std::cout << "\nCleaning up... ";
                if (ifs.is_open())
                    ifs.close();
                reg.clearRegister();
                std::cout << "OK\n\nGood bye!\n";
                return EXIT_SUCCESS;
                break;
            }

            // (L) Load register from file
            case 'l':
            case 'L':
            {
                // Try to open a file
                std::cout << "Enter the filename (current register will be overwritten): ";
                std::getline(std::cin, filename);
                std::cout << "\nLoading records... ";
                ifs.open(filename, std::ios::binary);
                if (!ifs.is_open())
                {
                    std::cout << "\033[91mFile cannot be opened\033[0m\n";
                    break;
                }
                // Clear current register first, so that new loaded records do not mix with old records
                reg.clearRegister();
                // Read file line by line and parse tab-separated fields
                std::string line, name, age, dep, pos, boss, days_str;
                line_count = 0;
                while (std::getline(ifs, line))
                {
                    std::istringstream iss1(line);
                    std::getline(iss1, name, '\t');
                    std::getline(iss1, age, '\t');
                    std::getline(iss1, dep, '\t');
                    std::getline(iss1, pos, '\t');
                    std::getline(iss1, boss, '\t');
                    std::getline(iss1, days_str);
                    // Parse the working days
                    Record::EmpWorkDays days;
                    std::istringstream iss2(days_str);
                    std::string day;
                    while (iss2.good() && !iss2.eof())
                    {
                        iss2 >> day;
                        days.push_back(day);
                    }
                    // Create a record on the heap and return a pointer to it
                    const Record* pRec = new Record(name, age, dep, pos, boss, days);
                    // Populate the register
                    reg.add(pRec);
                    // Show progress for large files
                    line_count++;
                    if (line_count % 1000000 == 0)
                        std::cout << line_count << "... ";
                }
                // Done!
                std::cout << "OK\n";
                printRecNum(reg);
                ifs.close();
                break;
            }

            // (N) Print number of records in current register
            case 'n':
            case 'N':
            {
                printRecNum(reg);
                break;
            }

            // (P) Print all records
            case 'p':
            case 'P':
            {
                // Get all employee records and print them
                const Register::EmpVec& emps = reg.getStorage();
                printRecNum(reg);
                printEmpCollection(emps);
                break;
            }

            // (C) Clear current register
            case 'c':
            case 'C':
            {
                std::cout << "\n\033[91mCurrent register with all records and indices will be deleted. Are you sure (y/n)?\033[0m ";
                std::getline(std::cin, user_input);
                action = user_input[0];
                switch (action)
                {
                case 'y':
                case 'Y':
                    // Clear
                    std::cout << "\nDeleting register... ";
                    reg.clearRegister();
                    std::cout << "OK\n";
                    printRecNum(reg);
                    break;
                case 'n':
                case 'N':
                    // Do nothing
                    break;
                default:
                    break;
                }
                break;
            }

            // (A) Find all employees with age in a given range
            case 'a':
            case 'A':
            {
                std::cout << "Enter age range (low high), eg. 18 70: ";
                size_t age_l, age_h;
                std::getline(std::cin, user_input);
                std::stringstream ss(user_input);
                ss >> age_l >> age_h;
                // Get the employees filtered by age range and print them
                const Register::EmpVec emps = reg.getStorage(age_l, age_h);
                std::cout << "\n\033[92mFound: " << emps.size() << " employee(s)\033[0m\n";
                printEmpCollection(emps);
                break;
            }

            // (S) Create a copy of current register and save it to disk
            case 's':
            case 'S':
            {
                // Instantiate a copy of current register
                std::cout << "\nCopying register... ";
                Register reg_copy(reg); // Test copy constructor
                //Register reg_copy; // Uncomment to test copy assignment operator
                //reg_copy = reg;   // Uncomment to test copy assignment operator
                std::cout << "OK\n";
                std::cout << "\nRegister copied. Showing all records from the copy:\n";
                printRecNum(reg_copy);
                const Register::EmpVec& emps_copy = reg_copy.getStorage();
                printEmpCollection(emps_copy);
                
                // Try to save to a file
                std::cout << "\nEnter filename to save the copied register: ";
                std::getline(std::cin, filename);
                std::cout << "\nSaving records... ";
                ofs.open(filename, std::ios::binary);
                if (!ofs.is_open())
                {
                    std::cout << "\033[91mFile cannot be opened\033[0m\n";
                    std::cout << "\nSwitching back to original register.\n";
                    break;
                } // `reg_copy` is destroyed here
                
                // Write records to file
                line_count = 0;
                for (const auto& pRec : emps_copy)
                {
                    // Write simple fields
                    ofs << pRec->getName() << '\t'
                        << pRec->getAge() << '\t'
                        << pRec->getDep() << '\t'
                        << pRec->getPos() << '\t';
                        if (pRec->getBoss() == "n/a")
                            ofs << '\t';
                        else
                            ofs << pRec->getBoss() << '\t';
                    // Write vector of working days
                    const Record::EmpWorkDays& days = pRec->getDays();
                    for (auto it = days.begin(); it != days.end(); ++it)
                    {
                        if (it != days.begin())
                            ofs << '\t';
                        ofs << *it;
                    }
                    ofs << '\n';
                    // Show progress for large files
                    line_count++;
                    if (line_count % 1000000 == 0)
                        std::cout << line_count << "... ";
                }
                // Done!
                std::cout << "OK\n";
                std::cout << "\nCreated file '" << filename << "'. You can load it now to check.\n";
                ofs.close();
                std::cout << "\nSwitching back to original register.\n";
                break;
            } // `reg_copy` is destroyed here

            // (E) Print an employee by their name
            case 'e':
            case 'E':
            {
                std::cout << "\nEnter employee's full name (case-sensitive), eg. John Smith: ";
                std::string name;
                std::getline(std::cin, name);
                try
                {
                    const Record* pRec = reg.getRecByName(name);
                    printOneRecord(pRec);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what();
                }
                break;
            }

            // (D) Print an employee by their department
            case 'd':
            case 'D':
            {
                const Register::DepPosIdx& dep_index = reg.getDepIdx();
                std::cout << "\nEnter employee's department (case-sensitive)\n"
                          << "Hint: ";
                printIdxKeys(dep_index);
                std::cout << ": ";
                std::string dep;
                std::getline(std::cin, dep);
                try
                {
                    const Register::EmpVec& emps = reg.getEmpByDep(dep);
                    std::cout << "\n\033[92mFound: " << emps.size() << " employee(s)\033[0m" << std::endl;
                    printEmpCollection(emps);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what();
                }
                break;
            }

            // (D) Print an employee by their position
            case 't':
            case 'T':
            {
                const Register::DepPosIdx& pos_index = reg.getPosIdx();
                std::cout << "\nEnter employee's position (case-sensitive)\n"
                          << "Hint: ";
                printIdxKeys(pos_index);
                std::cout << ": ";
                std::string pos;
                std::getline(std::cin, pos);
                try
                {
                    const Register::EmpVec& emps = reg.getEmpByPos(pos);
                    std::cout << "\n\033[92mFound: " << emps.size() << " employee(s)\033[0m\n";
                    printEmpCollection(emps);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what();
                }                
                break;
            }

            // (O) Show all subordinates of an employee recursively\n";
            case 'o':
            case 'O':
            {
                std::cout << "\nEnter employee's full name (case-sensitive), eg. John Smith\n"
                          << "Hint: enter n/a to see full subordinates tree: ";
                std::string boss;
                std::getline(std::cin, boss);
                
                // Get name index
                const Register::NameIdx& name_idx = reg.getNameIdx();
                
                // Get subordinate index
                const Register::SubordIdx& subord_idx = reg.getSubordIdx();
        
                if (name_idx.find(boss) == name_idx.end() && subord_idx.find(boss) == subord_idx.end())
                {
                    // Employee name not found
                    std::cout << "\n\033[91mName not found\033[0m\n";
                    break;
                }
                
                // Traverse the name and subordinate indices to print subordinates
                printSubordsByBoss(boss, reg);
                break;
            }

            // (W) Print a list of employees working on given days
            case 'w':
            case 'W':
            {
                // Input and parse working days
                std::cout << "\nEnter working days to check (case-sensitive), eg. Mon Wed Fri\n"
                          << "Hint: Mon, Tue, Wed, Thu, Fri, Sat, Sun: ";
                Record::DaySet days_to_check;
                std::getline(std::cin, user_input);
                std::stringstream ss(user_input);
                std::string day;
                while (ss.good() && !ss.eof())
                {
                    ss >> day;
                    // Add to a set instead of vector, so that there are no duplicates
                    days_to_check.insert(day);
                }
                // Get employee records with working days intersecting with given working days to check
                const Register::EmpSet emps = reg.getEmpByWorkDays(days_to_check);
                std::cout << "\n\033[92mFound: " << emps.size() << " employee(s)\033[0m\n";
                printEmpCollection(emps);
                break;
            }

            default:
                //std::cout << "\nYou have chosen: " << action << " (" << (int)action << ')' << std::endl;
                std::cout << "\n\033[91mPlease choose a correct action\033[0m\n";
                break;
        }
    }
    
    return EXIT_SUCCESS;
}