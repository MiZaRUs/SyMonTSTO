/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef TransPort_class
#define TransPort_class
//  -------------------
#include <sys/socket.h>
#include <netdb.h>
#include <string>
//  -------------------
#include "defs.h"
//  -------------------
using namespace std;
//----------------------------------------------------------------
class TransPort{
    string msg;
    int hd;

public:
    TransPort(string host, int port, int timeaut);
    ~TransPort();
//  --
// Передача данных в физическое устройство: возвращает количество переданных байт
    int Write(unsigned char *buf, int size);	// послать - Write()

// Прием данных из физического устройства: возвращает количество полученных байт
    int Read(unsigned char *buf, int size);	// принять - Read() окончание EOF или таймаут

    string getMsg(void){ return msg;};
    int status(void){ return hd;};
//  --
private:
// Корректное закрытие для деструктора
    void Close(void);

// проверка удалённого сетевого преобразователя
//int initMoxaNPort(int);

// Инициализация транспортного уровня:  возвращает файловый дескриптор (int msec - таймаут)
    int openSocket(string host, int port, int msec);
};
//----------------------------------------------------------------
#endif
