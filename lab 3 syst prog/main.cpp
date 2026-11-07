#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <windows.h>
#include "ast.h"

// Підключаємо парсер і наш корінь дерева
extern int yyparse();
extern ASTNode* root;

int main() {
    // Встановлюємо кодування UTF-8
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    // Перенаправляємо ввід з файлу замість консолі
    if (freopen("input.txt", "r", stdin) == NULL) {
        std::cerr << "Помилка: Створіть файл input.txt у папці з кодом!" << std::endl;
        return 1;
    }

    std::cout << "Читаємо вираз із файлу input.txt..." << std::endl;

    // Запускаємо синтаксичний аналізатор
    if (yyparse() == 0 && root != nullptr) {
        std::cout << "\n[УСПІХ] Синтаксичне дерево (AST) успішно побудовано!" << std::endl;

        // Генерація коду
        std::string generatedCode = root->generateCode();
        std::cout << "\n--- Згенерований код ---\n";
        std::cout << generatedCode << std::endl;

        // Генерація JSON для D3.js візуалізації
        std::string jsonStr = root->toJSON();
        std::ofstream jsonFile("ast.json");
        if (jsonFile.is_open()) {
            jsonFile << jsonStr;
            jsonFile.close();
            std::cout << "\n[INFO] Файл 'ast.json' успішно згенеровано!" << std::endl;
        }
        else {
            std::cerr << "Помилка: не вдалося створити файл ast.json" << std::endl;
        }
    }
    else {
        std::cout << "\n[ПОМИЛКА] Розбір завершився невдачею." << std::endl;
    }

    return 0;
}