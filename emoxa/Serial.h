/**************************************************************************
 *  emoxa                                                                 *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef Serial_class
#define Serial_class

#include <fcntl.h>
#include <termios.h>
#include <string>
//----------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------
class Serial{
    string msg;
    int serial;
    struct termios options;
    struct termios old_options;
//  --
public:
    Serial(string dn);
    ~Serial();
    int getHd();
    bool setRS(int speed, int vtim);// vtim * 100
    bool reset(){return fcntl(serial, F_SETFL, 0);}
//  --
};
//----------------------------------------------------------------
#endif
