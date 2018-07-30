TARGET	=	myclient	myserver
CC	=	gcc
CFLAGS	=	-g
normal	:	$(TARGET)

client	:	myclient.c
	$(CC)	$(CFLAGS)	myclient.c 	-pthread	-o	myclient

server	:	myserver.c
	$(CC)	$(CFLAGS)	myserver.c	-pthread	-o	myserver

clean	:
	$(RM) $(TARGET)
	make -f makefile.mk normal
