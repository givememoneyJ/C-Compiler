#include "global.h"


token tok;
token *tokPtr;
int t_num = -1;
int t_type = -1;

char* idPtr;
char* expecting;
char* value;



void parse(){
    tokPtr = head_tok;

    //temp = head_tok;
    parse_program();

    match(t_num,10);

}

void getNext(){
    tok = getNextTokenFromLine();
    tokPtr = tokPtr->next;
    t_num = tokPtr->attrs.intNum;
    if(t_num > 999999){
        t_num = 2;
    }
    t_type = tokPtr->type;
    //idPtr = tokPtr->attrs.pntr;
    //printf("Token: %d \n",t_num);

}

int match(int x, int y){
    if (x == 10){
        printf("End of File Reached.");
        exit(1);
    }
    if (x == y && x != 10){
        //printf("MATCHED '%s' \n",tokPtr->lexeme);
        getNext();
        return 1;
    }
    if (x != y && x !=10){
        synerr(tokPtr->lexeme,expecting);
        getNext();
    }
    return 0;
}


void parse_program()
{
    t_num = tokPtr->attrs.intNum;
    expecting = "PROGRAM";
	switch (t_num) {
        case T_PROGRAM:
            if(!match(T_PROGRAM, t_num)) { synch(PROGRAM); break;}
            expecting = "ID";
            // check add greennode
            checkAddGreenNode(tokPtr->lexeme,PGNAME);
            if(!match(T_ID, t_num)) { synch(PROGRAM); break;}
            expecting = "(";
            if(!match(T_LPAREN, t_num)){ synch(PROGRAM); break;}
            parse_id_list();
            expecting = ")";
            if(!match(T_RPAREN, t_num)) { synch(PROGRAM); break;}
            expecting = ";";

            if(!match(T_SEMICOLON, t_num)) { synch(PROGRAM); break;}
            parse_program_();
            //close the scope;
            //closeScope();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(PROGRAM);
            break;
	}
}

