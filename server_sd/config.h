/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef Config_class
#define Config_class

#include <mysql/mysql.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
//  --
#include "defs.h"
//----------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------
class Config{
public:
    Config(string name_server);
    ~Config();

    string getMsg(void){ return msg;};		// сообщения
    int getTCicle(void){ return tcicle;};	// пауза между опросами

// For TransPort
    string getHost(void){ return host;};	// localhost | 192.168.2.2
    int getPort(void){ return tcpport;};	// TCP-рорт преобразователя интерфейса

    int getTimeAut(void){ return timeaut;};
    int getTPause(void){ return tpause;};	// завершающая пауза (msec)

// For Device
    bool Next(void);
    string getDriver(){return driver;};		// "ELEMER"|"DCON"|"OWEN"|"MODBUS_RTU"|"MODBUS_ASCII"
    string DeviceName(){return name;};		// имя устройства сбора данных
    string DeviceParam(){return param;};	// параметры устройства
    string DeviceFormat(){return format;};	// формат данных
    int DeviceId(void){ return id;};
    unsigned char DeviceAdr(void){ return adr;};
    int maxDevice(void){ return maxdev;};
//  --
//    string getDBAddr(void){ return dbadr;};	// URL базы данных
//    string getDBName(void){ return dbname;};	// имя базы данных
//    string getDBDriv(void){ return dbdriv;};	// драйвер базы данных
//    string getTblDevice(void){ return tbl_dev;};
    string getTblData(void){ return tbl_dat;};
    string getTblArch(void){ return tbl_arc;};
//  --
private:
    string msg;
    MYSQL_RES *res;
//  --
    string nameserv;	// Уникальное имя сервиса			VARCHAR(24)
    int tcicle;		// задержка цикла, пауза между опросами

// For TransPort
    string host;	// 192.168.2.20	| localhost		VARCHAR(32)
    int tcpport;	// TCP-порт если = 0 то  EXIT
//    string remotedev;	// удаленное сетевое TCP-устройство (MOXA...)	VARCHAR(24)
    string driver;	// драйвер обмена (ELEMER | DCON | OWEN | MODBUS_RTU..)	VARCHAR(24)
    int timeaut;	// time-aut приема
    int tpause;		// завершающая пауза (msec)

// For Device
    int id;		// идентификатор
    unsigned char adr;	// сетевой адресс. при 0 исключается из опроса.
    unsigned char reg;	// внутренний адресс.  при 0 исключается из опроса.
    string name;	// название устройства					VARCHAR(24)
    string param;	// параметры устройства					VARCHAR(24)
    string format;	// формат данных 					VARCHAR(24)
    int maxdev;
// End For Device

// For DBWriter
//    string dbadr;	// URL базы данных
//    string dbname;	// имя базы данных
//    string dbdriv;	// драйвер базы данных
//    string tbl_dev;	// таблица устройств					VARCHAR(24)
    string tbl_dat;	// таблица текущих данных				VARCHAR(24)
    string tbl_arc;	// таблица арнивных данных				VARCHAR(24)
//  --
    bool loadNetConf(void);
    bool loadDevConf(void);
};
//----------------------------------------------------------------
#endif
