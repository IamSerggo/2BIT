PROJ=proj2
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread
CC=gcc
RM=rm -f

$(PROJ) : $(PROJ).c
	$(CC) $(PROJ).c -o $(PROJ) $(CFLAGS)

clean :
	$(RM) *.o $(PROJ) $(PROJ).out $(PROJ).zip 

zip :
	zip proj2 $(PROJ).h $(PROJ).c Makefile
