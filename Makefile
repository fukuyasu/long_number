CC = cc
AR = ar
RM = rm -f

.c.o:
	$(CC) -c -o $@ $<

all:	pi

pi.o:	pi.c longnumber.h
longnumber.o:	longnumber.c longnumber.h

pi:	pi.o liblongnumber.a
	$(CC) -o pi pi.o -llongnumber -L.

liblongnumber.a:	longnumber.o
	$(AR) -qc liblongnumber.a longnumber.o

clean::
	$(RM) pi
	$(RM) pi.o

clean::
	$(RM) liblongnumber.a
	$(RM) longnumber.o