void parse_program_(){
    expecting = "'BEGIN','PROCEDURE','VAR'";
    switch(t_num){
        case T_BEGIN:
            parse_compound_statement();
            expecting =".";
            if(!match(T_PERIOD, t_num)) { synch(PROGRAM_); break;}
            break;
        case T_PROCEDURE:
            parse_subprogram_declarations();
            parse_compound_statement();
            expecting =".";
            if(!match(T_PERIOD, t_num)) { synch(PROGRAM_); break;}
            break;
        case T_VAR:
            parse_declarations();
            parse_program__();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            printf("%d",t_num);
            synch(PROGRAM_);
            break;
    }
}
void parse_program__(){
    expecting = "BEGIN or PROCEDURE";
    switch(t_num){
        case T_BEGIN:
            parse_compound_statement();
            expecting =".";
            if(!match(T_PERIOD, t_num)) { synch(PROGRAM__); break;}
            break;
        case T_PROCEDURE:
            parse_subprogram_declarations();
            parse_compound_statement();
            expecting =".";
            if(!match(T_PERIOD, t_num)) { synch(PROGRAM__); break;}
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(PROGRAM__);
            break;
    }
}
void parse_id_list(){
    expecting = "ID";
    switch(t_num){
        case T_ID:
            // check add blueNode
            checkAddBlueNode(tokPtr->lexeme,PGPARAM);
            if(!match(T_ID, t_num)) { synch(ID_LIST); break;}
            parse_id_list_();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(ID_LIST);
            break;
    }
}
void parse_id_list_(){
    expecting =",' or ')";
    switch(t_num){
        case T_COMMA:
            if(!match(T_COMMA, t_num)) { synch(ID_LIST_); break;}
            expecting ="ID";
            // check add blueNode
            checkAddBlueNode(tokPtr->lexeme,PGPARAM);
            if(!match(T_ID, t_num)) { synch(ID_LIST_); break;}

            parse_id_list_();
            break;
        case T_RPAREN:
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(ID_LIST_);
            break;
    }
}
void parse_declarations(){
    expecting = "VAR";
    char* lex;
    int type = -1;
    switch(t_num){
        case T_VAR:
            if(!match(T_VAR, t_num)) { synch(DECLARATIONS); break;}
            expecting = "ID";
            lex = tokPtr->lexeme;
            if(!match(T_ID, t_num)) { synch(t_num); break;}
            expecting = ":";
            if(!match(T_COLON, t_num)) { synch(DECLARATIONS); break;}
            type = parse_type();
            // check add blueNode
            checkAddBlueNode(lex,type);
            expecting = ";";
            if(!match(T_SEMICOLON , t_num)) { synch(DECLARATIONS); break;}
            parse_declarations_();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(DECLARATIONS);
            break;
    }
}
void parse_declarations_(){
    expecting = "'BEGIN','PROCEDURE', or 'VAR'";
    char* lex;
    int type = -1;
    switch(t_num){
        case T_BEGIN:
        case T_PROCEDURE:
            break;
        case T_VAR:
            if(!match(T_VAR, t_num)) { synch(DECLARATIONS_); break;}
            expecting = "ID";
            lex = tokPtr->lexeme;
            if(!match(T_ID, t_num)) { synch(DECLARATIONS_); break;}
            expecting = ":";
            if(!match(T_COLON , t_num)) { synch(DECLARATIONS_); break;}
            type = parse_type();
            // check add blueNode
            checkAddBlueNode(lex,type);
            expecting = ";";
            if(!match(T_SEMICOLON , t_num)) { synch(DECLARATIONS_); break;}
            parse_declarations_();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(DECLARATIONS_);
            break;
    }
}
int parse_type(){
    expecting = "ARRAY','INTEGER' or 'REAL";
    int type = -1;
    int num1 = -1;
    int num2 = -1;
    int an1;
    int an2;
    switch(t_num){
        case T_ARRAY:
            if(!match(T_ARRAY, t_num)) { synch(TYPE); break;}
            expecting = "[";
            if(!match(T_LBRACK, t_num)) { synch(TYPE); break;}
            expecting = "NUM";
            num1 = t_type;
            an1 = atoi(tokPtr->lexeme);
            if(!match(T_NUM, t_type)) { synch(TYPE); break;}
            expecting = "..";
            if(!match(T_DOUBLEPERIOD, t_num)) { synch(TYPE); break;}
            expecting = "NUM";
            num2 = t_type;
            an2 = atoi(tokPtr->lexeme);
            arr_size = (an2 - an1) + 1;
            if(!match(T_NUM, t_type)) { synch(TYPE); break;}
            expecting = "]";
            if(!match(T_RBRACK, t_num)) { synch(TYPE); break;}
            // check num1 and num2 types
            if(num1 != num2){
                type = ERR;
                fprintf(outputfile,"ERROR: type mismatch. Need to be INT\n");
            } else if((num1 == INT && num2 == INT) ||
                        (num1 == REAL && num2 == REAL)){
                ;
            } else{
                type = ERR;
                fprintf(outputfile,"ERROR: type mismatch. Need to be INT \n");;
            }
            expecting = "of";
            if(!match(T_OF , t_num)) { synch(TYPE); break;}
            // type = std_type()
            type = parse_std_type();
            if(type == INT){
                type = AINT;
            }
            if(type == REAL){
                type = AREAL;
            }
            break;
        case T_INTEGER:
        case T_REAL:
            // type = std_type()
            type = parse_std_type();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(TYPE);
            break;
    }
    return type;
}
int parse_std_type(){
    expecting = "'INT' or 'REAL'";
    int type = -1;
    switch(t_num){
        case T_INTEGER:
            if(!match(T_INTEGER, t_num)) { synch(STD_TYPE); break;}
            // type = int
            type = INT;
            break;
        case T_REAL:
            if(! match(T_REAL, t_num)) { synch(STD_TYPE); break;}
            // type = real
            type = REAL;
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(STD_TYPE);
            break;
    }
    return type;
}
void parse_subprogram_declarations(){
    expecting="PROCEDURE";
    switch(t_num) {
        case T_PROCEDURE:
            parse_subprogram_declaration();
            expecting=";";
            // close scope
            closeScope();
            if(!match(T_SEMICOLON, t_num)) { synch(SUBPROGRAM_DECLARATIONS); break;}

            parse_subprogram_declarations_();

            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(SUBPROGRAM_DECLARATIONS);
            break;
    }
}
void parse_subprogram_declarations_(){
    expecting="'BEGIN' or 'PROCEDURE'";
    switch(t_num) {
        case T_BEGIN:
            break;
        case T_PROCEDURE:
            parse_subprogram_declaration();
            expecting = ";";
            // close scope of current procedure
            closeScope();
            if(!match(T_SEMICOLON, t_num)) {
                synch(SUBPROGRAM_DECLARATIONS_); break;
            }

            parse_subprogram_declarations_();

            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(SUBPROGRAM_DECLARATIONS_);
            break;
    }
}
void parse_subprogram_declaration(){
    expecting="PROCEDURE";
    switch(t_num) {
        case T_PROCEDURE:
            parse_subprogram_head();
            parse_subprogram_declaration_();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(SUBPROGRAM_DECLARATION);
            break;
    }
}
void parse_subprogram_declaration_(){
    expecting = "'BEGIN','PROCEDURE' or 'VAR'";
    switch(t_num) {
        case T_BEGIN:
            parse_compound_statement();
            break;
        case T_PROCEDURE:
            parse_subprogram_declarations();
            parse_compound_statement();
            break;
        case T_VAR:
            parse_declarations();
            parse_subprogram_declaration__();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(SUBPROGRAM_DECLARATION_);
            break;
    }
}
void parse_subprogram_declaration__(){
    expecting = "'BEGIN' or 'PROCEDURE'";
    switch(t_num){
        case T_BEGIN:
            parse_compound_statement();
            break;
        case T_PROCEDURE:
            parse_subprogram_declarations();
            parse_compound_statement();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(SUBPROGRAM_DECLARATION__);
            break;
    }
}
void parse_subprogram_head(){
    expecting = "'PROCEDURE'";
    switch(t_num){
        case T_PROCEDURE:
            if(!match(T_PROCEDURE, t_num)) { synch(SUBPROGRAM_HEAD); break;}
            expecting = "ID";
            checkAddGreenNode(tokPtr->lexeme, PROC);
            if(!match(T_ID, t_num)) { synch(SUBPROGRAM_HEAD); break;}
            // check add green node
            parse_subprogram_head_();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(SUBPROGRAM_HEAD);
            break;
    }
}
void parse_subprogram_head_(){
    expecting = "(' or ';";
    switch(t_num) {
        case T_LPAREN:
            parse_arguments();
            expecting = "';'";
            if(!match(T_SEMICOLON, t_num)) { synch(SUBPROGRAM_HEAD_); break;}
            break;
        case T_SEMICOLON:
            if(!match(T_SEMICOLON, t_num)) { synch(SUBPROGRAM_HEAD_); break;}
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(SUBPROGRAM_HEAD_);
            break;
    }
}
void parse_arguments(){
    expecting = "(";
    switch(t_num){
        case T_LPAREN:
            if(!match(T_LPAREN, t_num)) { synch(ARGUMENTS); break;}
            parse_param_list();
            expecting = ")";
            if(!match(T_RPAREN, t_num)) { synch(ARGUMENTS); break;}
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(ARGUMENTS);
            break;
    }
}

