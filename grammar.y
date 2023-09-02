%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include <stdbool.h>
#define MAX_RADIO_BUTTONS 100
#define MAX_USED_IDS 100  
#define MAX_ID_LENGTH 100 
#define MAX_IDS 100


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
char valid_radio_buttons[MAX_RADIO_BUTTONS][50];  // Array to store valid radio button ids
int valid_radio_button_count = 0;
int max_value = 0;  // Variable to store android:max values
char used_ids[MAX_USED_IDS][MAX_ID_LENGTH];  // Array to store used IDs
int used_id_count = 0;
static int child_count = 0;  // Counter for child elements within <RadioGroup>    ///3
static int child_count_rge = 0;  // value of radio_button_child_count
char radio_button_ids[MAX_IDS][MAX_ID_LENGTH];    
int radio_button_id_count = 0;
char temp_checked_button[MAX_ID_LENGTH];
char* checked_button_id = NULL;  
char used_checkb[MAX_USED_IDS][MAX_ID_LENGTH];
char check_radiob[MAX_USED_IDS][MAX_ID_LENGTH];
int flag_radio_button=0;



/*Specific Functions*/
void yyerror(const char *message);
bool containsDash(const char* str);
int dq_string_to_int(const char* str);
int isIdUsed(char* id);
void addUsedId(char* id);
void addUsedIdInt(int id);
void increment_child_count();
void check_radio_group_child_count(int expected_count, int cumulative_count);
int isIdUsedInRadioButtons(char* id);
int checkRadioButtonId();
void save_checked_button_id(char* id);
void checkRadioBId(char* id);
void check_values();
%}

%define parse.error verbose

%union{
   int intval;
   float floatval;
   char charval;
   char *strval;
}


/*Keywords*/
%token<strval> T_LINEAR_LAYOUT
%token<strval> T_ALFABETO
%token<strval> T_RELATIVE_LAYOUT
%token<strval> T_TEXT_VIEW
%token<strval> T_IMAGE_VIEW
%token<strval> T_BUTTON
%token<strval> T_RADIO_BUTTON
%token<strval> T_RADIO_GROUP
%token<strval> T_PROGRESS_BAR
%token<strval> T_END_TAG

%token<strval> T_END_ALFABETO
%token<strval> T_END_LINEAR_LAYOUT
%token<strval> T_END_RELATIVE_LAYOUT
%token<strval> T_END_TEXT_VIEW
%token<strval> T_END_IMAGE_VIEW
%token<strval> T_END_BUTTON
%token<strval> T_END_RADIO_BUTTON
%token<strval> T_END_RADIO_GROUP
%token<strval> T_END_PROGRESS_BAR

%token<strval> T_ANDROID_LAYOUT_WIDTH
%token<strval> T_ANDROID_LAYOUT_HEIGHT
%token<strval> T_ANDROID_ID
%token<strval> T_ANDROID_TEXT
%token<strval> T_ANDROID_ORIENTATION
%token<strval> T_ANDROID_SRC
%token<strval> T_ANDROID_PADDING
%token<strval> T_ANDROID_TEXTCOLOR
%token<strval> T_ANDROID_CHECKEDBUTTON
%token<strval> T_RADIO_BUTTON_CHILD_COUNT
%token<strval> T_ANDROID_PROGRESS
%token<strval> T_ANDROID_MAX

%token<strval> T_COMMENT_OPEN
%token<strval> T_COMMENT_CLOSE

%token<strval> T_STRING
%token<strval> T_STRING_SINGLE_QUOTE
%token<strval> T_POSITIVE_INTEGER
%token<strval> T_SLASH_END_TAG
%token<strval> T_STRING_DQ_SPACE

%token<strval> T_ESTADO_0
%token<strval> T_ESTADO_1
%token<strval> T_ESTADO_2
%token<strval> T_ESTADO_3

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
%type  program relativelayout2 linearlayout linearlayoutattributes  relativelayout relativelayoutattributes      
%type  textview textviewattributes  imageview imageviewattributes                                          
%type  button buttonattributes  radiogroup radiogroupattributes  radio_group_element    
%type  src  textColor padding checkedButton progress max android_orientation                        
%type  radiobutton  radiobuttonattributes   progressbar progressbarattributes                        
%type  text content contentempty element         
%type radio_start radio_end                                                            
                                                                                  
%type <strval> layout_width layout_height  
%type <strval> android_id


%start program

%%

/*Grammar Rules*/

