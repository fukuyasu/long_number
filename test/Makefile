CC = cc
AR = ar
RM = rm -f

.c.o:
	$(CC) -c -o $@ $< -I..

all:	test

test:	exec_test1

exec_test1:	test1
	./test1 |& tee test1.output
	@diff -u test1.expected test1.output & echo "Error!"

test1.o:	test1.c ../longnumber.h

test1:	test1.o ../liblongnumber.a
	$(CC) -o test1 test1.o -llongnumber -L..

clean::
	$(RM) test1
	$(RM) test1.o
