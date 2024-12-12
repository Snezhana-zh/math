import numpy as np
import math
import scipy.integrate as integrate
import csv

def f1(x):
    return 1.2 * x**2 - 10

def f2(x):
    return math.sqrt(x - 1)

def f3(x):
    return math.log(x + 1)

def f4(x):
    return math.sin(x)

def f5(x):
    return 1 / (1 + math.exp(x))

# Метод левых прямоугольников
def left_rectangles(f, a, b, N):
    h = (b - a) / N
    integral = 0
    for i in range(N):
        integral += f(a + i * h)
    return integral * h

# Метод правых прямоугольников
def right_rectangles(f, a, b, N):
    h = (b - a) / N
    integral = 0
    for i in range(1, N + 1):
        integral += f(a + i * h)
    return integral * h

# Метод средних прямоугольников
def middle_rectangles(f, a, b, N):
    h = (b - a) / N
    integral = 0
    for i in range(N):
        # xi = a + i * h
        # xi2 = a + (i + 1) * h
        # x = (xi + xi2) / 2 = (a + i * h + 0.5 * h)
        integral += f(a + i * h + 0.5 * h)
    return integral * h

# Метод трапеций
def trapezoid_method(f, a, b, N):
    h = (b - a) / N
    integral = 0.5 * (f(a) + f(b))
    for i in range(1, N):
        integral += f(a + i * h)
    return integral * h

# Метод Симпсона
def simpson(f, a, b, N):
    if N % 2 != 0:
        raise ValueError("N должно быть четным для метода Симпсона")
    h = (b - a) / N
    integral = f(a) + f(b)
    for i in range(1, N, 2): #1, n-1
        integral += 4 * f(a + i * h)
    for i in range(2, N-1, 2): #2, n-2
        integral += 2 * f(a + i * h)
    return integral * h / 3

# Метод Рунге
def runge(method, f, a, b, N):
    Ih = method(f, a, b, N)
    Ih2 = method(f, a, b, 2 * N)
    Ih4 = method(f, a, b, 4 * N)
    return np.log2(abs(Ih - Ih2) / abs(Ih2 - Ih4))

functions = [f1, f2, f3, f4, f5]
limits = [(-4, 1), (4, 10), (0, 1), (math.pi/3, math.pi), (0, 1)]
splits_nums = [10, 20, 50, 100, 1000]

methods = [left_rectangles, right_rectangles, middle_rectangles, trapezoid_method, simpson]
method_names = ["left_rectangles", "right_rectangles", "middle_rectangles", "trapezoid_method", "simpson"]

for f, (a, b) in zip(functions, limits):
    print("Функция: %s, Пределы: [%d, %d]" % (f.__name__, a, b))
    expected, error = integrate.quad(f, a, b)
    for method, name in zip(methods, method_names):
        print("Метод: %s" % (name))
        for N in splits_nums:
            try:
                result = method(f, a, b, N)                
                print("N = %d, Результат: %.10f, Ожидаемое значение: %.10f, eps = %.10f" % (N, result, expected, abs(expected - result)))
            except Exception as e:
                print("N = %d, Ошибка: %s" % (N, e))
        order = runge(method, f, a, b, 1000)
        print("Порядок метода(N = 1000): %.4f" % (order))

        order = runge(method, f, a, b, 2000)
        print("Порядок метода(N = 2000): %.4f" % (order))

        order = runge(method, f, a, b, 3000)
        print("Порядок метода(N = 3000): %.4f" % (order))