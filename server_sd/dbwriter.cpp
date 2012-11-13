/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
//  --
#include "dbwriter.h"

#define DEBUG
//  -------------------------------------------------------------------------
using namespace std;
//  -------------------------------------------------------------------------
DBWriter::DBWriter(Config *cfg){
//    Err = 0;
    msg  = "DBWriter: ";
    data_name = cfg->getTblData();
    arch_name = cfg->getTblArch();
#ifndef DEBUG
    mysql_init(&mysql_rw);
#else
msg.append(data_name);
msg.append(",");
msg.append(arch_name);
#endif
msg.append(".  OK.");
}// End
//  -------------------------------------------------------------------------
bool DBWriter::Write(Device *d){
//cout << "DBWriter::Writer - adr: " << d->getAdr() << endl;
    bool rez = true;
    msg  = "DBWriter-Write: ";

#ifdef DEBUG
//-------------------------------------
cout << "DBWriter-Write: adr ";
cout << d->getAdr() << "  regs: ";
            char str[16];
            for(int qq = 0; qq < d->getCount();  qq++){
                sprintf(str,"%d",d->getData(qq));
//        cout << (qq +1) << ". " << str << endl;
                cout << (qq +1) << " = " << str << ";\t ";
            }
cout << "-Integer." << endl;
//-------------------------------------
#else
    if(!mysql_real_connect(&mysql_rw, BDHOST, "oleg", "", BDNAME, 0,NULL,0)){
        msg.append(mysql_error(&mysql_rw));
        return false;
    }
//  --
    for(curd = 0; curd < d->getCount(); curd++){
        switch(test(d)){
//        case 0: break; ничего не делать
        case 1: // записать-обновить
//cout << "записать-обновить: " << d->getAdr() << " . " << curd+1 << endl;
            if(!write(d)) rez = false;
            if(!arc_wr(d)) rez = false;
            break;
        case 2: // создать
//cout << "создать" << endl;
            if(!new_wr(d)) rez = false;
            if(!arc_wr(d)) rez = false;
            break;
        case -1: // Error MySql
//cout << "Error MySql" << endl;
            rez = false; 
            break;
        }
//cout << "MSG: " << msg << endl;
    }
//  --
    mysql_close(&mysql_rw);
#endif
    return rez;
}// End
//  -------------------------------------------------------------------------
int DBWriter::test(Device *dd){
    string qu = "SELECT utime, data, diap FROM " + data_name + " WHERE id = ";
    char st[20];
    sprintf(st, "%d", dd->getID());
    qu.append(st);
    qu.append(" AND reg = ");
    sprintf(st, "%d", curd);
    qu.append(st);
//cout << "test = "<< qu << endl;
//  --
    if(mysql_query(&mysql_rw, qu.c_str())){
        msg.append("-test MySQL: ");
        msg.append(mysql_error(&mysql_rw));
        return -1;
    }
//  --
    MYSQL_RES *res = mysql_store_result(&mysql_rw);
    if(!res){
        msg.append("-test MySQL: ");
        msg.append(mysql_error(&mysql_rw));
        return -1;
    }
//  --
    MYSQL_ROW row;
    int utime = 0;
    int data = 0;	//data
    int diap = 0;	//diap
    int c = 0;
    while((row = mysql_fetch_row(res))){
        utime = atoi(row[0]);	//utime
        data = atoi(row[1]);	//data
        diap = atoi(row[2]);	//diap
        c++;
        if(c>1)break;
    }
    mysql_free_result(res);
    if(c == 0) return 2; // Нет записи - надо создать
//        Err = NO_REC; // Нет записи - надо создать
//  --
//cout << "> test: diap =" << diap << " " << " Old = " << data;
    int i = data - dd->getData( curd );
//cout << " getData: " << dd->getData( curd );
//cout << " Abs:" << abs(i) << endl ;

    if( abs(i) >= diap) return 1;
    i = dd->getTime() - utime;
//cout << " T:" << i << " - " << RE_DATA << " .\n";
    if( i > RE_DATA )  return 1;

return 0;
}//  End
//  -------------------------------------------------------------------------
bool DBWriter::write(Device *d){
//cout << "Изменить запись.\n";
    string qu = "UPDATE " + data_name + " SET ";
    char st[30];
    sprintf(st, "utime = %d, ", d->getTime());
    qu.append(st);
    sprintf(st, "data = %d ", d->getData(curd));
    qu.append(st);
    sprintf(st, "WHERE id = %d ", d->getID());
    qu.append(st);
    sprintf(st, "AND reg = %d", curd);
    qu.append(st);
//cout << qu << endl;
    if(mysql_query(&mysql_rw, qu.c_str())){
        msg.append("-write MySQL: ");
        msg.append(mysql_error(&mysql_rw));
        return false;
    }
return true;
}//  End
//  -------------------------------------------------------------------------
bool DBWriter::arc_wr(Device *dd){
//cout << "Добавить запись в архив.\n";
    string qu = "INSERT " + arch_name + " ( id, reg, utime, data ) VALUES ( ";
    char st[20];
    sprintf(st, "%d, ", dd->getID());
    qu.append(st);
    sprintf(st, "%d, ", curd);
    qu.append(st);
    sprintf(st, "%d, ", dd->getTime());
    qu.append(st);
    sprintf(st, "%d )", dd->getData(curd));
    qu.append(st);
//cout << qu << endl;
    if(mysql_query(&mysql_rw, qu.c_str())){
        msg.append("-arc_wr MySQL: ");
        msg.append(mysql_error(&mysql_rw));
        return false;
    }
return true;
}//  End
//  -------------------------------------------------------------------------
bool DBWriter::new_wr(Device *dd){
//cout << "Создать новую запись.\n";
    string qu = "INSERT " + data_name + " ( id, reg, utime, data, diap, modif, param, object, flag ) VALUES ( ";
    char st[20];
    sprintf(st, "%d, ", dd->getID());
    qu.append(st);
    sprintf(st, "%d, ", curd);
    qu.append(st);
    sprintf(st, "%d, ", dd->getTime());
    qu.append(st);
    sprintf(st, "%d, ", dd->getData(curd));
    qu.append(st);
    sprintf(st, "%d, ", dd->getDiap(curd));
    qu.append(st);
    sprintf(st, "%f, ", dd->getModif(curd));
    qu.append(st);
    qu.append("0, 0, 0)");
//cout << qu << endl;
    if(mysql_query(&mysql_rw, qu.c_str())){
        msg.append("-new_wr MySQL: ");
        msg.append(mysql_error(&mysql_rw));
        return false;
    }
return true;
}//  End
//  -------------------------------------------------------------------------
bool DBWriter::oldClear(void){
//cout << "Удалить старые записи из арнива.\n";
    msg  = "DBWriter-oldClear: ";
    time_t oldtime;
    oldtime = time(&oldtime) - OLD_DATA;
//  --
    if(!mysql_real_connect(&mysql_rw, BDHOST, "oleg", "", BDNAME, 0,NULL,0)){
        msg.append(mysql_error(&mysql_rw));
        return false;
    }
//  --
    string qu = "DELETE FROM " + arch_name + " WHERE utime < ";
    char st[30];
    sprintf(st, "%d", (int)oldtime);
    qu.append(st);
//cout << qu << endl;
    if(mysql_query(&mysql_rw, qu.c_str())){
        msg.append(mysql_error(&mysql_rw));
        return false;
    }
    mysql_close(&mysql_rw);
return true;
}//  End
//  -------------------------------------------------------------------------
DBWriter::~DBWriter(){}// End
//  -------------------------------------------------------------------------
/***************************************************************************/
