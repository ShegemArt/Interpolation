#include <iostream>
#include <vector>
#include <numbers>
#include <cmath>
#include "InterpolationFunctions.h"
#include <functional>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

/// <summary>
/// Выдает значение функции в заданной точке
/// </summary>
/// <param name="x">Точка</param>
/// <returns></returns>
double Function(double x) {
    return tan(x) - cos(x) + 0.1;
}

/// <summary>
/// Выдает производную функции в заданной точке
/// </summary>
/// <param name="x">Точка</param>
/// <returns></returns>
double dFunction(double x) {
    return 1 / (cos(x) * cos(x)) + sin(x);
}

/// <summary>
/// Выдает равноудаленные узлы
/// </summary>
/// <param name="a">Начало промежутка</param>
/// <param name="b">Клнец промежутка</param>
/// <param name="n">Кол-во узлов</param>
/// <returns></returns>
vector<double> uniformNodes(double a, double b, int n) {
    vector<double> p(n);
    for (int i = 0; i < n; i++) {
        p[i] = a + i * (b - a) / (n - 1);
    }
    return p;
}

/// <summary>
/// Выдает узлы Чебышева
/// </summary>
/// <param name="a">Начало промежутка</param>
/// <param name="b">Клнец промежутка</param>
/// <param name="n">Кол-во узлов</param>
/// <returns></returns>
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

int main() {

    while (true) { 

        double a, b;
        int n;

        cout << "Enter interval [a, b]: \n";
        cin >> a >> b;

        cout << "Enter number of nodes n: \n";
        cin >> n;

        int nodeType;
        cout << "Choose nodes:\n";
        cout << "1 - Uniform\n";
        cout << "2 - Chebyshev\n";
        cout << "0 - Exit\n";
        cin >> nodeType;

        if (nodeType == 0) {
            break;
        }

        vector<double> p;
        if (nodeType == 1) {
            p = uniformNodes(a, b, n);
        }
        else {
            p = chebyshevNodes(a, b, n);
        }

        while (true) {

            int method;
            cout << "\nChoose method:\n";
            cout << "1 - Lagrange\n";
            cout << "2 - Newton\n";
            cout << "3 - Spline_10\n";
            cout << "4 - Spline_21\n";
            cout << "5 - Spline_32\n";
            cout << "9 - Change interval (back)\n";
            cout << "0 - Exit\n";

            cin >> method;

            if (method == 0) {
                return 0;
            }
            if (method == 9) {
                break;
            }

            vector<double> coef;
            vector<double> y1(n - 1), y2(n), h(n - 1);

            if (method == 2) {
                coef = NewtonCoefficients(p, Function);
            }

            if (method == 4) {
                coef = ComputeQuadraticSplineCoeffs(p, Function, dFunction);
            }

            if (method == 5) {
                for (int i = 0; i < n - 1; i++) {
                    h[i] = p[i + 1] - p[i];
                }  
                for (int i = 0; i < n; i++) { 
                    y2[i] = 0; 
                }
                for (int i = 0; i < n - 1; i++) {
                    y1[i] = (Function(p[i + 1]) - Function(p[i])) / h[i]
                        - y2[i + 1] * h[i] / 6
                        - y2[i] * h[i] / 3;
                } 
            }

            const int W = 800, H = 600;
            RenderWindow window(VideoMode({ W, H }), "Function vs Interpolation");
            window.setFramerateLimit(60);

            double scaleX = W / (b - a);
            double scaleY = 50;
            double offsetY = H / 2;

            int k = 2000;
            vector<Vertex> funcGraph(k), methodGraph(k);

            double maxError = 0;

            for (int i = 0; i < k; i++) {
                double x = a + i * (b - a) / (k - 1);
                double y_real = Function(x);
                double y_interp = 0;

                switch (method) {
                case 1: y_interp = LagrangeMethod(p, x, Function); break;
                case 2: y_interp = NewtonMethod(p, x, coef); break;
                case 3: y_interp = SplineLinear(p, x, Function); break;
                case 4: y_interp = SplineQuadratic(p, coef, x, Function); break;
                case 5: y_interp = SplineCubic(p, y1, y2, h, x, Function); break;
                }

                maxError = max(maxError, abs(y_real - y_interp));

                funcGraph[i].position = Vector2f((x - a) * scaleX, offsetY - y_real * scaleY);
                funcGraph[i].color = Color::Red;

                methodGraph[i].position = Vector2f((x - a) * scaleX, offsetY - y_interp * scaleY);
                methodGraph[i].color = Color::Blue;
            }

            Font font;
            bool fontLoaded = font.openFromFile("timesbd.ttf");
            optional<Text> text;

            if (fontLoaded) {
                text.emplace(font);
                text->setCharacterSize(20);
                text->setFillColor(Color::Yellow);
                text->setPosition(Vector2f(10.f, 10.f));
            }

            while (window.isOpen()) {
                while (auto event = window.pollEvent()) {
                    if (event->is<Event::Closed>())
                        window.close();
                }

                window.clear(Color::Black);

                window.draw(funcGraph.data(), funcGraph.size(), PrimitiveType::LineStrip);
                window.draw(methodGraph.data(), methodGraph.size(), PrimitiveType::LineStrip);

                if (text) {
                    text->setString("Max Error = " + to_string(maxError));
                    window.draw(*text);
                }

                window.display();
            }

            cout << "\nWindow closed. Choose another method.\n";
        }
    }

    return 0;
}