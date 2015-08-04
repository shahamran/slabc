# Makes MyString file
TestString: MyString.o
	c99 -Wextra -Wvla MyString.o -o TestString
MyString.o: MyString.c MyString.h
	c99 -Wextra -Wvla -c MyString.c
# Makes LineSeparator file
TestLine: LineSeparator.o
	c99 -Wextra -Wvla LineSeparator.o -o TestLine
LineSeparator.o: LineSeparator.c LineSeparator.h
	c99 -Wextra -Wvla -c LineSeparator.c
# Removes the executables and object files
ex2.tar: MyString.c LineSeparator.c LineSeparator.h
	tar -cvf ex2.tar MyString.c LineSeparator.c LineSeparator.h
clean:
	rm -f *.o TestLine TestString
# Not really a file name
.PHONY: clean