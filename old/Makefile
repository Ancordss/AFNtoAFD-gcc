# Variables para los nombres de archivo
LEXICO_SRC = lexico.l
SINTAXIS_SRC = sintaxis.y
ANALIZADOR_EXEC = analizador.exe
ANALIZADOR_OUTPUT = analizador.output

# Variables para las rutas completas de los archivos
LEXICO_C = lex.yy.c
SINTAXIS_TAB_C = sintaxis.tab.c
SINTAXIS_TAB_H = sintaxis.tab.h

CLEANFILE_SRC = clean.bat


all: flex bison build

clean:
	$(CLEANFILE_SRC)

flex:
	flex $(LEXICO_SRC)

bison:
	bison -t -v -d $(SINTAXIS_SRC)

build:
	gcc -o $(ANALIZADOR_EXEC) $(SINTAXIS_TAB_C) $(LEXICO_C)

run:
	./$(ANALIZADOR_EXEC) test.xml



# all: flex bison build

# clean:
# 	rm .\sintaxis.tab.c 
# 	rm .\lex.yy.c 
# 	rm .\sintaxis.tab.h 
# 	rm .\analizador.output

# flex:
# 	flex lexico.l

# bison:
# 	bison -t -v -d sintaxis.y

# build:
# 	gcc -o analizador sintaxis.tab.c lex.yy.c

# run:
# 	./analizador test.xml



