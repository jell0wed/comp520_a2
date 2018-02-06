all: mini.l mini.y tree.c tree.h pretty.c pretty.h
	bison -d mini.y
	flex mini.l
	gcc mini.tab.c lex.yy.c tree.c pretty.c -o mini
clean:
	rm -rf lex.yy.c mini.tab.c mini.tab.h
