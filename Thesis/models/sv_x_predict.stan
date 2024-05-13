/**
 * @file sv_x_predict.stan
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief SV Exogenous model: Predict method
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
  int<lower=1> N_pred; // Number of test time points (equally spaced)
  vector[N_pred] h;    // Vector of learned log volatility at time t
  real y_mean;         // Learned mean price
  real alpha;          // Learned params of exogenous regressors
  real beta_1;
  real beta_2;
  real beta_3;
  real gamma;
  real xi;
  vector[N_pred] Temperature_pred; // Vector of test Temperature at t
  vector[N_pred] Weekday_pred;     // Vector of test Weekday at time t
}

generated quantities
{
  vector[N_pred] y_pred;
   
  // Predictions
  // We do not predict y_pred[t = 1],
  // since we do not have y_pred[t - 1 = 0] and Temperature[t - 1 = 0]
  // So predict the first value without Temperature and AR(1)
  // and continue from index 2
  y_pred[1] = normal_rng(y_mean + gamma * Weekday_pred[1], 
                         exp(h[1] / 2));
  for (t in 2:N_pred)
  {
    y_pred[t] = normal_rng(y_mean + 
                           alpha * y_pred[t - 1] +
                           beta_3 * pow(Temperature_pred[t - 1], 3) + 
                           beta_2 * pow(Temperature_pred[t - 1], 2) + 
                           beta_1 * Temperature_pred[t - 1] + 
                           gamma * Weekday_pred[t] +
                           xi, 
                           exp(h[t] / 2));
  }
}