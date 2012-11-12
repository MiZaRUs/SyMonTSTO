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
//#include <errno.h>
//  --
#include "config.h"
//  --
#define D_TEST
//#define DEBUG
//  -------------------------------------------------------------------------
#ifdef D_TEST
    int MD = 0;
#endif
//using namespace std;
//  -------------------------------------------------------------------------
Config::Config(string sname){
    msg = "Config";
    nameserv = sname;		// Уникальное имя сервиса	VARCHAR(32)
    tcicle = 4000;		// задержка цикла в mсекундах, пауза между опросами

// For TransPort
 // IP-адрес
    host  = "127.0.0.1";     // localhost | 192.168.0.10 | 192.168.2.22	VARCHAR(32)
// TCP-порт
    tcpport = 4001;		// Если = 0 то EXIT иначе SOCKET ( 950 )

// драйвер обмена ( "ELEMER" | "DCON" | "OWEN" | "MB_RTU" | "MB_ASC" | "MB_ASC_B" )
    driver = "MB_ASC_B";	//протокол обмена
//    driver = "OWEN";	//протокол обмена

    timeaut = 500;
    tpause = 200;		// завершающая пауза в милисек


// For Device
    id = 1;			// идентификатор
    adr = 3;			// сетевой адресс
//    name = "PLC160_AI";		// название устройства "TM5132" "MB110_16D" "UBZ_301_BO" "NORD_Z3M"
    name = "NORD_Z3M";
    param = "";		// параметры устройства
    format = "";		// формат данных
//  --
    reg = 4;
    maxdev = 1;
// End For Device
// --
    tbl_dat = "data";
    tbl_arc = "arch_x";
//  --
#ifndef D_TEST
//  Загрузка параметров сети
   if(!loadNetConf()) throw (string)msg;
// Загрузка параметров оборудования
   if(!loadDevConf()) throw (string)msg;
#else
    MD = maxdev;
#endif
}// End
//  -------------------------------------------------------------------------
// загрузка из локальной БД 
bool Config::loadNetConf(void){
    MYSQL mysql;
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql, BDHOST, BDUSER, BDPASS, BDNAME, 0,NULL,0)){
        msg.append(" MySQL: ");
        msg.append(mysql_error(&mysql));
        return false;
    }
//  --                                0        1        2         3       4          5          6        7        8
    string qu = "SELECT names,host,tcpport,tcicle,timeaut,tpause,tbldata,tblarch FROM ";
    qu.append(TBLCNF);
    qu.append(" WHERE tcpport > 0 AND names = '");
    qu.append(nameserv);
    qu.append("'");
    if(mysql_query(&mysql, qu.c_str())){
        msg.append(" MySQL: ");
        msg.append(mysql_error(&mysql));
        mysql_close(&mysql);
        return false;
    }
//  --
#ifdef DEBUG
cout << "> MySql(" << BDHOST << " : " << BDNAME << "." << nameserv << ")." << endl;
#endif
    res = mysql_store_result(&mysql);
    if(!res){
        msg.append(" MySQL: ");
        msg.append(mysql_error(&mysql));
        mysql_close(&mysql);
        return false;
    }
//  --
    MYSQL_ROW row = mysql_fetch_row(res);
    int num = mysql_num_rows(res);
    if(num == 1){
        nameserv= row[0];
        host  = row[1];
        tcpport  = atoi(row[2]);
        tcicle  = atoi(row[3]);
        timeaut = atoi(row[4]);
        tpause  = atoi(row[5]);
        tbl_dat = row[6];
        tbl_arc = row[7];
//  --
    }else{
        tcpport = 0;
        msg.append(" for ");
        msg.append(nameserv);
        msg.append(" Reader: 0 OR > 1 - records.");
        mysql_free_result(res);
        mysql_close(&mysql);
        return false;
    }
    mysql_free_result(res);
    mysql_close(&mysql);
    msg.append(" Ok.");
    return true;
}// End
//  -------------------------------------------------------------------------
bool Config::loadDevConf(void){
#ifdef DEBUG
cout << "DevWheel::readConf()\n";
#endif
    MYSQL mysql;
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql, BDHOST, BDUSER, BDPASS, BDNAME, 0,NULL,0)){
        msg.append(" MySQL: ");
        msg.append(mysql_error(&mysql));
        return false;
    }
//  --                              0   1     2        3        4     5         6
    string qu = "SELECT id, adr, driver, name, reg, param, format FROM ";
    qu.append(TBLDEV);
    qu.append(" WHERE adr > 0 AND reg > 0 AND sname = '");
    qu.append(nameserv);
    qu.append("'");

    if(mysql_query(&mysql, qu.c_str())){
        msg.append(" MySQL: ");
        msg.append(mysql_error(&mysql));
        mysql_close(&mysql);
        throw (string)msg;
        return false;
    }
//  --
    res = mysql_store_result(&mysql);
    if(!res){
        msg.append(" MySQL: ");
        msg.append(mysql_error(&mysql));
        mysql_close(&mysql);
        throw (string)msg;
        return false;
    }
//  --
    int num = mysql_num_rows(res);
#ifdef DEBUG
cout << ">>>  "<< num << "  <<<\n";
#endif
    if(num < 1 || num > 255){
        msg = " for ";
        msg.append(TBLDEV);
        msg.append(" Reader: 0 OR > 255 records.");
        maxdev = 0;
        return false;
    }
    maxdev = num;
//    mysql_free_result(res);
    mysql_close(&mysql);
    return true;
}// End
//  -------------------------------------------------------------------------
bool Config::Next(void){
#ifdef D_TEST
    if(MD < 1)return false;
    MD--;
    return true;
#endif
if(maxdev < 1)return false;
    MYSQL_ROW row;
    if((row = mysql_fetch_row(res))){
#ifdef DEBUG
cout << "DBNext: "<< row[0] << "  " << row[1] << "  " << row[2] << "  " << row[3] << "  " << row[4] << "  " << row[5] << "  " << row[6] << endl;
#endif
        id = atoi(row[0]);
        adr = atoi(row[1]);
        driver  = row[2];
        name =row[3];
        reg = atoi(row[4]);
        param =row[5];
        format =row[6];
    }else{
        id = 0;
        adr = 0;
        driver  = "";
        reg = 0;
        name = "",
        param = "",
        format = "";
        maxdev = 0;
        mysql_free_result(res);
        return false;
    }
    return true;
}// End 
//  -------------------------------------------------------------------------
Config::~Config(){}// End
//  -------------------------------------------------------------------------
/***************************************************************************/
