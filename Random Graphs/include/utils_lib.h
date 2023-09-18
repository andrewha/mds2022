/**
 * @file utils_lib.h
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief Utils library declarations
 * @version 0.1
 * @date 2023-08-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdbool.h>

void show_array(double *array, unsigned short length, char sep, bool wide);

int comp_func_asc(const void *a, const void *b);

int comp_func_desc(const void *a, const void *b);

void sort_array(double *array, unsigned short length, bool ascending);
