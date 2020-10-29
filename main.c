#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "global.h"


#define BSIZE 72
#define EOS '\0'


// global line counter
int lineno = 1;

// global booleans
bool flag = false;
bool isResword = false;
bool isReal = false;

// global pointers
int left = 0;
int right = 0;
bool newLine = false;
char line_in[BSIZE];   // max line size

FILE* load_buffer(FILE *file){
    token newToken;
    if (file != NULL){
        // get line
        if(fgets(line_in, BSIZE, file) != NULL){
            fprintf(outputfile,"%d:\t%s",lineno,line_in);
        } else{
            line_in[0] = '\0';
            //fprintf(tokenfile,"\t%10s\t\t\t%d (EOF)\t\t\t%d\n","EOF",9,10);
            newToken = createToken("EOF", 9, 10);
        }
    }
    else{
        error("file didn't open");  // error message for file not opening
    }
    return 0;
}

int error(char *m)    // generates the error messages
{
    fprintf(outputfile, "LEXERR: Unrecognized Symbol: \t %s\n", m);
    return 0;    // unsucessful termination
}


//Given a reference (pointer to pointer) to the head
void appendId(struct ids** head_ref, char *new_data, char* address)
{
    // allocate node
    struct ids* new_node = (struct ids*) malloc(sizeof(struct ids));
    struct ids* last = *head_ref;
    strcpy(new_node->name,new_data);    // add new id
    new_node->id = 2;                   // add type
    new_node->types.pntr = address;     // add address of new id

    new_node->next = NULL; // make last node null

    // If the Linked List is empty, then make the new node as head
    if (*head_ref == NULL) {
       *head_ref = new_node;
       return;
    }

    while (last->next != NULL){ // find the last node
        last = last->next;
    }

    //Change the next of last node
    last->next = new_node;
    return;
}


void appendToken(token tok) {
    // allocate node
    token *new_node = malloc(sizeof(token));
    token *last = head_tok;
    new_node->line_num = lineno; // add line number
    strcpy(new_node->lexeme,tok.lexeme);    // add new id
    new_node->type = tok.type;                   // add type
    new_node->attrs.intNum = tok.attrs.intNum;     // add address of new id

    new_node->next = NULL; // make last node null

    /* If the Linked List is empty, then make the new node as head */
    if (head_tok == NULL) {
       head_tok = new_node;
       return;
    }

    while (last->next != NULL){ // find the last node
        last = last->next;
    }

    //Change the next of last node
    last->next = new_node;

    return;
}
void appendToken2(token tok)
{
    // allocate node
    token *new_node = malloc(sizeof(token));
    token *last = head_tok;
    new_node->line_num = lineno; // add line number
    strcpy(new_node->lexeme,tok.lexeme);  // add new id
    new_node->type = tok.type;     // add type
    new_node->attrs.pntr = tok.attrs.pntr;     // add address of new id

    new_node->next = NULL; // make last node null

    /* If the Linked List is empty, then make the new node as head */
    if (head_tok == NULL) {
       head_tok = new_node;
       return;
    }

    while (last->next != NULL){ // find the last node
        last = last->next;
    }

    //Change the next of last node
    last->next = new_node;

    return;
}

token createToken(char *str,int id, int attr){
    token makeToken;
    makeToken.line_num = lineno;
    strcpy(makeToken.lexeme, str);
    makeToken.type = id;
    makeToken.attrs.intNum = attr;
    appendToken(makeToken);
    return makeToken;
}

token createToken2(char *str,int id, char* attr){
    token makeToken;
    makeToken.line_num = lineno;
    strcpy(makeToken.lexeme, str);
    makeToken.type = id;
    makeToken.attrs.pntr = attr;
    appendToken2(makeToken);
    return makeToken;
}

