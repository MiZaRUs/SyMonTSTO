/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
//  --
#include "transport.h"
//  --
#define DEBUG
//  --
/**************************************************************************/
TransPort::TransPort(string host, int port, int timeaut){
    msg = "TransPort: ";
    hd = openSocket(host, port, timeaut );
//  --
    if(hd < 1){
        msg.append(host);
        msg.append(":");
        char prt[7];
        sprintf(prt,"%d",port);
        msg.append(prt);
        msg.append(") - failed.\n");
    } else {
        msg.append("-- OK.\n");
    }
//  --
}// End TransPort
/**************************************************************************/
int TransPort::openSocket(string host, int port, int msec){
    int sock = -1;
    struct sockaddr_in *addrInfo;
    struct addrinfo hints, *addr;
    struct timeval tv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG;
    if(msec > 999){
        tv.tv_sec = msec / 1000;
        msec = msec % 1000;
    }else{
        tv.tv_sec = 0;
    }
    tv.tv_usec = msec * 1000;
//  --
    if(getaddrinfo(host.c_str(), NULL, &hints, &addr)){
        msg.append("getaddrinfo(");
        return -1;
    }
    addrInfo = (struct sockaddr_in *) addr->ai_addr;
    if((sock = socket(addrInfo->sin_family, addr->ai_socktype, addr->ai_protocol)) < 0){
        msg.append("socket(");
        return -1;
    }
//  --
    if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1){
        msg.append("setsockopt(");
        return -1;
    }
//  --
    addrInfo->sin_port = htons(port);
    if(connect(sock, (struct sockaddr *) addrInfo, addr->ai_addrlen) < 0){
        msg.append("connect(");
        return -1;
    }
//  --
    freeaddrinfo(addr);
    return sock;
}// End openSocket()
//  -------------------------------------------------------------------------
//Запись в устройство
int TransPort::Write(unsigned char *buf, int size ){
    if(hd < 1) return -1;
    if((buf == 0) || (size < 1) || (size > MAXBUF))return 0;
    int rez = send(hd, buf, size, MSG_NOSIGNAL);
#ifdef DEBUG
cout << endl << "WrireSocket:\nHEX: ";
if(rez == size){
    for(int i=0; i < rez; i++) printf(" %2x", buf[i]);
    cout << endl << "SIM:";
    for(int i=0; i < rez; i++) printf(" %2c", buf[i]);
    cout << endl;
}else{
    cout << " -- Error( " << rez << " != " << size << " )." << endl; // ПЕРЕПОДКЛЮЧИТЬ !!!
}
#endif
    return rez;
}// End Write
//  -------------------------------------------------------------------------
//Чтение из устройства
int TransPort::Read(unsigned char *buf, int size ){
    if(hd < 1) return -1;
    if((buf == 0) || (size < 1) || (size > MAXBUF)) return 0;
    int rez = recv(hd, buf, size, 0);
#ifdef DEBUG
cout << endl << "ReadSocket:\nHEX: ";
if(rez > 1){
    for(int i=0; i < rez; i++) printf(" %2x", buf[i]);
    cout << endl << "SIM:";
    for(int i=0; i < rez; i++) printf(" %2c", buf[i]);
    cout << endl;
}else{
    cout << " -- Error( " << rez << " )!" << endl; // После 50 циклов можно перегрузить ?
}
#endif
    return rez;
}// End Read
//  -------------------------------------------------------------------------
TransPort::~TransPort(){
    if(hd > 0)close(hd);
    hd = -1;
}// End ~TransPort().
/**************************************************************************/
