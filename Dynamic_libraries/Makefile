
all:
	$(CC) -c -g -O0 -Wall -Werror mn.c -o libmn.o -fPIC
	$(CC) -shared -o libmn.so libmn.o

	$(CC) -c -g -O0 -Wall -Werror m16.c -o libm16.o -fPIC
	$(CC) -shared -o libm16.so libm16.o

	$(CC) -c -g -O0 -Wall -Werror flib.c -o libsflib.o -fPIC
	$(CC) -shared -o libflib.so libflib.o

	-sudo unlink /lib/x86_64-linux-gnu/libflib.so
	-sudo ln -s $(shell pwd)/libflib.so /lib/x86_64-linux-gnu/libflib.so

	$(CC) -g -O0 main.c -o a.out -ldl -L $(shell pwd) -lflib