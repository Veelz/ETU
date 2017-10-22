all: 
	flex -osource\lex.yy.c source\lex.lex
	bison -o source\parser.tab.c source\parser.y
	bison -d -o source\parser.tab.h source\parser.y
	gcc -o parser.exe source\lex.yy.c source\parser.tab.c -lfl
	del source\parser.tab.c
	del source\lex.yy.c
	del source\parser.tab.h
	
