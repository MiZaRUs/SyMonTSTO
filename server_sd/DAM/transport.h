/**************************************************************************
 *  Server_SD  DAM  transport.h                                           *
 *  Copyright  (C)  2010-2013  by  Oleg Shirokov    olgshir@gmail.com     *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/

//#define SERIAL_ASCII
////#define SERIAL_MB_RTU - нету!
#ifndef _Transport_H_
#define _Transport_H_
//----------------------------------------------------------------
extern void create_transport( char *t_net, int t_ps, int timeaut );
extern void delete_transport( void );
extern int write_transport( unsigned char *buf, int len);
extern int read_transport( unsigned char *buf, int len);
//----------------------------------------------------------------
#endif