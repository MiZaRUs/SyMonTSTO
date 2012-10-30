# Makefile for the MAIN programming example
#
main_objects = config.o driver.o device.o transport.o dbwriter.o daemon.o wheel.o main.o

all : server_sd

server_sd: $(main_objects)
	g++ -o server_sd $(main_objects) -L/usr/lib/mysql -lmysqlclient -s -O3

config: config.cpp
driver: driver.cpp
device: device.cpp
transport: transport.cpp
dbwriter: dbwriter.cpp
daemon: daemon.cpp
wheel: wheel.cpp
main: main.cpp

clean:
	rm -f *.o

install:
	chmod 550 server_sd 
	mv -f server_sd /opt/server_sd/
#	mv -f server_sd ./script/server_sd/
#	cp -fR ./script/server_sd /opt/
#	cp -fR ./script/init.d/* /etc/init.d/
#	/sbin/chkconfig --add server_sd
#	/sbin/chkconfig --list server_sd
#	"/usr/sbin/ntsysv"