program :                 T_LINEAR_LAYOUT linearlayout
                        | T_RELATIVE_LAYOUT relativelayout2   //gia na min psaxnei duo fores to <Relative paei sto relativelayout2 poy exei kateuthian ta attributes
                        ;

// linearlayout:              T_LINEAR_LAYOUT linearlayoutattributes element T_END_LINEAR_LAYOUT
//                         |  T_LINEAR_LAYOUT linearlayoutattributes element  T_END_LINEAR_LAYOUT linearlayout
//                         ;

linearlayout:              T_ALFABETO linearlayoutattributes element T_END_ALFABETO
                        |  T_ALFABETO linearlayoutattributes element  T_END_ALFABETO linearlayout
                        ;
                              
linearlayout2:              linearlayoutattributes T_END_TAG element  T_END_LINEAR_LAYOUT
                        |   linearlayoutattributes T_END_TAG element  T_END_LINEAR_LAYOUT linearlayout
                        |   linearlayoutattributes T_END_TAG element element T_END_LINEAR_LAYOUT
                        ;                      
                        

linearlayoutattributes:       layout_width layout_height 
                            | layout_width layout_height android_id element 
                            | layout_width layout_height android_orientation element 
                            | layout_width layout_height android_id android_orientation element comment
                            | layout_width layout_height element 
                            ;

layout_width:               T_ANDROID_LAYOUT_WIDTH  // T_STRING //for strings "android:layout_width=" 
//                            {  
//                             if (strcmp($1, "android:layout_width=") == 0 || flag==1) {  //ama to flag exei ginei 1 exei mpei hdh mia fora kai einai swsto to android opote meta tha pairnei to string apla
//                                   //printf("EDW TO FLAG :\t%d\t",flag);
//                                   flag=1;
//                                if (strcmp($2, "\"match_parent\"") == 0 || strcmp($2, "\"wrap_content\"") == 0){   //|| strcmp($2, T_DIMENSION) == 0 || strcmp($2, T_PERCENTAGE) == 0) {
//                                   //printf("%s = %s\n", $1, $2);
//                                   valueflag=1;
//                                 } 
                                 
//                                 }
//                             else 
//                               yyerror("Expected android:layout_width=");

//                             if(valueflag==0 ) //an ta string != value poy prepei na exoyn
//                                   {
//                                     printf("Invalid value for android:layout_width: %s\n", $2);
//                                     yyerror("Invalid value.");
//                                   } 
//                            }

//                            | T_ANDROID_LAYOUT_WIDTH T_QUESTION_MARK
//                            {
//                             printf("\nWrong!\n");
//                             yyerror("Invalid value.");
//                            }

//                            |T_ANDROID_LAYOUT_WIDTH T_POSITIVE_INTEGER{
                            
//                             flag=0;
//                             if(strcmp($1, "android:layout_width=") == 0 || flag == 1){
//                                flag=1;
//                                pos_number=atoi($2);
//                                //printf("%s = %d\n", $1, pos_number);
//                                }
//                             else yyerror("Expected android:layout_width=");
//                            }
//                            ;

                            

layout_height:               T_ANDROID_LAYOUT_HEIGHT  //T_STRING //  "android:layout_height=" 
//                             {   flag=0;
//                             if (strcmp($1, "android:layout_height=") == 0 || flag == 1) {\
//                                 flag=1;
//                                if (strcmp($2, "\"match_parent\"") == 0 || strcmp($2, "\"wrap_content\"") == 0){ // || strcmp($2, T_DIMENSION) == 0 || strcmp($2, T_PERCENTAGE) == 0) {
//                                   //printf("%s = %s\n", $1, $2);
//                                 } else 
//                                    printf("Invalid value for android:layout_height: %s\n", $2);
//                                 }
//                             else 
//                               yyerror("Expected android:layout_height=");
//                             }
//                            |T_ANDROID_LAYOUT_HEIGHT T_POSITIVE_INTEGER{
//                             flag=0;
//                             if(strcmp($1, "android:layout_height=") == 0 || flag==1){
//                                 flag=1;
//                                pos_number=atoi($2);
//                                //printf("%s = %d\n", $1, pos_number);
//                                }
//                             else yyerror("Expected android:layout_height=");
//                             }
//                             ;
                        

