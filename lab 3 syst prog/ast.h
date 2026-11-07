#pragma once
#include <string>
#include <vector>

// Базовий клас для всіх вузлів AST
class ASTNode {
public:
    virtual ~ASTNode() = default;

    // Метод для трансляції (генерації цільового коду)
    virtual std::string generateCode() const = 0;

    // Метод для побудови JSON для візуалізації в D3.js
    virtual std::string toJSON() const = 0;
};

// Вузол для чисел
class NumberNode : public ASTNode {
    double value;
public:
    NumberNode(double val) : value(val) {}

    std::string generateCode() const override {
        return std::to_string(value);
    }

    std::string toJSON() const override {
        return "{\"name\": \"Number(" + std::to_string(value) + ")\"}";
    }
};

// Вузол для змінних (ідентифікаторів)
class VariableNode : public ASTNode {
    std::string name;
public:
    VariableNode(const std::string& n) : name(n) {}

    std::string generateCode() const override {
        return name;
    }

    std::string toJSON() const override {
        return "{\"name\": \"Var(" + name + ")\"}";
    }
};

// Вузол для бінарних операцій (+, -, *, /)
class BinaryOpNode : public ASTNode {
    char op;
    ASTNode* left, * right;
public:
    BinaryOpNode(char o, ASTNode* l, ASTNode* r) : op(o), left(l), right(r) {}

    // Рекурсивно генеруємо код для лівої та правої гілок
    std::string generateCode() const override {
        return "(" + left->generateCode() + " " + op + " " + right->generateCode() + ")";
    }

    std::string toJSON() const override {
        return "{\"name\": \"Op(" + std::string(1, op) + ")\", \"children\": [" +
            left->toJSON() + ", " + right->toJSON() + "]}";
    }
};