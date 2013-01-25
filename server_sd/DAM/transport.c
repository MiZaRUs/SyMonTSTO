/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright  (C)  2010-2013  by  Oleg Shirokov    olgshir@gmail.com     *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
//  --
#include "errors.h"
#include "transport.h"
//  -----------
//#define DEBUG
//  -----------
static int transport;
static char *tr_dev;
static int tr_ps;
static int timeaut;
//  -----------
static struct sockaddr_in *addrInfo;
static struct addrinfo hints, *addr;
static struct timeval tv;
//  -----------
static struct termios options;
static struct termios old_options;
//  -----------
#define MAX_RECONNECT 10
//  -------------------------------------------------------------------------
static int open_transport( void ){
    if( transport > 0 ) return 1;
#ifndef SERIAL_ASCII
// SOCKET
    if( getaddrinfo( tr_dev, NULL, &hints, &addr )){
        transport = -1;
        return 0;
    }
    addrInfo = ( struct sockaddr_in* ) addr->ai_addr;
//  --
    if(( transport = socket( addrInfo->sin_family, addr->ai_socktype, addr->ai_protocol )) < 0 ){
        transport = -1;
        return 0;
    }
//  --
    if( setsockopt( transport, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof( tv )) == -1 ){
        close( transport );
        transport = -1;
        return 0;
    }
//  --
    addrInfo->sin_port = htons( tr_ps );
    if( connect( transport, ( struct sockaddr* ) addrInfo, addr->ai_addrlen ) < 0 ){
        close( transport );
        transport = -1;
        return 0;
    }
//  --
    freeaddrinfo( addr );
#else
//  -------------------------------------------------------------------------
// SERIAL
//скорость обмена            - 9600 бит/сек (может изменяться);
//информационная длина слова - 8 бит;
//бит паритета               - нет;
//кол-во стоп-битов          - 1.
//  -------------------------------------------------------------------------
    if((transport = open(tr_dev, O_RDWR | O_NOCTTY | O_NDELAY)) < 0){
        transport = -1;
        return 0;
    }
    if(tcgetattr(transport, &options)!=0){
        close( transport );
        transport = -1;
        return 0;
    }
    old_options = options;
//  --
// Set speed
    speed_t tspd = B9600;
    switch( tr_ps ){
        case 600:	tspd = B600;	break;
        case 1200:	tspd = B1200;	break;
        case 2400:	tspd = B2400;	break;
        case 4800:	tspd = B4800;	break;
        case 9600:	tspd = B9600;	break;
        case 19200:	tspd = B19200;	break;
        case 38400:	tspd = B38400;	break;
        case 57600:	tspd = B57600;	break;
        case 115200:	tspd = B115200;	break;
    }
    cfsetispeed( &options, tspd );
    cfsetospeed( &options, tspd );
//  set_parity(no);
    options.c_cflag &= ~PARENB;
    options.c_cflag |= CS8; // 8bit
//  --
    options.c_cflag &= ~CRTSCTS; //disable hardware flow control
    options.c_cflag |= ( CLOCAL | CREAD ); //enable receiver & set local mode
    options.c_lflag &= ~( ICANON | ECHO | ECHOE | ISIG ); //row mode
    options.c_iflag &= ~( IXON | IXOFF | IXANY ); //disable software flow control
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = timeaut / 100; //time-out 0,5 sek
    if( tcsetattr( transport, TCSAFLUSH, &options ) != 0 ){
        close( transport );
        transport = -1;
        return 0;
    }
#endif
    return 1;
}// End open_transport()
//  -------------------------------------------------------------------------
/***************************************************************************/
extern void create_transport( char *dname, int ps, int taut ){
    if( transport ) close( transport );
    transport = 0;
    tr_dev = dname;
    tr_ps = ps;
    timeaut = taut;
//  --
    memset( &hints, 0, sizeof(hints) );
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG;
//  --
    if( timeaut > 999 ){
        tv.tv_sec = timeaut / 1000;
        timeaut = timeaut % 1000;
    }else{
        tv.tv_sec = 0;
    }
    tv.tv_usec = timeaut * 1000;
//  --
    transport = -1;
}// End create_transport
//  -------------------------------------------------------------------------
extern int write_transport( unsigned char *buf, int len ){	// передача в устройство
    int i = 0, rez = 0;
//  --
    if( transport == -1 ){	// RECONNECT do MAX_RECONNECT or CONNECT-OK.
        for( i = 0; i < MAX_RECONNECT; i++ ){
            if( open_transport() > 0 ) break;
            sleep( 1 );
        }
    }// transport -1
//  --
    if( transport > 0 ){	// CONNECT-OK
#ifdef SERIAL_ASCII
        fcntl( transport, F_SETFL, 0 );
        rez = write( transport, buf, len );
#else
        rez = send( transport, buf, len, MSG_NOSIGNAL );
#endif
//  --
#ifdef DEBUG
printf( "Передача:\nHEX: ");
if( rez == len ){
    for( i = 0; i < rez; i++ ) printf( " %2x", buf[i] );
    printf( "\nSIM:" );
    for( i = 0; i < rez; i++ ) printf( " %2c", buf[i] );
    printf( "\n" );
} else{
    printf( " -- Error( %d != %d ).\n", rez, len );
}
#endif
//  --
    } else {
        return E_TRANSPORT;
    }
//  --
    if( len != rez ){
        delete_transport();
        return E_TR_WRITE;
    }
//  --
return 0;
}// End write_transport
//  -------------------------------------------------------------------------
extern int read_transport( unsigned char *buf, int len ){	//Чтение из устройства
    int i = 0, rez = 0;
    if( transport < 1 ) return E_TRANSPORT;
//    memset(buffer, 0, sizeof(buffer));
#ifdef SERIAL_ASCII
    unsigned char *bufptr = buf;
    fcntl(transport, F_SETFL, 0);
    while(( rez = read(transport, bufptr, (sizeof(buf) - 1 ))) > 0){
        bufptr += rez;
        if( (bufptr[-1] == '\n' ) | (bufptr-buf == (sizeof(buf) - 1 ))) break;
    }
    *bufptr = '\0';
    rez = bufptr - buf;
#else
    rez = recv( transport, buf, len, 0 );
#endif
//  --
#ifdef DEBUG
printf( "Приём:\nHEX: " );
if( rez > 1 ){
    for( i = 0; i < rez; i++ ) printf( " %2x", buf[i] );
    printf( "\nSIM:" );
    for( i = 0; i < rez; i++ ) printf( " %2c", buf[i] );
    printf( "\n" );
}else{
    printf( " -- Error( %d )!\n", rez );
}
#endif
//  --
    if( rez < 2 ){
        return E_TR_READ;
    }
return rez;
}// End read_transport
//  -------------------------------------------------------------------------
extern void delete_transport( void ){
    if( transport ){
#ifdef SERIAL_ASCII
        fcntl(transport, F_SETFL, 0);
        tcsetattr(transport, TCSAFLUSH, &old_options);
#endif
        close( transport );
    }
    transport = -1;
}// End close_transport().
//  -------------------------------------------------------------------------
