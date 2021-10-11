#ifndef NFASTRUCTURE_H
#define NFASTRUCTURE_H

#include <stdio.h>

/*
  A struct that stores the var top and a array of NFA's
*/

struct Stack {
  int top;
  // Array of nfa's
  struct NFA ** arrayOfNFAs;
};

struct NFA {
  int startState;
  int acceptState;
  char spaceHolder;
  struct Transitions * tran;
};

struct Transitions {
  int stateOne;
  int stateTwo;
  char symbol;
  struct Transitions * tran;
};

/*
  A function that pushes a NFA onto the stack
*/

int push(struct Stack* stack, struct NFA* nfa);

/*
  A function that pops a NFA off the stack
*/

struct NFA* pop(struct Stack* stack);

/*
  A function that checks if the stack is empty
*/

int isEmpty(struct Stack* stack);

/*
  A function cleans up the nfa when it is finished
*/

int cleanUpNFA(struct NFA* nfa);

/*
  A function compares Transitions (Used for Qsort)
*/

int compareTrans(const void *compare1,const void *compare2);

/*
  A function that prints all the transitions using qsort
*/

int printSortedNFATrans(struct NFA* nfa, struct Transitions** arrayOfTrans, int sizeOfTrans, char* line);


#endif
