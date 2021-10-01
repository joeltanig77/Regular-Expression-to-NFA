#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>


struct Stack {
  int top;
  // Array of nfa's
  struct NFA ** arrayOfNFAs;
};

struct NFA {
  int startState;
  int acceptState;
  int size;
  char symbol;
  struct Transitions * tran;
};

struct Transitions {
  int stateOne;
  int stateTwo;
  char symbol;
  struct Transitions * tran;
};

int push(struct Stack* stack, struct NFA* nfa) {
    stack->arrayOfNFAs[++stack->top] = nfa;
    return 0;
}


struct NFA* pop(struct Stack* stack) {
  return stack->arrayOfNFAs[stack->top--];
}


int main(int argc,char *argv[]) {
  char line[256];
  memset(line, '\0', sizeof(line));
  int index = 0;
  int stateCounter = 0;
  struct Stack* stack = (struct Stack*)calloc(100000000,sizeof(stack));
  if (!stack) {
    fprintf(stderr,"%s\n","Failed to allocate memory");
    return 0;
  }
  struct NFA** nfaArray = (struct NFA**)calloc(100000000,sizeof(nfaArray));
  if (!nfaArray) {
    fprintf(stderr,"%s\n","Failed to allocate memory");
    free(stack);
    return 0;
  }

  if(argc == 1) {
    fprintf(stdout,"%s\n", "No file was passed in");
    free(stack);
    free(nfaArray);
    return 0;
  }

  FILE* fp = fopen(argv[1],"r");
  if (!fp) {
    fprintf(stderr,"%s\n","Failed to allocate memory");
    free(stack);
    free(nfaArray);
    return 0;
  }

  stack->arrayOfNFAs = nfaArray;
  stack->top = -1;

  while (fgets(line,sizeof(line), fp)) {
    index = 0;
  while (line[index] != '\0') {
      char c = line[index];
      index += 1;
      // if the char == '&'
      if (c == 38) {
        struct NFA* nfa2 = pop(stack); // b
        struct NFA* nfa1 = pop(stack); // a
        //printf("\n");
        struct NFA* added = (struct NFA*)calloc(1,sizeof(added));
        struct Transitions* tran = (struct Transitions*)calloc(1,sizeof(tran));
        added->tran = tran;
        added->tran->stateOne = nfa1->acceptState;
        added->tran->stateTwo = nfa2->startState;
        added->tran->symbol = 'E';
        added->startState = nfa1->startState;
        added->acceptState = nfa2->acceptState;
        push(stack,added);
        // NEED TO DEBUG TO SEE IF THIS FULLY WORKS
      }
      // if the char == '|'
      else if (c == 124) {
        struct NFA* nfa2 = pop(stack);
        struct NFA* nfa1 = pop(stack);
        // Do shit
      }
      // if the char == '*'
      else if (c == 42) {
        struct NFA* nfa2 = pop(stack);
        struct NFA* nfa1 = pop(stack);
        // Do shit
      }
      else {
        // Push
        // If c is a new line, continue
        if (c == 10) {
          continue;
        }
        struct NFA* nfa = (struct NFA*)calloc(1,sizeof(nfa));
        struct Transitions* tran = (struct Transitions*)calloc(1,sizeof(tran));
        nfa->tran = tran;
        nfa->tran->stateOne = stateCounter;
        nfa->startState = stateCounter;
        stateCounter += 1;
        nfa->tran->stateTwo = stateCounter;
        nfa->acceptState = stateCounter;
        stateCounter += 1;
        //stateCounter += 1;
        nfa->symbol = c;
        nfa->size += 2;
        push(stack,nfa);
      }
    }
  }
  return 0;
}
