#include <iostream>
#include <map>
#include <string>
#include <set>
#include <cctype>
#include <stack>
#include <algorithm>
#include "Expression.cpp"

size_t findMatchingParenthesis(const std::string &expr, size_t start) {
    int count = 1;
    for (size_t i = start + 1; i < expr.size(); ++i) {
        if (expr[i] == '(') count++;
        else if (expr[i] == ')') {
            count--;
            if (count == 0) return i;
        }
    }
    return std::string::npos;
}

size_t findAddOrSub(const std::string &expr) {
    int parenCount = 0;
    for (int i = expr.size() - 1; i >= 0; --i) {
        if (expr[i] == ')') parenCount++;
        else if (expr[i] == '(') parenCount--;
        else if ((expr[i] == '+' || expr[i] == '-') && parenCount == 0) {
            return i;
        }
    }
    return std::string::npos;
}

size_t findMulOrDiv(const std::string &expr) {
    int parenCount = 0;
    for (int i = expr.size() - 1; i >= 0; --i) {
        if (expr[i] == ')') parenCount++;
        else if (expr[i] == '(') parenCount--;
        else if ((expr[i] == '*' || expr[i] == '/') && parenCount == 0) {
            return i;
        }
    }
    return std::string::npos;
}

size_t findPow(const std::string &expr) {
    int parenCount = 0;
    for (int i = expr.size() - 1; i >= 0; --i) {
        if (expr[i] == ')') parenCount++;
        else if (expr[i] == '(') parenCount--;
        else if (expr[i] == '^' && parenCount == 0) {
            return i;
        }
    }
    return std::string::npos;
}

Expression<double> createExpression(const std::string &expr);

// Парсинг функций (sin, cos, ln, exp)
Expression<double> parseFunction(const std::string &trimmedExpr, const std::string &funcName, size_t funcLen) {
    size_t startParen = funcLen;
    if (trimmedExpr[startParen] != '(') {
        throw std::runtime_error("Неверный синтаксис функции " + funcName);
    }
    size_t endParen = findMatchingParenthesis(trimmedExpr, startParen);
    if (endParen == std::string::npos || endParen != trimmedExpr.size() - 1) {
        throw std::runtime_error("Несбалансированные скобки в функции " + funcName);
    }
    std::string arg = trimmedExpr.substr(startParen + 1, endParen - startParen - 1);
    if (funcName == "sin") {
        return sin(createExpression(arg));
    } else if (funcName == "cos") {
        return cos(createExpression(arg));
    } else if (funcName == "ln") {
        return ln(createExpression(arg));
    } else if (funcName == "exp") {
        return exp(createExpression(arg));
    }
    throw std::runtime_error("Неизвестная функция: " + funcName);
}

Expression<double> createExpression(const std::string &expr) {
    std::string trimmedExpr;
    for (char ch : expr) {
        if (!std::isspace(ch)) {
            trimmedExpr += ch;
        }
    }

    size_t pos;

    pos = findAddOrSub(trimmedExpr);
    if (pos != std::string::npos) {
        char op = trimmedExpr[pos];
        std::string left = trimmedExpr.substr(0, pos);
        std::string right = trimmedExpr.substr(pos + 1);
        if (op == '+') {
            return createExpression(left) + createExpression(right);
        } else {
            return createExpression(left) - createExpression(right);
        }
    }

    pos = findMulOrDiv(trimmedExpr);
    if (pos != std::string::npos) {
        char op = trimmedExpr[pos];
        std::string left = trimmedExpr.substr(0, pos);
        std::string right = trimmedExpr.substr(pos + 1);
        if (op == '*') {
            return createExpression(left) * createExpression(right);
        } else {
            return createExpression(left) / createExpression(right);
        }
    }

    pos = findPow(trimmedExpr);
    if (pos != std::string::npos) {
        std::string left = trimmedExpr.substr(0, pos);
        std::string right = trimmedExpr.substr(pos + 1);
        return createExpression(left) ^ createExpression(right);
    }

    if (trimmedExpr.compare(0, 4, "sin(") == 0) {
        return parseFunction(trimmedExpr, "sin", 3);
    } else if (trimmedExpr.compare(0, 4, "cos(") == 0) {
        return parseFunction(trimmedExpr, "cos", 3);
    } else if (trimmedExpr.compare(0, 3, "ln(") == 0) {
        return parseFunction(trimmedExpr, "ln", 2);
    } else if (trimmedExpr.compare(0, 4, "exp(") == 0) {
        return parseFunction(trimmedExpr, "exp", 3);
    }

    if (trimmedExpr.size() >= 2 && trimmedExpr.front() == '(' && trimmedExpr.back() == ')') {
        std::string inside = trimmedExpr.substr(1, trimmedExpr.size() - 2);
        return createExpression(inside);
    }

    return Expression<double>::fromString(trimmedExpr);
}

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
        throw std::runtime_error("Использование: differentiator --eval <выражение> [var=значение ...] ИЛИ differentiator --diff <выражение> --by <переменная>");
    }

    mode = argv[1];
    expression = argv[2];

    if (mode == "--eval") {
        for (int i = 3; i < argc; ++i) {
            std::string arg = argv[i];
            size_t eqPos = arg.find('=');
            if (eqPos == std::string::npos) {
                throw std::runtime_error("Неверное присваивание переменной: " + arg);
            }
            std::string varName = arg.substr(0, eqPos);
            double varValue = std::stod(arg.substr(eqPos + 1));
            vars[varName] = varValue;
        }

        auto variables = extractVariables(expression);
        for (const auto &var : variables) {
            if (vars.find(var) == vars.end()) {
                throw std::runtime_error("Переменная не задана: " + var);
            }
        }
    } else if (mode == "--diff") {
        if (argc < 5 || std::string(argv[3]) != "--by") {
            throw std::runtime_error("Использование для дифференцирования: differentiator --diff <выражение> --by <переменная>");
        }
        byVar = argv[4];
    } else {
        throw std::runtime_error("Неверный режим. Используйте --eval или --diff.");
    }
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
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
