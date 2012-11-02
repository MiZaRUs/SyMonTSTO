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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//  --
#include <memory.h>
#include <assert.h>
//  --
#include "device.h"
//  --
//#define DEBUG
//  -------------------------------------------------------------------------
Device::Device(Config *cfg) : Driver(cfg->getDriver()){
    driv = cfg->getDriver();
    id   = cfg->DeviceId();
    adr  = cfg->DeviceAdr();
    name = cfg->DeviceName();
    param = cfg->DeviceParam();
    time_refresh = 0;
    idata = NULL;
    creg = 1;
    idiap = 100;
    fmodif = 1;
    pause = cfg->getTPause();
//  --
    refresh = &Device::refreshXXX;
    msg.append("Device: ");
    msg.append(name);

// зависит от устройства
    if(name == "TM5132"){
        msg.append(" -Y");
        creg = 4;
        idiap = 70;
        fmodif = 0.01;
        refresh = &Device::refreshTM5132;
    }
//  --
    if(name == "MB110_16D"){
        msg.append(" -Y");
        creg = 16;
        idiap = 1;
        fmodif = 0;
        refresh = &Device::refreshMB110_16D;
    }
//  --
    if(name == "UBZ_301_BO"){
        msg.append(" -Y");
        creg = 13;
        idiap = 100;
        fmodif = 0;
        refresh = &Device::refreshUBZ_301_BO;
    }
//  --
    if(name == "PLC160_AI"){
        msg.append(" -Y");
        creg = 1;
        idiap = 50;
        fmodif = 1;
        refresh = &Device::refreshPLC160_AI;
    }
//----------------------------------------------------
    idata = new int[creg];
    for( int z = 0; z < creg; z++ ) idata[z] = E_ER;
    msg.append(".  OK.");
//  --
//    (*this.*init)();
}// End
//  -------------------------------------------------------------------------
/*************************************************************/
//              Обновление данных
//  -------------------------------------------------------------------------
int Device::Refresh(int cmd, TransPort *tr){
    trp = tr;
    int rez = (*this.*refresh)(cmd);
    time_refresh = time(&time_refresh);
    usleep( pause * 1000); // ждем
return rez;
}// End Refresh
//  -------------------------------------------------------------------------
int Device::refreshXXX(int i){
#ifdef DEBUG
//printf("БУФЕР:%s\n",buf);
//cout << ">Read: ";for(int i=0; i < rc; i++) printf(" %2x", buf[i]);cout << endl;
#endif
return 0;
}// End refreshXXX
//  -------------------------------------------------------------------------
int Device::refreshTM5132(int i){                   // ELEMER TM5132
    unsigned char fr[7];
    int len = sprintf((char*)fr,"6;0;%d;", creg);
//  --
    if(Request( adr, fr, len )) Response(adr);
//printf("RezultRefreshOwen: %d\n", err);
//  --
    if(err){
        setErrData(err);
    }else{
        for( int i = 0; i < creg; i++) idata[i] = getDanTM5132(i);
    }
    return err;
} // End
//  -------------------------------------------------------------------------
int Device::refreshMB110_16D(int i){ //MB110_16D
    unsigned char cmd[5];
    int cmd_len = 0;        //if(driv == "DCON") 
    if(driv == "OWEN"){
        cmd[0] = 0;          // внутренний адресс 4-бита
        cmd[0] |= 0x10;    //признак запроса
        cmd[0] |= 0;         // dataSize;
//    hash = 0x2D26;   // 0x2D26 - "r.Cn" | 0x2517 - "r.Cou" 
        cmd[1] = 0x2D;    //2d (hash >> 8) & 0xff;
        cmd[2] = 0x26;    //26  hash & 0xff;
//	if (dataSize)memcpy(frame+4, data, dataSize); - данные не передаём
        cmd_len = 3; 
    }// end owen
//  --
    if(( driv == "MB_ASCII" ) || ( driv == "MB_RTU" )){
        cmd[0] = 0x3;  //Func 03|04;
        cmd[1] = 0x0;      // hi reg;
        cmd[2] = 0x33;    // lo reg
        cmd[3] = 0;         // hi;
        cmd[4] = 1;         // lo;
        cmd_len = 5;
    }// end mb...
//  --
    if(Request( adr, cmd, cmd_len )) Response(adr);
//printf("RezultRefreshOwen: %d\n", err);
//  --
    if(err){
        setErrData(err);
        return err;
    }
    if(buf_len != 0x2){
        err = E_DN;   // Количество данных !!!!!!
        setErrData(err);
        return err;
    }
    if(!err){
        unsigned short x = buf[0];
        x = (x << 8) | buf[1];
        if(driv != "DCON") x = 0xffff ^ x; // если не DCON то инвертируем
//printf("Rez: %x\n", x);
        for(int i = 0; i < creg; i++){
            if(x & 0x0001){
                idata[i] = 0;
            } else {
                idata[i] = 1;
            }
            x = x >> 1;
        }
    }
    return err;
} // End
//  -------------------------------------------------------------------------
int Device::refreshUBZ_301_BO(int i){ //BO_01_MB_RTU - блок обмена ОБ-01 для УБЗ-301 
    unsigned char cmd[6];
    int cmd_len = 0;        //if(driv == "DCON") 
    cmd[0] = adr;     // ADR
//  --
    cmd[1] = 0x4;    // чтение: 03-рег. хранения(настроечные парам.) | 04-входные рег.(измерения)
//  --
    cmd[2] = 0x0;    //hi reg
    cmd[3] = 0x0;    //lo reg
//  --
    cmd[4] = 0;       //hi  количество
    cmd[5] = creg;   //lo количество двухбайтных данных
//  --
    if(Request( adr, cmd, cmd_len )) Response(adr);
//printf("RezultRefreshOwen: %d\n", err);
//  --
    if(err){
        setErrData(err);
        return err;
    }
#ifdef DEBUG
cout << "CMD_HEX:"; for(int i=0; i < cmd_len; i++) printf(" %2x", cmd[i]);
#endif
return 0;
} // End
//  -------------------------------------------------------------------------
int Device::refreshPLC160_AI(int i){
    unsigned char cmd[5];
    int cmd_len = 0;        //if(driv == "DCON") 
    cmd[0] = 0;          // внутренний адресс 4-бита
    cmd[0] |= 0x10;    //признак запроса
    cmd[0] |= 0;         // dataSize;
//    hash = 0x2D26;   // 0x2D26 - "r.Cn" | 0x2517 - "r.Cou" 
    cmd[1] = 0xFA;    //2d (hash >> 8) & 0xff;
    cmd[2] = 0x08;    //26  hash & 0xff;
//	if (dataSize)memcpy(frame+4, data, dataSize); - данные не передаём
    cmd_len = 3; 
//  --
//printf("OwenAdr: %x\n", adr);
    if(Request( adr, cmd, cmd_len )) Response(adr);
//printf("ErrorRefreshOwen: %d\n", err);
//  --
    if(err){
        setErrData(err);
    }else{
#ifdef DEBUG
//cout << "DeviceOwenSZ = " << buf_len << "  " << endl;
//for(int i=0; i < buf_len; i++) printf(" %2x, ", buf[i]);
//cout << endl;
#endif
        unsigned int x = buf[0];
        for( int a=1; a < buf_len; a++){
            x = (x << 8) | buf[a];
            if(a > 4)break;
        }
        idata[0] = x; // /100
#ifdef DEBUG
printf("DeviceOwenD: %d\n", x);
#endif
    }
    return err;
} // End
//  -------------------------------------------------------------------------


