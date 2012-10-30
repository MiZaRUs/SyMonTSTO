/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
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
#define MAXNAME 31  // Максимальная длинна имени сервера
//  --

#define E_ER  -2000000000	//ошибка ?
//  --
// ошибки обмена
#define E_SOCKET   E_ER - 9000	//E_SOCKET - ошибка соединения
#define E_TR_SND    E_ER - 9001	//ошибка передатчика
#define E_TR_RCV    E_ER - 9002	//ошибка приёмника
//  --
#define E_DN  E_ER - 9011	    //ошибка данных
#define E_CP  E_ER - 9012	    //чужой пакет
#define E_KS  E_ER - 9013	    //ошибка KS
//  --
// ошибки устройств
#define E_ZER   E_ER - 9900	//неисправен
#define E_KZ     E_ER - 9901	//переполнен вниз -1 короткое
#define E_NO    E_ER - 9903	//Нет канала
#define E_WNO E_ER - 9906	//Недоступно - отключен
#define E_OBR  E_ER - 9908	//переполнен вверх +1
//----------------------------------------------------------------
const char BDHOST[] = "192.168.2.2";	// IP MySql-сервера 
//const char BDHOST[] = "localhost";	// IP MySql-сервера 
const char BDNAME[] = "server_sd";	// Имя базы данных
const char BDUSER[] = "oleg";		// Имя пользователя
const char BDPASS[] = "";		// Пароль пользователя
const char TBLCNF[] = "netconf";	// Таблица конфигурации сети
const char TBLDEV[] = "devices";	// Таблица устройств ввода данных
//----------------------------------------------------------------
const int MAXBUF = 127;			// Максимальная длинна буфера обмена
const int MAXSTRBDATA = 10;
const int RE_DATA = 3600 * 4;		// Время принудительного обновления в секундах(часы)
const int OLD_DATA = 3600 * 24 * 1;	// Время хранения записей в архиве (сутки)
//----------------------------------------------------------------
#endif

