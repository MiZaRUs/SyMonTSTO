/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright  (C)  2010-2013  by  Oleg Shirokov    olgshir@gmail.com     *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
//-------------------------------------------------------------------------
extern int create_ipc( char *name );
extern int write_ipc( char *msg, int len );
//extern int read_ipc( char *msg, int len );
extern void delete_ipc( void );
//-------------------------------------------------------------------------
