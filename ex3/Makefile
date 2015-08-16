CC = c99
C_FLAGS = -Wvla -Wextra -Wall -g

MY_STRING = MyString.c MyString.h
LIB_OBJECTS = MyString.o
LIB_NAME = libmyString.a
VALGRIND_FLAGS = --leak-check=full --show-possibly-lost=yes --show-reachable=yes --undef-value-errors=yes

# Tests

valgrindMain: MyStringMain
	chmod 777 MyStringMain
	valgrind ${VALGRIND_FLAGS} ./MyStringMain >& valdbg.out

valgrindTests: MyStringTests
	chmod 777 MyStringTests
	valgrind ${VALGRIND_FLAGS} ./MyStringTests >& valdbg.out

tests: MyStringTests
	./MyStringTests

# Compiles MyString without the -DNDEBUG flag
MyStringTests: ${MY_STRING}
	${CC} ${C_FLAGS} -c ${MY_STRING}
	${CC} ${C_FLAGS} MyString.o -o MyStringTests
MyStringMain: MyStringMain.o
	${CC} ${C_FLAGS} MyStringMain.o -L. -lmyString -o MyStringMain
MyStringMain.o: MyStringMain.c ${LIB_NAME}
	${CC} ${C_FLAGS} -c -I. MyStringMain.c -o MyStringMain.o

# Static Library Creation

${LIB_NAME}: ${LIB_OBJECTS}
	ar rcs ${LIB_NAME} ${LIB_OBJECTS}
	ranlib ${LIB_NAME}

MyString.o: ${MY_STRING}
	${CC} ${C_FLAGS} -DNDEBUG -c ${MY_STRING}

# Cleaning

clean:
	rm -fv *.o *.gch MyStringMain MyStringTests *.out *.exe ${LIB_NAME}

.PHONY: clean