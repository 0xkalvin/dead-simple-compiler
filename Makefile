 
.PHONY: build run all clean

default: all

COMPILER=gcc
PATTERN=*.c
BINARY=dsc
FLAGS=-Wall

build:
	${COMPILER} ${FLAGS} ${PATTERN} -o ${BINARY}

run:
	./${BINARY}

all: build run

clean:
	rm ${BINARY}