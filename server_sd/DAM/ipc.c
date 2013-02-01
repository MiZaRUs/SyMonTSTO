/**************************************************************************
 *  Server_SD  DAM  ipc.c                                                 *
 *  Copyright  (C)  2012-2013  by  Oleg Shirokov    olgshir@gmail.com     *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
//  --
#include "errors.h"
#include "ipc.h"
//  --
//#define DEBUG
//  -----------
static int hd_fifo;
static char fifo_name[32];
//----------------------------------------------------------------
extern int create_ipc( char *name ){
    sprintf((char*)fifo_name, "%s.fifo", name);		//  !!! указать путь !!!
    delete_ipc();
    if( mkfifo(fifo_name, 0644 )) return 0; //error
    return 1;
}// End create_ipc
//----------------------------------------------------------------
extern int write_ipc( char *msg, int len ){
    int rez = 0;
    if(( hd_fifo = open( fifo_name, O_WRONLY )) > 0 ){
        rez = write( hd_fifo, msg, strlen( msg ));
        close( hd_fifo );
    } else {
        return 0;
    }
return rez;
}// End write_ipc
//  -------------------------------------------------------------------------
extern void delete_ipc( void ){
    if( hd_fifo ) close( hd_fifo );
    unlink(fifo_name);
    remove(fifo_name);
}// End delete_ipc
//  -------------------------------------------------------------------------
