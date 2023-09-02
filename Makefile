all: Bison flex gcc run

Bison:
	bison -d grammar.y

flex:
	flex lexical.l

gcc:
	gcc -c lex.yy.c -o lex.yy.o
	gcc -c grammar.tab.c -o grammar.tab.o
	gcc lex.yy.o grammar.tab.o -o myParser.exe

run: 
	./myParser.exe CodeXML.xml