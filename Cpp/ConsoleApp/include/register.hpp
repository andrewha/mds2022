/**
 * @file register.hpp
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief 
 * @version 0.1
 * @date 2024-02-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include "record.hpp"

/**
 * @brief Employees register class.
 * 
 */
class Register
{

    public:
        
        /**
         * @brief Type alias for vector of pointers to employee records.
         * 
         */
        using EmpVec = std::vector<const Record*>;

        /**
         * @brief Type alias for set of pointers to employee records.
         * 
         */
        using EmpSet = std::set<const Record*>;

        /**
         * @brief Type alias for employee name index.
         * 
         */
        using NameIdx = std::map<const std::string, const Record*>;

        /**
         * @brief Type alias for employee department or position index.
         * 
         */
        using DepPosIdx = std::map<const std::string, EmpVec>;

        /**
         * @brief Type alias for employee direct subordinates vector.
         * 
         */
        using SubordVec = std::vector<std::string>;

        /**
         * @brief Type alias for employee direct subordinates index.
         * 
         */
        using SubordIdx = std::map<std::string, SubordVec>;

        /**
         * @brief Default constructor of a new Register object.
         * 
         */
        Register();
        
        /**
         * @brief Copy constructor of an other Register object (deep copy).
         * 
         * @param other Register
         */
        Register(const Register& other);
        
        /**
         * @brief Destructor of the Register object.
         * 
         */
        ~Register();
        
        /**
         * @brief Assignment operator overload.
         * 
         * @param rhv Register object
         * @return Register& 
         */
        Register& operator=(const Register& rhv);
        
        /**
         * @brief Object swapper.
         * 
         * @param lhv Register object
         * @param rhv Register object
         */
        static void swap(Register& lhv, Register& rhv) noexcept;
        
        /**
         * @brief Deep clean the Register object.
         * 
         * 
         */
        void clearRegister();
        
        /**
         * @brief Get register size.
         * 
         * @return size_t
         */
        size_t getSize() const;
        
        /**
         * @brief Add (push) a pointer to the employee record into employees vector and insert/update indices.
         * 
         * Time complexity:
         * 
         * \f$ O(\text{vector push back}) + \sum O(\text{map insert for each index}) = O(1) + \sum O(\log \text{Each index size}) \f$
         * 
         * \f$ = \sum O(\log \text{Each index size}). \f$
         * 
         * @param pRec Pointer to employee record object
         */
        void add(const Record* pRec);
        
        /**
         * @brief Get a vector of pointers to all employee records.
         * 
         * @return const EmpVec&
         */
        const EmpVec& getStorage() const;
        
        /**
         * @brief Overload for getting a vector of pointers to employee records filtered by age range.
         * 
         * Time complexity:
         * 
         * \f$ O(\text{Number of records}) \times O(\text{vector push back}) = O(\text{Number of records}) \times O(1) \f$
         * 
         * \f$ = O(\text{Number of records}). \f$
         * 
         * @param age_l Lower age limit
         * @param age_h Higher age limit
         * @return const EmpVec
         */
        const EmpVec getStorage(size_t age_l, size_t age_h) const;
        
        /**
         * @brief Get a set of pointers to employee records filtered by working days.
         * 
         * Time complexity: 
         * 
         * \f$ O(\text{Number of records}) \times O(\text{Number of days to check} \ll \text{Number of records}) \f$
         * 
         * \f$ \times [ O(\text{vector find in Employee working days} \ll \text{Number of records}) + O(\text{set insert}) ] \f$
         * 
         * \f$ \approx O(\text{Number of records}) \times O(\log \text{Number of records}) \f$
         * 
         * \f$ = O(\text{Number of records} \log \text{Number of records}). \f$
         * 
         * @param days_to_check Vector of given working days (3-letter strings) to check against
         * @return const EmpSet
         */
        const EmpSet getEmpByWorkDays(const Record::DaySet& days_to_check) const;
        
        /**
         * @brief Get the pointer to an employee record by their name from index.
         * 
         * Time complexity: \f$ O(\text{map lookup}) = O(\log \text{Name index size}). \f$
         * 
         * @param name Employee name
         * @return Record* 
         */
        const Record* getRecByName(const std::string& name);
        
        /**
         * @brief Get a vector of pointers to employee records filtered by department from index.
         * 
         * Time complexity: \f$ O(\text{map lookup}) = O(\log \text{Department index size}). \f$
         * 
         * @param dep Employee department
         * @return const EmpVec&
         */
        const EmpVec& getEmpByDep(const std::string& dep);
        
        /**
         * @brief Get a vector of pointers to employee records filtered by position from index.
         * 
         * Time complexity: \f$ O(\text{map lookup}) = O(\log \text{Position index size}). \f$
         * 
         * @param pos Employee position
         * @return const EmpVec&
         */
        const EmpVec& getEmpByPos(const std::string& pos);
        
        /**
         * @brief Get a vector of direct subordinates names by boss name from index.
         * 
         * Time complexity: \f$ O(\text{map lookup}) = O(\log \text{Subordinates index size}). \f$
         * 
         * @param boss Employee boss name
         * @return const SubordVec&
         */
        const SubordVec& getSubordsByBoss(const std::string& boss);
        
        /**
         * @brief Get employee name index.
         * 
         * @return const NameIdx&
         */
        const NameIdx& getNameIdx() const;
        
        /**
         * @brief Get employee department index.
         * 
         * @return const DepIdx&
         */
        const DepPosIdx& getDepIdx() const;
        
        /**
         * @brief Get employee position index.
         * 
         * @return const PosIdx&
         */
        const DepPosIdx& getPosIdx() const;
        
        /**
         * @brief Get employee direct subordinates index.
         * 
         * @return const SubordIdx&
         */
        const SubordIdx& getSubordIdx() const;
        
    private:

        /**
         * @brief Vector of pointers to employee records.
         * 
         */
        EmpVec employees_;

        /**
         * @brief Employee name index (map).
         * 
         */
        NameIdx name_idx_;

        /**
         * @brief Employee department index (map).
         * 
         */
        DepPosIdx dep_idx_;

        /**
         * @brief Employee position index (map).
         * 
         */
        DepPosIdx pos_idx_;

        /**
         * @brief Employee direct subordinates index (map).
         * 
         */
        SubordIdx subord_idx_;
};

#endif // REGISTER_HPP