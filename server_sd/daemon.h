/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef Daemon_class
#define Daemon_class

#include <unistd.h>
#include <stdlib.h>
#include <string>
using namespace std;
//----------------------------------------------------------------
static string pidputh = "/var/run/";
//----------------------------------------------------------------
class Daemon{
public:
    Daemon(string);
    ~Daemon();
    void Command(string);
//    void Init(void);
    void Daemonize(void);
    void Run(void);
    void Exit(int);
//    pid_t getPid(void){return pid;};

protected:
//private:
    int fd;
    pid_t fid, pid;
    string d_name;
    string pidfile;

    bool createPidFile(void);
    pid_t readPidFile(void);
};
//----------------------------------------------------------------
#endif
