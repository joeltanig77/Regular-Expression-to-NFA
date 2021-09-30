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
  int stateOne;
  int stateTwo;
  char symbol;
  struct NFA * connect;
};

int push(struct Stack* stack, struct NFA* nfa) {
    stack->arrayOfNFAs[++stack->top] = nfa;
    return 0;
}


struct NFA* pop(struct Stack* stack) {
  return stack->arrayOfNFAs[stack->top--];
}


int main(int argc,char *argv[]) {
  int regIncrementor = 0;
  char line[256];
  memset(line, '\0', sizeof(line));
  int index = 0;
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
    fprintf(stdout,"%s\n", "No regular expression was passed in");
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
        struct NFA* nfa2 = pop(stack);
        struct NFA* nfa1 = pop(stack);
        // Do shit
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
        struct NFA* nfa = (struct NFA*)calloc(1,sizeof(nfa));
        nfa->startState = 0;
        nfa->acceptState = 1;
        nfa->stateOne = 0;
        nfa->stateTwo = 1;
        nfa->symbol = c;
        push(stack,nfa);
      }
    }
  }
  return 0;
}