android_id:     T_ANDROID_ID T_POSITIVE_INTEGER
                {
                    flag = 0;
                    if (strcmp($1, "android:id=") == 0 || flag == 1) {
                        flag = 1;
                        pos_number = dq_string_to_int($2);
                        printf("%s = %d\n", $1, pos_number);
                        char pos_number_str[20];  // Buffer to hold the string representation of pos_number
                        snprintf(pos_number_str, sizeof(pos_number_str), "%d", pos_number);

                        if (isIdUsed(pos_number_str)) {
                            yyerror("The android:id value is not unique.");
                        } else {
                            addUsedId(pos_number_str);
                        }
                    } else {
                        yyerror("Expected android:id=");
                    }
                }
                | T_ANDROID_ID T_STRING
                {
                    flag = 0;
                    if (strcmp($1, "android:id=") == 0 || flag == 1) {
                        flag = 1;
                        printf("%s = %s\n", $1, $2);

                        if(flag_radio_button==1){
                            checkRadioBId($2);
                        }

                        if(isIdUsed($2)){
                            yyerror("The android:id value is not unique.");
                        }else{
                            addUsedId($2);
                        }
                    } else {
                        yyerror("Expected android:id=");
                    }
                }
                ;
                                


android_orientation:              T_ANDROID_ORIENTATION  T_STRING 
                                 {  flag=0;
                                 if(strcmp($1, "android:orientation=") == 0 || flag==1){
                                        flag=1;
                                    if(strcmp($2, "\"horizontal\"") == 0 || strcmp($2, "\"vertical\"") == 0)
                                    printf("%s = %s\n", $1, $2);
                                    }
                                 else yyerror("Expected android:orientation="); 
                                 }
                                 ;

text:                            T_ANDROID_TEXT T_STRING 
                                 {  
                                    flag=0;
                                    if(strcmp($1, "android:text=") == 0 || flag==1){
                                        flag=1;
                                    printf("%s = %s\n", $1, $2);}
                                 else yyerror("Expected android:text="); 
                                }

                               |T_ANDROID_TEXT T_STRING_DQ_SPACE  //periptwsi na einai 2 lekseis me keno metaksu toys kai "" sta akra
                                {flag=0;
                                    if(strcmp($1, "android:text=") == 0 || flag==1){
                                        flag=1;
                                    printf("%s = %s\n", $1, $2);}
                                 else yyerror("Expected android:text="); 
                                }
                                ;

textColor:                        T_ANDROID_TEXTCOLOR T_STRING 
                                  { flag=0;
                                    if(strcmp($1, "android:textColor=") == 0 || flag==1){
                                        flag=1;
                                    printf("%s = %s\n", $1, $2);}
                                    else yyerror("Expected android:textColor=");    
                                  } 
                                  
                                  ;


checkedButton:       T_ANDROID_CHECKEDBUTTON T_STRING
                    {
                    save_checked_button_id($2);
                    printf("%s = %s\n", $1, $2);                    
                    } 
                    ;                    

//progress max kai padding einai arithmoi giauto vazw to atoi gia na kanw to string noymero
progress:                            T_ANDROID_PROGRESS T_POSITIVE_INTEGER
                                        {
                                            if (strcmp($1, "android:progress=") == 0 || flag == 1) {
                                                flag = 1;
                                                int progress = dq_string_to_int($2);
                                                if (progress >= 0 && progress <= max_value) {
                                                    printf("%s = %d\n", $1, progress);
                                                } else {
                                                    yyerror("Invalid android:progress value");
                                                }
                                            } else {
                                                yyerror("Expected android:progress=");
                                            }
                                        }
                                        ;


padding:                   T_ANDROID_PADDING T_POSITIVE_INTEGER
                            { flag=0;
                            if(strcmp($1, "android:padding=") == 0 || flag ==1) {
                                flag=1;
                                pos_number = atoi($2);
                                    if (pos_number < 0) {
                                      yyerror("Invalid progress value. It should be a positive integer.");
                                    }
                                    printf("%s = ''%d''\n", $1, pos_number);}
                             else {
                                yyerror("Expected android:padding=");
                                  }
                            };


max:                        T_ANDROID_MAX T_POSITIVE_INTEGER
                            {
                                if (strcmp($1, "android:max=") == 0) {
                                    max_value = dq_string_to_int($2);
                                    printf("%s = %d\n", $1, max_value);
                                } else {
                                    yyerror("Expected android:max=");
                                }
                            }
                            ;

src:                            T_ANDROID_SRC T_STRING
                                {   flag=0;
                                    if(strcmp($1, "android:src=") == 0|| flag==1){
                                        flag=1;
                                    printf("%s = %s\n", $1, $2);}
                                 else yyerror("Expected android:src="); 
                                }
                                ;


