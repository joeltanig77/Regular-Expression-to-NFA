CPP = gcc
FLAGS = -g -Wall -pedantic

EXEC = re-nfa
OBJS = re-nfa.o

default:${EXEC}

clean:
	rm -f ${EXEC}
	rm -f *.o

bug: ${EXEC}
	gdb -q -tui --args ./${EXEC} pp.txt

valgrind: ${EXEC}
	valgrind --leak-check=full --track-origins=yes ./${EXEC} pp.txt

run: ${EXEC}
	./${EXEC} pp.txt

${EXEC}:${OBJS}
	${CPP} ${FLAGS} -o ${EXEC} ${OBJS}

.c.o:
	${CPP} ${FLAGS} -c $<

re-nfa.o: re-nfa.c
