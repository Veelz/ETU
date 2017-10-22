/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER = 258,
     IDENTIFIER = 259,
     KW_skip = 260,
     KW_write = 261,
     KW_read = 262,
     KW_if = 263,
     KW_do = 264,
     KW_while = 265,
     KW_else = 266,
     KW_then = 267,
     EQ = 268,
     NE = 269,
     GE = 270,
     LE = 271,
     AND = 272,
     OR = 273,
     ASSIGN = 274,
     POW = 275,
	 LINE_COM = 276,
	 MULTILINE_COM = 277,
	 END = 278
   };
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

typedef struct nodeType{
	unsigned line;
	unsigned end_line;
	unsigned start;
	unsigned end;
	union {
		char *name;		//name of identifier
		char *text;		//comment's text
		int number; 	//constant
		int key_word;	//type of key word
		int oper;		//type of operator
	} param;

} nodeType;

typedef union YYSTYPE
{
	
	nodeType nPtr;

} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

YYSTYPE yylval;


