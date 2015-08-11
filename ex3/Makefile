CC = c99
C_FLAGS = -Wvla -Wextra -Wall -g -DNDEBUG

tests: MyStringMain
	./MyStringMain
MyStringMain: libmyString.a MyStringMain.o
	${CC} ${C_FLAGS} MyStringMain.o -L. -lmyString -o MyStringMain
MyStringMain.o: MyStringMain.c
	${CC} ${C_FLAGS} -c -I. MyStringMain.c -o MyStringMain.o

LIB_OBJECTS = MyString.o
libmyString.a: ${LIB_OBJECTS}
	ar rcs libmyString.a ${LIB_OBJECTS}
	ranlib libmyString.a

MY_STRING = MyString.c MyString.h
MyString.o: ${MY_STRING}
	${CC} ${C_FLAGS} -c ${MY_STRING}
clean:
	rm -fv *.o *.gch libmyString.a MyStringMain test.out *.exe 
.PHONY: clean