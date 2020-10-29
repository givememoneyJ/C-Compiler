#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct ids ids;
typedef struct tokens token;
typedef struct scopeNode node;
typedef struct currentStack stak;
typedef struct greenNodes greenNode;

node *currentGreenNode;
node *allNodes;
stak *top;
greenNode *greenNodeList;

int arr_size;


//global files
FILE *tokenfile;
FILE *outputfile;
FILE *file;
FILE *addressComp;

// union for memory addresses and integers
union idAttr{
    int integer;
    ids *pntr;
};

// scope node
struct scopeNode{
    bool isRoot;
    bool isParam;
    bool isArray;
    bool isGreenNode;
    int typeInt;
    int arrMin;
    int arrMax;
    int arrayLength;
    int offset;
    int length;
    char lex[25];
    int lineNum;
    int numParams;
    char *addr;
    struct scopeNode *right;
    struct scopeNode *down;
    struct scopeNode *prev;
} *head_node;

struct greenNodes{
    char id[10];
    int numParams;
    //node *addr;
    struct greenNodes *next;
};

struct currentStack{
    char id[10];
    node *addr;
    struct currentStack *next;
};

// reserved words linked list
struct reswords{
    char name[10];
    int id;
    int attr;
    struct reswords *next;
}*head_r;

// symbol linked list
struct symbols{
    char name[2];
    int id;
    int attr;
    struct symbols *next;
}*head_s;

// id linked list
struct ids{
    char name[10];
    int id;
    union idattr{
        int intNum;
        char* pntr;
    }types;
    struct ids *next;
}*head_id;

// token linked list
struct tokens{
    int line_num;
    char lexeme[10];
    int type;
    union attr{
        int intNum;
        char* pntr;
    }attrs;
    struct tokens *next;
}*head_tok;



int error(char *m);
void append(struct ids** head_ref, char *new_data, char* address);
token createToken(char *str,int id, int attr);
token createToken2(char *str,int id, char* attr); // creates token with pointer instead of int
int constructTokenSymbols();  // Constructs Symbols LinkedList
int constructTokenWords();  // Constructs reservedwords LinkedList
token getResword(char *s);
token getSymbol(char *str);
char isDelim(char ch);
token validId(char *str); // Returns a token if the string is a VALID Id.
token isInteger(char *str); // Returns an integer token
token isRealNumber(char *str); // Returns a REAL token
char *subString(char *str, int left, int right);  // Extracts the SUBSTRING.
token getToken();  // getting each token per buffer line
token getNextTokenFromLine();

void parse();
void getNext();
int match(int x, int y);

node* currentGreenNode;

// parsing functions
void parse_program();
void parse_program_();
void parse_program__();
void parse_id_list();
void parse_id_list_();
void parse_declarations();
void parse_declarations_();
int parse_type();
int parse_std_type();
void parse_subprogram_declarations();
void parse_subprogram_declarations_();
void parse_subprogram_declaration();
void parse_subprogram_declaration_();
void parse_subprogram_declaration__();
void parse_subprogram_head();
void parse_subprogram_head_();
void parse_arguments();
void parse_param_list();
void parse_param_list_();
void parse_compound_statement();
void parse_compound_statement_();
void parse_optional_statements();
void parse_statement_list();
void parse_statement_list_();
void parse_statement();
void parse_statement_();
int parse_var();
int parse_var_(int inherited);
int parse_procedure_stmt();
int parse_procedure_stmt_(int numParams, node* ptr);
void parse_expr_list(int numParams, node *ptr);
void parse_expr_list_(int numParams, node *ptr);
int parse_expr();
int parse_expr_(int inherited);
int parse_simple_expr();
int parse_simple_expr_(int inherited);
int parse_term();
int parse_term_(int inherited);
int parse_factor();
int parse_factor_(int inherited);
void parse_sign();
int getType(char* lex);
node* getTypeGreen(char* lex);
node* getParams(char* lex);
node* getParamType(char* lex);
char* getComparison(int x);

void synch();
void synerr(char *error, char *expected);
void checkAddGreenNode(char* lex, int type);
void checkAddBlueNode(char* lex, int type);
void closeScope();

typedef enum Production
{
	PROGRAM,
	PROGRAM_,
	PROGRAM__,
	ID_LIST,
	ID_LIST_,
	DECLARATIONS,
	DECLARATIONS_,
	TYPE,
	STD_TYPE,
	SUBPROGRAM_DECLARATIONS,
	SUBPROGRAM_DECLARATIONS_,
	SUBPROGRAM_DECLARATION,
	SUBPROGRAM_DECLARATION_,
	SUBPROGRAM_DECLARATION__,
	SUBPROGRAM_HEAD,
	SUBPROGRAM_HEAD_,
	ARGUMENTS,
	PARAM_LIST,
	PARAM_LIST_,
	COMPOUND_STATEMENT,
	COMPOUND_STATEMENT_,
	OPTIONAL_STATEMENTS,
	STATEMENT_LIST,
	STATEMENT_LIST_,
	STATEMENT,
	STATEMENT_,
	VAR,
	VAR_,
    PROCEDURE_STMT,
    PROCEDURE_STMT_,
	EXPR_LIST,
	EXPR_LIST_,
	EXPR,
	EXPR_,
	SIMPLE_EXPR,
	SIMPLE_EXPR_,
	TERM,
	TERM_,
	FACTOR,
	FACTOR_,
	SIGN
} Production;

// types for typeCheck
#define INT 6
#define REAL 8
#define BOOLEAN 3
#define AINT 4
#define AREAL 5
#define PPINT 16
#define PPREAL 18
#define PPAINT 14
#define PPAREAL 15
#define ERR 99
#define PGNAME 11
#define PGPARAM 12
#define PROC 13


// types
#define T_RESWORD 1
#define T_ID 2
#define T_ADDOP 3
#define T_MULOP 4
#define T_RELOP 5
#define T_NUM 6
#define T_ASSIGNOP 120
#define T_SYMBOL 9
#define T_EOF 10


#define T_COMMA 130
#define T_SEMICOLON 140
#define T_COLON 190
#define T_PERIOD 150
#define T_DOUBLEPERIOD 170
#define T_LBRACK 242
#define T_RBRACK 253
#define T_LPAREN 101
#define T_RPAREN 111
#define T_LEXERR 19
#define T_PROGRAM 1027
#define T_VAR 1036
#define T_ARRAY 1001
#define T_OF 1023
#define T_INTEGER 1018
#define T_REAL 1028
#define T_PROCEDURE 1026
#define T_BEGIN 1002
#define T_END 1010
#define T_IF 1016
#define T_THEN 1032
#define T_ELSE 1009
#define T_WHILE 1037
#define T_DO 1006
#define T_NOT 1022
#define T_CALL 1112
#define RELOP_EQ 200
#define RELOP_GE 301
#define RELOP_GT 220
#define RELOP_LE 300
#define RELOP_NE 302
#define RELOP_LT 230
#define NUM_INT 6
#define NUM_REAL 8
#define NUM_LONGREAL
#define ADDOP_PLUS 161
#define ADDOP_MINUS 162
#define ADDOP_OR 1024
#define MULOP_TIMES 181
#define MULOP_DIVIDE 182
#define MULOP_DIV 1005
#define MULOP_MOD 1020
#define MULOP_AND 1000
#define ERR_ID_LEN
#define ERR_INT_LEN
#define ERR_INT_LEADING_ZERO
#define ERR_DECIMAL_LEN
#define ERR_EXPONENT_LEN
#define ERR_EXPONENT_LEADING_ZERO
#define ERR_UNRECOG_TOKEN


#endif
