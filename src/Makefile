all: mini.l mini.y tree.c tree.h pretty.c pretty.h symbol.c symbol.h type.c type.h error.c error.h
	bison -d mini.y
	flex mini.l
	gcc mini.tab.c lex.yy.c tree.c pretty.c symbol.c type.c error.c -o mini
clean:
	rm -rf lex.yy.c mini.tab.c mini.tab.h
