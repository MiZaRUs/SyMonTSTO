/**************************************************************************
 *  emoxa                                                                 *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef ServerSocket_class
#define ServerSocket_class

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string>
//  ---------------------------------
const int MAXCONNECTIONS = 3;
//  --
using namespace std;
//  ---------------------------------
class ServerSocket{
//private:
    string msg;
    int sock;
    sockaddr_in address;// size_t addrLength = sizeof(address);
    addrinfo hints, *addr;

public:
    ServerSocket(int port);
    ServerSocket(){};
    ~ServerSocket();
    int getHd();
};
#endif
//  ---------------------------------
