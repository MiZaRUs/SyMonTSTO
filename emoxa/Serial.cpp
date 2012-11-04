/**************************************************************************
 *  emoxa                                                                 *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <iostream>
#include <unistd.h>
//  --
#include "Serial.h"
//  --
//скорость обмена            - 9600 бит/сек (может изменяться);
//информационная длина слова - 8 бит;
//бит паритета               - нет;
//кол-во стоп-битов          - 1.
// TIME_AUT = 0.5 sec
//  -------------------------------------------------------------------------
Serial::Serial(string devnet){
    msg = "SERIAL: ";
    if((serial = open(devnet.c_str(), O_RDWR | O_NOCTTY | O_NDELAY)) < 0){
        msg.append("open() failed.");
        throw msg;
    }
    if(tcgetattr(serial, &options)!=0){
        msg.append("tcgetattr() failed.");
        throw msg;
    }
    old_options = options;
    if(!setRS(9600, 500)){
        msg.append("setRS() failed.");
        throw msg;
    }
}// End Serial
//  -------------------------------------------------------------------------
bool Serial::setRS(int speed, int vtim){
// Set speed
    speed_t tspd = B9600;
    switch(speed){
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
    options.c_cflag |= (CLOCAL | CREAD); //enable receiver & set local mode
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //row mode
    options.c_iflag &= ~(IXON | IXOFF | IXANY); //disable software flow control
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = vtim / 100; //time-out 0,4 sek

    if(tcsetattr(serial, TCSAFLUSH, &options) != 0)return false;
return true;
}// End setRS
//  -------------------------------------------------------------------------
int Serial::getHd(){
    fcntl(serial, F_SETFL, 0);
    return serial;
}// End ~Serial().
//  -------------------------------------------------------------------------
Serial::~Serial(){
    fcntl(serial, F_SETFL, 0);
    tcsetattr(serial, TCSAFLUSH, &old_options);
    if(serial > 0) close(serial);
}// End ~Serial().
//**************************************************//
