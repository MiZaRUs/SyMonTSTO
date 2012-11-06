/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef Serv_class
#define Serv_class

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
//#include <mysql/mysql.h>
#include <string>
#include <iostream>
//  --
#include "daemon.h"
#include "defs.h"
#include "config.h"
#include "transport.h"
#include "device.h"
#include "dbwriter.h"
//----------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------
class Wheel : public Daemon{
public:
    Wheel(string);
    void Init(void);
    void Run(void);
    void ErrLog(string);

protected:
//private:
//  --
    int maxdev, curdev;
    Config *cfg;
    Device **dd;
    TransPort *trt;
    DBWriter *dbwr;
};
//----------------------------------------------------------------
#endif
