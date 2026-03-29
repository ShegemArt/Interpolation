#pragma once
#include <cmath>
#include <functional>
#include <vector>

using namespace std;

/// <summary>
/// Выдает значение функции, интерполированной методом Лагранжа, в заданной точке
/// </summary>
/// <param name="points">Узлы</param>
/// <param name="x">Точка</param>
/// <param name="func">Функция</param>
/// <returns></returns>
double LagrangeMethod(const vector<double>& points, double x,
    function<double(double)> func) {
    int n = points.size();
    double f = 0.0;

    for (int i = 0; i < n; i++) {
        double t = 1.0;
        for (int j = 0; j < n; j++) {
            if (j != i) {
                t *= (x - points[j]) / (points[i] - points[j]);
            }
        }
        f += t * func(points[i]);
    }
    return f;
}

/// <summary>
/// Выдает коэффициенты для интерполяции функции методом Ньюотона
/// </summary>
/// <param name="points">Узлы</param>
/// <param name="func">Функция</param>
/// <returns></returns>
vector<double> NewtonCoefficients(const vector<double>& points,
    function<double(double)> func) {
    int n = points.size();
    vector<double> p(n);

    p[0] = func(points[0]);

    for (int i = 1; i < n; i++) {
        double f = 0.0;
        for (int j = 0; j < i + 1; j++) {
            double g = 1.0;
            for (int k = 0; k < i + 1; k++) {
                if (k != j) {
                    g *= (points[j] - points[k]);
                }
            }
            g = 1 / g;
            f += (g * func(points[j]));
        }
        p[i] = f;
    }
    return p;
}

/// <summary>
/// Выдает значение функции, интерполированной методом Ньютона, в заданной точке
/// </summary>
/// <param name="points">Узлы</param>
/// <param name="x">Точка</param>
/// <param name="coef">Коэффициенты Ньютона</param>
/// <returns></returns>
double NewtonMethod(const vector<double>& points, double x,
    const vector<double>& coef) {
    int n = points.size();
    double f = coef[0];

    for (int i = 1; i < n; i++) {
        double g = coef[i];
        for (int j = 0; j < i; j++) {
            g *= (x - points[j]);
        }
        f += g;
    }
    return f;
}

/// <summary>
/// Выдает значение линейного сплайна функции в заданной точке
/// </summary>
/// <param name="points">Узлы</param>
/// <param name="x">Точка</param>
/// <param name="func">Функция</param>
/// <returns></returns>
double SplineLinear(const vector<double>& points, double x,
    function<double(double)> func) {
    int n = points.size();

    if (x == points[0]) return func(x);

    for (int i = 1; i < n; i++) {
        if (x < points[i]) {
            return ((func(points[i - 1]) - func(points[i])) /
                (points[i - 1] - points[i])) * x +
                ((func(points[i]) * points[i - 1] -
                    func(points[i - 1]) * points[i]) /
                    (points[i - 1] - points[i]));
        }
        if (x == points[i]) return func(x);
    }
    return func(points.back());
}

/// <summary>
/// Выдает значение квадратичного сплайна функции в заданной точке
/// </summary>
/// <param name="points">Узлы</param>
/// <param name="coef">Коэффициенты</param>
/// <param name="x">Точка</param>
/// <param name="func">Функция</param>
/// <returns></returns>
double SplineQuadratic(const vector<double>& points,
    const vector<double>& coef,
    double x,
    function<double(double)> func) {
    int n = points.size();

    if (x == points[0]) return func(x);

    for (int i = 1; i < n; i++) {
        if (x < points[i]) {
            return coef[3 * (i - 1)] * x * x +
                coef[3 * (i - 1) + 1] * x +
                coef[3 * (i - 1) + 2];
        }
        if (x == points[i]) return func(x);
    }
    return func(points.back());
}

/// <summary>
/// Выдает коэффициенты для квадратичного сплайна
/// </summary>
/// <param name="points">Узлы</param>
/// <param name="func">Функция</param>
/// <param name="dfunc">Производная функции</param>
/// <returns></returns>
vector<double> ComputeQuadraticSplineCoeffs(
    const vector<double>& points,
    function<double(double)> func,
    function<double(double)> dfunc
) {
    int n = points.size();
    vector<double> coef(3 * (n - 1));

    double x0 = points[0], x1 = points[1];
    double f0 = func(x0), f1 = func(x1);
    double a0 = (f1 - f0 - dfunc(x0) * (x1 - x0)) / ((x1 - x0) * (x1 - x0));
    double b0 = dfunc(x0) - 2 * a0 * x0;
    double c0 = f0 - a0 * x0 * x0 - b0 * x0;

    coef[0] = a0; coef[1] = b0; coef[2] = c0;

    // Остальные сегменты
    for (int i = 1; i < n - 1; i++) {
        double xi = points[i], xi1 = points[i + 1];
        double fi = func(xi), fi1 = func(xi1);

        double a_prev = coef[3 * (i - 1)];
        double b_prev = coef[3 * (i - 1) + 1];

        double ai = (fi1 - fi - (2 * a_prev * xi + b_prev) * (xi1 - xi)) / ((xi1 - xi) * (xi1 - xi));
        double bi = 2 * a_prev * xi + b_prev - 2 * ai * xi;
        double ci = fi - ai * xi * xi - bi * xi;

        coef[3 * i] = ai;
        coef[3 * i + 1] = bi;
        coef[3 * i + 2] = ci;
    }

    return coef;
}

/// <summary>
/// Выдает значение функции, интерполированной кубическим сплайном
/// </summary>
/// <param name="points">Узлы</param>
/// <param name="y1">Значение 1 производной в узле</param>
/// <param name="y2">Значение 2 производной в узле</param>
/// <param name="h">Длины между узлами</param>
/// <param name="x">Точка</param>
/// <param name="func">Функция</param>
/// <returns></returns>
double SplineCubic(const vector<double>& points,
    const vector<double>& y1,
    const vector<double>& y2,
    const vector<double>& h,
    double x,
    function<double(double)> func) {
    int n = points.size();

    if (x == points[0]) return func(x);

    for (int i = 1; i < n; i++) {
        if (x < points[i]) {
            return func(points[i - 1]) +
                y1[i - 1] * (x - points[i - 1]) +
                y2[i - 1] * pow(x - points[i - 1], 2) / 2 +
                (y2[i] - y2[i - 1]) *
                pow(x - points[i - 1], 3) / (6 * h[i - 1]);
        }
        if (x == points[i]) return func(x);
    }
    return func(points.back());
}