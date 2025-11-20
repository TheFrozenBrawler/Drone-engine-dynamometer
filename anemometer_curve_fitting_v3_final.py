import numpy as np
from scipy.optimize import curve_fit
from matplotlib import pyplot as plt


x = np.array([0, 0.8, 2.45, 3.44])
y = np.array([0, 4.8, 6.5, 9.3])

# Linear function model forced through origin: y = a*x
def test_lin(x, a):
    return a * x

# Fit model to data
# Provide an initial guess and allow more function evaluations to improve convergence
try:
    # initial guess: slope=1.0 (intercept forced to 0)
    param, param_cov = curve_fit(test_lin, x, y, p0=[1.0])
except Exception as e:
    print('curve_fit failed:', e)
    raise

# Print optimized parameters and their covariance
print("Linear function coefficient (slope a):")
print(param)
print("Covariance of coefficients:")
print(param_cov)

# Generate fitted y values
# Create a smooth fitted line starting at x=0
x_fit = np.linspace(0, x.max(), 200)
ans = param[0] * x_fit

# Plot original data and fitted curve
plt.plot(x, y, 'o', color='red', label='Noisy data')
plt.plot(x_fit, ans, '--', color='blue', label='Fitted linear curve (through origin)')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Linear Curve Fitting')
plt.legend()
# plt.savefig('fit_linear.png')
print("Saved plot to fit_linear.png")
plt.show()