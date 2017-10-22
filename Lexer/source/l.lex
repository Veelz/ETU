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

<LINE_COMMENT>[^\t\r\f\n\v]+ 	{
									pos += yyleng; 
									if (!ignore_comments)
									{
										yylval.nPtr.line = line;
										yylval.nPtr.start = pos - yyleng;
										yylval.nPtr.end =  pos - 1;
										yylval.nPtr.param.text = malloc(sizeof(char)*(yyleng+2));
										strcpy(yylval.nPtr.param.text, yytext);
										return LINE_COM;
									}
								}

"(*"				{/* begin multi-line comment */ 
						BEGIN(ML_COMMENT);
						if (!ignore_comments)
						{
							string_buf_ptr = string_buf;
							
							char *yptr = comment_string;
							while ( *yptr )
								*string_buf_ptr++ = *yptr++;
							
							char *yptr2 = yytext;
							while ( *yptr2 )
								*string_buf_ptr++ = *yptr2++;
						}
						ml_start_pos = pos;
						ml_start_line = line;
						ml_leng += yyleng;
						pos += yyleng;
					}
<ML_COMMENT>{
	\n				{
						if (!ignore_comments)
						{
							*string_buf_ptr++ = '\\';
							*string_buf_ptr++ = 'n';
						}
						ml_leng++;
						pos = 0;
						line++;
					}
	[^*\n]+			|
	"*"+[^*)\n]*	{
						if (!ignore_comments)
						{
							int i = 0;
							for (i = 0; i < yyleng; i++) {
								switch (yytext[i])
								{
								case '\t':
									*string_buf_ptr++ = '\\'; *string_buf_ptr++ = 't'; break;
								case '\r': 
									*string_buf_ptr++ = '\\'; *string_buf_ptr++ = 'r'; break;
								case '\v': 
									*string_buf_ptr++ = '\\'; *string_buf_ptr++ = 'v'; break;
								default: 
									*string_buf_ptr++ = yytext[i];
								}
							}
						}
						pos += yyleng;
					}
	"*"+")"			{/* end multi-line comment */
						pos += yyleng;
						BEGIN(INITIAL);		
						if (!ignore_comments)
						{
							ml_leng += yyleng;
							char *yptr = yytext;
							while ( *yptr )
								*string_buf_ptr++ = *yptr++;
							*string_buf_ptr++ = '\"';
							*string_buf_ptr++ = ',';
							*string_buf_ptr++ = ' ';
							write_number(string_buf_ptr, ml_start_line)
							*string_buf_ptr++ = ',';
							*string_buf_ptr++ = ' ';
							write_number(string_buf_ptr, ml_start_pos)
							*string_buf_ptr++ = ',';
							*string_buf_ptr++ = ' ';
							int ml_line = line;
							write_number(string_buf_ptr, ml_line)
							*string_buf_ptr++ = ',';
							*string_buf_ptr++ = ' ';
							write_number(string_buf_ptr, ml_leng)
							*string_buf_ptr++ = ')';
							*string_buf_ptr++ = ';';
							*string_buf_ptr++ = '\n';
							*string_buf_ptr++ = '\0';
							
							string_buf_ptr = string_buf;
							yylval.nPtr.line = ml_start_line;
							yylval.nPtr.start = ml_start_pos;
							yylval.nPtr.end_line = ml_line;
							yylval.nPtr.end = ml_leng;
							yylval.nPtr.param.text = malloc(sizeof(char)*150);
							strcpy(yylval.nPtr.param.text, string_buf_ptr);
							return MULTILINE_COM;
						}
						ml_start_pos = 0;
						ml_start_line = 0;
						ml_leng = 0;
										
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
						init_yylval()
						yylval.nPtr.param.number = atoi(yytext);
						return INTEGER;
					}
				
{KEY_W}				{
						pos += yyleng; 
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
						init_yylval()
						yylval.nPtr.param.name = (char *)malloc(sizeof(yytext) * yyleng);
						strcpy(yylval.nPtr.param.name, yytext);
						return IDENTIFIER;
					}

