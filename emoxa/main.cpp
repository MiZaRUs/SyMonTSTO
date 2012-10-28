/**************************************************************************
 *  emoxa                                                                 *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <syslog.h>
#include <signal.h>

#include "ServerSocket.h"
#include "Serial.h"

//#define DEBUG

const int BUF_SIZE = 127;
const int MAXBUF = 80;
//  -----------------------
using namespace std;
//--------------------------------------------------------------------
static void my_exit(int sig){
    syslog(LOG_LOCAL0|LOG_INFO,"Sig: %d - Exit OK!\n", sig);
    closelog();
    exit(EXIT_SUCCESS);
}
//--------------------------------------------------------------------
int main(int argc, char *argv[]){
    int tty, conn;
    char buf[BUF_SIZE];

/* open the system log - here we are using the LOCAL0 facility */
    openlog("emoxa",LOG_PID|LOG_CONS|LOG_NDELAY|LOG_NOWAIT,LOG_LOCAL0);
    (void)setlogmask(LOG_UPTO(LOG_DEBUG)); /* set logging level */

/* Настройка системных сигналов */
    signal(SIGINT, my_exit);
    signal(SIGUSR1, my_exit);
    signal(SIGQUIT, my_exit);
    signal(SIGTERM, my_exit);
    signal(SIGPIPE, SIG_IGN);

/////////////////////////////////
    try{
        Serial tty( "/dev/ttyS0");
        ServerSocket server(4001);
/////////////////////////////////
syslog(LOG_LOCAL0|LOG_INFO,"Run!\n");

#ifdef DEBUG
cout << "Running...." << endl;
#endif
        while((conn = server.getHd())){
#ifdef DEBUG
cout << "New " << conn << endl;
#endif
            int rc = 1;
            while(rc){
                memset(buf, 0, BUF_SIZE);
                rc = read(conn, buf, sizeof(buf) - 1);
// if(rc == 0) -клиент отключился
                if(rc < 1){
#ifdef DEBUG
cout << "Read: rezult = " << rc << endl;
#endif
                    break;
                }
//                buf[rc] = 0;
#ifdef DEBUG
cout << "Получил " << rc << " б. - Передаю в ttyS: " << buf << endl;
#endif
//  --
                int htty = tty.getHd();
                int rez = write(htty, buf, rc);
//  -- ушло в ttyS
                char bufp[MAXBUF + 1];
                memset(bufp, 0, MAXBUF + 1);
                char *bufptr = bufp;
                tty.reset();
                while(( rez = read(htty, bufptr, MAXBUF)) > 0){
                    bufptr += rez;
                    if( (bufptr[-1] == '\n' ) | (bufptr-bufp == MAXBUF)) break;
                }
                *bufptr = '\0';
                if (rez > 0){
                    rez = bufptr - bufp;
//                rez--;
                    bufp[rez] = 0;
                }
//  --
#ifdef DEBUG
cout << "Принял из ttyS " << rez << " б.";
#endif
                if(rez > 0){
                    rc = write(conn, bufp, rez);
                    if(rc < 1){
                        cout << "Write: rezult = " << rc << endl;
//                        break;
                    }
#ifdef DEBUG
cout << " Возвращяю: " << rc << " б. в Socket: "  << bufp << endl;
#endif
                }else{
#ifdef DEBUG
cout << " Нет возврата в сокет !!!" << endl;
#endif
//                    break;
                }
//                }//if(buf...
            }//while(rc)
#ifdef DEBUG
cout << "End RC." << endl;
#endif
        }// while((conn...

    }
    catch( char *e ){
        syslog(LOG_LOCAL0|LOG_ERR,"ERROR: %s\n", e);
    }
    catch( ... ){
        syslog(LOG_LOCAL0|LOG_ERR,"FATAL_ERROR!");
    }
//  --
    closelog();
    exit(EXIT_FAILURE);
return EXIT_FAILURE;
}//**************************************************//