// Constructs Symbols LinkedList
int constructTokenSymbols(){
    struct symbols *temp; //symbols is a type
    temp = malloc(sizeof(struct symbols));
    head_s = temp;
    temp->next = NULL;

    FILE *ifp;
    ifp = fopen("symbols.txt", "r");

    int line_no = 1;

    char buffer[BSIZE];
    memset(buffer, 0, BSIZE);
    fgets(buffer, BSIZE, ifp); // skip first line
    while(fgets(buffer, sizeof buffer, ifp) != NULL){
        sscanf(buffer, "%2s %d %d", temp->name, &temp->id, &temp->attr);
        temp->next = malloc(sizeof(struct symbols));
        temp = temp->next;
        temp->next = NULL;
        line_no++;
    }
    return (0);
}
// Constructs reservedwords LinkedList
int constructTokenWords(){
    struct reswords *temp;
    temp = malloc(sizeof(struct reswords));
    head_r = temp;
    temp->next = NULL;

    FILE *ifp;
    ifp = fopen("reservedwords.txt", "r");

    int line_no = 1;

    char buffer[BSIZE];
    memset(buffer, 0, BSIZE);
    fgets(buffer, BSIZE, ifp); // skip first line
    while(fgets(buffer, sizeof buffer, ifp) != NULL){
        sscanf(buffer, "%10s %d %d", temp->name, &temp->id, &temp->attr);
        temp->next = malloc(sizeof(struct reswords));
        temp = temp->next;
        temp->next = NULL;
        line_no++;
    }
    return (0);
}

token getResword(char *s){
    struct reswords* current = head_r; // Initialize linkedlist
    token newToken;
    while (current != NULL){
        if(strcmp(current->name,s) == 0){
            fprintf(tokenfile,"%d:\t%10s\t\t\t%d (RESWORD)\t\t%d\n",lineno, current->name, current->id, current->attr);
            newToken = createToken(current->name, current->id,current->attr);
            //appendToken(newToken);
            isResword = true;
            return (newToken);
        }
        current = current->next;
    }
    return (newToken);
}


