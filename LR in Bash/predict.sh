#!/usr/bin/bash

# @file predict.sh
# @author Andrei Batyrov (arbatyrov@edu.hse.ru)
# @brief Find and return (echo) `y_pred` for the given `x` (cli param $1) along with the slope and intercept read from stdin.
# @version 0.1
# @date 2023-09-09
# @copyright Copyright (c) 2023
# Based on https://en.wikipedia.org/wiki/Simple_linear_regression#Numerical_example
# Usage notes:
# 1. For best experience pipe this command with any command that writes to stdout to use it as the input:
# `echo 15 -3 | ./predict.sh 4` or 
# `echo 10 30 40 | ./fit.sh | ./predict.sh 4` or 
# `cat train_data.txt | ./fit.sh | ./predict.sh 4`.
# 2. Since bash does not support floats, input data and the model's parameters are integer values, which may lead to inaccurate results.

# Read space-separated slope and intercept from stdin into array `lr_params`
IFS=' ' read -a lr_params

slope=${lr_params[0]}
intercept=${lr_params[1]}

# Predict
x=$1
y_pred=$((x * slope + intercept))
echo $y_pred
