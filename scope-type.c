#include "global.h"

int line;
int size;
int flag;
int offset;

node* topofStack() {
    stak* temp;

    // check for stack underflow
    if (top == NULL) {
        printf("\nStack Underflow");
        exit(1);
    }
    else {
        temp = top;
        if (temp->next != NULL) {
            //printf("TOP of STACK: %s with address: %p\n", temp->id, temp->addr);
            return temp->addr;
        }
    }
    //printf("TOP of STACK: %s with address: %p\n", temp->id, temp->addr);
    return temp->addr;
}

void checkAddBlueNode(char* lex, int type){
    // check current stack
    node *currentNodePtr = currentGreenNode;
    if(currentGreenNode != NULL){
        // move pointer
        while(currentNodePtr->right != NULL){
            if (strcmp(currentNodePtr->lex,lex) == 0){
                fprintf(outputfile,"1Error in scope. Duplicate variable: %s \n",currentNodePtr->lex);
                return;
            }
            currentNodePtr = currentNodePtr->right;
        }
        if(strcmp(currentNodePtr->lex,lex) == 0){
                fprintf(outputfile,"Error in scope. Duplicate variable: %s \n",currentNodePtr->lex);
                return;
            }
    }
    // add to stack
    node *newNode = malloc(sizeof(node));

    // copy lex to new node
    strcpy(newNode->lex,lex);
    newNode->isGreenNode = false;

    // add the types

    if(type == PPINT){
        newNode->typeInt = INT;
        newNode->isParam = true;
    } else if (type == PPREAL){
        newNode->typeInt = REAL;
        newNode->isParam = true;
    } else{
        newNode->typeInt = type;
    }

    // if empty, create it

    newNode->prev = currentNodePtr;
    currentNodePtr->right = newNode;


    newNode->right = NULL;
    newNode->down = NULL;
    newNode->numParams = 0;

    allNodes = newNode;

    if(type == PPINT || type == PPREAL || type == PPAINT || type == PPAREAL){
        if(type == PPAINT){
            newNode->typeInt = AINT;
        }
        if(type == PPAREAL){
            newNode->typeInt = AREAL;
        }

        currentGreenNode->numParams++;
        allNodes->numParams++;
    } else {
        if(type != PGPARAM){
            line++;
        }
        if(newNode->typeInt == INT){
            offset+=size;
            size = 4;
            fprintf(addressComp,"%s\t%d%10s\t(INT)\t\t%d(bytes)\t\t%d\n",currentGreenNode->lex, line, lex, size,offset);
        } else if(newNode->typeInt == REAL){
            offset+=size;
            size = 8;
            fprintf(addressComp,"%s\t%d%10s\t(REAL)\t\t%d(bytes)\t\t%d\n",currentGreenNode->lex, line, lex, size,offset);
        } else if(newNode->typeInt == AINT){
            offset+=size;
            size = arr_size * 4;
            fprintf(addressComp,"%s\t%d%10s\t(A-INT)\t\t%d(bytes)\t\t%d\n",currentGreenNode->lex, line, lex, size,offset);
        } else if(newNode->typeInt == AREAL){
            offset+=size;
            size = arr_size * 8;
            fprintf(addressComp,"%s\t%d%10s\t(A-REAL)\t%d(bytes)\t\t%d\n",currentGreenNode->lex, line, lex, size,offset);
        }
        if(newNode->typeInt != PGPARAM){
            ;
        }
    }

}

void push(char lex[10]){
    // create new node temp and allocate memory
    stak* temp;
    temp = (stak*)malloc(sizeof(stak));

    // check if stack (heap) is full. Then inserting an element would
    // lead to stack overflow
    if (!temp) {
        printf("\nHeap Overflow");
        exit(1);
    }

    // initialize data into temp data field
    strcpy(temp->id,lex);
    temp->addr = currentGreenNode;

    // put top pointer reference into temp link
    temp->next = top;

    //printf("Adding Address: %p to greenNode: %s \n",temp->addr, currentGreenNode->lex);

    // make temp as top of Stack
    top = temp;
}

