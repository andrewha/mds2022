/**
 * @file printers.hpp
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief 
 * @version 0.1
 * @date 2024-02-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PRINTERS_HPP
#define PRINTERS_HPP

/**
 * @brief Type alias for map of visited employee flags.
 * 
 */
using VisMap = std::map<std::string, bool>;

/**
 * @brief Show the main menu.
 * 
 */
void printMenu();

/**
 * @brief Print employee working days.
 * 
 * @param s Output stream
 * @param d Vector of working days (3-letter strings)
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& s, const Record::EmpWorkDays& d);

/**
 * @brief Print one record as a table row (long).
 * 
 * @param s Output stream
 * @param pRec Pointer to the employee record object
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& s, const Record* pRec);

/**
 * @brief Print one record as a standalone card (tall).
 * 
 * @param pRec Pointer to the employee record object
 */
void printOneRecord(const Record* pRec);

/**
 * @brief Print number of employees records in current register.
 * 
 * @param r Register object
 */
void printRecNum(const Register& r);

/**
 * @brief Print header row for employees records table.
 * 
 */
void printEmpHeader();

/**
 * @brief Print keys of employee department or position index.
 * 
 * @param idx Employee index
 */
void printIdxKeys(const Register::DepPosIdx& idx);

/**
 * @brief Visit every employee once. Recursive depth-first search (DFS) algorithm.
 * 
 * @param subord_name Subordinate employee name
 * @param visited Map storing visited employee flags
 * @param level Level of subordination
 * @param r Register object
 */
void dfs(const std::string& subord_name, VisMap& visited, size_t level, Register& r);

/**
 * @brief Print all direct and indirect (recursively) subordinates from index.
 * 
 * @param boss Employee boss name
 * @param r Register object
 */
void printSubordsByBoss(const std::string& boss, Register& r);

/**
 * @brief Template for printing all employee records from the given collection of pointers.
 * 
 * This template is needed to avoid having several identical overloaded functions 
 * for printing employee collections stored in different types,
 * such as vectors or sets, having the same iteration mechanism.
 * 
 * Note: This project was compiled for C++11. Starting from C++20, the `auto` keyword is supported in declarations, 
 * so we could simply declare `void printEmpCollection(const auto& emps)` and the type would be deduced automatically 
 * from the actual type of the `emps` container, and this template would not be required.
 * 
 * @tparam T Type of container of pointers to employee records
 * @param emps Container of pointers to employee records
 */
template <typename T>
void printEmpCollection(const T& emps);

#endif // PRINTERS_HPP