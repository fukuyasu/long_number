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

machinespec:
	@uname -s -r
	@$(CC) --version | head -1
	@if [ -e "/proc/cpuinfo" ];\
	then\
		grep "model name" /proc/cpuinfo | head -1;\
	else\
		dmesg | grep "CPU" | head -1;\
	fi

benchmark:	pi machinespec
	@./pi -s 1000
	@./pi -s 2000
	@./pi -s 5000
