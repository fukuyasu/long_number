CC = cc
AR = ar
RM = rm -f

.c.o:
	$(CC) -c -o $@ $< -I..

all:	test

test:	exec_test1

exec_test1:	test1
	@echo "Test1..."
	@./test1 | tee test1.output 2>&1
	@if diff -u test1.expected test1.output; then \
		echo "Test1: clear."; \
	else \
		echo "Test1: failed."; \
	fi

test1.o:	test1.c ../longnumber.h

test1:	test1.o ../liblongnumber.a
	$(CC) -o test1 test1.o -llongnumber -L..

clean::
	$(RM) test1
	$(RM) test1.o
	$(RM) test1.output
