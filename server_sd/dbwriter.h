/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef DBWriter_class
#define DBWriter_class

#include <mysql/mysql.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <time.h>
//  --
#include "defs.h"
#include "config.h"
#include "device.h"
//----------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------
class DBWriter{
public:
    DBWriter(Config *cfg);
    ~DBWriter();

    bool Write(Device *d);
//    int status(void){return Err;};
    string getMsg(void){return msg;};
    bool oldClear(void);

protected:
//    int Err;
    string msg;
    string data_name;
    string arch_name;
    int curd;
    MYSQL mysql_rw;
//  --
    int test(Device *d);
    bool write(Device *d);
    bool new_wr(Device *d);
    bool arc_wr(Device *d);
};
//----------------------------------------------------------------
#endif
