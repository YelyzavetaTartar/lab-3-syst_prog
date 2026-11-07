%{
#include <string>
#include <iostream>
#include <io.h>
#define isatty _isatty
#include "ast.h"
#include "parser.tab.h"

extern "C" int yywrap() { return 1; }
%}

%option nounistd
%option never-interactive

%%

[0-9]+(\.[0-9]+)?       { yylval.num = std::stod(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.str = new std::string(yytext); return IDENTIFIER; }

"="                     { return ASSIGN; }
"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return MUL; }
"/"                     { return DIV; }
";"                     { return SEMI; }
"("                     { return LPAREN; }
")"                     { return RPAREN; }

[ \t\r\n]+              { /* Ігноруємо пробіли та переноси рядків */ }
.                       { std::cerr << "Невідомий символ: " << yytext << std::endl; }

%%


