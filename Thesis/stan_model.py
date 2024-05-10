import numpy as np
from sklearn.base import BaseEstimator, RegressorMixin
from sklearn.utils.validation import check_is_fitted
import stan
# This is needed to build Stan code in Jupyter Notebook
# https://github.com/langchain-ai/langchain/issues/8494#issuecomment-1658998064
import nest_asyncio
nest_asyncio.apply()

class StanModel(BaseEstimator, RegressorMixin):

    def __init__(self, 
                kind: str='',
                name: str='', 
                stan_code_fit: str='', 
                stan_code_predict: str='', 
                num_samples: int=1000, 
                random_seed: int=0, 
                num_chains=1):
        '''
        Init a Stan model.
        '''
        
        self.kind = kind
        self.name = name
        self.stan_code_fit = stan_code_fit
        self.stan_code_predict = stan_code_predict
        self.num_samples = num_samples
        self.random_seed = random_seed
        self.num_chains = num_chains
        self.fit_result_df_ = None # This will store all learned parameters
        self.h_ = None # Log volatility
        
        if self.kind not in ('sv_base', 'sv_x'):
            raise ValueError("Model's kind must be of one ('sv_base', 'sv_x')")
    

    def fit(self, X, y):
        '''
        Fit a Stan model over `self.num_predicts` inference draws.
        This method returns `self` to be in compliance with scikit-learn.
        '''
        
        if self.kind == 'sv_base':
            stan_data = {'N': y.shape[0], 
                         'y': y.values}
        
        if self.kind == 'sv_x':
            stan_data = {'N': y.shape[0], 
                         'y': y.values, 
                         'Temperature': X['Temperature'].values,
                         'Weekday': X['Weekday'].values}

        posterior = stan.build(self.stan_code_fit, data=stan_data, random_seed=self.random_seed)
        self.fit_result_df_ = posterior.sample(num_chains=self.num_chains, num_samples=self.num_samples).to_frame()
        self.h_ = self.fit_result_df_.filter(like='h.', axis=1)
        
        return self


    def predict_many(self, X):
        '''
        Generate distribution of predictions over `self.num_predicts` posterior draws.
        This method returns an array of shape (self.num_samples, length of X[0]).
        X: array-like of y for 'sv_base' or exogenous regressor(s) for 'sv_x'
        '''
        
        check_is_fitted(self)
        
        if self.kind == 'sv_base':
            h = self.h_.mean(axis=0).values[-X.shape[0]:] # Take most recent log volatility points
            y_mean = self.fit_result_df_['y_mean'].mean(axis=0)
            stan_data = {'N_pred': X.shape[0], 
                         'h': h,
                         'y_mean': y_mean}
        
        if self.kind == 'sv_x':
            h = self.h_.mean(axis=0).values[-X.shape[0]:] # Take most recent log volatility points
            y_mean = self.fit_result_df_['y_mean'].mean(axis=0)
            beta_1 = self.fit_result_df_['beta_1'].mean(axis=0)
            beta_2 = self.fit_result_df_['beta_2'].mean(axis=0)
            beta_3 = self.fit_result_df_['beta_3'].mean(axis=0)
            gamma = self.fit_result_df_['gamma'].mean(axis=0)
            alpha = self.fit_result_df_['alpha'].mean(axis=0)
            xi = self.fit_result_df_['xi'].mean(axis=0)
            stan_data = {'N_pred': X.shape[0], 
                         'h': h,
                         'y_mean': y_mean,
                         'beta_1': beta_1,
                         'beta_2': beta_2,
                         'beta_3': beta_3,
                         'gamma': gamma,
                         'alpha': alpha,
                         'xi': xi,
                         'Temperature_pred': X['Temperature'].values,
                         'Weekday_pred': X['Weekday'].values}
        
        posterior = stan.build(self.stan_code_predict, data=stan_data, random_seed=self.random_seed)
        y_pred = posterior.fixed_param(num_chains=self.num_chains, num_samples=self.num_samples)
        
        return y_pred.to_frame().filter(like='y_pred.', axis=1)


    def predict(self, X):
        '''
        Generate mean predictions.
        This method returns an array of shape (length of X[0], ) to be in compliance with scikit-learn.
        X: tuple of y and fit_df
        '''
        
        check_is_fitted(self)
        y_pred_mat = self.predict_many(X)

        return y_pred_mat.mean(axis=0)


    def predict_ci(self, X, ci: list):
        '''
        Generate low and high predictions.
        This method returns a tuple of low and high arrays of shape (length of X[0], ).
        X: tuple of y and fit_df
        ci: list of low and high percentiles (2 values)
        '''

        check_is_fitted(self)        
        y_pred_mat = self.predict_many(X)
        y_pred_ci = np.percentile(y_pred_mat, ci, axis=0)

        return y_pred_ci[0], y_pred_ci[1]


    def get_volatility(self):
        '''
        Return log volatility learned during fitting over `self.num_samples` inference draws.
        h[t] = exp(log_h[t] / 2)
        '''
    
        return np.exp(self.h_ / 2)


    def forecast_many(self, X, y, num_days=1):
        '''
        Generate distribution of forecasts for `num_days` ahead over `self.num_predicts` posterior draws.
        '''

        check_is_fitted(self)
        y_fores = []
        forecast_start_idx = X.shape[0] - num_days

        if self.kind == 'sv_base':
            for t in range(num_days):
                forecast_curr_idx = forecast_start_idx + t
                # Move forecast window and fit
                self.fit(X[t:forecast_curr_idx], y[t:forecast_curr_idx])
                y_mean = self.fit_result_df_['y_mean'].mean(axis=0)
                h = self.h_.mean(axis=0).values[-1] # Most recent learned volatility
                # Predict one day ahead
                y_fore = np.random.normal(y_mean, np.exp(h / 2), size=self.num_samples)
                y_fores.append(y_fore)
                
        if self.kind == 'sv_x':
            for t in range(num_days):
                forecast_curr_idx = forecast_start_idx + t
                # Move forecast window and fit
                self.fit(X[t:forecast_curr_idx], y[t:forecast_curr_idx])
                y_mean = self.fit_result_df_['y_mean'].mean(axis=0)
                h = self.h_.mean(axis=0).values[-1] # Most recent learned volatility
                beta_1 = self.fit_result_df_['beta_1'].mean(axis=0)
                beta_2 = self.fit_result_df_['beta_2'].mean(axis=0)
                beta_3 = self.fit_result_df_['beta_3'].mean(axis=0)
                gamma = self.fit_result_df_['gamma'].mean(axis=0)
                alpha = self.fit_result_df_['alpha'].mean(axis=0)
                xi = self.fit_result_df_['xi'].mean(axis=0)
                # Predict one day ahead
                y_fore = np.random.normal(y_mean + 
                                          alpha * y[forecast_curr_idx - 1] +
                                          beta_3 * X['Temperature'].values[forecast_curr_idx - 1] ** 3 + 
                                          beta_2 * X['Temperature'].values[forecast_curr_idx - 1] ** 2 + 
                                          beta_1 * X['Temperature'].values[forecast_curr_idx - 1] + 
                                          gamma * X['Weekday'].values[forecast_curr_idx] +
                                          xi, np.exp(h / 2), size=self.num_samples)
                y_fores.append(y_fore)
        
        return np.array(y_fores)
