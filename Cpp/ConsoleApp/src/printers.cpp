/**
 * @file printers.cpp
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief 
 * @version 0.1
 * @date 2024-02-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <iomanip>
#include <map>
#include "record.hpp"
#include "register.hpp"
#include "printers.hpp"

void printMenu()
{
    std::cout << "\n\033[33mEmployee database\n=================\033[0m\n";
    std::cout << "(L) Load register from file\n";
    std::cout << "(C) Clear current register\n";
    std::cout << "(S) Create a copy of current register and save it to disk\n";
    std::cout << "(N) Print number of records in current register\n";
    std::cout << "(P) Print all records\n";
    std::cout << "(A) Find all employees with age in a given range\n";
    std::cout << "(E) Find an employee by their name\n";
    std::cout << "(D) Find all employees by their department\n";
    std::cout << "(T) Find all employees by their position\n";
    std::cout << "(O) Show all subordinates of an employee recursively\n";
    std::cout << "(W) Find all employees working on given days\n";
    std::cout << "(X) Exit\n\n";
    std::cout << "Choose an action: ";
}

std::ostream& operator<<(std::ostream& s, const Record::EmpWorkDays& d)
{
    for (auto it = d.begin(); it != d.end(); ++it)
    {
        if (it != d.begin())
            s << ", ";
        s << *it;
    }
    
    return s;
}

std::ostream& operator<<(std::ostream& s, const Record* pRec)
{
    if (pRec)
    {
        s << std::left
        << std::setw(20) << pRec->getName() 
        << std::setw(5) << pRec->getAge()
        << std::setw(10) << pRec->getDep()
        << std::setw(10) << pRec->getPos()
        << std::setw(20) << pRec->getBoss()
        << pRec->getDays()
        << std::endl;
    }   
    return s;
}

void printOneRecord(const Record* pRec)
{
    if (pRec)
    {
        std::cout << "\nName         : " << pRec->getName() 
                << "\nAge          : " << pRec->getAge() 
                << "\nDepartment   : " << pRec->getDep() 
                << "\nPosition     : " << pRec->getPos()
                << "\nBoss name    : " << pRec->getBoss()
                << "\nWorking days : " << pRec->getDays()
                << std::endl;
    }
}

void printRecNum(const Register& r)
{
    std::cout << "\n\033[92mRegistered: " << r.getSize() << " employees\033[0m\n";
}

void printEmpHeader()
{
    std::cout << std::endl
              << std::left
              << std::setw(20) << "Name" 
              << std::setw(5) << "Age"
              << std::setw(10) << "Dept."
              << std::setw(10) << "Position"
              << std::setw(20) << "Boss name"
              << std::setw(35) << "Working days"
              << std::endl
              << std::right
              << std::setfill('-')
              << std::setw(20) << ' ' 
              << std::setw(5) << ' '
              << std::setw(10) << ' '
              << std::setw(10) << ' '
              << std::setw(20) << ' '
              << std::setw(35) << ' '
              << std::setfill(' ')
              << std::left
              << std::endl;
}

void printIdxKeys(const Register::DepPosIdx& idx)
{
    for (auto it = idx.begin(); it != idx.end(); ++it)
    {
        if (it != idx.begin())
            std::cout << ", ";
        std::cout << it->first;
    }
}

void dfs(const std::string& subord_name, VisMap& visited, size_t level, Register& r)
{
    // Employee is now visited
    visited[subord_name] = true;
    
    // Get next level subordinates from the index
    level++;
    try
    {
        // There are direct subordinates

        const Register::SubordVec& dir_subords = r.getSubordsByBoss(subord_name);
    
        for (auto dir_subord : dir_subords)
        {            
            // Print subordinate
            std::cout << std::right
                    << std::setfill('.')
                    << std::setw(level + 1) << ' '
                    << dir_subord
                    << std::setfill(' ')
                    << std::left
                    << std::endl;

            if (!visited[dir_subord])
                // Repeat recursively until all required subordinates are not visited
                dfs(dir_subord, visited, level, r);
        }
    }
    catch(const std::exception& e)
    {
        // Employee is not a boss, i.e. has no direct subordinates
        // For pretty printing of next level subordinates, do not show this warning
        //std::cerr << e.what();
    }
    
}

void printSubordsByBoss(const std::string& boss, Register& r)
{
    std::cout << boss << ':' << std::endl;
    
    // Get direct subordinates from the index
    try
    {
        // There are direct subordinates
        
        const Register::SubordVec& dir_subords = r.getSubordsByBoss(boss);
        
        // Map to store visited employee flags
        VisMap visited;
        // Init the map
        const Register::NameIdx& name_idx = r.getNameIdx();
        for (const auto& name_kv : name_idx)
            visited[name_kv.first] = false; // { Each name : false }
        
        // Traverse all direct subordinates
        size_t level = 1;
        for (auto dir_subord : dir_subords)
        {            
            // Print subordinate
            std::cout << std::right
                    << std::setfill('.')
                    << std::setw(level + 1) << ' '
                    << dir_subord
                    << std::setfill(' ')
                    << std::left
                    << std::endl;
            
            // Traverse subordinate to check if they also have their direct subordinates
            if (!visited[dir_subord])
                dfs(dir_subord, visited, level, r);
        }
    }
    catch(const std::exception& e)
    {
        // Employee is not a boss, i.e. has no direct subordinates
        std::cerr << e.what();
    }
}

template <typename T>
void printEmpCollection(const T& emps)
{
    // Print header
    printEmpHeader();

    // Print records
    std::string user_input;
    char action;
    size_t line_count = 0;
    for (const auto& pRec : emps)
    {
        std::cout << pRec;
        // Paginate output with 20 records per page
        if (line_count % 20 == 19)
        {
            std::cout << "\nShowed 20 records. More (y/n)? ";
            std::getline(std::cin, user_input);
            action = user_input[0];
            switch (action)
            {
            case 'y':
            case 'Y':
                // Continue
                printEmpHeader();
                break;
            case 'n':
            case 'N':
                // Exit
                return;
                break;
            default:
                // Exit
                return;
                break;
            }
        }
        line_count++;
    }
}

// Explicitly instantiate templates for actual required types
template void printEmpCollection<Register::EmpVec>(const Register::EmpVec&);
template void printEmpCollection<Register::EmpSet>(const Register::EmpSet&);
