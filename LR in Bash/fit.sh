#!/usr/bin/bash

# @file fit.sh
# @author Andrei Batyrov (arbatyrov@edu.hse.ru)
# @brief Find and return (echo) the slope and intercept for single variable simple linear regression model.
# @version 0.1
# @date 2023-09-09
# @copyright Copyright (c) 2023
# Based on https://en.wikipedia.org/wiki/Simple_linear_regression#Numerical_example
# Usage notes:
# 1. For best experience pipe this command with any command that writes to stdout to use it as the input:
# `echo 10 30 40 | ./fit.sh` or 
# `cat train_data.txt | ./fit.sh`.
# 2. Since bash does not support floats, input data and the model's parameters are integer values, which may lead to inaccurate results.

# Read newline-separated values from stdin and append to array `y_train`
y_train=()
while IFS= read val;
    do 
        y_train+=($val);
    done
n=${#y_train[@]}

# Find five sums
# `X_train` is just the index variable starting from 1
S_x=S_xx=S_y=S_yy=S_xy=0
for i in ${!y_train[@]};
    do
        X_train=$((i + 1))
        S_x=$((S_x + X_train))
        S_xx=$((S_xx + X_train ** 2))
        S_y=$((S_y + y_train[$i]))
        S_yy=$((S_yy + y_train[$i] ** 2))
        S_xy=$((S_xy + X_train * y_train[$i]))
    done

# Find slope and intercept
slope=$(((n * S_xy - S_x * S_y) / (n * S_xx - S_x ** 2)))
intercept=$(((S_y - slope * S_x) / n))
echo $slope $intercept
