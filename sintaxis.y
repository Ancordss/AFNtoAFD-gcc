%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaraciones de funciones y estructuras necesarias
extern int yylex();
extern FILE* yyin;

void yyerror(const char *s);
%}

%union {
    char *sval; // Para almacenar identificadores
}

%token LESS_THAN GREATER_THAN SELF_CLOSING CLOSING_TAG SLASH EQUALS
%token IDENTIFIER

%type <sval> tag

%%

document: tag
        | document tag
        ;

tag: LESS_THAN IDENTIFIER GREATER_THAN
   | LESS_THAN IDENTIFIER SLASH GREATER_THAN
   | LESS_THAN IDENTIFIER GREATER_THAN document LESS_THAN SLASH IDENTIFIER GREATER_THAN
   ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error de sintaxis: %s\n", s);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s archivo.xml\n", argv[0]);
        return 1;
    }

    FILE *xmlFile = fopen(argv[1], "r");
    if (!xmlFile) {
        perror("No se pudo abrir el archivo");
        return 1;
    }

    yyin = xmlFile;
    yyparse();

    fclose(xmlFile);
    return 0;
}
