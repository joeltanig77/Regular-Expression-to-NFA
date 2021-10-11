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
  char spaceHolder;
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


int main(int argc,char *argv[]) {
  char line[1024];
  memset(line, '\0', sizeof(line));
  int stateCounter;
  int index;
  int sizeOfNFA = 0;
  int illegalStateCounter = 0;
  struct Stack* stack = (struct Stack*)calloc(1,sizeof(struct Stack));
  struct NFA** nfaArray = (struct NFA**)calloc(1,sizeof(struct NFA*));
  if (argc == 1) {
    fprintf(stdout,"%s\n", "No file was passed in");
    free(stack);
    free(nfaArray);
    return 0;
  }
  FILE* fp = fopen(argv[1],"r");
  if (!fp) {
    fprintf(stderr,"%s\n","Could not open file");
    free(stack);
    free(nfaArray);
    return 0;
  }
  // Init the stack
  stack->arrayOfNFAs = nfaArray;
  stack->top = -1;
  while (fgets(line,sizeof(line),fp)) {
    index = 0;
    stateCounter = 1;
  while (line[index] != '\0') {
      char c = line[index];
      // Catch illegal arguments
      if((c != 38 && c != 42 && c != 69 && c != 124 && c != 10) && (c > 101 || c < 97) && (isEmpty(stack)) == 0) {
        // Delete the new line
        line[strlen(line)-1] = '\0';
        fprintf(stderr,"RE: %s is an illegal regular expression in postfix form\n",line);
        free(stack);
        free(nfaArray);
        exit(0);
      }
      index += 1;
      // If the char == '&' concat case
      if (c == 38) {
        illegalStateCounter = 0;
        struct NFA* nfa2 = pop(stack); // b
        struct NFA* nfa1 = pop(stack); // a
        struct NFA* added = (struct NFA*)calloc(1,sizeof(struct NFA));
        struct Transitions* nfaTranCursor1 = nfa1->tran;
        struct Transitions* nfaTranCursor2 = nfa2->tran;
        struct Transitions* addedCursor = (struct Transitions*)added;
        // Add everything from the old nfa to the new nfa (added)
        while(nfaTranCursor1 != NULL) {
          struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(struct Transitions));
          collect->stateOne = nfaTranCursor1->stateOne;
          collect->stateTwo = nfaTranCursor1->stateTwo;
          collect->symbol = nfaTranCursor1->symbol;
          // Iterate the pointers
          addedCursor->tran = collect;
          addedCursor = addedCursor->tran;
          nfaTranCursor1 = nfaTranCursor1->tran;
        }
        sizeOfNFA += 1;
        // Add the epsilon state
        struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(struct Transitions));
        collect->stateOne = nfa1->acceptState;
        collect->stateTwo = nfa2->startState;
        collect->symbol = 'E';
        added->startState = nfa1->startState;
        added->acceptState = nfa2->acceptState;
        // Add the transitionn to the master transition and iterate the pointers
        addedCursor->tran = collect;
        addedCursor = addedCursor->tran;
        // Add everything from the old nfa to the new nfa (added)
        while(nfaTranCursor2 != NULL) {
          struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(struct Transitions));
          collect->stateOne = nfaTranCursor2->stateOne;
          collect->stateTwo = nfaTranCursor2->stateTwo;
          collect->symbol = nfaTranCursor2->symbol;
          // Iterate the pointers
          addedCursor->tran = collect;
          addedCursor = addedCursor->tran;
          nfaTranCursor2 = nfaTranCursor2->tran;
        }
        // Clean up nfa's here
        cleanUpNFA(nfa1);
        cleanUpNFA(nfa2);
        push(stack,(struct NFA*)added);
      }
      // If the char == '|' union case
      else if (c == 124) {
        illegalStateCounter = 0;
        struct NFA* nfa2 = pop(stack);
        struct NFA* nfa1 = pop(stack);
        struct NFA* added = (struct NFA*)calloc(1,sizeof(struct NFA));
        struct Transitions* nfaTranCursor1 = nfa1->tran;
        struct Transitions* nfaTranCursor2 = nfa2->tran;
        struct Transitions* addedCursor = (struct Transitions*)added;
        // Add everything from the old nfa to the new nfa (added)
        while(nfaTranCursor1 != NULL) {
          struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(struct Transitions));
          collect->stateOne = nfaTranCursor1->stateOne;
          collect->stateTwo = nfaTranCursor1->stateTwo;
          collect->symbol = nfaTranCursor1->symbol;
          // Iterate the pointers
          addedCursor->tran = collect;
          addedCursor = addedCursor->tran;
          nfaTranCursor1 = nfaTranCursor1->tran;
        }
        // Add a new start state s and make a ε-transition from this state to the start states of FA1 and FA2.
        sizeOfNFA += 1;
        struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(struct Transitions));
        collect->stateOne = stateCounter;
        collect->stateTwo = nfa1->startState;
        collect->symbol = 'E';
        added->startState = stateCounter;
        addedCursor->tran = collect;
        addedCursor = addedCursor->tran;
        sizeOfNFA += 1;
        struct Transitions* collect2 = (struct Transitions*)calloc(1,sizeof(struct Transitions));
        collect2->stateOne = stateCounter;
        collect2->stateTwo = nfa2->startState;
        collect2->symbol = 'E';
        addedCursor->tran = collect2;
        addedCursor = addedCursor->tran;
        stateCounter += 1;
        // Add a new final state f and make a ε-transition to this state from each of the final states of FA 1 and FA 2
        sizeOfNFA += 1;
        struct Transitions* collect3 = (struct Transitions*)calloc(1,sizeof(struct Transitions));
        collect3->stateTwo = stateCounter;
        collect3->stateOne = nfa1->acceptState;
        collect3->symbol = 'E';
        added->acceptState = stateCounter;
        addedCursor->tran = collect3;
        addedCursor = addedCursor->tran;
        sizeOfNFA += 1;
        struct Transitions* collect4 = (struct Transitions*)calloc(1,sizeof(struct Transitions));
        collect4->stateTwo = stateCounter;
        collect4->stateOne = nfa2->acceptState;
        collect4->symbol = 'E';
        addedCursor->tran = collect4;
        addedCursor = addedCursor->tran;
        stateCounter += 1;
        // Add everything from the old nfa to the new nfa (added)
        while(nfaTranCursor2 != NULL) {
          struct Transitions* collect5 = (struct Transitions*)calloc(1,sizeof(struct Transitions));
          collect5->stateOne = nfaTranCursor2->stateOne;
          collect5->stateTwo = nfaTranCursor2->stateTwo;
          collect5->symbol = nfaTranCursor2->symbol;
          // Iterate the pointers
          addedCursor->tran = collect5;
          addedCursor = addedCursor->tran;
          nfaTranCursor2 = nfaTranCursor2->tran;
        }
        cleanUpNFA(nfa1);
        cleanUpNFA(nfa2);
        push(stack,(struct NFA*)added);
      }
      // If the char == '*' kleene star case
      else if (c == 42) {
        illegalStateCounter = 0;
        struct NFA* nfa1 = pop(stack);
        struct NFA* added = (struct NFA*)calloc(1,sizeof(struct NFA));
        struct Transitions* nfaTranCursor1 = nfa1->tran;
        struct Transitions* addedCursor = (struct Transitions*)added;
        // Add everything from the old nfa to the new nfa (added)
        while(nfaTranCursor1 != NULL) {
          struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(struct Transitions));
          collect->stateOne = nfaTranCursor1->stateOne;
          collect->stateTwo = nfaTranCursor1->stateTwo;
          collect->symbol = nfaTranCursor1->symbol;
          // Iterate the pointers
          addedCursor->tran = collect;
          addedCursor = addedCursor->tran;
          nfaTranCursor1 = nfaTranCursor1->tran;
        }
        // Save the old start state
        added->startState = nfa1->startState;
        sizeOfNFA += 1;
        // Add a new start state s and make a ε-transition from this state to the start stateof FA 1
        struct Transitions* collect = (struct Transitions*)calloc(1,sizeof(struct Transitions));
        collect->stateOne = stateCounter;
        collect->stateTwo = nfa1->startState;
        collect->symbol = 'E';
        addedCursor->tran = collect;
        addedCursor = addedCursor->tran;
        sizeOfNFA += 1;
        // Make a ε-transition from the final state of F1 to the new start state s
        struct Transitions* collect2 = (struct Transitions*)calloc(1,sizeof(struct Transitions));
        collect2->stateOne = nfa1->acceptState;
        collect2->stateTwo = stateCounter;
        collect2->symbol = 'E';
        addedCursor->tran = collect2;
        added->startState = stateCounter;
        added->acceptState = stateCounter;
        stateCounter += 1;
        cleanUpNFA(nfa1);
        push(stack,(struct NFA*)added);
      }
      else {
        // Push the char nfa into the stack
        if (c == 10) {
          struct NFA* finalNFA = pop(stack);
          if ((isEmpty(stack)) == 0) {
            line[strlen(line)-1] = '\0';
            fprintf(stderr,"RE: %s is an illegal regular expression in postfix form\n",line);
            exit(0);
          }
          illegalStateCounter = 0;
          // If the char is a new line then print the nfa transitions
          struct Transitions** arrayOfTrans = (struct Transitions**)calloc(sizeOfNFA,sizeof(struct Transition*));
          // Insert all transitions into a array of transitions and qsort it
          printSortedNFATrans(finalNFA,arrayOfTrans,sizeOfNFA,line);
          sizeOfNFA = 0;
          memset(line, '\0', sizeof(line));
          cleanUpNFA(finalNFA);
          free(arrayOfTrans);
          continue;
        }
        illegalStateCounter += 1;
        sizeOfNFA += 1;
        struct NFA* nfa = (struct NFA*)calloc(1,sizeof(struct NFA));
        struct Transitions* tran = (struct Transitions*)calloc(1,sizeof(struct Transitions));
        nfa->tran = tran;
        nfa->tran->stateOne = stateCounter;
        nfa->startState = stateCounter;
        stateCounter += 1;
        nfa->tran->stateTwo = stateCounter;
        nfa->acceptState = stateCounter;
        stateCounter += 1;
        nfa->tran->symbol = c;
        push(stack,nfa);
      }
    }
  }
  fclose(fp);
  if(errno) {
    fprintf(stderr,"%s\n", strerror(errno));
  }
  free(stack);
  free(nfaArray);
  return 0;
}