void pop(){
    stak* temp;

    // check for stack underflow
    if (top == NULL) {
        printf("\nStack Underflow");
        exit(1);
    }
    else {
        // top assign into temp
        temp = top;

        // assign second node to top
        top = top->next;

        // destroy connection between first and second
        temp->next = NULL;

        // release memory of top node
        free(temp);
    }
}



int inStack(char* lex){
    stak* temp;
    // check for stack underflow
    if (top == NULL) {
        printf("\nStack Underflow");
        exit(3);
    }
    else {
        temp = top;
        //printf("\n\nLooking at stack Element: %s\n", temp->id);
        while (temp->next != NULL) {
            //printf("\n\nLooking at stack Element: %s\n", temp->id);
            if(strcmp(temp->id,lex) == 0){
                //printf("\nthe GreenNode %s is in the stack",currentGreenNode->lex);
                return 1;
            }
            if(temp->next != NULL){
                temp = temp->next;
            }
        }
        if(strcmp(temp->id,lex) == 0){
                //printf("\n\nthe GreenNode %s is in the stack",currentGreenNode->lex);
                return 1;
        }
    }
    //printf("TOP of STACK: %s with address: %p\n", temp->id, temp->addr);
    return 0;
}

void checkAddGreenNode(char* lex, int type){
    // check current stack
    node *nodePtr = currentGreenNode;
    line = -1;
    offset = 0;
    size = 0;

    if(currentGreenNode != NULL){
        // move pointer
        while(nodePtr->right != NULL && nodePtr->right->isGreenNode == false){
            printf("checking nodePtr: %s\n",nodePtr->lex);
            if (strcmp(nodePtr->lex,lex) == 0){
                fprintf(outputfile,"Error in scope. Duplicate Procedures\n");
                return;
            }
            nodePtr = nodePtr->right;
        }
        if(nodePtr->right != NULL && nodePtr->right->isGreenNode == true){
            nodePtr = nodePtr->right;
        }
        if (strcmp(nodePtr->lex,lex) == 0){
            fprintf(outputfile,"Error in scope. Duplicate Procedures\n");
            return;
        }
    }


    // add to stack
    node *newNode = malloc(sizeof(node));

    // copy lex to new node
    strcpy(newNode->lex,lex);
    newNode->typeInt = type;
    newNode->isGreenNode = true;


    // if empty, create it
    if(type == PGNAME){
        newNode->isRoot = true;
        newNode->prev = NULL;
        newNode->right = NULL;
        newNode->down = NULL;
        newNode->numParams = 0;
        currentGreenNode = newNode;
        //printf("current Green Node %s: \n", currentGreenNode->lex);
        push(lex);
    } else {
        printf("Current nodePtr: %s\n",nodePtr->lex);
        if(nodePtr->right == NULL && nodePtr->isGreenNode == false){
            newNode->numParams = 0;
            newNode->prev = nodePtr;
            nodePtr->right = newNode;
            newNode->right = NULL;
            newNode->down = NULL;
            currentGreenNode = newNode;
            //printf("1: adding %s %d to %s Previous = %s\n",lex, type,currentGreenNode->lex,currentGreenNode->prev->lex);
            push(lex);
        } else if(nodePtr->isGreenNode == true){
            newNode->numParams = 0;
            newNode->prev = nodePtr;
            nodePtr->down = newNode;
            newNode->right = NULL;
            newNode->down = NULL;
            currentGreenNode = newNode;
            //printf("2: adding %s %d to %s Previous = %s\n",lex, type,currentGreenNode->lex,currentGreenNode->prev->lex);
            push(lex);
        }
    }
    allNodes = newNode;

}

void closeScope(){
    //printf("inside closeScope()");

    pop();
    currentGreenNode = topofStack();
    //printf("Green Node Address: %p",currentGreenNode);

}

