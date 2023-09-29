CLEANFILE_SRC = clean.bat
GRAMMAR_FILE = grammar.y
LEXICAL_FILE = lexical.l
TEST_SOURCE_C = test.c
TEST_SOURCE_CPP = test.cpp
LIB_DIR = "C:\Users\HP\Desktop\AFNtoAFD-gcc\parser"
OUTPUT_SHARED_LIB = libmi_parser.so
# all: Bison flex gcc run (old)
all: Bison flex buildcpp run
Bison:
	bison -d $(GRAMMAR_FILE)

flex:
	flex $(LEXICAL_FILE)

# gcc:
# 	gcc -c lex.yy.c -o lex.yy.o
# 	gcc -c grammar.tab.c -o grammar.tab.o
# 	gcc lex.yy.o grammar.tab.o -o myParser.exe

buildc:
	gcc -shared -o $(OUTPUT_SHARED_LIB) grammar.tab.c lex.yy.c
	gcc -o src/testc $(TEST_SOURCE_C) $(LIB_DIR)/$(OUTPUT_SHARED_LIB)
	
 
buildcpp:
	gcc -shared -o $(OUTPUT_SHARED_LIB) grammar.tab.c lex.yy.c
	g++ -o src/testcpp $(TEST_SOURCE_CPP) $(LIB_DIR)/$(OUTPUT_SHARED_LIB)
run:
	src/testcpp

cl:
	$(CLEANFILE_SRC)

