/**
 * @file record.hpp
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief 
 * @version 0.1
 * @date 2024-02-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef RECORD_HPP
#define RECORD_HPP

#include <string>
#include <vector>
#include <set>

/**
 * @brief Employee record class.
 * 
 */
class Record
{
    public:

        /**
         * @brief Type alias for vector of employee working days (3-letter strings).
         * 
         */
        using EmpWorkDays = std::vector<std::string>;

        /**
         * @brief Type alias for set of given working days (3-letter strings).
         * 
         */
        using DaySet = std::set<std::string>;
        
        /**
         * @brief Initializing constructor of a new Record object.
         * 
         * @param name Name
         * @param age Age
         * @param dep Department
         * @param pos Position
         * @param boss Boss name
         * @param days Working days
         */
        Record(const std::string& name, 
               const std::string& age, 
               const std::string& dep, 
               const std::string& pos, 
               const std::string& boss, 
               const EmpWorkDays& days
               );

        /**
         * @brief Copy constructor of a new Record object.
         * 
         * @param other Record object
         */
        Record(const Record& other);

        /**
         * @brief Destructor of the Record object.
         * 
         */
        ~Record();

        /**
         * @brief Assignment operator overload.
         * 
         * @param rhv Record object
         * @return Record& 
         */
        Record& operator=(const Record& rhv);

        /**
         * @brief Object swapper.
         * 
         * @param lhv Record object
         * @param rhv Record object
         */
        static void swap(Record& lhv, Record& rhv) noexcept;
        
        /**
         * @brief Get employee name.
         * 
         * @return const std::string& const
         */
        const std::string& getName() const;
        
        /**
         * @brief Get employee age.
         * 
         * @return size_t
         */
        size_t getAge() const;
        
        /**
         * @brief Get employee department.
         * 
         * @return const std::string&
         */
        const std::string& getDep() const;
        
        /**
         * @brief Get employee position.
         * 
         * @return const std::string&
         */
        const std::string& getPos() const;
        
        /**
         * @brief Get employee boss name.
         * 
         * @return const std::string&
         */
        const std::string& getBoss() const;
        
        /**
         * @brief Get employee working days.
         * 
         * @return const EmpWorkDays&
         */
        const EmpWorkDays& getDays() const;
        
    private:
        
        /**
         * @brief Employee name.
         * 
         */
        std::string emp_name_;

        /**
         * @brief Employee age.
         * 
         */
        size_t emp_age_;

        /**
         * @brief Employee department.
         * 
         */
        std::string emp_dep_;

        /**
         * @brief Employee position.
         * 
         */
        std::string emp_pos_;

        /**
         * @brief Employee boss name.
         * 
         */
        std::string emp_boss_;

        /**
         * @brief Employee working days.
         * 
         */
        EmpWorkDays emp_days_;
};

#endif // RECORD_HPP