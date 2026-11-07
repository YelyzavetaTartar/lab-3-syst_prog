%{
#include <iostream>
#include <string>
#include "ast.h"

// Оголошення функцій, які генеруються Lex/Bison
extern int yylex();
void yyerror(const char* s);

// Глобальна змінна, куди ми збережемо корінь нашого готового дерева
ASTNode* root = nullptr; 
%}

/* Визначаємо типи даних, які можуть повертати токени та нетерміналі */
%union {
    double num;
    std::string* str;
    ASTNode* ast;
}

/* Оголошуємо токени, які надходять з lexer.l */
%token <num> NUMBER
%token <str> IDENTIFIER
%token ASSIGN PLUS MINUS MUL DIV SEMI LPAREN RPAREN

/* Оголошуємо тип для нетерміналів (вони повертають вказівник на вузол дерева) */
%type <ast> expression

/* ВИРІШЕННЯ КОНФЛІКТІВ SHIFT-REDUCE: 
   Вказуємо пріоритет операторів (нижче = вищий пріоритет). 
   %left означає ліву асоціативність (наприклад, 1 - 2 - 3 рахується як (1-2)-3) */
%left PLUS MINUS
%left MUL DIV

%%

/* Головне правило граматики: програма складається з виразу та крапки з комою */
program:
    expression SEMI { root = $1; }
    ;

/* Правила для виразів. 
   Тут ми беремо значення токенів ($1, $3) і конструюємо об'єкти AST (з ast.h) */
expression:
    NUMBER                      { $$ = new NumberNode($1); }
  | IDENTIFIER                  { $$ = new VariableNode(*$1); delete $1; }
  | expression PLUS expression  { $$ = new BinaryOpNode('+', $1, $3); }
  | expression MINUS expression { $$ = new BinaryOpNode('-', $1, $3); }
  | expression MUL expression   { $$ = new BinaryOpNode('*', $1, $3); }
  | expression DIV expression   { $$ = new BinaryOpNode('/', $1, $3); }
  | LPAREN expression RPAREN    { $$ = $2; }
  ;

%%

// Функція для обробки синтаксичних помилок
void yyerror(const char* s) {
    std::cerr << "Помилка синтаксису: " << s << std::endl;
}