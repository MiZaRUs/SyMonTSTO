/**************************************************************************
 *  Server_SD  DAM  errors.h                                              *
 *  Copyright  (C)  2010-2013  by  Oleg Shirokov     olgshir@gmail.com    *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#ifndef _Errors_H_
#define _Errors_H_

//----------------------------------------------------------------
#define E_ER  -2000000000	//ошибка ?
//  --
#define E_MEM		E_ER - 9000	//Ошибкa выделения памяти - Авар.
//  --
// ошибки IPC
#define E_IPC		E_ER - 9200	//ошибка создания IPC!!
#define E_IPC_RD	E_ER - 9201	//ошибка чтения IPC
#define E_IPC_WR	E_ER - 9202	//ошибка записи IPC
//  --
// ошибки транспорта
#define E_TRANSPORT	E_ER - 9600	//ошибка соединения	!!!
#define E_TR_WRITE	E_ER - 9601	//ошибка передатчика
#define E_TR_READ	E_ER - 9602	//ошибка приёмника
//  --
// ошибки драйвера обмена (протокола)
#define E_DRIVER	E_ER - 9700	//ошибка пакета
#define E_DRV_ID	E_ER - 9701	//ошибка адреса
#define E_DRV_KS	E_ER - 9702	//ошибка контрольной суммы
#define E_DRV_FR	E_ER - 9703	//ошибка фрейма
#define E_DRV_NO	E_ER - 9704	//отсутствует драйвер
//  --
// ошибки устройств
#define E_DEVICE	E_ER - 9800	//неверные данные
#define E_CODER		E_ER - 9801	//неверное кодирование
#define E_DEV_DN	E_ER - 9802	//неверное преобразование
//  --
#define E_ZER		E_ER - 9900	//неисправен
#define E_KZ		E_ER - 9901	//переполнен вниз -1 короткое
#define E_NO		E_ER - 9903	//Нет канала
#define E_WNO		E_ER - 9906	//Недоступно - отключен
#define E_OBR		E_ER - 9908	//переполнен вверх +1
//  --
// ошибки БД
//#define E_DB		E_ER - 9300	//ошибка соединения с бд - аварийный выход
//#define E_DB_REZ	E_ER - 9301	//ошибка результата ???
//#define E_NO_QU  ?		//ошибка запроса к бд - попробовать пересоздать таблицу
//#define E_NO_REC ?		//нет записей - надо создать
//#define E_CNF    ?		//неверное количество записей ??? сонфиг
//----------------------------------------------------------------
#endif
