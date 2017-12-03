CC = cc
RM = rm -f

.c.o:
	$(CC) -c -o $@ $<

all:	pi

pi.o:	pi.c longnumber.h
longnumber.o:	longnumber.c longnumber.h

pi:	pi.o longnumber.o
	$(CC) -o pi pi.o longnumber.o

clean::
	$(RM) pi
	$(RM) pi.o

clean::
	$(RM) longnumber.o
