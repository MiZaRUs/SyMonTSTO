/**************************************************************************
 *  Device Access Manager (DAM)                                           *
 *  Copyright  (C)  2010-2013  by  Oleg Shirokov    olgshir@gmail.com     *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <signal.h>
#include <syslog.h>
#include <errno.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
//  --
#include "errors.h"
#include "ipc.h"
#include "transport.h"
#include "devices.h"
//  --
#define DEBUG
//  --
//#define MSG_JSON
//  -----------
#define BUFSIZE 255
#define MSGSIZE 1024
//  -----------
//static int err;
static char *str_err;
//  --
static int cur_device;
static int max_device;
static int max_point;
static int time_wait;
//  --
static int buf_len;
static unsigned char buffer[BUFSIZE];
//----------------------------------------------------------------
// Обновление данных
static int refresh(Device *d);
//  --
//----------------------------------------------------------------
static void sign_exit( int sig );
static void sign_segv( int sig );
//  --
/***************************************************************************/
int main( int argc, char *argv[] ) {
//    err = 1;
    str_err = "!!";
    time_wait = 2;
    max_point = 0;
    buf_len = 0;
    cur_device = 0;
//  --
    if( argc < 1 ) exit( 0 );// простотакчтобнекричал!
    char *d_name = ( strrchr( argv[0], '/' ) + 1 );
//    openlog(d_name,LOG_PID|LOG_CONS|LOG_NDELAY|LOG_NOWAIT,LOG_LOCAL0);
//  --
//printf( "Получаем конфигурацию сервера по имени: %s\n", d_name );
//  --
    if( !create_ipc( d_name )){
#ifdef DEBUG
printf( "Error: open_ipc(%s)\n", d_name );
#endif
//        syslog(LOG_LOCAL0|LOG_INFO, "Error: open_ipc(%s)", d_name);
        exit( 0 );
    }
//  ---------------

/* Инициализация основных объектов данными из конфиг-файла */
#include "0_inc_dev.h"
//    create_transport( "10.0.10.47", 4001, 500 )
// Устройства в цикле из БД:
//    dev[].x = x; err = 0;
//  ---------------
    int ij = 0;
    for(; ij < max_device; ij++ ){
#ifdef DEBUG
printf( "Device[%d] = %d, %d, %s, %d\n", ij, dev[ij].id, dev[ij].adr, dev[ij].name, dev[ij].reg );
#endif
        max_point += dev[ij].reg;
    }
#ifdef DEBUG
printf( "max_point = %d\n", max_point );
#endif
//  ---------------

/* Настройка системных сигналов */
    signal( SIGSEGV, sign_segv );	// !!!
    signal( SIGPIPE, SIG_IGN );
//  --
    signal( SIGINT, sign_exit );
    signal( SIGUSR1, sign_exit );
    signal( SIGQUIT, sign_exit );
    signal( SIGTERM, sign_exit );
//  ---------------

// Run
//    syslog(LOG_LOCAL0|LOG_INFO, "Run.");
    while( 1 ){
str_err = "start while";
        int i;
        char fifo_msg[MSGSIZE];
//  --
#ifdef MSG_JSON
        int i_msg = sprintf((char*)fifo_msg, "#%d={", max_point);
        for( i = 0; i < max_device; i++ ){
            if(refresh(&dev[i]) > 0 ){
                int j, k;
                for( j = 0; j < dev[i].reg; j++ ){
                    int id = dev[i].id * 1000 + j + 1;
                    int da = dev[i].get_data( j, buffer, buf_len );
                    k = sprintf((char*)fifo_msg + i_msg, "\"%d\":[\"%d\"],", id, da);
                    i_msg += k;
                }
            }// refresh
            usleep(50);
        }// for max_device
str_err = "write_ipc";
        sprintf(( char* )fifo_msg + i_msg -1, "}#%d\n", i_msg );
        if( write_ipc( fifo_msg, strlen( fifo_msg )) < 1) break;;
#else
        int i_msg = sprintf((char*)fifo_msg, "#%s:%d\n", d_name, max_point);
        for( i = 0; i < max_device; i++ ){
            if(refresh(&dev[i]) > 0 ){
                int j, k;
                for( j = 0; j < dev[i].reg; j++ ){
                    int id = dev[i].id * 1000 + j + 1;
                    int da = dev[i].get_data( j, buffer, buf_len );
                    k = sprintf((char*)fifo_msg + i_msg, "%d:%d\n", id, da);
                    i_msg += k;
                }
//                k = sprintf((char*)fifo_msg + i_msg, "\n");	// ???
//                i_msg += k;					// ???
            } else {//if refresh or dev[].err
                i_msg = sprintf((char*)fifo_msg, "#%s:%d\n", d_name, dev[i].err );
            }// end if( refresh )
            usleep(50);
        }// for max_device
//  --
str_err = "write_ipc";
        sprintf(( char* )fifo_msg + i_msg, "%d#\n", i_msg );
        if( write_ipc( fifo_msg, strlen( fifo_msg )) < 1) break;;
#endif
//  --
//fprintf( stderr, "*" );
        sleep(time_wait);
    }// while
//  -------------------------------------------------------------------------
    fprintf( stderr,"\nBREAK: Error(%s).\n", str_err );
    delete_transport();
    delete_ipc();
//    syslog( LOG_NOTICE, "BREAK: Error(%s)", str_err);
//    closelog();
    exit( EXIT_FAILURE );
}// End main ----------------------------------------------------------------