int getType(char* lex){
    node *checkNode = currentGreenNode;

    while (checkNode->right != NULL && checkNode->right->isGreenNode == false){
        //printf("Fwd Checking node: %s\n",checkNode->lex);
        if(strcmp(checkNode->lex,lex) == 0){
            return checkNode->typeInt;
        }
        checkNode = checkNode->right;
    }
    if(strcmp(checkNode->lex,lex) == 0){
        return checkNode->typeInt;
    }
    //printf("\n Current GREEN: %s \n", currentGreenNode->lex);
    // now search back for global variables:
    checkNode = currentGreenNode;
    //printf("\n Current node: %s \n", checkNode->lex);

    while (checkNode->prev != NULL){
        //printf("Checking node: %s\n",checkNode->lex);
        if(strcmp(checkNode->lex,lex) == 0){
            return checkNode->typeInt;
        }
        checkNode = checkNode->prev;
    }

    fprintf(outputfile,"node: %s has never been declared. \n",lex);
    return checkNode->typeInt;
}

node* getTypeGreen(char* lex){
    node *checkNode = currentGreenNode;

    //printf("\n Current node: %s \n", checkNode->lex);

    while (checkNode->prev != NULL){
        printf("REV Checking Green node: %s\n",checkNode->lex);
        if(checkNode->prev->isGreenNode == true){
            checkNode = checkNode->prev;
            if(strcmp(checkNode->lex,lex) == 0){
                return checkNode;
            } else {
                break;
            }
        }
        checkNode = checkNode->prev;
    }
    if(strcmp(checkNode->lex,lex) == 0){
        return checkNode;
    }


    checkNode = currentGreenNode;

    //printf("\n Current node: %s \n", checkNode->lex);
    while (checkNode->right != NULL){
        //printf("Fwd Checking for Green node: %s\n",checkNode->lex);
        if(strcmp(checkNode->lex,lex) == 0){
                return checkNode;
        }
        if(checkNode->right->isGreenNode == true && checkNode->right->down != NULL){
            checkNode = checkNode->right;
            if(strcmp(checkNode->lex,lex) == 0){
                return checkNode;
            }
            while(checkNode->down != NULL){
                checkNode = checkNode->down;
                //printf("Dwn Checking for Green node: %s\n",checkNode->lex);
                if(strcmp(checkNode->lex,lex) == 0){
                    return checkNode;
                }
            }
            if(strcmp(checkNode->lex,lex) == 0){
                return checkNode;
            } else {
                fprintf(outputfile,"node: %s is not in scope or hasn't been declared. \n",lex);
                break;
            }
        }
        checkNode = checkNode->right;

    }
    if(strcmp(checkNode->lex,lex) == 0){
        return checkNode;
    }
    return checkNode;

}

node* getParams(char* lex){
    node *checkNode = allNodes;

    while (checkNode->right != NULL){
        checkNode = checkNode->right;
    }

    //printf("\n Current GREEN: %s \n", currentGreenNode->lex);
    while (checkNode->prev != NULL){
        //printf("checking lexeme: %s, type: %d\n",checkNode->lex,checkNode->typeInt);
        if(strcmp(checkNode->lex,lex) == 0){
                return checkNode;
        }
        // move next nodecheckNode->right = checkNode;
        checkNode = checkNode->prev;

    }
    printf("Error in getParams %d\n",checkNode->typeInt);
    return checkNode;
}

node* getParamType(char* lex){
    node *checkNode = allNodes;

    while (checkNode->right != NULL){
        checkNode = checkNode->right;
    }

    //printf("\n Current GREEN: %s \n", currentGreenNode->lex);
    while (checkNode->prev != NULL){
        printf("checking lexeme: %s, type: %d\n",checkNode->lex,checkNode->typeInt);
        if(strcmp(checkNode->lex,lex) == 0){
            return checkNode;

        }
        checkNode = checkNode->prev;

    }
    printf("Error in getParams %d\n",checkNode->typeInt);
    return checkNode;
}

