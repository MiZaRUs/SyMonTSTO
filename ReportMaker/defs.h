/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright  (C)  2010-2013  by  Oleg Shirokov     olgshir@gmail.com    *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef Main_Defs
#define Main_Defs
//----------------------------------------------------------------
const char BDHOST[] = "192.168.2.2";	// IP MySql-сервера 
//const char BDHOST[] = "localhost";	// IP MySql-сервера 
const char BDNAME[] = "server_sd";	// Имя базы данных
const char BDUSER[] = "oleg";		// Имя пользователя
const char BDPASS[] = "";		// Пароль пользователя
const char TBLCNF[] = "netconf";	// Таблица конфигурации сети
const char TBLDEV[] = "devices";	// Таблица устройств ввода данных
const char TBLOBJ[] = "object";		// Таблица объектов наблюдения
const char TBLPRM[] = "param";		// Таблица параметров объекта
const char TBLDAN[] = "control";	// Таблица идентификации параметров объекта
//const char TBLDAN[] = "devdata";	// Таблица идентификации параметров объекта
//----------------------------------------------------------------
const int MAXBUF = 127;			// Максимальная длинна буфера обмена
const int MAXSTRBDATA = 10;
const int MAXTBLARC = 10;	// количество архивов
//const int FIXGROUP = 1000;	// деление на группы
//const int FIXGROUP = 10000;	// деление на группы
//const int MAXGROUP = 11;	// М. количество групп отображения(шкал)
//----------------------------------------------------------------
#endif
