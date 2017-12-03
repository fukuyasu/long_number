CC = cc
RM = rm -f

.c.o:
	$(CC) -c -o $@ $<

all:	pi

pi.o:	pi.c longnumber.h

pi:	pi.o
	$(CC) -o pi pi.o

clean::
	$(RM) pi
	$(RM) pi.o
