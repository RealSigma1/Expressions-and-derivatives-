#include <iostream>
#include "Expression.cpp"
#include <cmath>
#include <complex>

void trig() {
    using namespace std;

    Expression<double> x("x");
    Expression<double> y("y");
    auto expr = sin(x) + cos(y);

   auto substituted = expr.substitute("x", Expression<double>(3.1415926535 / 6))
                          .substitute("y", Expression<double>(3.1415926535 / 3));

    double result = substituted.evaluate();
    cout << "Result: " << result << endl; 
}

void complex_example() {
    using namespace std;

    Expression<complex<double>> x("x");
    Expression<complex<double>> y("y");
    Expression<complex<double>> z("z");
    auto expr = (x + y) * exp(z);
	
	auto substituted = expr.substitute("x", Expression<complex<double>>(complex<double>(1.0, 2.0)))
                          .substitute("y", Expression<complex<double>>(complex<double>(3.0, 4.0)))
                          .substitute("z", Expression<complex<double>>(complex<double>(0.0, 3.1415926535)));
    complex<double> result = substituted.evaluate();
    cout << "Result: " << result << endl; 
}

void diff() {
    using namespace std;

    Expression<double> x("x");
    auto expr = sin(x) *ln(x);

  	double h = 1e-5; 
    auto f_x_plus_h = expr.substitute("x", Expression<double>(2.0 + h));
    auto f_x = expr.substitute("x", Expression<double>(2.0));

    double derivative = (f_x_plus_h.evaluate() - f_x.evaluate()) / h;
    cout << "Derivative at x = 2: " << derivative << endl; 

}

void simple() {
    using namespace std;

    Expression<double> x("x");
    Expression<double> y("y");
    auto expr = (x + 5.0) * (y - 3.0);

    auto substituted = expr.substitute("x", Expression<double>(2.0))
                          .substitute("y", Expression<double>(4.0));
	
	double result = substituted.evaluate();
    cout << "Result: " << result << endl; 
}

void logari() {
    using namespace std;

    Expression<double> x("x");
    Expression<double> y("y");
    auto expr = ln(x) + exp(y);

    auto substituted = expr.substitute("x", Expression<double>(2.71828))
                          .substitute("y", Expression<double>(1.0));

    double result = substituted.evaluate();
    cout << "Result: " << result << endl; 

}

int main()
{
    trig();
    complex_example();
    diff();
    simple();
    logari();
}