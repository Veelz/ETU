%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(char *);
int yylex(void);
static int deep;
extern FILE *yyin;
void print_border(int n)
{
	int i = 0;
	for (i = 0; i < n; i++)
		printf("    \t");
}
%}

%code requires {#include "parser_types.h"}

%union {
	nodeType nPtr;
};

%token <nPtr> INTEGER IDENTIFIER KW_skip KW_write KW_read KW_if KW_do KW_while
%nonassoc <nPtr> KW_then KW_else

%token <nPtr> '+' '-' '*' '/' '%' '>' '<' EQ NE GE LE AND OR ASSIGN POW '(' ')' ';'
%%
program
		: statement		{ print_border(++deep); printf(" root(program)\n"); }
		;
		
statement
		: KW_skip                                        { 
															print_border(deep);
															printf(" KW_skip(%d, %d, %d);\n", $1.line, $1.start, $1.end); 
														 }
        | IDENTIFIER ASSIGN expr                         { 
															print_border(++deep);
															printf(" Var(\"%s\", %d, %d, %d);\n", $1.param.name, $1.line, $1.start, $1.end);
														    printf(" Op(ASSIGNMENT, %d, %d, %d);\n", $2.line, $2.start, $2.end);
														 }
		| KW_write expr                                  { 
															print_border(++deep);
															printf(" KW_write(%d, %d, %d);\n", $1.line, $1.start, $1.end); 
														 }
		| KW_read IDENTIFIER                             { 
															print_border(deep);
															printf(" KW_read(%d, %d, %d);\n", $1.line, $1.start, $1.end);
															print_border(deep);
															printf(" Var(\"%s\", %d, %d, %d);\n", $2.param.name, $2.line, $2.start, $2.end);
														 }
		| KW_while expr KW_do statement                  { 
															print_border(++deep);
															printf(" KW_while(%d, %d, %d);\n", $1.line, $1.start, $1.end);
															print_border(deep);
															printf(" KW_do(%d, %d, %d);\n", $3.line, $3.start, $3.end);
														 }
		| KW_if expr KW_then statement KW_else statement { 
														    print_border(++deep);
															printf(" KW_if(%d, %d, %d);\n", $1.line, $1.start, $1.end);
															print_border(deep);
															printf(" KW_then(%d, %d, %d);\n", $3.line, $3.start, $3.end);
															print_border(deep);
															printf(" KW_else(%d, %d, %d);\n", $5.line, $5.start, $5.end);
														 }
		| statement delim statement						 { 
														 }
        ;
delim
		: '\n' ';'										{ print_border(++deep); printf("SEMICOLON(%d, %d, %d);\n", $2.line, $2.start, $2.end); }
		| ';' '\n'										{ print_border(++deep); printf("SEMICOLON(%d, %d, %d);\n", $1.line, $1.start, $1.end); }
		| ';'											{ print_border(++deep); printf("SEMICOLON(%d, %d, %d);\n", $1.line, $1.start, $1.end); }
		;
expr:   
		INTEGER					  { print_border(--deep); printf(" Num(%d, %d, %d, %d);\n", $1.param.number, $1.line, $1.start, $1.end); }
		| IDENTIFIER              { print_border(--deep); printf(" Var(\"%s\", %d, %d, %d);\n", $1.param.name, $1.line, $1.start, $1.end); }
        | expr '+' expr           { print_border(++deep); printf(" Op(+, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
        | expr '-' expr           { print_border(++deep); printf(" Op(-, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
        | expr '*' expr           { print_border(++deep); printf(" Op(*, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
        | expr '/' expr           { print_border(++deep); printf(" Op(/, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
		| expr '%' expr           { print_border(++deep); printf(" Op(%, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
		| expr '>' expr           { print_border(++deep); printf(" Op(>, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
		| expr '<' expr           { print_border(++deep); printf(" Op(<, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
		| expr EQ expr            { print_border(++deep); printf(" Op(==, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
		| expr NE expr            { print_border(++deep); printf(" Op(!=, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
		| expr GE expr            { print_border(++deep); printf(" Op(<=, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
		| expr LE expr            { print_border(++deep); printf(" Op(>=, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
		| expr AND expr           { print_border(++deep); printf(" Op(&&, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
		| expr OR expr            { print_border(++deep); printf(" Op(||, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
		| expr POW expr           { print_border(++deep); printf(" Op(**, %d, %d, %d);\n", $2.line, $2.start, $2.end); }
        | '(' expr ')'            { 
									print_border(++deep);
									printf(" OPEN_BRACKET(%d, %d, %d);\n", $1.line, $1.start, $1.end); 
									printf(" CLOSE_BRACKET(%d, %d, %d);\n", $3.line, $3.start, $3.end);
								  }
        ;

%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main(int argc, char *argv[]) {
	++argv, --argc;
	if ( argc > 0 )
	{
        yyin = fopen(argv[0], "r");
		deep = 5;
		yyparse();
	}
    else
	{
		printf("using: parser filename \n");
	}
	
    return 0;
}