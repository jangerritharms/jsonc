all: libjsonc.a
	
libjsonc.a: jsonc.o
	$(AR) rc $@ $^

%.o: %.c jsonc.h
	$(CC) -c $(CFLAGS) $< -o $@

test: jsonc_test

jsonc_test: jsonc_test.o
	$(CC) $< -o $@ -L. -ljsonc

jsonc_test.o: jsonc_test.c libjsonc.a

clean:
	rm -f jsonc.o jsonc_test.o
	rm -f jsonc_test
	rm -f jsonc_test.exe
	rm -f libjsonc.a

.PHONY: all clean test