relativelayout:          T_RELATIVE_LAYOUT  relativelayoutattributes T_END_TAG element T_END_RELATIVE_LAYOUT
                        |T_RELATIVE_LAYOUT  relativelayoutattributes T_END_TAG element element T_END_RELATIVE_LAYOUT
                        |T_RELATIVE_LAYOUT  relativelayoutattributes T_END_RELATIVE_LAYOUT element relativelayout
                        |T_RELATIVE_LAYOUT  relativelayoutattributes relativelayoutattributes T_END_TAG element T_END_RELATIVE_LAYOUT
                        |T_RELATIVE_LAYOUT  relativelayoutattributes element T_END_RELATIVE_LAYOUT progressbar progressbarattributes T_END_LINEAR_LAYOUT
                        |T_RELATIVE_LAYOUT  relativelayoutattributes T_END_TAG element element T_END_RELATIVE_LAYOUT T_END_LINEAR_LAYOUT
                        ;

relativelayout2:           relativelayoutattributes T_END_RELATIVE_LAYOUT element
                        |  relativelayoutattributes T_END_RELATIVE_LAYOUT element relativelayout   //exei idi diavastei to ena <RelativeLayout den theloyme 2o
                        |  relativelayoutattributes T_END_RELATIVE_LAYOUT element 
                        ;


relativelayoutattributes:   layout_width layout_height 
                         |  layout_width layout_height android_id 
                         |  layout_width layout_height element
                         | %empty   
                         ;


textview:                 T_TEXT_VIEW  textviewattributes T_END_TEXT_VIEW  
                        ;

textviewattributes:      T_ESTADO_0 T_ESTADO_1 
                        |T_ESTADO_0 T_ESTADO_1 T_ESTADO_2 T_ESTADO_3 
                        |T_ESTADO_0 T_ESTADO_1 T_ESTADO_2 
                        ;

imageview:                T_IMAGE_VIEW  imageviewattributes T_SLASH_END_TAG 
                        ;

imageviewattributes:     layout_width layout_height 
                        |layout_width layout_height src android_id 
                        |layout_width layout_height src android_id padding
                        |layout_width layout_height src padding
                        |layout_width layout_height src 
                        ;

button:                   T_BUTTON  buttonattributes T_END_BUTTON 
                        ;

buttonattributes:        layout_width layout_height
                        |layout_width layout_height text android_id 
                        |layout_width layout_height text android_id padding
                        |layout_width layout_height text
                        ;

radiogroup:               T_RADIO_GROUP radio_start radiogroupattributes T_END_TAG radiobutton radio_end T_END_RADIO_GROUP  
                        ; 

radio_start:            %empty
                        {
                        flag_radio_button =1;
                        };   


radio_end:          %empty
                        {
                            flag_radio_button = 0;
                            check_values();
                            check_radio_group_child_count(child_count, child_count_rge);
                        }
                    ;



radiogroupattributes:    layout_width layout_height
                        | layout_width layout_height text
                        | layout_width layout_height checkedButton
                        | layout_width layout_height checkedButton radio_group_element      ///////
                        | layout_width layout_height radiobutton
                        | layout_width layout_height radiobutton checkedButton
                        | layout_width layout_height text android_id
                        | layout_width layout_height text android_id checkedButton
                        | layout_width layout_height text radiobutton
                        | layout_width layout_height text android_id radio_group_element radiobutton
                        | layout_width layout_height text android_id  radiobutton
                        | layout_width layout_height text android_id checkedButton radiobutton
                        | layout_width layout_height text android_id checkedButton radio_group_element radiobutton 
                        ;


radio_group_element:        T_RADIO_BUTTON_CHILD_COUNT T_POSITIVE_INTEGER
                            {
                                const char* str = $2;  // Get the string value
                                int len = strlen(str);
                                char* int_str = malloc(len - 1);  // Allocate memory for the integer string
                                strncpy(int_str, str + 1, len - 2);  // Copy the inner part of the string without the quotes
                                int_str[len - 2] = '\0';  // Add null terminator
                                child_count_rge = atoi(int_str);  // Convert the string to an integer
                                free(int_str);  // Free the allocated memory
                                printf("Number of children for <RadioGroup>: %d\n", child_count_rge);
                            }
                            ;



radiobutton:    T_RADIO_BUTTON  radiobuttonattributes T_SLASH_END_TAG 
              {
                // Increment child count when a <RadioButton> is encountered
                increment_child_count();
              }
              | T_RADIO_BUTTON  radiobuttonattributes T_SLASH_END_TAG radiobutton  
              {
                // Increment child count when a <RadioButton> is encountered
                increment_child_count();
              }
             
              ;

