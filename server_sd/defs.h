/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright  (C)  2010-2013  by  Oleg Shirokov    olgshir@gmail.com     *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef Main_Defs
#define Main_Defs
//  --
#define MAXNAME 31		// Максимальная длинна имени сервера
//----------------------------------------------------------------
const int MAXSTRBDATA = 10;
//const int RE_DATA = 3600 * 4;		// Время принудительного обновления в секундах(часы)
//const int RE_DATA = 3600;		// Время принудительного обновления в секундах
//const int OLD_DATA = 3600 * 24 * 366;	// Время хранения записей в архиве (сутки)
//----------------------------------------------------------------
const char BDHOST[] = "localhost";	// IP MySql-сервера 
const char BDNAME[] = "server_sd";	// Имя базы данных
const char BDUSER[] = "oleg";		// Имя пользователя
const char BDPASS[] = "";		// Пароль пользователя
const char TBLDEV[] = "devices";	// Таблица устройств ввода данных
const char TBLOBJ[] = "object";		// Таблица объектов наблюдения
const char TBLPRM[] = "param";		// Таблица параметров объекта
const char TBLDAN[] = "devdata";	// Таблица идентификации параметров объекта
//----------------------------------------------------------------
const int FIXGROUP = 10000;		// деление на группы
const int MAXGROUP = 11;		// М. количество групп отображения(шкал)
//----------------------------------------------------------------
#endif
