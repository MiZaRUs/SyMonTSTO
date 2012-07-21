# Makefile for the socket programming example
#

objects = ServerSocket.o Serial.o main.o

all : emoxa

emoxa: $(objects)
	g++ -o emoxa $(objects)

ServerSocket: ServerSocket.cpp
Serial: Serial.cpp
emoxa: main.cpp

clean:
	rm -f *.o