/***************************************************************************/
// Обработчики сигналов
static void sign_segv( int sig ) {
    fprintf( stderr,"\nSIGSEGV: Step(%s).\n", str_err );
    delete_ipc();
//    syslog( LOG_NOTICE, "SIGSEGV: Step(%s)", str_err);
//    closelog();
    exit( EXIT_FAILURE );
}// End _segv
//  -------------------------------------------------------------------------
static void sign_exit( int sig ) {
    fprintf( stderr,"\nEXIT: Signal(%d).\n", sig );
    delete_transport();
    delete_ipc();
//    syslog( LOG_NOTICE, "EXIT: Signal(%d)", sig);
//    closelog();
    exit( EXIT_SUCCESS );
}// End _exit
/***************************************************************************/
//  -------------------------------------------------------------------------
static int refresh( Device *dv ){
//#ifdef DEBUG
//printf( "Refresh: %d, %d, %s, %d\n", dv->id, dv->adr, dv->name, dv->reg );
//#endif
    dv->err = 0;
// закодировать dv->cmd, dv->cmd_len в buffer
str_err = "dv->coder_frm";
    memset( buffer, 0, sizeof( buffer ));
    buf_len = dv->coder_frm( dv->adr, dv->frm, dv->frm_len, buffer );
    if( buf_len < 2 ){
        dv->err = E_CODER;
        buf_len = 0;
        return 0;
    }
//  --
// передача в устройство
str_err = "write_transport";
    int tmp = write_transport( buffer, buf_len );
    if( tmp < 0 ){
        dv->err = tmp;
        return 0;
    }
//  --

// чтение из устройства
str_err = "read_transport";
    memset( buffer, 0, sizeof( buffer ));
    buf_len = read_transport( buffer, ( sizeof( buffer ) - 1 ));
    if( buf_len < 0 ){
        dv->err = buf_len;
        buf_len = 0;
        return 0;
    }
//  --
    if( buf_len >= (signed)sizeof( buffer ) ){
        dv->err = E_TR_READ;
        buf_len = 0;
        return 0;
    }
//  --

// распаковать ответ
str_err = "dv->decoder";
    buf_len = dv->decoder( dv->adr, buffer, buf_len );
    if( buf_len < 0 ){
        dv->err = buf_len;
        buf_len = 0;
        return 0;
    }
//  --
str_err = "end refresh";
return 1;
}//  End refresh()
//  -------------------------------------------------------------------------
/***************************************************************************/