"**"				{
						pos += yyleng;
						init_yylval()
						return POW;
					}
					
[+\-*/%><]			{
						int i = 0; 
						while (i < 13 && (strstr(symbols[i], yytext) == NULL) ) 
							i++;
						pos += yyleng;
						init_yylval()
						return *yytext;
					}
"=="				{
						pos += yyleng;
						init_yylval()
						return EQ;
					}
"!="				{
						pos += yyleng;
						init_yylval()
						return NE;
					}
">="				{
						pos += yyleng;
						init_yylval()
						return GE;
					}
"<="				{
						pos += yyleng;
						init_yylval()
						return LE;
					}
"&&"				{
						pos += yyleng;
						init_yylval()
						return AND;
					}
"||"				{
						pos += yyleng;
						init_yylval()
						return OR;
					}
":="				{
						pos += yyleng;
						init_yylval()
						return ASSIGN;
					}

\(					{
						pos += yyleng; 
						init_yylval()
						return *yytext;
					}
				
\)					{
						pos += yyleng; 
						init_yylval()
						return *yytext;
					}
				
;					{
						pos += yyleng; 
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
					}
<<EOF>>				return END;
%%

void print_tokens()
{
	int token;
	while (yywrap())
	{
		token = yylex();
		switch(token)
		{
			case INTEGER :  	printf("Num(%d, %d, %d, %d);\n", yylval.nPtr.param.number, yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end); break;
			case IDENTIFIER : 	printf("Var(\"%s\", %d, %d, %d);\n", yylval.nPtr.param.name, yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end); break;
			case KW_skip : 		printf("KW_skip(%d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end); break;
			case KW_write :		printf("KW_write(%d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end); break;
			case KW_read :		printf("KW_read(%d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);  break;
			case KW_if :		printf("KW_if(%d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);    break;
			case KW_do :		printf("KW_do(%d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);    break;
			case KW_while :		printf("KW_while(%d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end); break;
			case KW_else :		printf("KW_else(%d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);  break;
			case KW_then :		printf("KW_then(%d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);  break;
			case '+' :			printf("Op(+, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);    break;
			case '-' :          printf("Op(-, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);    break;
			case '*' :          printf("Op(*, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);    break;
			case '/' :          printf("Op(/, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);    break;
			case '%' :          printf("Op(%, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);    break;
			case '>' :          printf("Op(>, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);    break;
			case '<' :          printf("Op(<, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);    break;
			case EQ  :          printf("Op(==, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);   break;
			case NE  :          printf("Op(!=, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);   break;
			case GE  :          printf("Op(<=, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);   break;
			case LE  :          printf("Op(>=, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);   break;
			case AND :          printf("Op(&&, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);   break;
			case OR  :          printf("Op(||, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);   break;
			case POW :          printf("Op(**, %d, %d, %d);\n", yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end);   break;
			case MULTILINE_COM: printf("%s", yylval.nPtr.param.text ); break;
			case LINE_COM:		printf("LineComment(\"//%s\", %d, %d, %d);\n", yylval.nPtr.param.text, yylval.nPtr.line, yylval.nPtr.start, yylval.nPtr.end); break;
			case END: 			return;
		}
	}
}

int main(int argc, char** argv)
{
	++argv, --argc;
	if (argc > 0 && strstr(argv[0],"-filter"))
	{
		ignore_comments = 1;
		argv++;
		argc--;
	}
	else
	{
		ignore_comments = 0;
	}
	if ( argc > 0 )
	{
        yyin = fopen( argv[0], "r" );
		if (yyin == NULL)
		{
			printf("file can not be open");
			yyterminate();
		}
	}
    else
	{
		printf("using: lexer [-filter] filename\n");
		yyterminate();
	}
	filelist = argv;
	printf("FILENAME(\"%s\");\n", *filelist);

	print_tokens();	
}