token getSymbol(char *str){
    struct symbols* current = head_s; // Initialize linkedlist
    token newToken;
    while (current != NULL){
        if(strcmp(current->name,str) == 0){
            newToken = createToken(current->name, current->id, current->attr);
            fprintf(tokenfile,"%d:\t%10s\t\t\t%d (SYMBOL)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
            flag = true;
            return (newToken);
        }
        current = current->next;
    }
    newToken = createToken(str, 99, 99);
    error(str);
    fprintf(tokenfile,"%d:\t%10s\t\t\t%d (LEXERR)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
    return (newToken);
}

char isDelim(char ch) {
    if (flag == true || ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0'){
        flag = false;
        return (ch);
    }
    return (ch);
}


// Returns a token if the string is a VALID Id.
token validId(char *str)
{
    char* address;
    struct ids* current = head_id; // Initialize linkedlist
    token newToken;
    int len = strlen(str);
    if (len > 10){
        fprintf(outputfile, "LEXERR: ID TOO LONG: \t %s\n", str);
        newToken = createToken(str, 99, 99);
        fprintf(tokenfile,"%d:\t%10s\t\t\t%d (LEXERR)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
		return (newToken);
    }
	for (int i = 0; i < len; i++) {
		if (isalpha(str[0])){
            if(isalnum(str[i]))
			    ;
        } else {
            fprintf(outputfile, "LEXERR: Invalid ID: \t %s\n", str);
            newToken = createToken(str, 99, 99);
            fprintf(tokenfile,"%d:\t%10s\t\t\t%d (LEXERR)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
		    return (newToken);
        }
	}
    while (current != NULL){
        if(strcmp(current->name,str) == 0){
            newToken = createToken2(current->name, current->id,current->types.pntr);
            fprintf(tokenfile,"%d:\t%10s\t\t\t%d (ID)\t\t\t%p\n",lineno, current->name,        current->id, current->types.pntr);
            return (newToken);
        }
        current = current->next;
    }
    address = malloc(sizeof(str));
    newToken = createToken2(str, 2, address);
    appendId(&head_id,str,address); // append Id_list
    //appendToken2(newToken);         // append Token with pointer
    fprintf(tokenfile,"%d:\t%10s\t\t\t%d (ID)\t\t\t%p\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.pntr);
    return (newToken);
}

// Returns an integer token
token isInteger(char *str)
{
	int i;
    int len = strlen(str);
    token newToken;

	if (len == 0){
        fprintf(outputfile, "LEXERR: Invalid number: \t %s\n", str);
		newToken = createToken(str, 99, 99);
        fprintf(tokenfile,"%d:\t%10s\t\t\t%d (LEXERR)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
		return (newToken);
    }
    else if(str[0] == '0'){
        fprintf(outputfile, "LEXERR: Leading Zeros: \t %s\n", str);
		newToken = createToken(str, 99, 99);
        fprintf(tokenfile,"%d:\t%10s\t\t\t%d (LEXERR)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
		return (newToken);
    }
    else if(len > 10){
        fprintf(outputfile, "LEXERR: Extra Long Integer: \t %s\n", str);
        newToken = createToken(str, 99, 99);
        fprintf(tokenfile,"%d:\t%10s\t\t\t%d (LEXERR)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
		return (newToken);
    }
	for (i = 0; i < len; i++) {
		if (!isdigit(str[i]) || (str[i] == '-' && i > 0)){
			fprintf(outputfile, "LEXERR: Extra Long Integer: \t %s\n", str);
            newToken = createToken(str, 99, 99);
            fprintf(tokenfile,"%d:\t%10s\t\t\t%d (LEXERR)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
            return (newToken);
        }
	}
    newToken = createToken(str, 6, 6);
    fprintf(tokenfile,"%d:\t%10s\t\t\t%d (INT)\t\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
	return (newToken);
}

// Returns a REAL token
token isRealNumber(char *str) {
	int i = 0;
    int len = strlen(str);
	bool hasDecimal = false;
    bool hasE = false;
    int num_of_digits = 0;
    int fract = 0;

    token newToken;

	if (len == 0){
		fprintf(outputfile, "LEXERR: Invalid number: \t %s\n", str);
		newToken = createToken(str, 99, 99);
        fprintf(tokenfile,"%d:\t%10s\t\t\t%d (LEXERR)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
		return (newToken);
    }
    if(str[0] == '0' && str[1] != '.'){
        fprintf(outputfile, "LEXERR: LEADING ZEROS: \t %s\n", str);
        newToken = createToken(str, 99, 99);
        fprintf(tokenfile,"%d:\t%10s\t\t\t%d (LEXERR)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
        return (newToken);
    }
	for (i = 0; i < len; i++) {
		if ((str[i] != '0' && str[i] != '1' && str[i] != '2'
			&& str[i] != '3' && str[i] != '4' && str[i] != '5'
			&& str[i] != '6' && str[i] != '7' && str[i] != '8'
			&& str[i] != '9' && str[i] != '.' && str[i] != 'e' && str[i] != 'E') ||
			(str[i] == '-' && i > 0)){
			fprintf(outputfile, "LEXERR: Invalid number: \t %s\n", str);
            newToken = createToken(str, 99, 99);
            fprintf(tokenfile,"%d:\t%10s\t\t\t%d (LEXERR)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
            return (newToken);
        }
		if (str[i] == '.'){
			hasDecimal = true;
            num_of_digits = 0;
        }
        if(isdigit(str[i])){
            num_of_digits++;
        }
        if((str[i] == 'e' || str[i] == 'E') && isdigit(str[i+1]) == true
        && hasDecimal == true && isdigit(str[i-1] == true)){
            hasE = true;
            num_of_digits = 0;
        }
        if(num_of_digits > 5){
            fprintf(outputfile, "LEXERR: Extra Long Fractional: \t %s\n", str);
            newToken = createToken(str, 99, 99);
            fprintf(tokenfile,"%d:\t%10s\t\t\t%d (LEXERR)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
            return (newToken);
        }
        if(hasE == true){
            fract++;
        }
        if(fract > 3){
            fprintf(outputfile, "LEXERR: Extra long E: \t %s\n", str);
            newToken = createToken(str, 99, 99);
            fprintf(tokenfile,"%d:\t%10s\t\t\t%d (LEXERR)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
            return (newToken);
        }
	}
    if(str[len-1] == '0' && hasDecimal == true){
        if(str[len-1] == '0' && str[len-2] == '0'){
            fprintf(outputfile, "LEXERR: TRAILING ZEROS: \t %s\n", str);
            newToken = createToken(str, 99, 99);
            fprintf(tokenfile,"%d:\t%10s\t\t\t%d (LEXERR)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
            return (newToken);
        }
    }
	newToken = createToken(str, 6, 8);
    fprintf(tokenfile,"%d:\t%10s\t\t\t%d (REAL)\t\t%d\n",lineno,newToken.lexeme,newToken.type,newToken.attrs.intNum);
    return (newToken);
}

// Extracts the SUBSTRING.
char *subString(char *str, int left, int right) {
	char* subStr = (char*)malloc(sizeof(char) * (right - left + 2));

	for (int i = left; i <= right; i++){
		subStr[i - left] = str[i];
    }
	subStr[right - left + 1] = '\0';
	return (subStr);
}

// Parsing the input buffer string
// Parsing the input buffer string
token getToken() {

    char* str = line_in;


	int len = strlen(str + 1);
    token newToken;
    strcpy(newToken.lexeme,"NULL");
    newToken.type = -3;
    newToken.attrs.intNum = -3;

    char tempString[3] = "";



    if((str[right] == '\n' || str[right] == '\0') && feof(file)){
        fprintf(tokenfile,"\t%10s\t\t\t%d (EOF)\t\t\t%d\n","EOF",9,10);
        newToken = createToken("EOF", 9, 10);
        return newToken;
    }

    // check for right and left pointer
	while (right <= len && left <= right) {
        while(str[right] == ' ' || str[right] == '\t' || str[right] == '\r'){
            right++;
            left = right;
        }
        if(str[right] == '\n' || str[right] == '\0'){
            //newLine = true;
            strcpy(newToken.lexeme,"BLANK");
            newToken.type = -1;
            newToken.attrs.intNum = -1;
            left = 0;
            right = 0;
            return newToken;
        }
        if(isalpha(str[right])) {
            while(isalnum(str[right])){
                right++;
            }
            // extract substring
            char *subStr = subString(str, left, right - 1);
            newToken = getResword(subStr);

            if(isResword == false){
                newToken = validId(subStr);
            }
            else if(isResword == true){
                //return newToken;
                isResword = false;
            }
            return newToken;
            left = right;
        }
         // check numbers
        else if(isdigit(str[right])){
            // keep getting digits
            while(isdigit(str[right])){
                right++;
            }
            // check if real number
            if(isdigit(str[right-1]) && str[right] == '.' && isdigit(str[right+1])){
                right++;
                isReal = true;
            }
            while(isdigit(str[right])){
                right++;
            }
            if((str[right] == 'e' || str[right] == 'E') && isReal == true && isdigit(str[right+1])){
                right++;
            }
            while(isdigit(str[right])){
                right++;
            }
            char *subStr = subString(str, left, right - 1);
            if(isReal == true){
                newToken = isRealNumber(subStr);
                left = right;
                isReal = false;
                return newToken;
            }
            // check integer
            if(isReal == false){
                newToken = isInteger(subStr);
                left = right;
                return newToken;
            }
            isReal = false;
            left = right;
        }
        // check for symbols
        else if(isDelim(str[right])){

            // check for 2 character symbols
            if(str[right] == ':' && str[right+1] == '='){
                tempString[0] = ':';
                tempString[1] = '=';
                tempString[2] = '\0';
                right++;
            } else if(str[right] == '.' && str[right+1] == '.'){
                tempString[0] = '.';
                tempString[1] = '.';
                tempString[2] = '\0';
                right++;
            } else if(str[right] == '>' && str[right+1] == '='){
                tempString[0] = '>';
                tempString[1] = '=';
                tempString[2] = '\0';
                right++;
            } else if(str[right] == '<' && str[right+1] == '>'){
                tempString[0] = '<';
                tempString[1] = '>';
                tempString[2] = '\0';
                right++;
            } else if(str[right] == '<' && str[right+1] == '='){
                tempString[0] = '<';
                tempString[1] = '=';
                tempString[2] = '\0';
                right++;
            }
             else {
                tempString[0] = str[right];
                tempString[1] = '\0';
            }
            newToken = getSymbol(tempString);
            right++;
            left = right;
            return newToken;
        }
        else {
            printf("unrecognized symbol '%c' \n",str[right]);
            right++;
            left = right;
        }
	}

	return newToken;
}



void nextFileLine(){
    lineno++;
    left = 0;
    right = 0;
    load_buffer(file);
}


token getNextTokenFromLine(){
    token tok;
    tok = getToken();


    if(tok.type == -1){
        nextFileLine();
        //tok = getToken();
        return getNextTokenFromLine();

    }
    return tok;
}

// main function
int main()
{
    // open the files
    file = fopen("TEST.txt", "r");
    tokenfile = fopen("tokenfile.txt","w");
    outputfile = fopen("output.txt", "w");
    addressComp = fopen("address.txt","w");
    fprintf(addressComp,"(Scope)\t(loc)\t (VARIABLE)\t\t(SIZE)\t\t(OFFSET)\n");

    // build LLs
    constructTokenWords();
    constructTokenSymbols();

	load_buffer(file); // first line of file
    getNextTokenFromLine(); // get first token
    parse();

    // close files
    fclose(addressComp);
    fclose(file);
    fclose(tokenfile);
    fclose(outputfile);

	return (0);
}
