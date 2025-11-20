import numpy as np
from scipy.optimize import curve_fit
from matplotlib import pyplot as plt

# Generate x values between 0 and 1
# x = np.linspace(0, 1, num=40)

# Generate y values using exponential function with added noise
# y = 3.45 * np.exp(1.334 * x) + np.random.normal(size=40)
x = np.array([0, 0.8, 2.45, 3.44])
y = np.array([0, 4.8, 6.5, 9.3])

# Exponential function model
def test_exp(x, a, b):
    # Use a form that is zero at x=0: a*(exp(b*x)-1)
    # This forces the fitted curve to pass through (0, 0) while
    # still allowing exponential growth for x>0.
    return a * (np.exp(b * x) - 1)

# Fit model to data
# Provide an initial guess and allow more function evaluations to improve convergence
try:
    param, param_cov = curve_fit(test_exp, x, y, p0=[y.max() or 1.0, 0.5], maxfev=5000)
except Exception as e:
    print('curve_fit failed:', e)
    raise

# Print optimized parameters and their covariance
print("Exponential function coefficients:")
print(param)
print("Covariance of coefficients:")
print(param_cov)

# Generate fitted y values
ans = param[0] * (np.exp(param[1] * x) - 1)

# Plot original data and fitted curve
plt.plot(x, y, 'o', color='red', label='Noisy data')
plt.plot(x, ans, '--', color='blue', label='Fitted curve (passes through origin)')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Exponential Curve Fitting')
plt.legend()
plt.show()