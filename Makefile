CC = cc
AR = ar
RM = rm -f

.c.o:
	$(CC) -c -o $@ $<

all:	pi napier

pi.o:	pi.c longnumber.h
napier.o:	napier.c longnumber.h
longnumber.o:	longnumber.c longnumber.h

pi:	pi.o liblongnumber.a
	$(CC) -o pi pi.o -llongnumber -L.

napier:	napier.o liblongnumber.a
	$(CC) -o napier napier.o -llongnumber -L.

liblongnumber.a:	longnumber.o
	$(AR) -qc liblongnumber.a longnumber.o

clean::
	$(RM) pi
	$(RM) pi.o

clean::
	$(RM) napier
	$(RM) napier.o

clean::
	$(RM) liblongnumber.a
	$(RM) longnumber.o
