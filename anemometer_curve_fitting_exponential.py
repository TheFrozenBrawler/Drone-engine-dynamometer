import numpy as np
from scipy.optimize import curve_fit
from matplotlib import pyplot as plt


x = np.array([0, 0.7, 0.85, 2.45, 3.2, 6.3])  # [ V ] - raw data from anemometer
y = np.array([0, 14.5, 15.5, 22, 31.5, 43])    # [ m/s ] - data from commercial anemometer

def test_exp(x, a, b):
    return a * np.sqrt(b*x)

# Fit model to data
param, param_cov = curve_fit(test_exp, x, y)

# Print optimized parameters and their covariance
print("Exponential function coefficients:")
print(param)
print("Covariance of coefficients:")
print(param_cov)

# Generate fitted y values
# ans_exp = param[0] * np.exp(param[1] * x)
ans_sqrt = param[0] * np.sqrt(param[1]*x)

# Plot original data and fitted curve
plt.plot(x, y, 'o', color='red', label='Noisy data')
plt.plot(x, ans_sqrt, '--', color='blue', label='Fitted curve')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Exponential Curve Fitting')
plt.legend()
plt.show()