radiobuttonattributes:    layout_width layout_height 
                        | layout_width layout_height android_id text  
                        | layout_width layout_height android_id text checkedButton
                        | layout_width layout_height text
                        ;                       


progressbar:              T_PROGRESS_BAR  progressbarattributes T_SLASH_END_TAG
                        ;

progressbarattributes:    layout_width layout_height
                        | layout_width layout_height android_id
                        | layout_width layout_height android_id max
                        | layout_width layout_height android_id max progress
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

element:                  textview element
                        | radiogroup element
                        | radiobutton element
                        | relativelayout element
                        | imageview element  
                        | imageview 
                        | progressbar 
                        | checkedButton                  
                        |%empty
                        ;                /*ebgala to linear*/
                       



%%

int main(int argc, char *argv[]){
    int token;

    /*Διάβασμα του αρχείου*/


    if(argc > 1){
        yyin = fopen(argv[1], "r");
        if (yyin == NULL){
            perror ("Error opening file"); 
            return -1;
        }
    }

    /*Κάνε συνατικτική ανάλυση*/
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
        printf("-> ERROR at line %d caused by %s : %s\n", lineno, yytext, message);
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


int dq_string_to_int(const char* str) {
    if (str[0] == '"' && str[strlen(str)-1] == '"') {
        char* new_str = malloc(strlen(str) - 1);
        strncpy(new_str, str + 1, strlen(str) - 2); 
        new_str[strlen(str) - 2] = '\0'; 

        int result = atoi(new_str); 
        free(new_str); 
        return result;
    } else {
        
        return 0;
    }
}

/////////////// functions to ensure every android:id is unique //////////////////////

int isIdUsed(char* id) {                      
    for (int i = 0; i < used_id_count; i++) {
        if (strcmp(used_ids[i], id) == 0) {
            return 1;  // ID is used
        }
    }
    return 0;  // ID is not used
}

void addUsedId(char* id) {
    strncpy(used_ids[used_id_count++], id, MAX_ID_LENGTH - 1);
    used_ids[used_id_count - 1][MAX_ID_LENGTH - 1] = '\0';  // Ensure null-termination
}

void addUsedIdInt(int id) {
    char id_string[MAX_ID_LENGTH];
    snprintf(id_string, sizeof(id_string), "%d", id);
    addUsedId(id_string);
}

//////////////////////////////////////////////////////////////////////////////// 3o erwthma /////////////////////////////////////////////////////////////

void increment_child_count() {
    child_count++;
}

void check_radio_group_child_count(int expected_count, int cumulative_count) {
    if (cumulative_count != expected_count) {
        char error_message[100];
        printf("cumulative_count: %d\n", cumulative_count);
        sprintf(error_message, "Invalid number of children. Expected %d children for <RadioGroup>.", expected_count);
        yyerror(error_message);
    } else {
        printf("The number of children is correct!\n\n");
    }
}


///////////////////////////////// ERWTHMA 2 CHECKED_BUTTON //////////////////////////////////////////////////////////////////////////////////////////////


void save_checked_button_id(char* id){
  static int k = 0;
  size_t len = strlen(id);

     strncpy(used_checkb[k], id, len);
     k++;
     
     /* for(int i=0;i<k;i++){
        //printf("\n%s\n",used_checkb[i]);
     } */

}

void checkRadioBId(char* id){
      static int k = 0;
      size_t len = strlen(id);

     strncpy(check_radiob[k], id, len);
     k++;
     
     for(int i=0;i<k;i++){
     printf("\n%s\n",check_radiob[i]);
     }
    
}

void check_values() {
    int i, j;
    int check_flag = 0;
    int counter = 0;

    for (i = 0; i < 5; i++) {
        int match_found = 0; //  track if a match is found for each used_checkb value
        for (j = 0; j < 5; j++) {
            if (strcmp(used_checkb[i], check_radiob[j]) == 0) {
                match_found = 1; // Match found, set the flag
                counter++;
                break; 
            }
        }
        if (match_found == 0) {
            check_flag = 1; // No match found, set the flag to indicate mismatch
            break; // No need to continue the outer loop if a mismatch is found
        }
    }

    //printf("\n\n FLAG VALUE: \n %d \n", check_flag);

    if (check_flag == 1) {
        char error_message[100];
        sprintf(error_message, "CHECKED BUTTON VALUE ISN'T IN THE USED RadioButton IDs LIST");
        yyerror(error_message);
    }
    else {
        printf("The values are the same!\n\n");
    }
}
