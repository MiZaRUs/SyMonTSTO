/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <iostream>
//#include <fcntl.h>
//#include <stdio.h>
#include <errno.h>
//  --
#include "wheel.h"
#include "device.h"
#include "config.h"
#include "dbwriter.h"
//  --
#define DEBUG
//  ---------------------------------
using namespace std;
//  -------------------------------------------------------------------------
Wheel::Wheel(string nameserv) : Daemon(nameserv){
//    string nameserv = "serv_s"; // Уникальное имя сервиса		VARCHAR(32)
#ifdef DEBUG
cout << "\nWheel::Wheel(" << nameserv << ")           *******************************************************************\n\n";
#endif
    maxdev = 0;
    curdev = -1;
    trt = NULL;

// Получаем конфигурацию для данного сервера ( параметров сети, оборудования и имена таблиц )
    cfg = new Config(nameserv); // при неудаче исключение FError(msg)

#ifdef DEBUG
cout << "Wheel >> NetConf: " << cfg->getHost() << "  PS: " << cfg->getPort() << "  TM: " << cfg->getTimeAut() << endl;
#endif
}// End Wheel
//**************************************************//
void Wheel::Init(void){
/* Инициализация основных объектов */
    syslog(LOG_LOCAL0|LOG_INFO, "Init.");

// Подключение
#ifdef DEBUG
cout << "Connect...\n";
#endif
    int cn = 10; // попытки
    do{
        if(trt) delete trt;
        trt = new TransPort(cfg->getHost(), cfg->getPort(), cfg->getTimeAut() );
        if(trt->status() > 0) break;
#ifdef DEBUG
cout << "WARNING - " << cn << " : " << trt->getMsg();
#endif
        syslog(LOG_LOCAL0|LOG_INFO, "WARNING: Init: %s", trt->getMsg().c_str());
        sleep(2);
        if(!--cn) throw (string)trt->getMsg();
    } while(trt->status() < 1);
//-------------------------------------



// Выделяем память для указателей на виртуальные устройства
    maxdev = cfg->maxDevice();
#ifdef DEBUG
cout << "MaxDev: " << maxdev << endl;
#endif
    dd = new Device* [maxdev];
    if(!dd) throw (string)"Serv >> Memory - failed";
//-------------------------------------



// Инициализируем виртуальные устройства
#ifdef DEBUG
cout << "Init Device...\n";
#endif
//    curdev = 0;   dd[curdev] = new Device( cfg, trt );      //          !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for(curdev = 0;cfg->Next(); curdev++){
         dd[curdev] = new Device( cfg, trt );
#ifdef DEBUG
cout << dd[curdev]->getMsg() << endl;
#endif
    }
//-------------------------------------



// Инициализируем "самописец"
#ifdef DEBUG
cout << "Init DBWriter...\n";
#endif
    dbwr = new DBWriter(cfg);
#ifdef DEBUG
cout << dbwr->getMsg() << endl;
#endif
}// End Init
//  -------------------------------------------------------------------------



/***************************************************************************/
/* Запуск */
void Wheel::Run(void){// Цикл опроса
#ifdef DEBUG
cout << "\n\tWheel::Run()\n\n";
//int pid = 7;	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#endif
    syslog(LOG_LOCAL0|LOG_INFO, "Run.");

//    int timclear = 3600 * 27;
    while(pid){
#ifdef DEBUG
cout << "New" << endl;
#endif
        int rez = 0;
        for( curdev = 0; curdev < maxdev; curdev++){
            rez = dd[curdev]->Refresh(0);
//cout << endl << "MainDevRefreshRez: " << rez << endl;
//-------------------------------------
//            if((rez == E_RCV)||(rez == E_SND)){// Через 10 минут Повторное Подключение
            if(rez == E_SOCKET){// Повторное Подключение
#ifdef DEBUG
cout << "ReConnect..." << endl;
#endif
                int cn = 20000; // попытки
                do{
                    if(trt) delete trt;
                    trt = new TransPort(cfg->getHost(), cfg->getPort(), cfg->getTimeAut());
                    if(trt->status() > 0) break;
                    string mess = trt->getMsg();

#ifdef DEBUG
cout << "WARNING - " << cn << " : " << mess << " - " << endl;
#endif
                    syslog(LOG_LOCAL0|LOG_INFO, "WARNING: Reconnect: %s", mess.c_str());
                    sleep(10);
                    if(!--cn) throw (string)mess;
                } while(trt->status() < 1);
#ifdef DEBUG
cout << "OK." << endl;
#endif
            }
//-------------------------------------

            if(!dbwr->Write(dd[curdev])){
                syslog(LOG_LOCAL0|LOG_INFO, "ERROR: DBWriter: %s", dbwr->getMsg().c_str());
            }
//-------------------------------------
        } // for
//-------------------------------------
//	if(timclear >= (3600 * 24)){
//            if(!dbwr->oldClear()){
//cout << "Error." << dbwr->getMsg() << endl;
//                syslog(LOG_LOCAL0|LOG_ERR, "ERROR: %s", dbwr->getMsg().c_str());
//            }
//	timclear = 0;
//	}// if timclear
//    timclear++;
#ifdef DEBUG
cout << "End. Ждем: " << (cfg->getTCicle() / 1000 )<< " сек.\n";
#endif
//    sleep(cfg->getTCicle()); //  ждем в секундах
    usleep( cfg->getTCicle() * 1000); // ждем милисекундах
    }// while
}// End Run

/***************************************************************************/
void Wheel::ErrLog(string st){
    syslog(LOG_LOCAL0|LOG_INFO,"ERROR: %s", st.c_str());
#ifdef DEBUG
    cout << "ERROR: " << st.c_str() << endl;
#endif
}
/***************************************************************************/




