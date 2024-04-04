/**
 * @file record.cpp
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief 
 * @version 0.1
 * @date 2024-02-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "record.hpp"

Record::Record(const std::string& name, 
               const std::string& age, 
               const std::string& dep, 
               const std::string& pos, 
               const std::string& boss, 
               const EmpWorkDays& days
               )
    : emp_name_(name)
    , emp_age_(std::stoi(age))
    , emp_dep_(dep)
    , emp_pos_(pos)
    , emp_boss_(boss.length() ? boss : "n/a")
    , emp_days_(days)
{
    // Record object is rather simple, so no further actions is required
}

Record::Record(const Record& other)
    : emp_name_(other.emp_name_)
    , emp_age_(other.emp_age_)
    , emp_dep_(other.emp_dep_)
    , emp_pos_(other.emp_pos_)
    , emp_boss_(other.emp_boss_)
    , emp_days_(other.emp_days_)
{
    // Record object is rather simple, so no further actions is required
}

Record::~Record()
{
    // Record object is rather simple: almost all members are simple types
    // `emp_days_` is a vector of strings, but it will also be automatically deleted, so no explicit action is required
}

Record& Record::operator=(const Record& rhv)
{
    // No action, if assigning this object to the same other object
    if (this == &rhv)
        return *this;

    // First, create a temp copy of the other object
    Record temp(rhv);

    // Next, swap this object with the temp copy
    swap(temp, *this);
    return *this;            
} // `temp` is destroyed here

void Record::swap(Record& lhv, Record& rhv) noexcept
{
    // Swap all members
    std::swap(lhv.emp_name_, rhv.emp_name_);
    std::swap(lhv.emp_age_, rhv.emp_age_);
    std::swap(lhv.emp_dep_, rhv.emp_dep_);
    std::swap(lhv.emp_pos_, rhv.emp_pos_);
    std::swap(lhv.emp_boss_, rhv.emp_boss_);
    std::swap(lhv.emp_days_, rhv.emp_days_);
}

const std::string& Record::getName() const
{
    return emp_name_;
}

size_t Record::getAge() const
{
    return emp_age_;
}

const std::string& Record::getDep() const
{
    return emp_dep_;
}

const std::string& Record::getPos() const
{
    return emp_pos_;
}

const std::string& Record::getBoss() const
{
    return emp_boss_;
}

const Record::EmpWorkDays& Record::getDays() const
{
    return emp_days_;
}
