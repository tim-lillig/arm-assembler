SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

%.o: %.c
	gcc -Wall -std=c18 -g -c -o $@ $<

AssemblerTest: $(OBJ)
	gcc -o $@ $^ -lcheck

clean:
	rm -rf *.o AssemblerTest
