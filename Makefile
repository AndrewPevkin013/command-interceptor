all:
	gcc -Wall -fPIC -shared -o liblogger.so logger.c -ldl
run: all
	LD_PRELOAD=$(PWD)/liblogger.so bash
clean:
	rm -f liblogger.so
