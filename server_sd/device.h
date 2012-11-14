/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef Device_class
#define Device_class

#include <time.h>
#include <string>
//  --
#include "driver.h"
#include "config.h"
#include "transport.h"
#include "defs.h"
//----------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------
class Device : Driver{	// виртуальная копия устройства
    int id;		// идентификатор устройства
    int adr;		// адрес устройства в сети
    string name;	// имя устройства
    string param;	// параметры устройства
    string form;	// формат запроса
//  --
    int creg;		// количество регистров в устройстве
    time_t time_refresh;// время обновления данных
    int    *idata;	// массив данных в формате b, i, или f *100
    int    idiap;	// диапазон
    float  fmodif;	// модификотор
    int    pause;
    int    count;	// подсчет циклов обновления
//  --
    string driv;
//  --
public:
    Device(Config *cfg);
    ~Device();
    int Refresh(int c, TransPort *t);	// Обновление данных(эапрос-ответ), возвращяет код ошибки
    string getMsg(void){return msg;};
//  --
    int getID(void){return id;};	// возвращает идентификатор устройства
    int getAdr(void){return adr;};	// возвращает адрес устройства
    int getCount(void){return creg;};	// возвращает количество регистров устройства
    int getData(int rg = 0){return idata[rg];};	// возвращает данные
    int getTime(void){ return (int)time_refresh;};// возвращает unix-время обновления
//  --
    int getDiap(int rg = 0){return idiap;};	// возвращает предел изменений данных
    float getModif(int rg = 0){return fmodif;};	// возвращает предел изменений данных
//  --
//protected:
private:
    int (Device::*refresh)(int i);        // обновление
//  ------------------------------------------------------------------------
    int refreshXXX(int i);
    int refreshTRM101(int i);	// прибор ТРМ101 - Овен.
    int refreshTRM138(int i);	// прибор ТРМ138 - Овен.
    int refreshNORD_Z3M(int i); // блок электронный НОРД-Э3М
    int refreshTM5132(int i); // ELEMER
    int refreshMB110_16D(int i);
    int refreshUBZ_301_BO(int i); // блок обмена БО-01 для УБЗ-301 (универс.блок защиты)
//    int refreshPLC160_DI(int i); // чтение дискретных данных - 4 байта
    int refreshPLC160_AI(int i); // чтение аналоговых данных
//  --

// Вспомогательные функции
    int getDanTM5132(int poz);
//  ----------------------
//    char getParam(int poz);
    void setErrData(int);
};
//  ----------------------------------------------------------------
#endif
