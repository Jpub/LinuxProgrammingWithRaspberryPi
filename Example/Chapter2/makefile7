# makefile
.SUFFIXES : .c .o

OBJECTS = main.o \
	  print.o \
          input.o
SRCS = $(OBJECTS:.o=.c)

CC = gcc
CFLAGS = -g 
#TARGET = test

all : test helloworld

helloworld : helloworld.c 
	$(CC) -o $@ $<

test : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

.c.o :
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(RM) $(OBJECTS) $(TARGET) core

main.o : main.c
print.o : print.c 
input.o : input.c 
