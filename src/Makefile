all: mini.l mini.y tree.c tree.h pretty.c pretty.h symbol.c symbol.h type.c type.h error.c error.h code.c code.h
	bison -v -d mini.y
	flex mini.l
	gcc mini.tab.c lex.yy.c tree.c pretty.c symbol.c type.c error.c code.c -o mini
clean:
	rm -rf lex.yy.c mini.tab.c mini.tab.h
