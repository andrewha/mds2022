/**
 * @file register.cpp
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief 
 * @version 0.1
 * @date 2024-02-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdexcept>
#include <algorithm>
#include "register.hpp"

Register::Register()
    // New object is empty first
    : _employees()
    , _name_idx()
    , _dep_idx()
    , _pos_idx()
    , _subord_idx()
{
    // Register object is rather simple, so no further actions is required
}

Register::Register(const Register& other)
    // Copy is empty first
    : _employees()
    , _name_idx()
    , _dep_idx()
    , _pos_idx()
    , _subord_idx()
{
    // Populate the copy employees vector from the original employees vector
    for (const auto& pRec : other._employees)
    {
        // Create a new record on heap using its copy constructor
        const Record* pRec_copy = new Record(*pRec);
        // Insert into copy register: employees vector and all indices
        add(pRec_copy);
    }
}

Register::~Register()
{
    clearRegister();
}

Register& Register::operator=(const Register& rhv)
{
    // No action, if assigning this object to the same other object
    if (this == &rhv)
        return *this;

    // First, create a temp copy of the other object using its copy constructor
    Register temp(rhv);

    // Next, swap this object with the temp copy
    swap(temp, *this);
    return *this;            
} // `temp` is destroyed here

void Register::swap(Register& lhv, Register& rhv) noexcept
{
    // Swap all members
    std::swap(lhv._employees, rhv._employees);
    std::swap(lhv._name_idx, rhv._name_idx);
    std::swap(lhv._dep_idx, rhv._dep_idx);
    std::swap(lhv._pos_idx, rhv._pos_idx);
    std::swap(lhv._subord_idx, rhv._subord_idx);
}

void Register::clearRegister()
{
    for (const auto& pRec : _employees)
    {
        // Delete record by its pointer, if the pointer is not null
        if (pRec)
            delete pRec;
    }
    
    // Clear the employees vector
    _employees.clear();

    // Clear the indices
    _name_idx.clear();
    _dep_idx.clear();
    _pos_idx.clear();
    _subord_idx.clear();
}

size_t Register::getSize() const
{
    return _employees.size();
}

void Register::add(const Record* pRec)
{
    // Add employee record;
    _employees.push_back(pRec);

    // Add employee name to its index
    const std::string& name = pRec->getName();
    _name_idx[name] = pRec;

    // Add employee department to its index
    const std::string& dep = pRec->getDep();            
    _dep_idx[dep].push_back(pRec);

    // Add employee position to its index
    const std::string& pos = pRec->getPos();
    _pos_idx[pos].push_back(pRec);

    // Add employee direct subordinates to its index
    const std::string& boss = pRec->getBoss();
    _subord_idx[boss].push_back(name);
}

const Register::EmpVec& Register::getStorage() const
{
    return _employees;
}
        
const Register::EmpVec Register::getStorage(size_t age_l, size_t age_h) const
{            
    // Create and return a new vector of pointers to employee records filtered by their age
    EmpVec emps;
    for (const auto& pRec : _employees)
    {
        size_t age = pRec->getAge();
        if (age_l <= age && age <= age_h)
            emps.push_back(pRec);
    }
    return emps;
}
        
const Register::EmpSet Register::getEmpByWorkDays(const Record::DaySet& days_to_check) const
{            
    // For every employee working days
    EmpSet emps;
    for (const auto& pRec : _employees)
    {
        const Record::EmpWorkDays& work_days = pRec->getDays();
        // For every given day
        for (const auto& day_to_check : days_to_check)
        {
            // Check if employee works on that day
            if (std::find(work_days.begin(), work_days.end(), day_to_check) != work_days.end())
            {
                // Employee works on at least one of the given days
                emps.insert(pRec);
                // No need to continue search, iterate to next employee
                break;
            }
        }
    }
    return emps;
}

const Record* Register::getRecByName(const std::string& name)
{
    // Throw, if name not found
    if (_name_idx.find(name) == _name_idx.end())
        throw std::invalid_argument("\n\033[91mName not found\033[0m\n");
    
    return _name_idx[name];
}

const Register::EmpVec& Register::getEmpByDep(const std::string& dep)
{
    // Throw, if department not found
    if (_dep_idx.find(dep) == _dep_idx.end())
        throw std::invalid_argument("\n\033[91mDepartment not found\033[0m\n");
    
    return _dep_idx[dep];
}

const Register::EmpVec& Register::getEmpByPos(const std::string& pos)
{
    // Throw, if position not found
    if (_pos_idx.find(pos) == _pos_idx.end())
        throw std::invalid_argument("\n\033[91mPosition not found\033[0m\n");
    
    return _pos_idx[pos];
}

const Register::SubordVec& Register::getSubordsByBoss(const std::string& boss)
{
    // Throw, if boss name not found: employee is not a boss, i.e. has no direct subordinates
    if (_subord_idx.find(boss) == _subord_idx.end())
        throw std::invalid_argument("\n\033[91mNo subordinates\033[0m\n");
    
    return _subord_idx[boss];
}

const Register::NameIdx& Register::getNameIdx() const
{
    return _name_idx;
}

const Register::DepPosIdx& Register::getDepIdx() const
{
    return _dep_idx;
}

const Register::DepPosIdx& Register::getPosIdx() const
{
    return _pos_idx;
}

const Register::SubordIdx& Register::getSubordIdx() const
{
    return _subord_idx;
}
