/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C)  2010-2012  by  Oleg Shirokov    olgshir@gmail.com      *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef Main_Defs
#define Main_Defs

#include <string>
//  --
#define MAXNAME 31		// Максимальная длинна имени сервера
//  --
#define E_ER  -2000000000	//ошибка ?
//  --
//#define E_MEM  		//Ошибкa выделения памяти - Авар.
//  --
// ошибки БД
#define E_DB      E_ER - 10000	//ошибка соединения с бд - аварийный выход
#define E_DB_REZ  E_ER - 10001	//ошибка результата ???
//#define E_NO_QU  ?		//ошибка запроса к бд - попробовать пересоздать таблицу
//#define E_NO_REC ?		//нет записей - надо создать
//#define E_CNF    ?		//неверное количество записей ??? сонфиг
//----------------------------------------------------------------
// ошибки обмена - с 9000 по 9019 - переподключить транспорт
#define E_SOCKET  E_ER - 9000	//E_SOCKET - ошибка соединения	!!!
#define E_TR_SND  E_ER - 9001	//ошибка передатчика
#define E_TR_RCV  E_ER - 9002	//ошибка приёмника
//  --
#define E_DN      E_ER - 9011	//ошибка транспорта
#define E_CP      E_ER - 9012	//чужой пакет			!!!
#define E_KS      E_ER - 9013	//ошибка KS
//  --
#define E_TR_XX   E_ER - 9019	//неизвестная ошибка транспорта
//  --
// общии ошибки устройств
#define E_DEV_DN  E_ER - 9020	//ошибка данных
#define E_DEV_PR  E_ER - 9021	//ошибка протокола
// специфичные ошибки устройств
#define E_ZER     E_ER - 9900	//неисправен
#define E_KZ      E_ER - 9901	//переполнен вниз -1 короткое
#define E_NO      E_ER - 9903	//Нет канала
#define E_WNO     E_ER - 9906	//Недоступно - отключен
#define E_OBR     E_ER - 9908	//переполнен вверх +1
//----------------------------------------------------------------
//----------------------------------------------------------------
const int MAXBUF = 127;			// Максимальная длинна буфера обмена
const int MAXSTRBDATA = 10;
//const int RE_DATA = 3600 * 4;		// Время принудительного обновления в секундах(часы)
const int RE_DATA = 3600;		// Время принудительного обновления в секундах
const int OLD_DATA = 3600 * 24 * 366;	// Время хранения записей в архиве (сутки)
//----------------------------------------------------------------
const char BDHOST[] = "localhost";	// IP MySql-сервера 
const char BDNAME[] = "server_sd";	// Имя базы данных
const char BDUSER[] = "oleg";		// Имя пользователя
const char BDPASS[] = "";		// Пароль пользователя
const char TBLCNF[] = "netconf";	// Таблица конфигурации сети
const char TBLDEV[] = "devices";	// Таблица устройств ввода данных
const char TBLOBJ[] = "object";		// Таблица объектов наблюдения
const char TBLPRM[] = "param";		// Таблица параметров объекта
const char TBLDAN[] = "devdata";	// Таблица идентификации параметров объекта
//----------------------------------------------------------------
//----------------------------------------------------------------
const int MAXTBLARC = 7;		// количество архивов
const int FIXGROUP = 10000;		// деление на группы
const int MAXGROUP = 11;		// М. количество групп отображения(шкал)
//----------------------------------------------------------------
#endif
