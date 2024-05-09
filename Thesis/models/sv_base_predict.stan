// SV Baseline model
// Volatility: stochastic

data
{
  int<lower=1> N_pred; // Number of test time points (equally spaced)
  vector[N_pred] h;    // Vector of learned log volatility at time t
  real y_mean;         // Learned mean price
}

generated quantities
{
  vector[N_pred] y_pred;

  // Predictions
  for (t in 1:N_pred)
  {
    y_pred[t] = normal_rng(y_mean, exp(h[t] / 2));  
  }
}