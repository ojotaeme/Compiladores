# Makefile

CC = cc
CFLAGS = -I. -g
OBJ = main.o lexer.o parser.o keywords.o symtab.o

mypas: $(OBJ)
	$(CC) -o mypas $(OBJ)

main.o: main.c lexer.h parser.h keywords.h symtab.h
	$(CC) $(CFLAGS) -c main.c

lexer.o: lexer.c lexer.h keywords.h
	$(CC) $(CFLAGS) -c lexer.c

parser.o: parser.c parser.h lexer.h keywords.h symtab.h
	$(CC) $(CFLAGS) -c parser.c

keywords.o: keywords.c keywords.h
	$(CC) $(CFLAGS) -c keywords.c

symtab.o: symtab.c symtab.h
	$(CC) $(CFLAGS) -c symtab.c

clean:
	rm -f *.o mypas
