CC = c99
C_FLAGS = -Wvla -Wextra -Wall -g

MY_STRING = MyString.c MyString.h
LIB_OBJECTS = MyString.o
LIB_NAME = libmyString.a
VALGRIND_FLAGS = --leak-check=full --show-possibly-lost=yes --show-reachable=yes --undef-value-errors=yes
TAR_OBJS = valdbg_MyStringMain valdbg_myStringTests Makefile MyStringMain.c ${MY_STRING} README

ex3.tar: ${TAR_OBJS}
	tar -cvf ex3.tar ${TAR_OBJS} valdbg_MyStringMain

# Tests

valdbg_MyStringMain: MyStringMain
	chmod 777 MyStringMain
	valgrind ${VALGRIND_FLAGS} ./MyStringMain >& valdbg_MyStringMain

valdbg_myStringTests: MyStringTests
	chmod 777 MyStringTests
	valgrind ${VALGRIND_FLAGS} ./MyStringTests >& valdbg_myStringTests

tests: MyStringTests
	./MyStringTests
main: MyStringMain
	./MyStringMain

# Compiles MyString without the -DNDEBUG flag
MyStringTests: ${MY_STRING}
	${CC} ${C_FLAGS} -c ${MY_STRING}
	${CC} ${C_FLAGS} MyString.o -o MyStringTests
	rm -fv *.o
MyStringMain: MyStringMain.o
	${CC} ${C_FLAGS} MyStringMain.o -L. -lmyString -o MyStringMain
MyStringMain.o: MyStringMain.c myString
	${CC} ${C_FLAGS} -c -I. MyStringMain.c -o MyStringMain.o

# Static Library Creation

myString: ${LIB_OBJECTS}
	ar rcs ${LIB_NAME} ${LIB_OBJECTS}
	ranlib ${LIB_NAME}
	rm -fv *.o

MyString.o: ${MY_STRING}
	${CC} ${C_FLAGS} -DNDEBUG -c ${MY_STRING}

# Cleaning

clean:
	rm -fv *.o *.gch MyStringMain MyStringTests *.out *.exe ${LIB_NAME} valdbg*

.PHONY: clean