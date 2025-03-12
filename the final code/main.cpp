#include <iostream>
#include <map>
#include <string>
#include <set>
#include <cctype>
#include <stack>
#include <algorithm>
#include "Expression.cpp"

std::set<std::string> extractVariables(const std::string &expr) {
    std::set<std::string> variables;
    for (size_t i = 0; i < expr.size(); ++i) {
        if (isalpha(expr[i])) { 
            std::string token;
            while (i < expr.size() && (isalnum(expr[i]) || expr[i] == '_')) {
                token += expr[i];
                ++i;
            }
            if (token != "sin" && token != "cos" && token != "ln" && token != "exp") {
                variables.insert(token); 
            }
        }
    }
    return variables;
}

void parseArguments(int argc, char *argv[], std::string &mode, std::string &expression, std::string &byVar, std::map<std::string, double> &vars) {
    if (argc < 3) {
        throw std::runtime_error("Usage: differentiator --eval <expression> [var=value ...] OR differentiator --diff <expression> --by <variable>");
    }

    mode = argv[1];
    expression = argv[2];

    if (mode == "--eval") {
        for (int i = 3; i < argc; ++i) {
            std::string arg = argv[i];
            size_t eqPos = arg.find('=');
            if (eqPos == std::string::npos) {
                throw std::runtime_error("Invalid variable assignment: " + arg);
            }
            std::string varName = arg.substr(0, eqPos);
            double varValue = std::stod(arg.substr(eqPos + 1));
            vars[varName] = varValue;
        }

        auto variables = extractVariables(expression);
        for (const auto &var : variables) {
            if (vars.find(var) == vars.end()) {
                throw std::runtime_error("Variable not assigned: " + var);
            }
        }
    } else if (mode == "--diff") {
        if (argc < 5 || std::string(argv[3]) != "--by") {
            throw std::runtime_error("Usage for differentiation: differentiator --diff <expression> --by <variable>");
        }
        byVar = argv[4];
    } else {
        throw std::runtime_error("Invalid mode. Use --eval or --diff.");
    }
}

Expression<double> createExpression(const std::string &expr) {
    std::string trimmedExpr;
    for (char ch : expr) {
        if (!std::isspace(ch)) {
            trimmedExpr += ch;
        }
    }

    size_t pos;

    if ((pos = trimmedExpr.find('*')) != std::string::npos) {
        std::string left = trimmedExpr.substr(0, pos);
        std::string right = trimmedExpr.substr(pos + 1);
        return createExpression(left) * createExpression(right);
    } else if ((pos = trimmedExpr.find('+')) != std::string::npos) {
        std::string left = trimmedExpr.substr(0, pos);
        std::string right = trimmedExpr.substr(pos + 1);
        return createExpression(left) + createExpression(right);
    } else if ((pos = trimmedExpr.find('-')) != std::string::npos) {
        std::string left = trimmedExpr.substr(0, pos);
        std::string right = trimmedExpr.substr(pos + 1);
        return createExpression(left) - createExpression(right);
    } else if ((pos = trimmedExpr.find('/')) != std::string::npos) {
        std::string left = trimmedExpr.substr(0, pos);
        std::string right = trimmedExpr.substr(pos + 1);
        return createExpression(left) / createExpression(right);
    } else if ((pos = trimmedExpr.find('^')) != std::string::npos) {
        std::string left = trimmedExpr.substr(0, pos);
        std::string right = trimmedExpr.substr(pos + 1);
        return createExpression(left) ^ createExpression(right);
    }

    if ((pos = trimmedExpr.find("sin(")) != std::string::npos) {
        std::string arg = trimmedExpr.substr(pos + 4, trimmedExpr.size() - pos - 5); 
        return sin(createExpression(arg));
    } else if ((pos = trimmedExpr.find("cos(")) != std::string::npos) {
        std::string arg = trimmedExpr.substr(pos + 4, trimmedExpr.size() - pos - 5); 
        return cos(createExpression(arg));
    } else if ((pos = trimmedExpr.find("ln(")) != std::string::npos) {
        std::string arg = trimmedExpr.substr(pos + 3, trimmedExpr.size() - pos - 4); 
        return ln(createExpression(arg));
    } else if ((pos = trimmedExpr.find("exp(")) != std::string::npos) {
        std::string arg = trimmedExpr.substr(pos + 4, trimmedExpr.size() - pos - 5); 
        return exp(createExpression(arg));
    }

    return Expression<double>::fromString(trimmedExpr);
}

int main(int argc, char *argv[]) {
    try {
        std::string mode, expression, byVar;
        std::map<std::string, double> vars;

        parseArguments(argc, argv, mode, expression, byVar, vars);

        Expression<double> expr = createExpression(expression);

        if (mode == "--eval") {
            // Вычисление выражения
            double result = expr.evaluate(vars);
            std::cout << result << std::endl;
        } else if (mode == "--diff") {
            // Символьное дифференцирование
            Expression<double> derivative = expr.derivative(byVar);
            std::cout << derivative.toString() << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