/*************************************************************/
//              Вспомогательные функции
//  -------------------------------------------------------------------------
int Device::getDanTM5132(int poz){
    int xss = E_DN;
    char st[7];
    st[0] = 'x';
    st[1] = '\0';

    int i, j;
    for(i = 0, j = 0; i <= buf_len; i++){
        if(buf[i]==';') j++;
        if(j == poz)break;
    }
    i++;
    if(poz == 0) i =0;
    if( i >= buf_len )return E_DN;

    j = 0;
    for(;((buf[i] >= '0' )&&(buf[i] <= '9')) || (buf[i] == '.') || (buf[i] == '$') || (buf[i] == '-'); i++, j++){
        if( i >= buf_len )return E_DN;
        st[j] = buf[i];
//cout << st << " ";
    }
    st[j] = '\0';
//cout << "  " << st << endl;
    if((st[0] >= '0')&&((st[0] <= '9')||(st[0] == '-' ))) xss = (int)(atof(st) * 100);
    if(st[0] == '$'){
        if(st[1] == '0')xss = E_ZER; //неисправен 
        if(st[1] == '1')xss = E_KZ; 
        if(st[1] == '3')xss = E_NO;  //нет канала
        if(st[1] == '6')xss = E_WNO; //недоступны
        if(st[1] == '8')xss = E_OBR;
        if(st[1] == '9')xss = E_ZER; //неисправен
    }
    return xss;
};// End getDanTM5132
//  -------------------------------------------------------------------------
/*************************************************************/
/*char Device::getParam(int poz){
    if(poz < 1)return ';';
    int i, j = 0;
    for(i = 0; i <= (int)param.size(); i++){
        if(param[i]==';') j++;
        if(j == poz)break;
    }
    if( i >= (int)param.size() )return ';';
    return param[i-1];
}// End getParam */
//  -------------------------------------------------------------------------
/*************************************************************/
void Device::setErrData(int i){
    int z = 0;
    while( z < creg){
        idata[z] = i;
        z++;
    }
}// End setErrData
//  -------------------------------------------------------------------------
/*************************************************************/
Device::~Device(){
    delete[] idata;
}// End
/*************************************************************/
