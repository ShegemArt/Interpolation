#include <iostream>
#include <vector>
#include <numbers>
#include <cmath>
#include "InterpolationFunctions.h"
#include <functional>
#include <SFML/Graphics.hpp>

using namespace std;

// функция
double Function(double x) {
    return tan(x) - cos(x) + 0.1;
}

// производная
double dFunction(double x) {
    return 1 / (cos(x) * cos(x)) + sin(x);
}

vector<double> uniformNodes(double a, double b, int n) {
    vector<double> p(n);
    for (int i = 0; i < n; i++) {
        p[i] = a + i * (b - a) / (n - 1);
    }
    return p;
}

vector<double> chebyshevNodes(double a, double b, int n) {
    vector<double> p(n);
    p[0] = a;
    p[n - 1] = b;

    for (int i = 0; i < n - 2; i++) {
        p[i + 1] = 0.5 * ((b - a) *
            cos((2 * i + 1) * std::numbers::pi / (2 * n)) + (b + a));
    }
    return p;
}

//int main() {
//
//    double a, b;
//    int n;
//
//    cout << "Enter interval [a, b]: ";
//    cin >> a >> b;
//
//    cout << "Enter number of nodes n: ";
//    cin >> n;
//
//    int nodeType;
//    cout << "Choose nodes:\n";
//    cout << "1 - Uniform\n";
//    cout << "2 - Chebyshev\n";
//    cin >> nodeType;
//
//    vector<double> p;
//
//    if (nodeType == 1) {
//        p = uniformNodes(a, b, n);
//    }
//    else {
//        p = chebyshevNodes(a, b, n);
//    }
//
//    int method;
//    cout << "\nChoose method:\n";
//    cout << "1 - Lagrange\n";
//    cout << "2 - Newton\n";
//    cout << "3 - Spline 1 (linear)\n";
//    cout << "4 - Spline 2 (quadratic)\n";
//    cout << "5 - Spline 3 (cubic)\n";
//    cin >> method;
//
//    vector<double> coef;
//    vector<double> y1(n - 1), y2(n), h(n - 1);
//
//    // Newton
//    if (method == 2) {
//        coef = NewtonCoefficients(p, Function);
//    }
//
//    // spline32
//    if (method == 5) {
//        for (int i = 0; i < n - 1; i++) {
//            h[i] = p[i + 1] - p[i];
//        }
//
//        for (int i = 0; i < n; i++) y2[i] = 0;
//
//        for (int i = 0; i < n - 1; i++) {
//            y1[i] = (Function(p[i + 1]) - Function(p[i])) / h[i]
//                - y2[i + 1] * h[i] / 6
//                - y2[i] * h[i] / 3;
//        }
//    }
//    
//    int k = 1000;
//    double maxError = 0;
//
//    for (int i = 0; i < k; i++) {
//        double x = a + i * (b - a) / (k - 1);
//        double real = Function(x);
//        double approx = 0;
//
//        switch (method) {
//        case 1:
//            approx = LagrangeMethod(p, x, Function);
//            break;
//        case 2:
//            approx = NewtonMethod(p, x, coef);
//            break;
//        case 3:
//            approx = SplineLinear(p, x, Function);
//            break;
//        case 4:
//            approx = SplineQuadratic(p, coef, x, Function);
//            break;
//        case 5:
//            approx = SplineCubic(p, y1, y2, h, x, Function);
//            break;
//        }
//
//        maxError = max(maxError, abs(real - approx));
//    }
//
//    //================ ВЫВОД =================
//    cout << "\nRESULT:\n";
//    cout << "Interval: [" << a << ", " << b << "]\n";
//    cout << "Nodes count: " << n << "\n";
//
//    if (nodeType == 1)
//        cout << "Nodes type: Uniform\n";
//    else
//        cout << "Nodes type: Chebyshev\n";
//
//    cout << "Max error = " << maxError << endl;
//
//    return 0;
//}

int main() {

    //const int W = 800;
    //RenderWindow window(VideoMode(W, W), "Functions");
    //RenderWindow window2(VideoMode(W, W), "Errors");

    //double a = 5, b = 7.5;
    //int n = 10;

    ////====== УЗЛЫ ======
    //vector<double> p(n);
    //for (int i = 0; i < n; i++) {
    //    p[i] = a + i * (b - a) / (n - 1);
    //}

    ////====== НЬЮТОН ======
    //vector<double> coef = NewtonCoefficients(p, Function);

    ////====== СПЛАЙН ======
    //vector<double> h(n - 1), y2(n, 0), y1(n - 1);

    //for (int i = 0; i < n - 1; i++)
    //    h[i] = p[i + 1] - p[i];

    //for (int i = 0; i < n - 1; i++) {
    //    y1[i] = (Function(p[i + 1]) - func(p[i])) / h[i]
    //        - y2[i + 1] * h[i] / 6
    //        - y2[i] * h[i] / 3;
    //}

    ////====== НАСТРОЙКИ ======
    //int x0 = W / 4;
    //int y0 = W / 2;
    //double scale = 80;

    //CircleShape realP(1.5f), lagP(1.5f), newtP(1.5f), splP(1.5f);
    //realP.setFillColor(Color::Red);
    //lagP.setFillColor(Color::Blue);
    //newtP.setFillColor(Color::Green);
    //splP.setFillColor(Color::Magenta);

    //CircleShape errN(2), errS(2);
    //errN.setFillColor(Color::Green);
    //errS.setFillColor(Color::Magenta);

    //int k = 2000;

    //double maxErrN = 0;
    //double maxErrS = 0;

    ////================ ОСНОВНОЙ ЦИКЛ =================
    //while (window.isOpen() || window2.isOpen()) {

    //    Event e;
    //    while (window.pollEvent(e))
    //        if (e.type == Event::Closed) window.close();

    //    while (window2.pollEvent(e))
    //        if (e.type == Event::Closed) window2.close();

    //    window.clear(Color::Black);
    //    window2.clear(Color::Black);

    //    for (int i = 0; i < k; i++) {
    //        double x = a + i * (b - a) / k;

    //        double real = Function(x);
    //        double lag = LagrangeMethod(p, x, Function);
    //        double newt = NewtonMethod(p, x, coef);
    //        double spl = SplineCubic(p, y1, y2, h, x, Function);

    //        double err1 = abs(real - newt);
    //        double err2 = abs(real - spl);

    //        maxErrN = max(maxErrN, err1);
    //        maxErrS = max(maxErrS, err2);

    //        double X = x0 + x * scale;

    //        realP.setPosition(X, y0 - real * scale);
    //        lagP.setPosition(X, y0 - lag * scale);
    //        newtP.setPosition(X, y0 - newt * scale);
    //        splP.setPosition(X, y0 - spl * scale);

    //        errN.setPosition(X, y0 - err1 * scale);
    //        errS.setPosition(X, y0 - err2 * scale);

    //        window.draw(realP);
    //        window.draw(lagP);
    //        window.draw(newtP);
    //        window.draw(splP);

    //        window2.draw(errN);
    //        window2.draw(errS);
    //    }

    //    window.display();
    //    window2.display();
    //}

    //cout << "Max Newton error = " << maxErrN << endl;
    //cout << "Max Spline error = " << maxErrS << endl;

    //return 0;
}