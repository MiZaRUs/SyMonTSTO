/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef Driver_class
#define Driver_class

#include <string>
//  --
#include "transport.h"
#include "defs.h"
//----------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------
class Driver{
//  --
public:
    Driver(string drv);// "OWEN"|"ELEMER"|"DCON"|"MODBUS_RTU"|"MODBUS_ASCII"
    ~Driver();
//  --
    bool Request(unsigned char adr,unsigned char *data, int len);	// запрос
    bool Response(unsigned char adr);				// ответ
    string getMsg(void){return msg;};
//  --------------------------------------------------------
private:
    int (Driver::*request)(unsigned char adr,unsigned char *data, int len);
    int (Driver::*unpak)(unsigned char adr);
//  --
    int requestELEMER(unsigned char adr,unsigned char *data, int len);	// Элемер
    int requestDCON(unsigned char adr,unsigned char *data, int len);	// DCON_@
    int requestOWEN(unsigned char adr,unsigned char *data, int len);	// Овен
    int requestMB_RTU(unsigned char adr,unsigned char *data, int len);	// ModBus-rtu
    int requestMB_ASC(unsigned char adr,unsigned char *data, int len);	// ModBus-ascii
    int requestXXX(unsigned char adr,unsigned char *data, int len);	// посылает data[] как есть
//  --
    int unpakELEMER(unsigned char adr);
    int unpakDCON(unsigned char adr);
    int unpakOWEN(unsigned char adr);
    int unpakMB_RTU(unsigned char adr);
    int unpakMB_ASC(unsigned char adr);
    int unpakXXX(unsigned char adr);	//пустышка
//  --
protected:
    TransPort *trp;
//  --
    int err;
    string msg;
    int buf_len;
    unsigned char buf[MAXBUF];
//  --
    char char2hex(char c);
    char hex2char(char c);
// --
    unsigned short ksumOwen(unsigned char* buf, int len);
    unsigned short ksumCRC(unsigned char *buf, int len);
    unsigned char ksumLRC(unsigned char *buf, int len);
    unsigned char ksumDCON(unsigned char *buf, int len);
};
//----------------------------------------------------------------
#endif
