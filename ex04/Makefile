# Makefile for OPS exercise 2
# Use tabs for indentation, not spaces!
TARGET = parent

all: printversion parent

# Link object files and create a binary executable:
parent: parent.o displayFunctions.o
	${CC} -Wall -o ${TARGET} ${TARGET}.o displayFunctions.o ../ex02/syntaxCheck.o

# Compile C-files and create object files (.o).
# The option -c ensures no linking takes place:
${TARGET}.o: ${TARGET}.c displayFunctions.h errorCodes.h
	${CC} -c -Wall ${TARGET}.c

displayFunctions.o: displayFunctions.c displayFunctions.h syntaxCheck.h errorCodes.h
	${CC} -c -Wall displayFunctions.c

clean:
	rm -f *.o

cleanall: clean
	rm -f ${TARGET}

printversion:
	${CC} --version | grep ${CC}
