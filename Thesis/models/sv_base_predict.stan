/**
 * @file sv_base_predict.stan
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief SV Baseline model: Predict method
 * Exogenous regressors: None
 * @version 0.1
 * @date 2024-05-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

data
{
  int<lower=1> N_pred; // Number of test time points (equally spaced)
  vector[N_pred] h;    // Vector of learned log volatility at time t
  real y_mean;         // Learned mean price
}

generated quantities
{
  vector[N_pred] y_pred;

  // Generate posterior predictive distribution
  for (t in 1:N_pred)
  {
      y_pred[t] = normal_rng(y_mean, exp(h[t] / 2));
  }
  
}