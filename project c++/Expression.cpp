#include "Expression.hpp"

template<typename T>
class Expression<T>::NumberNode : public Expression<T>::Node {
    T value;
public:
    NumberNode(T val) : value(val) {}
    T evaluate(const std::map<std::string, T>&) const override { return value; }
    typename Expression<T>::Node* clone() const override { return new NumberNode(*this); }
    std::string toString() const override {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
    Expression<T> substitute(const std::string&, const Expression<T>&) const override {
        return Expression<T>(new NumberNode(value));
    }
};

template<typename T>
class Expression<T>::VariableNode : public Expression<T>::Node {
    std::string name;
public:
    VariableNode(const std::string& n) : name(n) {}
    T evaluate(const std::map<std::string, T>& vars) const override {
        auto it = vars.find(name);
        if (it == vars.end()) throw std::runtime_error("Variable not found: " + name);
        return it->second;
    }
    typename Expression<T>::Node* clone() const override { return new VariableNode(*this); }
    std::string toString() const override { return name; }
    Expression<T> substitute(const std::string& varName, const Expression<T>& value) const override {
        return (name == varName) ? value : Expression<T>(new VariableNode(name));
    }
};

template<typename T>
class Expression<T>::BinaryOpNode : public Expression<T>::Node {
    typename Expression<T>::Node* left;
    typename Expression<T>::Node* right;
    typename Expression<T>::BinaryOp op;
public:
    BinaryOpNode(typename Expression<T>::Node* l, typename Expression<T>::Node* r, typename Expression<T>::BinaryOp o) 
        : left(l), right(r), op(o) {}
    ~BinaryOpNode() { delete left; delete right; }
    T evaluate(const std::map<std::string, T>& vars) const override {
        T lVal = left->evaluate(vars);
        T rVal = right->evaluate(vars);
        switch(op) {
            case Expression<T>::BinaryOp::Add: return lVal + rVal;
            case Expression<T>::BinaryOp::Subtract: return lVal - rVal;
            case Expression<T>::BinaryOp::Multiply: return lVal * rVal;
            case Expression<T>::BinaryOp::Divide: return lVal / rVal;
            case Expression<T>::BinaryOp::Power: return std::pow(lVal, rVal);
            default: throw std::runtime_error("Unknown binary operation");
        }
    }
    typename Expression<T>::Node* clone() const override { return new BinaryOpNode(left->clone(), right->clone(), op); }
    std::string toString() const override {
        const char* opStr = "";
        switch(op) {
            case Expression<T>::BinaryOp::Add: opStr = "+"; break;
            case Expression<T>::BinaryOp::Subtract: opStr = "-"; break;
            case Expression<T>::BinaryOp::Multiply: opStr = "*"; break;
            case Expression<T>::BinaryOp::Divide: opStr = "/"; break;
            case Expression<T>::BinaryOp::Power: opStr = "^"; break;
        }
        return "(" + left->toString() + " " + opStr + " " + right->toString() + ")";
    }
    Expression<T> substitute(const std::string& varName, const Expression<T>& value) const override {
        Expression<T> newLeft = left->substitute(varName, value);
        Expression<T> newRight = right->substitute(varName, value);
        return Expression<T>(new BinaryOpNode(newLeft.node->clone(), newRight.node->clone(), op));
    }
};

template<typename T>
class Expression<T>::UnaryOpNode : public Expression<T>::Node {
    typename Expression<T>::Node* operand;
    typename Expression<T>::UnaryOp op;
public:
    UnaryOpNode(typename Expression<T>::Node* o, typename Expression<T>::UnaryOp op) : operand(o), op(op) {}
    ~UnaryOpNode() { delete operand; }
    T evaluate(const std::map<std::string, T>& vars) const override {
        T val = operand->evaluate(vars);
        switch(op) {
            case Expression<T>::UnaryOp::Sin: return std::sin(val);
            case Expression<T>::UnaryOp::Cos: return std::cos(val);
            case Expression<T>::UnaryOp::Ln: return std::log(val);
            case Expression<T>::UnaryOp::Exp: return std::exp(val);
            default: throw std::runtime_error("Unknown unary operation");
        }
    }
    typename Expression<T>::Node* clone() const override { return new UnaryOpNode(operand->clone(), op); }
    std::string toString() const override {
        const char* func = "";
        switch(op) {
            case Expression<T>::UnaryOp::Sin: func = "sin"; break;
            case Expression<T>::UnaryOp::Cos: func = "cos"; break;
            case Expression<T>::UnaryOp::Ln: func = "ln"; break;
            case Expression<T>::UnaryOp::Exp: func = "exp"; break;
        }
        return std::string(func) + "(" + operand->toString() + ")";
    }
    Expression<T> substitute(const std::string& varName, const Expression<T>& value) const override {
        Expression<T> newOperand = operand->substitute(varName, value);
        return Expression<T>(new UnaryOpNode(newOperand.node->clone(), op));
    }
};

template<typename T>
Expression<T>::Expression(T value) : node(new NumberNode(value)) {}

template<typename T>
Expression<T>::Expression(const std::string& varName) : node(new VariableNode(varName)) {}

template<typename T>
Expression<T>::Expression(Node* n) : node(n) {}

template<typename T>
Expression<T>::Expression(const Expression& other) : node(other.node->clone()) {}

template<typename T>
Expression<T>::Expression(Expression&& other) noexcept : node(other.node) {
    other.node = nullptr;
}

template<typename T>
Expression<T>::~Expression() {
    delete node;
}

template<typename T>
Expression<T>& Expression<T>::operator=(const Expression& other) {
    if (this != &other) {
        delete node;
        node = other.node->clone();
    }
    return *this;
}

template<typename T>
Expression<T>& Expression<T>::operator=(Expression&& other) noexcept {
    if (this != &other) {
        delete node;
        node = other.node;
        other.node = nullptr;
    }
    return *this;
}

template<typename T>
T Expression<T>::evaluate(const std::map<std::string, T>& vars) const {
    return node->evaluate(vars);
}

template<typename T>
std::string Expression<T>::toString() const {
    return node->toString();
}

template<typename T>
Expression<T> Expression<T>::substitute(const std::string& varName, const Expression& value) const {
    return node->substitute(varName, value);
}

template class Expression<double>;