void parse_param_list(){
    expecting = "ID";
    int type = -1;
    char* lex;
    switch(t_num){
        case T_ID:
            lex = tokPtr->lexeme;
            if(!match(T_ID , t_num)) { synch(PARAM_LIST); break;}
            expecting = ":";
            if(! match(T_COLON , t_num)) { synch(PARAM_LIST); break;}
            // add 10 for PPINT or PPREAL
            type = parse_type();

            // check add blue node
            checkAddBlueNode(lex, type+10);
            parse_param_list_();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(PARAM_LIST);
            break;
    }
}
void parse_param_list_(){
    expecting = ")' or ';";
    int type = -1;
    char* lex;
    switch(t_num){
        case T_RPAREN:
            break;
        case T_SEMICOLON:
            if(!match(T_SEMICOLON, t_num)) { synch(PARAM_LIST_); break;}
            expecting = "ID";
            lex = tokPtr->lexeme;
            if(!match(T_ID, t_num)) { synch(PARAM_LIST_); break;}
            expecting = ":";
            if(!match(T_COLON, t_num)) { synch(PARAM_LIST_); break;}
            // add 10 for PPINT or PPREAL
            type = parse_type();
            // check add blue node
            checkAddBlueNode(lex,type+10);
            parse_param_list_();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(t_num);
            break;
    }
}
void parse_compound_statement(){
    expecting = "BEGIN";
    switch(t_num){
        case T_BEGIN:
            if(!match(T_BEGIN, t_num)) { synch(COMPOUND_STATEMENT); break;}
            parse_compound_statement_();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(COMPOUND_STATEMENT);
            break;
    }
}
void parse_compound_statement_(){
    expecting = "'BEGIN','CALL','IF','WHILE','END','ID'";
    switch(t_num) {
        case T_BEGIN:
        case T_CALL:
        case T_ID:
        case T_IF:
        case T_WHILE:
            parse_optional_statements();
            expecting = "END";
            if(!match(T_END, t_num)) { synch(COMPOUND_STATEMENT_); break;}
            break;
        case  T_END:
            if(!match(T_END, t_num)) { synch(COMPOUND_STATEMENT_); break;}
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(COMPOUND_STATEMENT_);
            break;
    }
}
void parse_optional_statements(){
    expecting = "'BEGIN','CALL','IF','WHILE','ID'";
    switch(t_num){
        case T_BEGIN:
        case T_CALL:
        case T_ID:
        case T_IF:
        case T_WHILE:
            parse_statement_list();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(OPTIONAL_STATEMENTS);
            break;
    }
}
void parse_statement_list(){
    expecting = "'BEGIN','CALL','IF','WHILE','ID'";
    switch(t_num){
        case T_BEGIN:
        case T_CALL:
        case T_ID:
        case T_IF:
        case T_WHILE:
            parse_statement();
            parse_statement_list_();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(STATEMENT_LIST);
            break;
    }
}
void parse_statement_list_(){
    expecting = ";' or 'END";
    switch(t_num){
        case T_END:
            break;
        case T_SEMICOLON:
            if(!match(T_SEMICOLON,t_num)) { synch(STATEMENT_LIST); break;}
            parse_statement();
            parse_statement_list_();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(STATEMENT_LIST);
            break;
    }
}
void parse_statement(){
    expecting = "BEGIN','CALL','IF','WHILE','ID";
    int type1 = -1;
    int type2 = -1;
    char* e;
    char* e2;
    switch(t_num){
        case T_BEGIN:
            parse_compound_statement();
            break;
        case T_CALL:
            parse_procedure_stmt();
            break;
        case T_ID:
            type1 = parse_var();
            e = getComparison(type1);

            expecting=":=";
            if(!match(T_ASSIGNOP, t_num)) { synch(STATEMENT); break;}

            type2 = parse_expr();

            printf("lex: %s",tokPtr->lexeme);

            e2 = getComparison(type2);
            // check var() and expr()
            printf("type1: %d\n",type1);
            printf("type2: %d\n",type2);
            if(type1 != type2){
                if(type1 != ERR && type2 != ERR){
                    fprintf(outputfile,"Error: type mismatch. Can't assign type '%s' to %s\n", e2, e);
                }
            }

            break;
        case T_IF:
            if(!match(T_IF, t_num)) { synch(STATEMENT); break;}
            type1 = parse_expr();
            printf("if type: %d",type1);
            // check bool
            if(type1 == BOOLEAN || type1 == ERR){
                //type = ERR;
                ;
            }
            else {
                fprintf(outputfile,"ERROR: type mismatch in if statement \n");
                printf("\n\n Error in statement");
            }
            expecting="THEN";
            if(!match(T_THEN, t_num)) { synch(STATEMENT); break;}
            parse_statement();
            parse_statement_();
            break;
        case T_WHILE:
            if(!match(T_WHILE, t_num)) { synch(STATEMENT); break;}
            type1 = parse_expr();
            // check bool
            if(type1 == BOOLEAN || type1 == ERR){
                //type = ERR;
                ;
            } else {
                fprintf(outputfile,"ERROR: type mismatch in while statement \n");;
            }
            // check bool
            expecting="DO";
            if(!match(T_DO, t_num)) { synch(STATEMENT); break;}
            parse_statement();
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(STATEMENT);
            break;
    }
}
void parse_statement_(){
    expecting = "ELSE','END',';";
    switch(t_num){
        case T_ELSE:
            if(!match(T_ELSE, t_num)) { synch(STATEMENT_); break;}
            parse_statement();
            break;
        case T_END:
        case T_SEMICOLON:
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(STATEMENT_);
            break;
    }
}
int parse_var(){
    expecting = "ID";
    int type = ERR;
    int type_i = -1;
    switch(t_num) {
        case T_ID:
            type_i = getType(tokPtr->lexeme);
            //value = tokPtr->lexeme;
            printf("getType: %d\n", type_i);
            value = tokPtr->lexeme;
            if(!match(T_ID, t_num)) { synch(VAR); break;}
            // factor.i = getType(id.lex)
            type = parse_var_(type_i);
            // type = factor.i
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(VAR);
            break;
    }
    return type;
}
int parse_var_(int inherited){
    expecting = ":=' or '(";
    int type = ERR;
    switch(t_num){
        case T_ASSIGNOP:
            // inherited
            type = inherited;
            break;
        case T_LBRACK:
            if(!match(T_LBRACK , t_num)) { synch(VAR_); break;}
            // type = expr()
            type = parse_expr();
            expecting = "]";
            if(!match(T_RBRACK , t_num)) { synch(VAR_); break;}

            printf("type: %d\n",type);
            if(type == INT && inherited == AINT){
                type = INT;
            } else if (type == INT && inherited == PPAINT){
                type = INT;
            } else if (type == INT && inherited == PPINT){
                type = INT;
            } else if (type == INT && inherited == AREAL){
                type = REAL;
            } else if (type == INT && inherited == PPREAL){
                type = REAL;
            } else if (type == INT && inherited == PPAREAL){
                type = REAL;
            } else if (type == ERR || inherited == ERR){
                type = ERR;
            } else {
                type = ERR;
                fprintf(outputfile, "Error in var': type mismatch");
            }
            break;

        default:
            synerr(tokPtr->lexeme,expecting);
            synch(VAR_);
            break;
    }
    return type;
}
int parse_procedure_stmt(){
    node* paramPtr;
    expecting = "CALL";
    int type = ERR;
    int numParams = 0;
    switch(t_num){
        case T_CALL:
            if(!match(T_CALL, t_num)) { synch(PROCEDURE_STMT); break;}
            expecting = "ID";
            printf("ID = %s\n",tokPtr->lexeme);
            paramPtr = getTypeGreen(tokPtr->lexeme);
            numParams = paramPtr->numParams;
            //paramPtr = getParams(tokPtr->lexeme);

            if(!match(T_ID, t_num)) { synch(PROCEDURE_STMT); break;}

            printf("returned Green Node: %s\n",paramPtr->lex);
            printf("Num of params is: %d \n",numParams);
            printf("type of parameter: %d\n",paramPtr->right->typeInt);
            type = parse_procedure_stmt_(numParams,paramPtr);

            //printf("TypeInt:: %d\n", paramPtr->typeInt);

            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(PROCEDURE_STMT);
            break;
    }
    return type;
}
int parse_procedure_stmt_(int numParams,node* ptr){
    expecting = "ELSE','END',';','(";
    int type = ERR;
    switch(t_num) {
        case T_ELSE:
        case T_END:
        case T_SEMICOLON:
            if(numParams != 0){
                fprintf(outputfile,"Error, found zero parameters, expecting: %d\n",ptr->numParams);
            }
            break;
        case T_LPAREN:
            if(!match(T_LPAREN, t_num)) { synch(PROCEDURE_STMT_); break;}
            parse_expr_list(numParams, ptr);
            // expr_list(num_of_child, childPtr)
            expecting = ")";
            if(!match(T_RPAREN, t_num)) { synch(PROCEDURE_STMT_); break;}
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(PROCEDURE_STMT_);
            break;
    }
    return type;
}
void parse_expr_list(int numParams, node* ptr){
    expecting = "ID','(','+','-','NOT','INT','REAL";
    int type = -1;
    switch(t_num){
        case T_ID:
        case T_LPAREN:
        case ADDOP_MINUS:
        case T_NOT:
        case NUM_INT:
        case NUM_REAL:
        case ADDOP_PLUS:
            type = parse_expr();
            printf("Parameter Type1: %d\n",type);
            printf("Parameter type2: %d\n",ptr->right->typeInt);
            if(numParams == 0){
                printf("Error, zero params");
            } else if(ptr->right->typeInt != type){
                numParams = 100;
            }
            ptr = ptr->right;
            numParams--;

            parse_expr_list_(numParams, ptr);
            // inherit num_of_child
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(EXPR_LIST);
            break;
    }
}
void parse_expr_list_(int numParams, node* ptr){
    expecting = ",' or ')";
    int expr_list_ = ERR;
    switch(t_num){
        case T_COMMA:
            if(!match(T_COMMA, t_num)) { synch(EXPR_LIST_); break;}
            // check number of children
            if(numParams == 0){
                printf("Error, zero params");
            }
            // numChild--
            numParams--;


            expr_list_ = parse_expr();
            printf("\n ExprList: %d\n",expr_list_);

            printf("current param pointer %s\n",ptr->right->lex);
            printf("param type: %d\n",ptr->right->typeInt);
            if(expr_list_ == ptr->right->typeInt){
                ;
            } else {
                numParams = 99;
                //fprintf(outputfile,"TYPE mismatch in procedure\n");
            }
            ptr = ptr->right;
            parse_expr_list_(numParams, ptr);

            break;
        case T_RPAREN:
            // check num_of_child == 0
            printf("numParams: %d \n", numParams);
            if(numParams > 80){
                fprintf(outputfile,"Error: Parameter types don't match\n");
            } else if (numParams != 0){
                fprintf(outputfile,"Error: Number of Parameters doesn't match\n");
            }
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(EXPR_LIST_);
            break;
    }
}
int parse_expr(){
    expecting = "ID','(','+','-','NOT','INT','REAL";
    int type = ERR;
    int expr_i = -1;
    switch(t_num){
        case T_ID:
        case T_LPAREN:
        case ADDOP_MINUS:
        case T_NOT:
        case NUM_INT:
        case NUM_REAL:
        case ADDOP_PLUS:
            expr_i = parse_simple_expr();
            type = parse_expr_(expr_i);
            // type = expr_.i
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(EXPR);
            break;
    }
    return type;
}
int parse_expr_(int inherited){
    int type = ERR;
    expecting = ",','DO','ELSE','END',']',')',';','THEN','RELOP";
    char* e;
    char* e2;
    char* r;
    switch(t_num) {
        case T_COMMA:
        case T_DO:
        case T_ELSE:
        case T_END:
        case T_RBRACK:
        case T_RPAREN:
        case T_SEMICOLON:
        case T_THEN:
            type = inherited;
            break;
        case RELOP_EQ:
        case RELOP_GE:
        case RELOP_GT:
        case RELOP_LE:
        case RELOP_LT:
        case RELOP_NE:
            // type = bool or err*
            r = tokPtr->lexeme;
            if(!match(T_RELOP, t_type)) { synch(EXPR_); break;}
            int simple_expr_i = parse_simple_expr();

            e = getComparison(simple_expr_i);
            e2 = getComparison(inherited);


            printf("inherited: %d",inherited);
            printf("type = %d",simple_expr_i);
            if (inherited == simple_expr_i){
                if(simple_expr_i == REAL || simple_expr_i == INT){
                    type = BOOLEAN;
                } else if (simple_expr_i == ERR){
                    type = ERR;
                } else{
                    type = ERR;
                    fprintf(outputfile, "Error in expr_\n");
                }
            } else if (simple_expr_i == ERR || inherited == ERR){
                type = ERR;
            } else {
                type = ERR;
                fprintf(outputfile,"Error: type mismatch. Can't perform relop '%s' between type '%s' and '%s'\n",r, e2 ,e);
            }
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(EXPR_);
            break;
    }
    return type;
}
int parse_simple_expr(){
    expecting = "ID','(','+','-','NOT','INT','REAL";
    int type = ERR;
    int simp_expr_i = -1;
    switch(t_num){
        case T_ID:
        case T_LPAREN:
        case T_NOT:
        case NUM_INT:
        case NUM_REAL:
            // simple_expr_.i =
            simp_expr_i = parse_term();

            // type = simple_expr_.i
            type = parse_simple_expr_(simp_expr_i);

            break;
        case ADDOP_MINUS:
        case ADDOP_PLUS:
            parse_sign();
            // simple_expr_.i =
            simp_expr_i = parse_term();

            // type = simple_expr_.i
            type = parse_simple_expr_(simp_expr_i);
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(SIMPLE_EXPR);
            break;
    }
    return type;
}
int parse_simple_expr_(int inherited){
    expecting = "ID','(','ADDOP','DO',']',')','INT','REAL','RELOP','THEN";
    int type = ERR;
    int addop = -1;
    char* e;
    char* e2;
    char* a;
    switch(t_num){
        case ADDOP_PLUS:
        case ADDOP_MINUS:
        case ADDOP_OR:
            addop = t_num;
            if(!match(T_ADDOP, t_type)) { synch(SIMPLE_EXPR_); break;}
            // simple_expr_.i =
            int simple_expr_i = parse_term();
            printf("sim: %d", simple_expr_i);
            printf("inh: %d", inherited);
            if(addop == ADDOP_MINUS){
                a = "-";
            } else if(addop == ADDOP_PLUS){
                a = "+";
            } else if(addop == ADDOP_OR){
                a = "or";
            }

            e = getComparison(simple_expr_i);
            e2 = getComparison(inherited);

            if(simple_expr_i == inherited){
                if (simple_expr_i == INT){
                    if(addop == ADDOP_PLUS || addop == ADDOP_MINUS || addop == ADDOP_OR){
                        simple_expr_i = INT;
                    } else{
                        fprintf(outputfile,"Error in simpexp_ int\n");
                    }
                }
                else if (simple_expr_i == REAL){
                    if(addop == ADDOP_OR || addop == ADDOP_PLUS || addop == ADDOP_MINUS){
                        simple_expr_i = REAL;
                    } else {
                        fprintf(outputfile,"Error in simpexp_ real\n");
                    }
                }
                else if (simple_expr_i == BOOLEAN){
                    if(addop == ADDOP_OR){
                        simple_expr_i = BOOLEAN;
                    } else{
                        fprintf(outputfile,"Error in simpexp_ BOOL\n");
                    }
                }
                else if (simple_expr_i == ERR || inherited == ERR){
                    simple_expr_i = ERR;
                }
                else{
                    simple_expr_i = ERR;
                    fprintf(outputfile,"Error in simpexp_\n");
                }
            } else if(simple_expr_i == ERR || inherited == ERR){
                simple_expr_i = ERR;
            } else {
                simple_expr_i = ERR;
                fprintf(outputfile,"Error: type mismatch. Can't perform addop '%s' between type '%s' and '%s'\n",a, e2 ,e);
            }

            // type = simple_exp_(in)
            type = parse_simple_expr_(simple_expr_i);

            break;
        case T_COMMA:
        case T_DO:
        case T_ELSE:
        case T_END:
        case T_RBRACK:
        case RELOP_EQ:
        case RELOP_GE:
        case RELOP_GT:
        case RELOP_LE:
        case RELOP_LT:
        case RELOP_NE:
        case T_RPAREN:
        case T_SEMICOLON:
        case T_THEN:
            // type = inherited
            type = inherited;
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(SIMPLE_EXPR_);
            break;
    }
    return type;
}
int parse_term(){
    expecting = "ID','(','NOT','INT','REAL";
    int type = ERR;
    int term_i = -1;
    switch(t_num){
        case T_ID:
        case T_LPAREN:
        case T_NOT:
        case NUM_REAL:
        case NUM_INT:
            term_i = parse_factor();
            // type = term'(inher)
            type = parse_term_(term_i);
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(TERM);
            break;
    }
    return type;
}
int parse_term_(int inherited){
    expecting = "ID','(','ADDOP','DO','ELSE','END',']',')','INT','REAL','RELOP','THEN',';";
    int term_i = -1;
    int type = ERR;
    int mulop = -1;
    char* e;
    char* e2;
    char* m;
    switch(t_num){
        case ADDOP_OR:
        case ADDOP_PLUS:
        case ADDOP_MINUS:
        case T_COMMA:
        case T_DO:
        case T_ELSE:
        case T_END:
        case T_RBRACK:
        case RELOP_EQ:
        case RELOP_GE:
        case RELOP_GT:
        case RELOP_LE:
        case RELOP_LT:
        case RELOP_NE:
        case T_RPAREN:
        case T_SEMICOLON:
        case T_THEN:
            // type = term_.i
            type = inherited;
            break;
        case MULOP_AND:
        case MULOP_DIV:
        case MULOP_MOD:
        case MULOP_TIMES:
        case MULOP_DIVIDE:
            mulop = t_num;
            if(!match(T_MULOP, t_type)) { synch(TERM_); break;}
            int factor_i = parse_factor();
            printf("Inherited term: %d", inherited);
            printf("\nterm_i = %d",factor_i);

            e = getComparison(factor_i);
            e2 = getComparison(inherited);
            if(mulop == MULOP_AND){
                m = "and";
            } else if(mulop == MULOP_MOD){
                m = "mod";
            } else if(mulop == MULOP_DIV){
                m = "div";
            } else if(mulop == MULOP_TIMES){
                m = "*";
            }

            if(factor_i == inherited){
                if (factor_i == INT){
                    if(mulop == MULOP_MOD || mulop == MULOP_TIMES || mulop == MULOP_DIVIDE || mulop == MULOP_DIV){
                        term_i = INT;
                    } else{
                        term_i = ERR;
                        fprintf(outputfile,"Error in term_ int");
                    }
                }
                else if (factor_i == REAL){
                    if(mulop == MULOP_MOD || mulop == MULOP_TIMES || mulop == MULOP_DIVIDE || mulop == MULOP_DIV){
                        term_i = REAL;
                    } else {
                        term_i = ERR;
                        fprintf(outputfile,"Error in term_ real");
                    }
                }
                else if (factor_i == BOOLEAN){
                    if(mulop == MULOP_AND){
                        term_i = BOOLEAN;
                    } else{
                        term_i = ERR;
                        fprintf(outputfile,"Error in term_ BOOL");
                    }
                }
                else if (factor_i == ERR || inherited == ERR){
                    term_i = ERR;
                }
                else{
                    term_i = ERR;
                    fprintf(outputfile,"Error in term_");
                }
            } else if (factor_i == ERR || inherited == ERR) {
                term_i = ERR;
            } else{
                term_i = ERR;
                fprintf(outputfile,"Error: type mismatch. Can't perform mulop '%s' between type '%s' and '%s'\n",m, e2 ,e);
            }
            type = parse_term_(term_i);

            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(TERM_);
            break;
    }
    return type;
}

