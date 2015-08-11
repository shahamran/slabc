CC = c99
C_FLAGS = -Wvla -Wextra -Wall
MY_STRING = MyString.c MyString.h
LIB_OBJECTS = MyString.o
tests: MyStringMain
	./MyStringMain
MyStringMain: libmyString.a
	${CC} ${C_FLAGS} -c -I. MyStringMain.c -o MyStringMain.o
	${CC} MyStringMain.o -L. -lmyString -o MyStringMain
libmyString.a: ${LIB_OBJECTS}
	ar rcs libmyString.a ${LIB_OBJECTS}
	ranlib libmyString.a
MyString.o: ${MY_STRING}
	${CC} ${C_FLAGS} -c ${MY_STRING}
clean:
	rm -fv *.o *.gch libmyString.a MyStringMain test.out *.exe 
.PHONY: clean