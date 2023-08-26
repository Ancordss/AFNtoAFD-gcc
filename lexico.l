%{
#include <stdio.h>
#include "sintaxis.tab.h" // Incluye el archivo de cabecera generado por Bison
%}

%%

"<"         { return LESS_THAN; }
">"         { return GREATER_THAN; }
"/>"        { return SELF_CLOSING; }
"</"        { return CLOSING_TAG; }
"/"         { return SLASH; }
"="         { return EQUALS; }
[ \t\n\r]+  ; // Ignora espacios en blanco y saltos de línea
[a-zA-Z_][a-zA-Z0-9_\-]* { yylval.sval = strdup(yytext); return IDENTIFIER; }

.           { 
              fprintf(stderr, "Carácter no válido: %s\n", yytext);
            }

%%

int yywrap() {
    return 1; // Indica que hemos terminado de analizar
}