int parse_factor(){
    expecting = "ID','(','NOT','INT','REAL";
    int type = ERR;
    int factor_i = -1;
    switch(t_num){
        case T_ID:
            factor_i = getType(tokPtr->lexeme);
            printf("getType: %d\n", factor_i);
            value = tokPtr->lexeme;
            if(!match(T_ID, t_num)) { synch(FACTOR); break;}

            type = parse_factor_(factor_i);

            break;
        case T_LPAREN:
            if(!match(T_LPAREN, t_num)) { synch(FACTOR); break;}
            // type = expr()
            type = parse_expr();
            expecting = ")";
            if(!match(T_RPAREN, t_num)) { synch(FACTOR); break;}
            break;
        case T_NOT:
            if(!match(T_NOT, t_num)) { synch(FACTOR); break;}
            //type = factor();
            type = parse_factor();
            if(type == ERR){
                type = ERR;
            } else if (type == BOOLEAN){
                type = BOOLEAN;
            } else {
                type = ERR;
                fprintf(outputfile,"Something wrong in factor");
            }
            break;
        case NUM_INT:
        case NUM_REAL:
            //printf("t_type: %d",t_num);
            if(t_num == INT){
                type = INT;
            } else if (t_num == REAL){
                type = REAL;
            } else {
                type = ERR;
                fprintf(outputfile,"Something wrong in factor num. Need to be int or real\n");
            }
            if(!match(T_NUM, t_type)) { synch(FACTOR); break;}
            // type = num.type

            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(FACTOR);
            break;
    }
    return type;
}
int parse_factor_(int inherited){
    expecting = "ID','(','ADDOP','MULOP','DO','ELSE','END',']',')','INT','REAL','RELOP','THEN',';";
    int type = ERR;
    switch(t_num){
        case ADDOP_PLUS:
        case ADDOP_MINUS:
        case ADDOP_OR:
        case T_COMMA:
        case T_DO:
        case T_ELSE:
        case T_END:
        case MULOP_AND:
        case MULOP_DIV:
        case MULOP_MOD:
        case MULOP_TIMES:
        case MULOP_DIVIDE:
        case T_RBRACK:
        case RELOP_EQ:
        case RELOP_GE:
        case RELOP_GT:
        case RELOP_LE:
        case RELOP_LT:
        case RELOP_NE:
        case T_RPAREN:
        case T_SEMICOLON:
        case T_THEN:
            type = inherited;
            break;
        case T_LBRACK:
            if(!match(T_LBRACK , t_num)) { synch(FACTOR_); break;}
            // type = expr()
            type = parse_expr();
            expecting = "]";
            if(!match(T_RBRACK , t_num)) { synch(FACTOR_); break;}

            if(type == INT && inherited == AINT){
                type = INT;
            } else if (type == INT && inherited == AREAL){
                type = REAL;
            } else if (type == ERR || inherited == ERR){
                type = ERR;
            } else {
                type = ERR;
                fprintf(outputfile, "Error in factor': type mismatch\n");
            }
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(FACTOR_);
            break;
    }
    return type;
}

