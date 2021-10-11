#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nfaStructure.h"

int push(struct Stack* stack, struct NFA* nfa) {
    stack->arrayOfNFAs[++stack->top] = nfa;
    return 0;
}

struct NFA* pop(struct Stack* stack) {
  return stack->arrayOfNFAs[stack->top--];
}

int isEmpty(struct Stack* stack) {
  if(stack->top == -1) {
    // empty
    return 1;
  }
  return 0;
}

int cleanUpNFA(struct NFA* nfa) {
    struct Transitions* cursor = (struct Transitions*)nfa;
    struct Transitions* temp = NULL;
    while(cursor->tran != NULL) {
      temp = cursor;
      cursor = cursor->tran;
      free(temp->tran);
    }
    free(nfa);
    return 0;
}

int compareTrans(const void *compare1,const void *compare2) {
    // Cast to double pointer then defererence twice to compare actual values
    struct Transitions **node1 = (struct Transitions **)compare1;
    struct Transitions **node2 = (struct Transitions **)compare2;
    if(((*node1)->stateOne - (*node2)->stateOne) == 0) {
      return (*node1)->stateTwo - (*node2)->stateTwo;
    }
    return (*node1)->stateOne - (*node2)->stateOne;
}

int printSortedNFATrans(struct NFA* nfa, struct Transitions** arrayOfTrans, int sizeOfTrans, char* line) {
    struct Transitions* cursor = (struct Transitions*)nfa->tran;
    int i = 0;
    while(cursor != NULL) {
      arrayOfTrans[i] = cursor;
      cursor = cursor->tran;
      i+=1;
    }
    qsort(arrayOfTrans,sizeOfTrans,sizeof(struct Transitions*),compareTrans);
    printf("RE: %s", line);
    printf("Start: q%d\n", nfa->startState);
    printf("Accept: q%d\n", nfa->acceptState);
    for(int i = 0; i < sizeOfTrans; i++) {
        printf("(q%d, %c) -> q%d\n",arrayOfTrans[i]->stateOne, arrayOfTrans[i]->symbol, arrayOfTrans[i]->stateTwo);
    }
    printf("\n");
    return 0;
}
