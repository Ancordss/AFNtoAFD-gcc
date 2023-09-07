%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include <stdbool.h>


/** Extern from Flex **/
extern int lineno, 
           line_init;

extern char str_buf[256];    
extern char* str_buf_ptr;


/*Flex and bison*/
  extern int yylex();
  extern char *yytext;
  extern FILE *yyin;
  

  extern void yyterminate();

/* Variables for error handling and saving */
int error_count=0; 
int flag_err_type=0; // 0: Token Error (YYTEXT) || 1: String Error (STRBUF)
int scope=0;
int pos_number=0;
int flag=0;  //flag gia to token ean einai swsto to android
int valueflag=0;
char* strint;



// vars james
char* t_alfabeto0; // Variable para el primer T_STRING
char* t_alfabeto1; // Variable para el segundo T_STRING
int found_match = 0; // Bandera para indicar si se encontró una coincidencia


/*Specific Functions*/
void yyerror(const char *message);
%}

%define parse.error verbose

%union{
   int intval;
   float floatval;
   char charval;
   char *strval;
}


/*Keywords*/
%token<strval> T_AUTOMATA_AFN
%token<strval> T_ALFABETO
%token<strval> T_ESTADO
%token<strval> T_INICIAL
%token<strval> T_FINAL
%token<strval> T_TRANSICIONES
%token<strval> T_END_TAG
%token<strval> T_INT

%token<strval> T_END_ALFABETO
%token<strval> T_END_AUTOMATA_AFN
%token<strval> T_END_ESTADO
%token<strval> T_END_INICIAL
%token<strval> T_END_FINAL
%token<strval> T_END_TRANSICIONES

%token<strval> T_SIMBOLO

%token<strval> T_COMMENT_OPEN
%token<strval> T_COMMENT_CLOSE

%token<strval> T_STRING
%token<strval> T_STRING_SINGLE_QUOTE
%token<strval> T_POSITIVE_INTEGER
%token<strval> T_SLASH_END_TAG
%token<strval> T_STRING_DQ_SPACE


/*Other tokens*/
%left  <strval> T_DOT                      "."
%left  <strval> T_COMMA                    ","
%right <strval> T_ASSIGN                   "="
%token <strval> T_COLON                    ":"
%left  <strval> T_LBRACK                   "["
%left  <strval> T_RBRACK                   "]"
%token <strval> T_SLASH                    "/"
%token <strval> T_EXCLAMATION              "!"
%token <strval> T_DASH                     "-"
%token <strval> T_LBRACES                  "{"
%token <strval> T_RBRACES                  "}"
%left  <strval> T_AT                       "@"
%token <strval> T_QUESTION_MARK            "?"
%token <strval> T_UNDERSCORE               "_"
%token <strval> T_HASH                     "#"
%token <strval> T_SQUOTES                  "'"

/*EOF*/
%token <strval> T_EOF          0           "end of file"

/*Non-Terminal*/
%type  program linearlayout linearlayoutattributes        
%type  estados estadosattributes                                                                                    
%type  text content contentempty element         
                                                        
                                                                                  
%type <strval> letra_a letra_b 


%start program

%%

/*Grammar Rules*/

program :                 T_AUTOMATA_AFN linearlayout estados iniciales finales transicionales T_END_AUTOMATA_AFN
                        | linearlayout estados iniciales finales transicionales //gia na min psaxnei duo fores to <Relative paei sto relativelayout2 poy exei kateuthian ta attributes
                        ;

// linearlayout:              T_LINEAR_LAYOUT linearlayoutattributes element T_END_LINEAR_LAYOUT
//                         |  T_LINEAR_LAYOUT linearlayoutattributes element  T_END_LINEAR_LAYOUT linearlayout
//                         ;

linearlayout:              T_ALFABETO linearlayoutattributes element T_END_ALFABETO
                        |  T_ALFABETO linearlayoutattributes element  T_END_ALFABETO linearlayout
                        ;
                              
                        
linearlayoutattributes: T_STRING T_STRING {
    t_alfabeto0 = strdup($1); // Almacena el valor del primer T_STRING en t_alfabeto0
    t_alfabeto1 = strdup($2); // Almacena el valor del segundo T_STRING en t_alfabeto1
}

                        
simbolos: T_SIMBOLO
                
                
estados:                 T_ESTADO  estadosattributes T_END_ESTADO  
                        ;
                   
estadosattributes:      T_INT T_INT
                        |T_INT T_INT T_INT T_INT
                        |T_INT T_INT T_INT  
                        ;

