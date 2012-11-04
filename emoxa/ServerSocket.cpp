/**************************************************************************
 *  emoxa                                                                 *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
//  --
#include "ServerSocket.h"
//  --------------------------------------------
ServerSocket::ServerSocket ( int port ){
    msg = "SOCKET: ";
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
    char st[6];
    sprintf(st, "%d", port);
    if(getaddrinfo(NULL, st, &hints, &addr)){
        msg.append("getaddrinfo() failed.");
        throw msg;
    }
    if((sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)) < 0){
        msg.append("socket() failed.");
        throw msg;
    }
//  --
    int on = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1){
        msg.append("setsockopt() failed.");
        throw msg;
    }
//  --
    if(bind(sock, addr->ai_addr, addr->ai_addrlen)){
        msg.append("bind() failed.");
        throw msg;
    }
//  --
    freeaddrinfo(addr);
//  --
    if(listen(sock, MAXCONNECTIONS)){
        msg.append("listen() failed.");
        throw msg;
    }
}// End ServerSocket()
//  ---------------------------------
ServerSocket::~ServerSocket(){
    if(sock > 0 ) close(sock);
}// End ~ServerSocket()
//  ---------------------------------
int ServerSocket::getHd(){
    if(sock < 1 ){
        msg.append("Socket failed");
        throw msg;
    }
    int addrLen = sizeof(address);
    return accept(sock, (struct sockaddr *) &address, (socklen_t *)&addrLen);
}// End getHd()
//**************************************************//
