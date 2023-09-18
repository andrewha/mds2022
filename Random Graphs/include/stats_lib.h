/**
 * @file stats_lib.h
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief Mathematical statistics library declarations
 * @version 0.1
 * @date 2023-08-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdbool.h>

unsigned long long binom(unsigned short n, unsigned short k);

bool bernoulli_rv(double p);

double mean(double *array, unsigned short length);

double max(double *array, unsigned short length);

double min(double *array, unsigned short length);

double quantile(double *array, unsigned short length, double q);
