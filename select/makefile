src = $(wildcard *.c) 
obj = $(patsubst %.c,%.o,$(src))
all:select_server client
select_server:select_server.o wrap.o
	gcc select_server.o wrap.o -o select_server -Wall 
client:client.o wrap.o
	gcc client.o wrap.o -o client -Wall 
%.o:%.c
	gcc -c $<  -Wall
.PHONY:clean all
clean :
	-rm -rf select_server client $(boj)
	-rm -rf *~

