# Makefile for the Device Access Manager
#
#PROG	=	s_vds1_tm
PROG	=	s_vds1_zz
#PROG	=	s_vds5_di
#PROG	=	s_vds5_pv
#PROG	=	s_vds5_tm
#
CC 	=	gcc
IFLAGS  = -idirafter dummyinc
#CFLAGS = -g
#CFLAGS	=	-O2 -Wall -W -Wshadow #-pedantic -Werror -Wconversion
CFLAGS	=	-O2 -W -Wshadow #-pedantic -Werror -Wconversion
#LIBS	=	 -L/usr/lib/mysql -lmysqlclient
LINK	=	-Wl,-s

OBJS	=	ipc.o transport.o devices.o main.o

.c.o:
	$(CC) -c $*.c $(CFLAGS) $(IFLAGS)

$(PROG): $(OBJS) 
	$(CC) -o $(PROG).x $(OBJS) $(LINK) $(LIBS) $(LDFLAGS)

config:
	cp -fR ./$(PROG)/0_inc_dev.h ./

clean:
	rm -f *.o

install:
	chmod 550 $(PROG).x
#	mv -f $(PROG).x /opt/bin/$(PROG)
##	mv -f $(PROG) ./script/server_sd/
##	cp -fR ./script/s_vds5_tm /opt/
##	cp -fR ./script/init.d/* /etc/init.d/
#	/sbin/chkconfig --add $(PROG)
#	/sbin/chkconfig --list $(PROG)
#	"/usr/sbin/ntsysv"
	rm -f *.o
