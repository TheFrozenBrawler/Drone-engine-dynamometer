import numpy as np
import matplotlib.pyplot as plt

x = np.array([0, 0.8, 2.45, 3.44])
y = np.array([0, 4.8, 6.5, 9.3])

# macierz dopasowania: [x, x^2]
A = np.vstack([x, x**2]).T
coeffs, _, _, _ = np.linalg.lstsq(A, y, rcond=None)

a1, a2 = coeffs
print("a1 =", a1, "a2 =", a2)

x_dense = np.linspace(0, max(x), 200)
y_fit = a1 * x_dense + a2 * x_dense**2

plt.scatter(x, y)
plt.plot(x_dense, y_fit)
plt.show()
