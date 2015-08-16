CC = c99
C_FLAGS = -Wvla -Wextra -Wall -g

LIB_NAME = libmyString.a

# Tests

tests: MyStringMain ${MY_STRING}
	${CC} ${C_FLAGS} -c ${MY_STRING}
	${CC} ${C_FLAGS} MyString.o -o MyString
	./MyString
	./MyStringMain
MyStringMain: MyStringMain.o
	${CC} ${C_FLAGS} MyStringMain.o -L. -lmyString -o MyStringMain
MyStringMain.o: MyStringMain.c ${LIB_NAME}
	${CC} ${C_FLAGS} -c -I. MyStringMain.c -o MyStringMain.o

# Static Library Creation

LIB_OBJECTS = MyString.o
${LIB_NAME}: ${LIB_OBJECTS}
	ar rcs ${LIB_NAME} ${LIB_OBJECTS}
	ranlib ${LIB_NAME}

MY_STRING = MyString.c MyString.h
MyString.o: ${MY_STRING}
	${CC} ${C_FLAGS} -DNDEBUG -c ${MY_STRING}

# Clean

clean:
	rm -fv *.o *.gch libmyString.a MyStringMain *.out *.exe
.PHONY: clean