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

install:
	chmod 550 emoxa 
	mv -f emoxa /opt/
	cp -fR ./init.d/* /etc/init.d/
	/sbin/chkconfig --add emoxa
	/sbin/chkconfig --list emoxa
#	"/usr/sbin/ntsysv"