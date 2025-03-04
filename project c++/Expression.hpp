#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cmath>
#include <complex>
#include <sstream>

template<typename T>
class Expression {
public:
    Expression(T value);
    Expression(const std::string& varName);
    Expression(const Expression& other);
    Expression(Expression&& other) noexcept;
    ~Expression();

    Expression& operator=(const Expression& other);
    Expression& operator=(Expression&& other) noexcept;

    T evaluate(const std::map<std::string, T>& vars = {}) const;
    std::string toString() const;
    Expression substitute(const std::string& varName, const Expression& value) const;

    friend Expression operator+(const Expression& lhs, const Expression& rhs) {
        return Expression(new BinaryOpNode(lhs.node->clone(), rhs.node->clone(), BinaryOp::Add));
    }
    friend Expression operator-(const Expression& lhs, const Expression& rhs) {
        return Expression(new BinaryOpNode(lhs.node->clone(), rhs.node->clone(), BinaryOp::Subtract));
    }
    friend Expression operator*(const Expression& lhs, const Expression& rhs) {
        return Expression(new BinaryOpNode(lhs.node->clone(), rhs.node->clone(), BinaryOp::Multiply));
    }
    friend Expression operator/(const Expression& lhs, const Expression& rhs) {
        return Expression(new BinaryOpNode(lhs.node->clone(), rhs.node->clone(), BinaryOp::Divide));
    }
    friend Expression operator^(const Expression& lhs, const Expression& rhs) {
        return Expression(new BinaryOpNode(lhs.node->clone(), rhs.node->clone(), BinaryOp::Power));
    }
    friend Expression sin(const Expression& expr) {
        return Expression(new UnaryOpNode(expr.node->clone(), UnaryOp::Sin));
    }
    friend Expression cos(const Expression& expr) {
        return Expression(new UnaryOpNode(expr.node->clone(), UnaryOp::Cos));
    }
    friend Expression ln(const Expression& expr) {
        return Expression(new UnaryOpNode(expr.node->clone(), UnaryOp::Ln));
    }
    friend Expression exp(const Expression& expr) {
        return Expression(new UnaryOpNode(expr.node->clone(), UnaryOp::Exp));
    }

private:
    enum class BinaryOp { Add, Subtract, Multiply, Divide, Power };
    enum class UnaryOp { Sin, Cos, Ln, Exp };

    class Node {
    public:
        virtual ~Node() = default;
        virtual T evaluate(const std::map<std::string, T>& vars) const = 0;
        virtual Node* clone() const = 0;
        virtual std::string toString() const = 0;
        virtual Expression substitute(const std::string& varName, const Expression& value) const = 0;
    };

    class NumberNode;
    class VariableNode;
    class BinaryOpNode;
    class UnaryOpNode;

    Node* node;
    explicit Expression(Node* n);
};

#endif 