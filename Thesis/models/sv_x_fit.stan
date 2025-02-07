/**
 * @file sv_x_fit.stan
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief SV Exogenous model: Fit method
 * Exogenous regressors:
 * - Temperature
 * - Weekday
 * - AR(1)
 * @version 0.1
 * @date 2024-05-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

data
{
  int<lower=1> N;        // Number of train time points (equally spaced)
  vector[N] y;           // Vector of train prices at time t
  real y_mean;           // Mean price
  vector[N] Temperature; // Vector of train Temperature at time t
  vector[N] Weekday;     // Vector of train Weekday at time t
}

parameters
{
  real mu;                     // Mean log volatility
  real<lower=-1, upper=1> phi; // Persistence of volatility
  real<lower=0> sigma;         // White noise shock scale
  vector[N] h_std;             // Standardized log volatility at time t
  real alpha;                  // Param of AR(1) component
  real beta_1;                 // Params of Temperature regressor
  real beta_2;
  real beta_3;
  real gamma;                  // Param of Weekday regressor
  real xi;                     // Intercept for all exogenous regressors
  
}

transformed parameters
{
  // Log volatility at time t; now h ~ normal(0, sigma)
  vector[N] h = h_std * sigma;
  h[1] = h[1] / sqrt(1 - pow(phi, 2)); // Rescale h[1]
  h = h + mu;
  for (t in 2:N)
  {
    h[t] = h[t] + phi * (h[t - 1] - mu);
  }
}

model
{
  // Priors recommended by Stan
  phi ~ uniform(-1, 1);
  sigma ~ cauchy(0, 5);
  mu ~ cauchy(0, 10);
  h_std ~ std_normal();
  
  // Model
  // We do not model y[t = 1],
  // since we do not have y[t - 1 = 0] and Temperature[t - 1 = 0]
  // So assume the first value without Temperature and AR(1)
  // and continue from index 2
  y[1] ~ normal(y_mean + gamma * Weekday[1], 
                exp(h[1] / 2));
  for (t in 2:N)
  {
    y[t] ~ normal(y_mean + 
                  alpha * y[t - 1] +
                  beta_3 * pow(Temperature[t - 1], 3) + 
                  beta_2 * pow(Temperature[t - 1], 2) + 
                  beta_1 * Temperature[t - 1] + 
                  gamma * Weekday[t] +
                  xi, 
                  exp(h[t] / 2));
  }
}