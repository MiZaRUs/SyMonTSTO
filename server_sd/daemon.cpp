/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include "daemon.h"
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

#define DEBUG
//  -------------------------------------------------------------------------
Daemon::Daemon(string name){
    d_name = name;
    pidfile = pidputh + name + ".pid";
    pid = 0;
    fid = readPidFile();

/* open the system log - here we are using the LOCAL0 facility */
    openlog(d_name.c_str(),LOG_PID|LOG_CONS|LOG_NDELAY|LOG_NOWAIT,LOG_LOCAL0);
    (void)setlogmask(LOG_UPTO(LOG_DEBUG)); /* set logging level */
//    cout << "SyslogON for message.\n";

//  --
    if(fid > 0){
//cout << "Test foreground" << ": ";
        if(kill(pid,0) == 0){
            pid = fid;// daemon activen!
//            cout << d_name <<" Active! Pid = " << pid << " ";
        }else{
            if(errno==ESRCH){ /* non-existent process */
                cout << "ERROR\n Bad pidfile. Delete the "<< pidfile << "\n";
//            }else{
//                cout << "Could not acquire exclusive lock on pidfile";
            }
        pid = -1;
        }
    }//END proverka
//cout << "PidFile: " << pidfile << "\n";
}// End Daemon
//  -------------------------------------------------------------------------
//  -------------------------------------------------------------------------
void Daemon::Run(void){
    int t = 3;
    while(pid){
        sleep(10);
        t--;
        if(!t)pid = 0;
    }
}// End Run
//  -------------------------------------------------------------------------
void Daemon::Command(string com){
    if(fid != pid){
        cout << "Error:\nCommand:" << com << " Fid = " << fid << "   Pid = " << pid  << "\n";
        exit (EXIT_FAILURE);
    }
    if( pid == 0 && com != "start"){
        cout << "\n\nUsage: " << d_name << " [start|stop|reload]\n\n";
        exit (EXIT_FAILURE);
    }
    if(pid > 0){ //active!
        if( com == "stop"){
            kill(pid, SIGUSR1);
            cout << "- kill\n";
            exit(EXIT_SUCCESS);
        }
        if(com == "reload"){
            kill(pid, SIGHUP);
            cout << "- reload config\n";
            exit(EXIT_SUCCESS);
        }
        cout << "\nUsage " << d_name << " [stop|reload]\n\n";
        exit (EXIT_FAILURE);
    }
//cout << "Command: " << com << endl;
}// End Command
//  -------------------------------------------------------------------------
void Daemon::Daemonize(void){
#ifdef DEBUG
pid = 7;
cout << "Daemon: " << " Fid = " << fid << "   Pid = " << pid  << "\n";
#else
    pid_t sid;
    if(fid != 0 && pid != 0) exit (EXIT_SUCCESS);
/* Ответвляемся от родительского процесса */
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
/* Если с PID'ом все получилось, то родительский процесс можно завершить. */
    if (pid > 0) exit(EXIT_SUCCESS);
/* Изменяем файловую маску */
    umask(0);
#endif

#ifndef DEBUG
/* Создание нового SID для дочернего процесса */
    sid = setsid();
    if (sid < 0) {
        syslog(LOG_LOCAL0|LOG_ERR,"Error setsid(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
/* Изменяем текущий рабочий каталог */
    if ((chdir("/")) < 0) {
        syslog(LOG_LOCAL0|LOG_ERR,"chdir(\"/\"): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
/* Закрываем стандартные файловые дескрипторы */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    setpgrp();

    if(!createPidFile()){
        syslog(LOG_LOCAL0|LOG_ERR,"createPidFile : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    pid = fid = sid;
#endif
}// End Daemonize
//  -------------------------------------------------------------------------
void Daemon::Exit(int sig){
    syslog(LOG_LOCAL0|LOG_INFO,"Sig: %d - Exit OK!\n", sig);
    close(fd);
    unlink(pidfile.c_str());
    closelog();
}// End Exit
//  -------------------------------------------------------------------------
bool Daemon::createPidFile(void){
//    int fd;
    unlink(pidfile.c_str());
/* open the pidfile */
    fd = open(pidfile.c_str(), O_WRONLY|O_CREAT|O_EXCL, 0644);
    if(fd >= 0){
        FILE *f;
/* write our pid to it */
        f = fdopen(fd, "w");
        if(f != NULL){
            fprintf(f, "%d\n", getpid());
            fclose(f);
            /* leave the fd open */
            return true;
        }
    close(fd);
    }
    syslog(LOG_LOCAL0|LOG_INFO,"Can't create pidfile %s \n", strerror(errno));
    return false;
}// End createPidFile
//  -------------------------------------------------------------------------
pid_t Daemon::readPidFile(void){
    int fd, len;
    char pid_buf[16];
    if ((fd = open(pidfile.c_str(), O_RDONLY)) < 0) return 0;
    len = read(fd, pid_buf, 16);
    pid_buf[len] = 0;
    return atoi(pid_buf);
}// End readPidFile
//  -------------------------------------------------------------------------
Daemon::~Daemon(){}// End ~Daemon
/***************************************************************************/
