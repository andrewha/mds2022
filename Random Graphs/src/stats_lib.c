/**
 * @file stats_lib.c
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief Library for mathematical statistics functions
 * @version 0.1
 * @date 2023-08-28
 *
 * @copyright Copyright (c) 2023
 */
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "utils_lib.h"

/**
 * @brief Macro for `bernoulli_rv()` non-success return value.
 */
#define NON_SUCCESS false

/**
 * @brief Macro for `bernoulli_rv()` success return value.
 */
#define SUCCESS true

/**
 * @brief Calculate binomial coefficient \f$ C(n, k) \f$.
 *
 * @param n Integer number \f$ n \f$
 * @param k Integer number \f$ k \f$
 * @return Binomial coefficient
 *
 * Implementation notes:
 * 1. Optimized algorithm without explicit calculation of factorial.
 * 2. Integer overflow unsafe, since the result size is not checked during calculation and before return.
 * 3. Time complexity: \f$ \mathcal{O}(r) \f$, where \f$ r = \min(k, n - k) \f$.
 */
unsigned long long binom(unsigned short n, unsigned short k)
{
    unsigned short i;
    unsigned long long coeff = 1;

    /* Special cases */
    if ((k == 0) | (k == n))
    {
        return coeff;
    }
    if ((k == 1) | (k == n - 1))
    {
        return n;
    }
    if ((k == 2) | (k == n - 2))
    {
        return n * (n - 1) / 2;
    }

    /* General case */
    if (k > n - k)
    {
        k = n - k;
    }
    for (i = 0; i < k; i++)
    {
        coeff *= (n - i);
        coeff /= (i + 1);
    }
    return coeff;
}

/**
 * @brief Return Bernoulli distributed random variable.
 *
 * @param p Probability of a single outcome
 * @return `True (1)` or `false (0)`
 *
 * Implementation notes:
 * 1. Simulates Bernoulli process and returns either 1 or 0. If \f$ p = 0.5 \f$, returned values will simulate tosses of a fair coin (H = 1, T = 0).
 * 2. First uniformly distributed random variable with `p = 1 / RAND_MAX` is obtained with `rand()` which is then transformed into Bernoulli distributed random variable.
 * 3. Assumes that `RAND_MAX` might be at least 16 bit, thus the result of `rand()` is stored in the unsigned int variable.
 * 4. Time complexity: \f$ \mathcal{O}( \f$ `rand()` \f$ ) \f$.
 *
 * Usage notes:
 * 1. Init random seed by calling `srand(time(NULL))`, if needed, before calling `bernoulli_rv()`.
 */
bool bernoulli_rv(double p)
{
    bool outcome = NON_SUCCESS;
    unsigned int quantile_p;
    unsigned int uniform_rv;

    quantile_p = p * RAND_MAX;
    uniform_rv = rand();
    if (uniform_rv <= quantile_p)
    {
        outcome = SUCCESS;
    }
    return outcome;
}

/**
 * @brief Find mean of an array.
 *
 * @param array Array of real numbers
 * @param length Length of the array
 * @return Arithmetic mean of the array's values
 *
 * Implementation notes:
 * 1. Array's length is limited by `unsigned short` size, i.e. most likely 65536 elements max.
 * 2. Time complexity: \f$ \mathcal{O}(length) \f$.
 */
double mean(double *array, unsigned short length)
{
    double mean = 0.0;
    unsigned short i;
    for (i = 0; i < length; i++)
    {
        mean += array[i];
    }
    mean /= length;
    return mean;
}

/**
 * @brief Find maximal value in an array.
 *
 * @param array Array of real numbers
 * @param length Length of the array
 * @return Arithmetic maximum of the array's values
 *
 * Implementation notes:
 * 1. Array's length is limited by `unsigned short` size, i.e. most likely 65536 elements max.
 * 2. Time complexity: \f$ \mathcal{O}(length) \f$.
 */
double max(double *array, unsigned short length)
{
    double max = array[0];
    unsigned short i;
    for (i = 0; i < length; i++)
    {
        if (max < array[i])
        {
            max = array[i];
        }
    }
    return max;
}

/**
 * @brief Find minimal value in an array.
 *
 * @param array Array of real numbers
 * @param length Length of the array
 * @return Arithmetic minimum of the array's values
 *
 * Implementation notes:
 * 1. Array's length is limited by `unsigned short` size, i.e. most likely 65536 elements max.
 * 2. Time complexity: \f$ \mathcal{O}(length) \f$.
 */
double min(double *array, unsigned short length)
{
    double min = array[0];
    unsigned short i;
    for (i = 0; i < length; i++)
    {
        if (min > array[i])
        {
            min = array[i];
        }
    }
    return min;
}

/**
 * @brief Find \f$ q \f$-th quantile of an array.
 *
 * @param array Array of real numbers
 * @param length Length of the array
 * @param q Quantile \f$ \in [0, 1] \f$
 * @return Array's value corresponding to \f$ q \f$
 *
 * Implementation notes:
 * 1. Array's length is limited by `unsigned short` size, i.e. most likely 65536 elements max.
 * 2. Input array is first copied to a new array, so that the original array is not corrupted,
 * and then the copy is sorted in ascending order in place by employing the `sort_array()` function.
 * 3. If array's length is even, the arithmetic mean (average) of two neighbor elements is returned.
 * Example: median \f$ (q = 0.5) \f$ for the array `{1, 2, 3, 4}` is \f$ (2 + 3) / 2 = 2.5 \f$.
 * 4. Time complexity: \f$ \max(\mathcal{O}(length), \mathcal{O}( \f$ `sort_array()` \f$ )) \f$.
 */
double quantile(double *array, unsigned short length, double q)
{
    double sorted_array[length];
    unsigned short i, quantile_idx;
    for (i = 0; i < length; i++)
    {
        sorted_array[i] = array[i];
    }
    sort_array(sorted_array, length, true); /* Sort in place */
    quantile_idx = (unsigned short)(q * (length - 1));
    if (length & 1)
    { /* Odd length */
        return sorted_array[quantile_idx];
    }
    return (sorted_array[quantile_idx] + sorted_array[quantile_idx + 1]) / 2;
}