				char **filelist;
				int pos = 0;
				int line = 0;
				int ignore_comments;
DIGIT 			[0-9]
ID  			[A-Za-z_][A-Za-z0-9_]*
SPACES			[ \f\r\t\v]
OPERATOR		\+|\-|\*|\/|\%|==|!=|>|>=|<|<=|&&|\|\|
KEY_W			skip|write|read|if|then|else|do|while

%{
#pragma GCC diagnostic ignored "-Wformat"
#include <string.h>
#include "parser.tab.h"
#include "parser_types.h"

#define INT				"Num"
#define KW 		        "KW_"
#define OP 				"Op"
#define IDENT			"Var"
#define SEMICOLON 		"Semicolon"
#define ASSIGNMENT		"Assignment"
#define OPEN_BRACKET 	"Open_bracket"
#define CLOSE_BRACKET 	"Close_bracket"

int ml_start_pos = 0; 
int ml_start_line = 0;
int ml_leng = 0;
char string_buf[150];
char *string_buf_ptr;
char comment_string[] = "MultiLineComment(\"";
char *op[] = {"Plus", "Minus", "Mul", "Div", "Mod", "Eq","Neq","Greater","Geq","Less","Leq","And","Or"};
char *key_words[] = {"skip", "write", "read", "if", "then", "else", "do", "while" };
char *symbols[] = {"+", "-", "*", "/", "%", "==", "!=", ">", ">=", "<", "<=", "&&", "||"};

#define write_number(ptr, number) \
	{ \
		itoa(number, ptr, 10); \
		int count = 0; \
		do { \
			ptr++; \
			number = number / 10;\
		}while (number > 0); \
	}
#define init_yylval() \
	{ \
		yylval.nPtr.line = line; \
		yylval.nPtr.start = pos - yyleng; \
		yylval.nPtr.end =  pos - 1; \
	}
%}

%x LINE_COMMENT	ML_COMMENT

%%
"//"				BEGIN(LINE_COMMENT);					/* begin one-line comment */

<LINE_COMMENT>[\n]  {
						BEGIN(INITIAL);						/* end one-line comment */
						line++;
						pos = 0;
					}

<LINE_COMMENT>. 	{
							
					}

"(*"				{/* begin multi-line comment */ 
						BEGIN(ML_COMMENT);
						pos += yyleng;
					}
<ML_COMMENT>{
	\n				{
						pos = 0;
						line++;
					}
	[^*\n]+			|
	"*"+[^*)\n]*	{
						pos += yyleng;
					}
	"*"+")"			{/* end multi-line comment */
						pos += yyleng;
						BEGIN(INITIAL);							
					}
	}


{SPACES}			/*do nothing*/	

0{DIGIT}+			|

{DIGIT}+{ID}		{
						pos += yyleng;
						//printf("error(\"%s\", %d, %d, %d);\n", yytext, line, pos - yyleng, pos - 1); ;
					}

{DIGIT}+			{
						pos += yyleng;
						//printf("%s(%s, %d, %d, %d);\n", INT, yytext, line, pos - yyleng, pos - 1);
						init_yylval()
						yylval.nPtr.param.number = atoi(yytext);
						return INTEGER;
						//yylval = atoi(yytext);
						//return INTEGER;
					}
				
{KEY_W}				{
						pos += yyleng; 
						//printf("%s%s(%d, %d, %d);\n", KW, yytext, line, pos - yyleng, pos - 1);
						init_yylval()
						int i = 0;
						while ( strstr(key_words[i], yytext) == NULL ) 
							i++;
						switch (i)
						{
						case 0: return KW_skip;
						case 1: return KW_write;
						case 2: return KW_read;
						case 3: return KW_if;
						case 4: return KW_then;
						case 5: return KW_else;
						case 6: return KW_do;
						case 7: return KW_while;
						}
					}

				
{ID} 				{
						pos += yyleng; 
						//printf("%s(\"%s\", %d, %d, %d);\n", IDENT, yytext, line, pos - yyleng, pos - 1);
						init_yylval()
						yylval.nPtr.param.name = (char *)malloc(sizeof(yytext) * yyleng);
						strcpy(yylval.nPtr.param.name, yytext);
						//printf("%s\n", yylval.nPtr.param.name);
						return IDENTIFIER;
					}

"**"				{
						pos += yyleng;
						//printf("%s(%s, %d, %d, %d);\n", OP, "Pow", line, pos - yyleng, pos - 1);
						init_yylval()
						return POW;
					}
					
[+\-*/%><]			{
						int i = 0; 
						while (i < 13 && (strstr(symbols[i], yytext) == NULL) ) 
							i++;
						pos += yyleng;
						//printf("%s(%s, %d, %d, %d);\n", OP, op[i], line, pos - yyleng, pos - 1);
						init_yylval()
						return *yytext;
					}
"=="				{
						pos += yyleng;
						//printf("%s(%s, %d, %d, %d);\n", OP, op[5], line, pos - yyleng, pos - 1);
						init_yylval()
						return EQ;
					}
"!="				{
						pos += yyleng;
						//printf("%s(%s, %d, %d, %d);\n", OP, op[6], line, pos - yyleng, pos - 1);
						init_yylval()
						return NE;
					}
">="				{
						pos += yyleng;
						//printf("%s(%s, %d, %d, %d);\n", OP, op[7], line, pos - yyleng, pos - 1);
						init_yylval()
						return GE;
					}
"<="				{
						pos += yyleng;
						//printf("%s(%s, %d, %d, %d);\n", OP, op[8], line, pos - yyleng, pos - 1);
						init_yylval()
						return LE;
					}
"&&"				{
						pos += yyleng;
						//printf("%s(%s, %d, %d, %d);\n", OP, op[9], line, pos - yyleng, pos - 1);
						init_yylval()
						return AND;
					}
"||"				{
						pos += yyleng;
						//printf("%s(%s, %d, %d, %d);\n", OP, op[10], line, pos - yyleng, pos - 1);
						init_yylval()
						return OR;
					}
":="				{
						pos += yyleng;
						//printf("%s(%s, %d, %d, %d);\n", OP, ASSIGNMENT, line, pos - yyleng, pos - 1);
						init_yylval()
						return ASSIGN;
					}

\(					{
						pos += yyleng; 
						//printf("%s(%d, %d, %d);\n", OPEN_BRACKET, line, pos - yyleng, pos - 1);
						init_yylval()
						return *yytext;
					}
				
\)					{
						pos += yyleng; 
						//printf("%s(%d, %d, %d);\n", CLOSE_BRACKET, line, pos - yyleng, pos - 1);
						init_yylval()
						return *yytext;
					}
				
;					{
						pos += yyleng; 
						//printf("%s(%d, %d, %d);\n", SEMICOLON, line, pos - yyleng, pos - 1);
						init_yylval()
						return *yytext;
					}

\n 					{
						line++;
						pos = 0;
						return *yytext;
					}				
				
.					{
						pos += yyleng;
						//printf("error(\"%s\", %d, %d, %d);\n", yytext, line, pos - yyleng, pos - 1); 
					}
%%