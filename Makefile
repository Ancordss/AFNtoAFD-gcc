
SOURCE_CPP = main.cpp
CLEANFILE_SRC = src/clean.bat
LIB_DIR = "D:\UNI\6to semestre\automatas\AFNtoAFD-gcc\parser"
OUTPUT_SHARED_LIB = libmi_parser.so
# all: Bison flex gcc run (old)
all: buildcpp run

 
buildcpp:
	gcc -shared -o $(OUTPUT_SHARED_LIB) parser/grammar.tab.c parser/lex.yy.c
	g++ -o src/AFNtoAFD $(SOURCE_CPP) $(LIB_DIR)/$(OUTPUT_SHARED_LIB)
run:
	src/AFNtoAFD.exe
cl:
	$(CLEANFILE_SRC)




