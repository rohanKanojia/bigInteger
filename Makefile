CC = g++
OBJ = BIG_INTEGER
all:
	$(CC) -c Integer.cxx 
	$(CC) IntegerTest.cpp -o IntegerTest Integer.o
