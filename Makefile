SOURCE := main.c perms.c perms.h common.h
EXEC   := main.out

CC       := gcc
CC_FLAGS := -g

all: ${EXEC}

run: ${EXEC}
	./${EXEC}

${EXEC}: ${SOURCE}
	${CC} $^ -o $@ ${CC_FLAGS}

clean:
	rm ${EXEC}