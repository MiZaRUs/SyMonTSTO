/**************************************************************************
 *  Simple Server_SD (SCADA)                                              *
 *  Copyright  (C)  2010-2013  by  Oleg Shirokov    olgshir@gmail.com     *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef _Devices_H_
#define _Devices_H_
//  ------------------------------------------------------------------------
typedef struct DeviceStruct{	// виртуальная копия устройства
    char *name;		// название устройства
    int id;		// идентификатор устройства
    int adr;		// адрес устройства в сети
    int reg;		// количество регистров в устройстве
    int err;
    int frm_len;
    unsigned char frm[15];
//  --
    int ( *coder_frm )( unsigned char adr, unsigned char *frm, int frm_len, unsigned char *buf );
    int ( *decoder )( unsigned char adr, unsigned char *buf, int len );
    int ( *get_data )( int poz, unsigned char *buf, int len );
//  --
} Device;
//  ------------------------------------------------------------------------
extern void init_device( Device *d, char *name, int id, int adr );
//  ------------------------------------------------------------------------
#endif
