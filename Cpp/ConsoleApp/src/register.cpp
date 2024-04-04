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
    : employees_()
    , name_idx_()
    , dep_idx_()
    , pos_idx_()
    , subord_idx_()
{
    // Register object is rather simple, so no further actions is required
}

Register::Register(const Register& other)
    // Copy is empty first
    : employees_()
    , name_idx_()
    , dep_idx_()
    , pos_idx_()
    , subord_idx_()
{
    // Populate the copy employees vector from the original employees vector
    for (const auto& pRec : other.employees_)
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
    std::swap(lhv.employees_, rhv.employees_);
    std::swap(lhv.name_idx_, rhv.name_idx_);
    std::swap(lhv.dep_idx_, rhv.dep_idx_);
    std::swap(lhv.pos_idx_, rhv.pos_idx_);
    std::swap(lhv.subord_idx_, rhv.subord_idx_);
}

void Register::clearRegister()
{
    for (const auto& pRec : employees_)
    {
        // Delete record by its pointer, if the pointer is not null
        if (pRec)
            delete pRec;
    }
    
    // Clear the employees vector
    employees_.clear();

    // Clear the indices
    name_idx_.clear();
    dep_idx_.clear();
    pos_idx_.clear();
    subord_idx_.clear();
}

size_t Register::getSize() const
{
    return employees_.size();
}

void Register::add(const Record* pRec)
{
    // Add employee record;
    employees_.push_back(pRec);

    // Add employee name to its index
    const std::string& name = pRec->getName();
    name_idx_[name] = pRec;

    // Add employee department to its index
    const std::string& dep = pRec->getDep();            
    dep_idx_[dep].push_back(pRec);

    // Add employee position to its index
    const std::string& pos = pRec->getPos();
    pos_idx_[pos].push_back(pRec);

    // Add employee direct subordinates to its index
    const std::string& boss = pRec->getBoss();
    subord_idx_[boss].push_back(name);
}

const Register::EmpVec& Register::getStorage() const
{
    return employees_;
}
        
const Register::EmpVec Register::getStorage(size_t age_l, size_t age_h) const
{            
    // Create and return a new vector of pointers to employee records filtered by their age
    EmpVec emps;
    for (const auto& pRec : employees_)
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
    for (const auto& pRec : employees_)
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
    if (name_idx_.find(name) == name_idx_.end())
        throw std::invalid_argument("\n\033[91mName not found\033[0m\n");
    
    return name_idx_[name];
}

const Register::EmpVec& Register::getEmpByDep(const std::string& dep)
{
    // Throw, if department not found
    if (dep_idx_.find(dep) == dep_idx_.end())
        throw std::invalid_argument("\n\033[91mDepartment not found\033[0m\n");
    
    return dep_idx_[dep];
}

const Register::EmpVec& Register::getEmpByPos(const std::string& pos)
{
    // Throw, if position not found
    if (pos_idx_.find(pos) == pos_idx_.end())
        throw std::invalid_argument("\n\033[91mPosition not found\033[0m\n");
    
    return pos_idx_[pos];
}

const Register::SubordVec& Register::getSubordsByBoss(const std::string& boss)
{
    // Throw, if boss name not found: employee is not a boss, i.e. has no direct subordinates
    if (subord_idx_.find(boss) == subord_idx_.end())
        throw std::invalid_argument("\n\033[91mNo subordinates\033[0m\n");
    
    return subord_idx_[boss];
}

const Register::NameIdx& Register::getNameIdx() const
{
    return name_idx_;
}

const Register::DepPosIdx& Register::getDepIdx() const
{
    return dep_idx_;
}

const Register::DepPosIdx& Register::getPosIdx() const
{
    return pos_idx_;
}

const Register::SubordIdx& Register::getSubordIdx() const
{
    return subord_idx_;
}
