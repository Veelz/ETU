#ifndef PARSER_TYPES_H
#define PARSER_TYPES_H

typedef enum {typeId, typeOp, typeKW, typeInt} nodeEnum;

struct nodeType{
	unsigned line;
	unsigned start;
	unsigned end;
	nodeEnum type;
	union {
		char *name;		//name of identifier
		int number; 	//constant
		int key_word;	//type of key word
		int oper;		//type of operator
	} param;

};

typedef struct nodeType nodeType;

#endif