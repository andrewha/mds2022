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
    : _emp_name(name)
    , _emp_age(std::stoi(age))
    , _emp_dep(dep)
    , _emp_pos(pos)
    , _emp_boss(boss.length() ? boss : "n/a")
    , _emp_days(days)
{
    // Record object is rather simple, so no further actions is required
}

Record::Record(const Record& other)
    : _emp_name(other._emp_name)
    , _emp_age(other._emp_age)
    , _emp_dep(other._emp_dep)
    , _emp_pos(other._emp_pos)
    , _emp_boss(other._emp_boss)
    , _emp_days(other._emp_days)
{
    // Record object is rather simple, so no further actions is required
}

Record::~Record()
{
    // Record object is rather simple: almost all members are simple types
    // `_emp_days` is a vector of strings, but it will also be automatically deleted, so no explicit action is required
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
    std::swap(lhv._emp_name, rhv._emp_name);
    std::swap(lhv._emp_age, rhv._emp_age);
    std::swap(lhv._emp_dep, rhv._emp_dep);
    std::swap(lhv._emp_pos, rhv._emp_pos);
    std::swap(lhv._emp_boss, rhv._emp_boss);
    std::swap(lhv._emp_days, rhv._emp_days);
}

const std::string& Record::getName() const
{
    return _emp_name;
}

size_t Record::getAge() const
{
    return _emp_age;
}

const std::string& Record::getDep() const
{
    return _emp_dep;
}

const std::string& Record::getPos() const
{
    return _emp_pos;
}

const std::string& Record::getBoss() const
{
    return _emp_boss;
}

const Record::EmpWorkDays& Record::getDays() const
{
    return _emp_days;
}