void parse_sign(){
    expecting = "'+' or '-'";
    switch(t_num) {
        case ADDOP_MINUS:
            if(!match(ADDOP_MINUS , t_num)) { synch(SIGN); break;}
            break;
        case ADDOP_PLUS:
            if(!match(ADDOP_PLUS , t_num)) { synch(SIGN); break;}
            break;
        default:
            synerr(tokPtr->lexeme,expecting);
            synch(SIGN);
            break;
    }
}

char* getComparison(int x){
    char* m;
    if(x == INT){
        m = "INT";
    } else if(x == REAL){
        m = "REAL";
    } else if(x == AREAL){
        m = "ARR-REAL";
    } else if(x == AINT){
        m = "INT-ARRAY";
    } else {
        m = "ERR";
    }
    return m;
}

void synch(int x) {
    int *synchSet;
    int len;
    switch(x) {
        case PROGRAM:
        case PROGRAM_:
        case PROGRAM__:
            synchSet = (int[]){T_EOF};
            len = 1;
            break;
        case ID_LIST:
        case ID_LIST_:
        case PARAM_LIST:
        case PARAM_LIST_:
        case EXPR_LIST:
        case EXPR_LIST_:
            synchSet = (int[]){T_EOF, T_RPAREN};
            len = 2;
            break;
        case DECLARATIONS:
        case DECLARATIONS_:
            synchSet = (int[]){T_EOF, T_BEGIN, T_SEMICOLON, T_PROCEDURE};
            len = 4;
            break;
        case TYPE:
        case STD_TYPE:
            synchSet = (int[]){T_EOF, T_SEMICOLON, T_RPAREN};
            len = 3;
            break;
        case SUBPROGRAM_DECLARATIONS:
        case SUBPROGRAM_DECLARATIONS_:
            synchSet = (int[]){T_EOF, T_BEGIN, T_SEMICOLON};
            len = 3;
            break;
        case SUBPROGRAM_DECLARATION:
        case SUBPROGRAM_DECLARATION_:
        case SUBPROGRAM_DECLARATION__:
        case ARGUMENTS:
            synchSet = (int[]){T_EOF, T_SEMICOLON};
            len = 2;
            break;
        case COMPOUND_STATEMENT:
        case COMPOUND_STATEMENT_:
            synchSet = (int[]){T_EOF, T_PERIOD, T_END, T_ELSE, T_SEMICOLON};
            len = 5;
            break;
        case SUBPROGRAM_HEAD:
        case SUBPROGRAM_HEAD_:
            synchSet = (int[]){T_EOF, T_BEGIN, T_VAR, T_SEMICOLON, T_PROCEDURE};
            len = 5;
            break;
        case OPTIONAL_STATEMENTS:
        case STATEMENT_LIST:
        case STATEMENT_LIST_:
            synchSet = (int[]){T_EOF, T_END};
            len = 2;
            break;
        case STATEMENT:
        case STATEMENT_:
        case PROCEDURE_STMT:
        case PROCEDURE_STMT_:
            synchSet = (int[]){T_EOF, T_END, T_ELSE, T_SEMICOLON};
            len = 4;
            break;
        case VAR:
        case VAR_:
            synchSet = (int[]){T_EOF, T_ASSIGNOP};
            len = 2;
            break;
        case EXPR:
        case EXPR_:
            synchSet = (int[]){T_EOF, T_THEN, T_COMMA, T_SEMICOLON, T_RBRACK, T_DO, T_ELSE, T_RPAREN, T_END};
            len = 9;
            break;
        case SIMPLE_EXPR:
        case SIMPLE_EXPR_:
            synchSet = (int[]){T_EOF, T_THEN, T_COMMA, T_SEMICOLON, T_RBRACK, T_DO, T_ELSE, T_RPAREN, T_END, RELOP_EQ,RELOP_GE,RELOP_GT,RELOP_LE,RELOP_LT,RELOP_NE};
            len = 15;
            break;
        case TERM:
        case TERM_:
            synchSet = (int[]){T_EOF, T_THEN, T_COMMA, T_SEMICOLON, T_RBRACK, T_DO, T_ELSE, T_RPAREN, T_END, RELOP_EQ,RELOP_GE,RELOP_GT,RELOP_LE,RELOP_LT,RELOP_NE, ADDOP_OR,ADDOP_PLUS,ADDOP_MINUS};
            len = 18;
            break;
        case FACTOR:
        case FACTOR_:
            synchSet = (int[]){T_EOF, T_THEN, T_COMMA, T_SEMICOLON, T_RBRACK, T_DO, T_ELSE, T_RPAREN, T_END, RELOP_EQ,RELOP_GE,RELOP_GT,RELOP_LE,RELOP_LT,RELOP_NE, ADDOP_OR,ADDOP_PLUS,ADDOP_MINUS, MULOP_AND,MULOP_DIV, MULOP_MOD};
            len = 21;
            break;
        case SIGN:
            synchSet = (int[]){T_EOF, T_ID, NUM_INT, NUM_REAL, T_LPAREN, T_NOT};
            len = 6;
            break;
    }
    while(true) {
        for(int i = 0; i < len; i++) {
            if(t_num ==  synchSet[i]) {
                char* lexPrint = tokPtr->lexeme;
                if(t_num == T_EOF) {
                    printf("Reached end-of-file; terminating\n");
                    exit (1);
                } else {
                    printf("Synchronized on term '%s' (%d)\n",lexPrint,tokPtr->attrs.intNum);
                    return;
                }
            }
        }
        getNext();
    }
}

void synerr(char *received, char* expected )
{
	// output to console
	fprintf(outputfile,"SYNERR: Unexpected token '%s'. Expected: '%s' \n",received,expected);

}
