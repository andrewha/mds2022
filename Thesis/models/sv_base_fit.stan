/**
 * @file sv_base_fit.stan
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief SV Baseline model: Fit method
 * Exogenous regressors: None
 * @version 0.1
 * @date 2024-05-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

data
{
  int<lower=1> N; // Number of train time points (equally spaced)
  vector[N] y;    // Vector of train prices at time t
  real y_mean;    // Mean price
}

parameters
{
  real mu;                     // Mean log volatility
  real<lower=-1, upper=1> phi; // Persistence of volatility
  real<lower=0> sigma;         // White noise shock scale
  vector[N] h_std;             // Standardized log volatility at time t
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
  y ~ normal(y_mean, exp(h / 2));
}