iniciales:               T_INICIAL  T_INT T_END_INICIAL 
                        ; 

finales:                 T_FINAL finalesattributes T_END_FINAL
                        ;

finalesattributes:       T_INT
                        |T_INT T_INT                        
                        ;

transicionales:         T_TRANSICIONES transicionalesattributes T_END_TRANSICIONES
                        ;

transicionalesattributes: T_INT T_COMMA  T_STRING  T_COMMA  T_INT
                         {
                             int error_line = lineno;

                             if (strcmp($3, t_alfabeto0) != 0 && strcmp($3, t_alfabeto1) != 0)
                                {
                                    char error_message[100];
                                    sprintf(error_message, "One T_STRING at line %d does not match values %s or %s that were entered in T_ALFABETO found %s ", error_line, t_alfabeto0, t_alfabeto1, $3);
                                    yyerror(error_message);
                                }

                         }
                          
                         T_INT T_COMMA T_SIMBOLO T_COMMA  T_INT
                          
                         T_INT T_COMMA  T_STRING T_COMMA  T_INT
                         {
                             int error_line = lineno;

                             if (strcmp($14, t_alfabeto0) != 0 && strcmp($14, t_alfabeto1) != 0)
                                {
                                    char error_message[100];
                                    sprintf(error_message, "One T_STRING at line %d does not match values %s or %s that were entered in T_ALFABETO found %s ", error_line, t_alfabeto0, t_alfabeto1, $14);
                                    yyerror(error_message);
                                }

                         }
                          
                         T_INT T_COMMA  T_STRING T_COMMA  T_INT
                         {
                             int error_line = lineno;

                             if (strcmp($20, t_alfabeto0) != 0 && strcmp($20, t_alfabeto1) != 0)
                                {
                                    char error_message[100];
                                    sprintf(error_message, "One T_STRING at line %d does not match values %s or %s that were entered in T_ALFABETO found %s ", error_line, t_alfabeto0, t_alfabeto1, $20);
                                    yyerror(error_message);
                                }

                         }
                          
                         T_INT T_COMMA  T_STRING T_COMMA  T_INT
                         {
                             int error_line = lineno;

                             if (strcmp($26, t_alfabeto0) != 0 && strcmp($26, t_alfabeto1) != 0)
                                {
                                    char error_message[100];
                                    sprintf(error_message, "One T_STRING at line %d does not match values %s or %s that were entered in T_ALFABETO found %s ", error_line, t_alfabeto0, t_alfabeto1, $26);
                                    yyerror(error_message);
                                }

                         }
                        
                         T_INT T_COMMA  T_STRING T_COMMA  T_INT
                         {
                             int error_line = lineno;

                             if (strcmp($32, t_alfabeto0) != 0 && strcmp($32, t_alfabeto1) != 0)
                                {
                                    char error_message[100];
                                    sprintf(error_message, "One T_STRING at line %d does not match values %s or %s that were entered in T_ALFABETO found %s ", error_line, t_alfabeto0, t_alfabeto1, $32);
                                    yyerror(error_message);
                                }

                         }
                         ;
                          


                             
comment:                 T_COMMENT_OPEN T_STRING T_COMMENT_CLOSE
                          ;


content:                  element
                        | element content
                        ;

contentempty:            element
                        | element content
                        | %empty
                        ;

element:                  estados element
                        | iniciales element
                        | finales element             
                        |%empty
                        ;               
                       



%%

int main(int argc, char *argv[]){
    int token;

    


    if(argc > 1){
        yyin = fopen(argv[1], "r");
        if (yyin == NULL){
            perror ("Error opening file"); 
            return -1;
        }
    }

    
    yyparse();


   if(error_count > 0){
        printf("Syntax Analysis failed due to %d errors\n", error_count);
      }
        
   else{
        printf("Syntax Analysis completed successfully.\n");
      }

    return 0;
    yyrestart(yyin);
    fclose(yyin);
}


void yyerror(const char *message)
{
    error_count++;
    
    if(flag_err_type==0){
        printf("-> ERROR at line %d caused by %s : %s\n", lineno,  message);
    }else if(flag_err_type==1){
        *str_buf_ptr = '\0'; 
        printf("-> ERROR at line %d near \"%s\": %s\n", lineno, str_buf, message);
    }

    flag_err_type = 0; 
    if(MAX_ERRORS <= 0) return;
    if(error_count == MAX_ERRORS){
        printf("Max errors (%d) detected. ABORTING...\n", MAX_ERRORS);
        exit(-1);
    }
}