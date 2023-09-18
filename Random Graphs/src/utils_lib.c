/**
 * @file utils_lib.c
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief Library for various handy functions
 * @version 0.1
 * @date 2023-08-28
 *
 * @copyright Copyright (c) 2023
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

/**
 * @brief Print out an array
 *
 * @param array Array of real numbers
 * @param length Length of the array
 * @param sep Character to separate array's elements
 * @param wide Display style: `true` -- shape \f$ (1 \times length) \f$ (wide), `false` -- shape \f$ (length \times 1) \f$ (tall).
 *
 * Implementation notes:
 * 1. Array's length is limited by `unsigned short` size, i.e. most likely 65536 elements max.
 * 2. Display precision cannot be set -- as per `printf()`, by default 6 decimal places are printed.
 * 4. Time complexity: \f$ \mathcal{O}(length) \f$.
 */
void show_array(double *array, unsigned short length, char sep, bool wide)
{
    unsigned short i;
    // printf("[");
    if (wide)
    {
        for (i = 0; i < length; i++)
        {
            printf("%lf%c", array[i], sep);
        }
        printf("\n");
    }
    else
    {
        for (i = 0; i < length; i++)
        {
            printf("%lf%c\n", array[i], sep);
        }
    }
    // printf("%lf]\n", array[length - 1]);
}

/**
 * @brief Compare function for ascending `qsort()` used in `sort_array()`.
 *
 * @param a The first value to compare
 * @param b The second value to compare
 * @return Sort flag: `0 (a == b), 1 (a > b), -1 (a < b)`
 */
int comp_func_asc(const void *a, const void *b)
{
    double *x = (double *)a;
    double *y = (double *)b;
    if (*x < *y)
    {
        return -1;
    }
    else if (*x > *y)
    {
        return 1;
    }
    return 0; /* *x == *y */
}

/**
 * @brief Compare function for descending `qsort()` used in `sort_array()`.
 *
 * @param a The first value to compare
 * @param b The second value to compare
 * @return Sort flag: `0 (a == b), 1 (a < b), -1 (a > b)`
 */
int comp_func_desc(const void *a, const void *b)
{
    double *x = (double *)a;
    double *y = (double *)b;
    if (*x > *y)
    {
        return -1;
    }
    else if (*x < *y)
    {
        return 1;
    }
    return 0; /* *x == *y */
}

/**
 * @brief Sort array's elements either in ascending or descending order.
 *
 * @param array Array of real numbers
 * @param length Length of the array
 * @param ascending If `true`, sort in ascending order, otherwise in descending order
 *
 * Implementation notes:
 * 1. Array's length is limited by `unsigned short` size, i.e. most likely 65536 elements max.
 * 2. Employs `qsort()` with `comp_func_asc()` and `comp_func_desc()`.
 * 3. Time complexity: might be \f$ \mathcal{O}(length \log(length)) \f$, but in fact is not guaranteed \cite 7.
 */
void sort_array(double *array, unsigned short length, bool ascending)
{
    if (ascending)
    {
        qsort(array, length, sizeof(*array), comp_func_asc);
    }
    else
    {
        qsort(array, length, sizeof(*array), comp_func_desc);
    }
}