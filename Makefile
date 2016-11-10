#connect: connect.o
#	gcc connect.o -o connect

#connect.o: connect.c
#	gcc connect.c -c -o connect.o

#all: send.o client.o server.o
#	gcc send.o -o send -I.
#	gcc client.o -o client -I.
#	gcc server -o server.o -I.

send: send.o
	gcc send.o -o send -I.

send.o: send.c
	gcc send.c -c -o send.o -I.

client: client.o
	gcc client.o -o client -I.

client.o: client2.c
	gcc client2.c -c -o client.o -I.

server: server.o
	gcc server -o server.o -I.

server.o: server2.c
	gcc server2.c -c -o server2.o -I.
