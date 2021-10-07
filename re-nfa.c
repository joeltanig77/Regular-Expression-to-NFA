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

int cleanUpNFA(struct NFA* nfa) {
    struct Transitions* temp = NULL;
    struct Transitions* cursor = (struct Transitions*)nfa;
    while(cursor->tran != NULL) {
      temp = cursor->tran;
      cursor = cursor->tran;
      free(temp->tran);
    }
    free(cursor->tran);
    free(cursor);
    return 0;
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

  if (argc == 1) {
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
  // Init the stack
  stack->arrayOfNFAs = nfaArray;
  stack->top = -1;

  while (fgets(line,sizeof(line), fp)) {
    index = 0;
    stateCounter = 1;
  while (line[index] != '\0') {
      char c = line[index];
      index += 1;
      // if the char == '&'
      if (c == 38) {
        struct NFA* nfa2 = pop(stack); // b
        struct NFA* nfa1 = pop(stack); // a
        struct NFA* added = (struct NFA*)calloc(1,sizeof(added));
        struct Transitions* nfaTranCursor1 = nfa1->tran;
        struct Transitions* nfaTranCursor2 = nfa2->tran;
        struct Transitions* addedCursor = (struct Transitions*)added;

        while(nfaTranCursor1 != NULL) {
          struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(collect));
          collect->stateOne = nfaTranCursor1->stateOne;
          collect->stateTwo = nfaTranCursor1->stateTwo;
          collect->symbol = nfaTranCursor1->symbol;
          // Iterate the pointers
          addedCursor->tran = collect;
          addedCursor = addedCursor->tran;
          nfaTranCursor1 = nfaTranCursor1->tran;
        }

        // Add the epsilon state
        struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(collect));
        collect->stateOne = nfa1->acceptState;
        collect->stateTwo = nfa2->startState;
        collect->symbol = 'E';
        added->startState = nfa1->startState;
        added->acceptState = nfa2->acceptState;

        addedCursor->tran = collect;
        addedCursor = addedCursor->tran;
        while(nfaTranCursor2 != NULL) {
          struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(collect));
          collect->stateOne = nfaTranCursor2->stateOne;
          collect->stateTwo = nfaTranCursor2->stateTwo;
          collect->symbol = nfaTranCursor2->symbol;
          // Iterate the pointers
          addedCursor->tran = collect;
          addedCursor = addedCursor->tran;
          nfaTranCursor2 = nfaTranCursor2->tran;
        }
        // clean up nfa's here
        //cleanUpNFA(nfa1);
        //cleanUpNFA(nfa2);
        push(stack,(struct NFA*)added);
      }
      // if the char == '|'
      else if (c == 124) {
        struct NFA* nfa2 = pop(stack);
        struct NFA* nfa1 = pop(stack);
        struct NFA* added = (struct NFA*)calloc(1,sizeof(added));
        struct Transitions* nfaTranCursor1 = nfa1->tran;
        struct Transitions* nfaTranCursor2 = nfa2->tran;
        struct Transitions* addedCursor = (struct Transitions*)added;

        while(nfaTranCursor1 != NULL) {
          struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(collect));
          collect->stateOne = nfaTranCursor1->stateOne;
          collect->stateTwo = nfaTranCursor1->stateTwo;
          collect->symbol = nfaTranCursor1->symbol;
          // Iterate the pointers
          addedCursor->tran = collect;
          addedCursor = addedCursor->tran;
          nfaTranCursor1 = nfaTranCursor1->tran;
        }

        // Do shit
        // Add a new start state s and make a ε-transition from this state to the start states of FA1 and FA2.
        struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(collect));
        collect->stateOne = stateCounter;
        collect->stateTwo = nfa1->startState;
        collect->symbol = 'E';
        added->startState = stateCounter;

        addedCursor->tran = collect;
        addedCursor = addedCursor->tran;

        struct Transitions* collect2 = (struct Transitions*)calloc(1,sizeof(collect));
        collect2->stateOne = stateCounter;
        collect2->stateTwo = nfa2->startState;
        collect2->symbol = 'E';

        addedCursor->tran = collect2;
        addedCursor = addedCursor->tran;

        stateCounter += 1;


        // Add a new final state f and make a ε-transition to this state from each of the final states of FA1 and FA2.
        struct Transitions* collect3 = (struct Transitions*)calloc(1,sizeof(collect));
        collect3->stateTwo = stateCounter;
        collect3->stateOne = nfa1->acceptState;
        collect3->symbol = 'E';
        added->acceptState = stateCounter;

        addedCursor->tran = collect3;
        addedCursor = addedCursor->tran;

        struct Transitions* collect4 = (struct Transitions*)calloc(1,sizeof(collect));
        collect4->stateTwo = stateCounter;
        collect4->stateOne = nfa2->acceptState;
        collect4->symbol = 'E';

        addedCursor->tran = collect4;
        addedCursor = addedCursor->tran;
        stateCounter += 1;

        while(nfaTranCursor2 != NULL) {
          struct Transitions* collect5 = (struct Transitions*)calloc(1,sizeof(collect));
          collect5->stateOne = nfaTranCursor2->stateOne;
          collect5->stateTwo = nfaTranCursor2->stateTwo;
          collect5->symbol = nfaTranCursor2->symbol;
          // Iterate the pointers
          addedCursor->tran = collect5;
          addedCursor = addedCursor->tran;
          nfaTranCursor2 = nfaTranCursor2->tran;
        }

        push(stack,(struct NFA*)added);

      }
      // if the char == '*'
      else if (c == 42) {
        struct NFA* nfa1 = pop(stack);
        struct NFA* added = (struct NFA*)calloc(1,sizeof(added));
        struct Transitions* nfaTranCursor1 = nfa1->tran;
        struct Transitions* addedCursor = (struct Transitions*)added;


        // Add everything from the old nfa to the new nfa (added)
        while(nfaTranCursor1 != NULL) {
          struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(collect));
          collect->stateOne = nfaTranCursor1->stateOne;
          collect->stateTwo = nfaTranCursor1->stateTwo;
          collect->symbol = nfaTranCursor1->symbol;
          // Iterate the pointers
          addedCursor->tran = collect;
          addedCursor = addedCursor->tran;
          nfaTranCursor1 = nfaTranCursor1->tran;
        }

        added->startState = nfa1->startState;

        // Add a new start state s and make a ε-transition from this state to the start stateof FA1
        struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(collect));
        collect->stateOne = stateCounter;
        collect->stateTwo = nfa1->startState;
        collect->symbol = 'E';
        addedCursor->tran = collect;
        addedCursor = addedCursor->tran;

        // Make a ε-transition from the final state of F1 to the new start state s
        struct Transitions* collect2 = (struct Transitions*)calloc(1,sizeof(collect));
        collect2->stateOne = nfa1->acceptState;
        collect2->stateTwo = stateCounter;
        collect2->symbol = 'E';
        addedCursor->tran = collect2;
        added->startState = stateCounter;
        added->acceptState = stateCounter;
        stateCounter += 1;
        push(stack,(struct NFA*)added);
        // Check if I even fucking did it right
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
        nfa->tran->symbol = c;
        push(stack,nfa);
      }
    }
    fclose(fp);
    if(errno) {
      fprintf(stderr,"%s\n", strerror(errno));
    }
  }
  return 0;
}
