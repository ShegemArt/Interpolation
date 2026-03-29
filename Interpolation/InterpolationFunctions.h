#pragma once
#include <cmath>
#include <functional>
#include <vector>

using namespace std;

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