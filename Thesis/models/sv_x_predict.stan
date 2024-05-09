// SV Exogenous model
// Volatility: stochastic
// Exogenous regressors

data
{
  int<lower=1> N_pred;             // Number of test time points (equally spaced)
  vector[N_pred] h;                // Vector of learned log volatility at time t
  real y_mean;                     // Learned mean price
  real alpha;                      // Learned params of exogenous regressor(s)
  real beta_1;
  real beta_2;
  real beta_3;
  real gamma;
  real xi;
  vector[N_pred] Temperature_pred; // Vector of test Air temperature exogenous regressor at time t
  vector[N_pred] Weekday_pred;     // Vector of test Weekday exogenous regressor at time t
}

generated quantities
{
  vector[N_pred] y_pred;
   
  // Predictions
  // We do not predict y_pred[t = 1], since we do not have y_pred[t - 1 = 0] and Temperature[t - 1 = 0]
  // So predict the first value without Temperature and AR(1) and continue from index 2
  y_pred[1] = normal_rng(y_mean + gamma * Weekday_pred[1], exp(h[1] / 2));
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