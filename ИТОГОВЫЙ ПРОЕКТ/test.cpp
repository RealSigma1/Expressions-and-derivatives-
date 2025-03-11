#include <iostream>
#include "Expression.cpp"
#include <cmath>
#include <complex>
#include <iomanip>

void diff() {
    Expression<double> x("x");
    auto expr = (x 	^ 3.0) + (3.0 * x) + 5.0;
    auto derivative = expr.derivative("x");
    
    std::cout << "Derivative: " << derivative.toString() << std::endl;
    // Вывод: 3x² + 3
}


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

void diff_with_substitution() {
    Expression<double> x("x");  
    auto expr = (x ^ 3.0) + (3.0 * x) + 5.0;
    auto derivative = expr.derivative("x");  // Символьная производная: 3x^2 + 3

    auto derivative_substituted = derivative.substitute("x", Expression<double>(2.0));
    double result = derivative_substituted.evaluate();
    
    std::cout << "Derivative at x = 2: " << result << std::endl; // Должно вывести 15
}


int main()
{
    trig();
    diff();
    simple();
    logari();
    complex_example();
    diff_with_substitution();
    auto expr1 = Expression<double>::fromString("x + 5");
	std::cout << "Expression: " << expr1.toString() << std::endl;

    
}