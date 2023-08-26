all: flex bison build

clean:
	rm ./analizador.exe sintaxis.tab.c lex.yy.c sintaxis.tab.h analizador.output

flex:
	flex lexico.l

bison:
	bison -t -v -d sintaxis.y

build:
	gcc -o analizador sintaxis.tab.c lex.yy.c

run:
	./analizador test.xml


