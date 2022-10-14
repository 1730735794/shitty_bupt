CC=gcc
CFLAGS=-O2

dnsrelay: main.c protocol.c log.c resolver.c trietree.c
	gcc -g main.c protocol.c log.c resolver.c trietree.c id_map.c -o dnsrelay

clean:
	${RM} *.o dnsrelay

