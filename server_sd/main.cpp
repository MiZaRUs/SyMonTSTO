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
#include <string>
#include <signal.h>
#include <string.h>
#include <syslog.h>
//  --
#include "wheel.h"
#include "defs.h"

#define DEBUG

static Wheel *serv;
#ifndef DEBUG
static void clean_exit(int sig);
#endif
//using namespace std;
/***************************************************************************/
int main(int argc, char *argv[]){
    string d_name = (strrchr(argv[0], '/') + 1);

/* Инициализация основных объектов */
try{
    if(d_name.length() > MAXNAME) throw (string)"Main:d_name.length()!";

#ifdef DEBUG
    serv = new Wheel("s_vds5_tm"); // (perent Daemon)
    serv->Command("start");
#else
    serv = new Wheel(d_name); // (perent Daemon)
    serv->Command((argc > 1) ? argv[1] : "");
#endif

/* Инициализация основных объектов */
    serv->Init();

#ifndef DEBUG
/* Настройка системных сигналов */
    signal(SIGINT, clean_exit);
    signal(SIGUSR1, clean_exit);
    signal(SIGQUIT, clean_exit);
    signal(SIGTERM, clean_exit);
    signal(SIGPIPE, SIG_IGN);
#endif

/* Ответвляемся и завершаем родительский процесс */
    serv->Daemonize();

/* Запуск */
    serv->Run();
//  --

//  -------------------------------------------------------------------------
    }catch( string e){
#ifdef DEBUG
cout << "FATAL_ERROR " << e << endl << "Exit." << endl;
#endif
    syslog(LOG_LOCAL0|LOG_INFO|LOG_ERR, "FATAL_ERROR: %s", e.c_str());
//    syslog(LOG_LOCAL0|LOG_ERR, "FATAL_ERROR: %s", e);
    closelog();
    }
return EXIT_FAILURE;
}// End main ----------------------------------------------------------------
/***************************************************************************/

// Обработчики сигналов
#ifndef DEBUG
static void clean_exit(int sig){
    serv->Exit(sig);
    exit(EXIT_SUCCESS);
}// End clean_exit
#endif
/***************************************************************************/
