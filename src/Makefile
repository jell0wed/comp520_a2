all: mini.l mini.y tree.c tree.h
	bison -d mini.y
	flex mini.l
	gcc mini.tab.c lex.yy.c tree.c -o mini
clean:
	rm -rf lex.yy.c mini.tab.c mini.tab.h
