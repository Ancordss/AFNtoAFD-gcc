%{
/* libraries */
#include "grammar.tab.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>

extern int flag_err_type;
extern void yyerror(char *message);

/*Define variables and functions*/
int lineno=1; 
int line_init=-1; 

/*Κρατάμε το πόσα errors έχουν προκύψει*/
char str_buf[256];
char* str_buf_ptr;



%}


%option noyywrap 
%option case-sensitive 
%option yylineno

/* Initial States */
%x STATE_COMMENT SKIP_COMMENT
%x STATE_INLINE_COMMENT
%x STATE_STRING

/*Sublexical analyze*/
%x STRING



/*Keywords*/
LETTER              [a-zA-Z]
LETTERS             [LETTER]+
DIGIT               [0-9]
NZDIGIT             [1-9]{DIGIT}*
ALPHANUM            ({LETTER}|{DIGIT})
ALPHANUM_           ({ALPHANUM}|_)
ASCII               [ !"#$%&'()*+,-./:;<=>?@\[\\\]^_`{|}~]
WHITESPACE          [ \t]

%%

"<AUTOMATA_AFN>"                                             { token_print(T_LINEAR_LAYOUT); yylval.strval=strdup(yytext); return T_LINEAR_LAYOUT;}
"<ALFABETO>"                                                  { token_print(T_ALFABETO); yylval.strval=strdup(yytext); return T_ALFABETO;}
"<RelativeLayout"                                             { token_print(T_RELATIVE_LAYOUT); yylval.strval=strdup(yytext); return T_RELATIVE_LAYOUT;}
"<TextView>"                                             { token_print(T_TEXT_VIEW); yylval.strval=strdup(yytext); return T_TEXT_VIEW;}
"<ImageView"                                             { token_print(T_IMAGE_VIEW); yylval.strval=strdup(yytext); return T_IMAGE_VIEW;}
"<Button"                                             { token_print( T_BUTTON); yylval.strval=strdup(yytext); return T_BUTTON;}
"<RadioButton"                                             { token_print( T_RADIO_BUTTON); yylval.strval=strdup(yytext); return T_RADIO_BUTTON;}
"<RadioGroup"                                             { token_print( T_RADIO_GROUP); yylval.strval=strdup(yytext); return T_RADIO_GROUP;}
"<ProgressBar"                                             { token_print( T_PROGRESS_BAR); yylval.strval=strdup(yytext); return T_PROGRESS_BAR;}
">"                                             { token_print( T_END_TAG);   yylval.strval=strdup(yytext); return T_END_TAG;}
"/>"                                            { token_print( T_SLASH_END_TAG);   yylval.strval=strdup(yytext); return T_SLASH_END_TAG;}

"</AUTOMATA_AFN>"                                { token_print(T_END_LINEAR_LAYOUT); yylval.strval=strdup(yytext); return T_END_LINEAR_LAYOUT;}
"</ALFABETO>"                                    { token_print(T_END_ALFABETO); yylval.strval=strdup(yytext); return T_END_ALFABETO;}
"</RelativeLayout>"                              { token_print(T_END_RELATIVE_LAYOUT); yylval.strval=strdup(yytext); return T_END_RELATIVE_LAYOUT;}
"</TextView>"                                    { token_print(T_END_TEXT_VIEW); yylval.strval=strdup(yytext); return T_END_TEXT_VIEW;}
"</ImageView>"                                   { token_print(T_END_IMAGE_VIEW); yylval.strval=strdup(yytext); return T_END_IMAGE_VIEW;}
"</Button>"                                      { token_print(T_END_BUTTON); yylval.strval=strdup(yytext);  return T_END_BUTTON;}
"</RadioButton>"                                 { token_print(T_END_RADIO_BUTTON); yylval.strval=strdup(yytext); return T_END_RADIO_BUTTON;}
"</RadioGroup>"                                  { token_print(T_END_RADIO_GROUP); yylval.strval=strdup(yytext);  return T_END_RADIO_GROUP;}
"</ProgressBar>"                                 { token_print(T_END_PROGRESS_BAR); yylval.strval=strdup(yytext);  return T_END_PROGRESS_BAR;}

"a"                           {token_print(T_ANDROID_LAYOUT_WIDTH);yylval.strval=strdup(yytext);return T_ANDROID_LAYOUT_WIDTH;}
"b"                          {token_print(T_ANDROID_LAYOUT_HEIGHT); yylval.strval=strdup(yytext); return T_ANDROID_LAYOUT_HEIGHT;}
"android:id="                                     {token_print(T_ANDROID_ID); yylval.strval=strdup(yytext); return T_ANDROID_ID;}
"android:text="                                   {token_print(T_ANDROID_TEXT); yylval.strval=strdup(yytext); return T_ANDROID_TEXT;}
"android:orientation="                            {token_print(T_ANDROID_ORIENTATION); yylval.strval=strdup(yytext); return T_ANDROID_ORIENTATION;}
"android:src="                                    {token_print(T_ANDROID_SRC); yylval.strval=strdup(yytext); return T_ANDROID_SRC;}
"android:padding="                                {token_print(T_ANDROID_PADDING); yylval.strval=strdup(yytext); return T_ANDROID_PADDING;}
"android:textColor="                              {token_print(T_ANDROID_TEXTCOLOR); yylval.strval=strdup(yytext); return T_ANDROID_TEXTCOLOR;}
"android:checkedButton="                          {token_print(T_ANDROID_CHECKEDBUTTON); yylval.strval=strdup(yytext); return T_ANDROID_CHECKEDBUTTON;}
"android:progress="                               {token_print(T_ANDROID_PROGRESS); yylval.strval=strdup(yytext); return T_ANDROID_PROGRESS;}
"android:max="                                    {token_print(T_ANDROID_MAX); yylval.strval=strdup(yytext); return T_ANDROID_MAX;}
"radio_button_child_count="                       {token_print(T_RADIO_BUTTON_CHILD_COUNT); yylval.strval = strdup(yytext); return T_RADIO_BUTTON_CHILD_COUNT; }
"0"                      {token_print(T_ESTADO_0);yylval.strval=strdup(yytext);return T_ESTADO_0;}
"1"                      {token_print(T_ESTADO_1);yylval.strval=strdup(yytext);return T_ESTADO_1;}
"2"                      {token_print(T_ESTADO_2);yylval.strval=strdup(yytext);return T_ESTADO_2;}
"3"                      {token_print(T_ESTADO_3);yylval.strval=strdup(yytext);return T_ESTADO_3;}


\"[1-9][0-9]*\"                                 { token_print(T_POSITIVE_INTEGER); yylval.strval=strdup(yytext); return T_POSITIVE_INTEGER;}  
\"[a-zA-Z0-9_\-\.]*\"                           { token_print(T_STRING); yylval.strval=strdup(yytext); return T_STRING;}
\'[a-zA-Z0-9_\-\.]*\'                           { token_print(T_STRING_SINGLE_QUOTE); yylval.strval=strdup(yytext); return T_STRING_SINGLE_QUOTE;}  
\"[a-zA-Z0-9_\-\.]*" "[a-zA-Z0-9_\-\.]*\"       { token_print(T_STRING_DQ_SPACE); yylval.strval=strdup(yytext); return T_STRING_DQ_SPACE;}






"."                                           { token_print(T_DOT); yylval.strval=strdup(yytext); return T_DOT;}
","                                           { token_print(T_COMMA); yylval.strval=strdup(yytext); return T_COMMA;}
"="                                           { token_print(T_ASSIGN); yylval.strval=strdup(yytext); return T_ASSIGN;}
":"                                           { token_print(T_COLON); yylval.strval=strdup(yytext); return T_COLON;}
"["                                           { token_print(T_LBRACK); yylval.strval=strdup(yytext); return T_LBRACK;}
"]"                                           { token_print(T_RBRACK); yylval.strval=strdup(yytext); return T_RBRACK;}
"/"                                           { token_print(T_SLASH); yylval.strval=strdup(yytext); return T_SLASH;}
"!"                                           { token_print(T_EXCLAMATION); yylval.strval=strdup(yytext); return T_EXCLAMATION ;}
"-"                                           { token_print(T_DASH); yylval.strval=strdup(yytext); return T_DASH;}
"{"                                           { token_print(T_LBRACES); yylval.strval=strdup(yytext); return T_LBRACES;}
"}"                                           { token_print(T_RBRACES); yylval.strval=strdup(yytext); return T_RBRACES;}
"@"                                           { token_print(T_AT); yylval.strval=strdup(yytext); return T_AT;}
"?"                                           { token_print(T_QUESTION_MARK); yylval.strval=strdup(yytext); return T_QUESTION_MARK ;}
"_"                                           { token_print(T_UNDERSCORE); yylval.strval=strdup(yytext); return T_UNDERSCORE;}
"#"                                           { token_print(T_HASH); yylval.strval=strdup(yytext); return T_HASH;}
"'"                                           { token_print(T_SQUOTES); yylval.strval=strdup(yytext); return T_SQUOTES;}
[  \t]+                                         {   } /*Αναγνώριση κενών και \n */     
<<EOF>>                                       { token_print(T_EOF); yylval.strval=strdup(yytext); return T_EOF;}
.                                             { printf("UNRECOGNIZED_CHAR: %s\n", yytext); }

[\n]+                                           {lineno++;}

"<!--"                  { BEGIN(STATE_COMMENT); str_buf_ptr = str_buf; *str_buf_ptr = '\0'; }
<STATE_COMMENT>"-->"          { BEGIN(INITIAL); 
                          if(strstr(str_buf, "--") != NULL) {
                            fprintf(stderr, "Invalid comment on line %d: comment contains '--'\n", lineno);
                            yyerror("Wrong token");
                          } else {
                            comment_print();
                          }
                        }
<STATE_COMMENT>.|\n           { *str_buf_ptr = *yytext; str_buf_ptr++; *str_buf_ptr = '\0'; ECHO;}



%%


void token_print(){            ////////////////////// ** error 
    if(FLEX_DEBUG && SHOW_TOKENS){
        printf("TOKEN FOUND: %s  | LINE: %d\n", yytext,lineno);
    }    
}

//print if comment is found 

void comment_print(){            ////////////////////// ** error 
    if(FLEX_DEBUG && SHOW_COMMENTS){
        printf("COMMENT FOUND: %s  | LINE: %d\n", yytext,lineno);
    }    
}
