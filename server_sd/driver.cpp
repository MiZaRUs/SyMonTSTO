/**************************************************************************
 *  Server_SD (SCADA)                                                     *
 *  Copyright (C) 2010-2012 by Oleg Shirokov   olgshir@gmail.com          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; version 2 of the License, or            *
 *  (at your option) any later version.                                   *
 **************************************************************************/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//  --
//#include <memory.h>
//  --
#include "driver.h"
//  --
////#define DEBUG
//  ----------------------------------------------------------
Driver::Driver(string name){
    msg = "Driver: ";
    msg.append(name);
    trp = NULL;
    buf[0]=0;
    buf_len=0;
//  --
    request = &Driver::requestXXX;	// заглушка
    unpak = &Driver::unpakXXX;		// заглушка

// зависит от устройства
    if( name == "ELEMER" ){
        msg.append(" -Y");
        request = &Driver::requestELEMER;
        unpak = &Driver::unpakELEMER;
    }
//  --
    if( name == "DCON" ){
        msg.append(" -Y");
        request = &Driver::requestDCON;
        unpak = &Driver::unpakDCON;
    }
//  --
    if( name == "OWEN" ){
        msg.append(" -Y");
        request = &Driver::requestOWEN;
        unpak = &Driver::unpakOWEN;
    }
//  --
    if( name == "MB_RTU" ){
        msg.append(" -Y");
        request = &Driver::requestMB_RTU;
        unpak = &Driver::unpakMB_RTU;
    }
//  --
    if( name == "MB_ASC" ){
        msg.append(" -Y");
        request = &Driver::requestMB_ASC;
        unpak = &Driver::unpakMB_ASC;
    }
//  --
    if( name == "MB_ASC_B" ){
        msg.append(" -Y");
        request = &Driver::requestMB_ASC;
        unpak = &Driver::unpakMB_ASC_B;
    }
    msg.append(" >> ");
}// End
//  ----------------------------------------------------------
/************************************************************/
bool Driver::Request(unsigned char adr, unsigned char *data, int len){
    if( trp == NULL ) return false;
    err = 0; 
    buf_len = 0;
    memset(buf, 0, sizeof(buf));
    int rez = (*this.*request)(adr, data, len); // if = -1 => Error, перезапустить TransPort !!!
    if(rez == buf_len)return true;
    if(rez == -1){
        err = E_SOCKET; // Требуется переподключениеTransPort
    }else{
        err = E_TR_SND;
    }
    return false;
}
//  ----------------------------------------------------------
bool Driver::Response(unsigned char adr){
    if( trp == NULL ) return false;
    err = E_TR_RCV ;
    buf_len = 0;
    memset(buf, 0, sizeof(buf));
    buf_len = trp->Read( buf, (sizeof(buf) - 1)); // if = -1 => FatalError, перезапустить TransPort ???
    if(buf_len > 1)err = (*this.*unpak)( adr );
    if(!err) return true;
    return false;
}
/************************************************************/
//  ----------------------------------------------------------
//		 ЗАПРОС
int Driver::requestELEMER(unsigned char adr,unsigned char *data, int len){ // ELEMER
    unsigned char cmd[(len + 8)];
    buf_len = 0;
    int i = sprintf((char*)cmd,":%d;", adr);
    if(len)memcpy(cmd + i, data, len);
    i += len;
    i += sprintf((char*)cmd + i, "%d", ksumCRC(cmd+1, i -1));
    cmd[i] = 0xD;
    buf_len = i + 1;
    cmd[buf_len] = 0;
//  --
return trp->Write(cmd, buf_len);
} // End coderELEMER
//  -------------------------------------------------------------------------
int Driver::requestDCON(unsigned char adr,unsigned char *data, int len){ // DCON
//    if( len > 0 ){"!..."}else
    if(adr < 0x10){
        sprintf((char*)buf,"@0%X", adr);
    } else{
        sprintf((char*)buf,"@%X", adr);
    }
//  --
    int i = 3;
    i += sprintf((char*)buf + i, "%X", ksumDCON(buf, i));
    buf[i] = 0x0D;
    buf_len = i + 1;
    buf[buf_len] = 0;
return trp->Write(buf, buf_len);
} // End coderDCON
//  -------------------------------------------------------------------------
int Driver::requestOWEN(unsigned char adr,unsigned char *data, int len){ // OWEN
// максимальная длина пакета согласно протоколу ОВЕН  = 21
    unsigned char frame[len + 3];
    frame[0] = adr;
    if(len)memcpy(frame+1, data, len);
    unsigned short crc = ksumOwen(frame, len+1 );
    frame[len+1] = (crc >> 8) & 0xff;
    frame[len+2] = crc & 0xff;
    int frameSize = len+3;
// максимальная длина ASCII-пакета включая маркеры и символ '\0' = 45;
    int i, j;
    buf[0] = '#';
    for (i = 0, j = 1; i < frameSize; ++i, j += 2){
        buf[j] = 'G' + ((frame[i] >> 4) & 0x0f);
        buf[j + 1] = 'G' + (frame[i] & 0x0f);
    }
    buf[frameSize*2+1] = '\xD';
    buf[frameSize*2+2] = '\0';
    buf_len = frameSize * 2 + 2;
return trp->Write(buf, buf_len);
} // End coderOWEN
//  -------------------------------------------------------------------------
int Driver::requestMB_ASC(unsigned char adr,unsigned char *data, int len){ // MODBUS_ASCII
    unsigned char frame[len + 2 ];
    frame[0] = adr;
    if(len)memcpy(frame+1, data, len);
    len++;
    frame[len] = ksumLRC(frame, len);
    len++;
//  --
    int i, j;
    buf[0] = ':';
    for (i = 0, j = 1; i < len; ++i, j += 2){
        buf[j] = hex2char(frame[i] >> 4);
        buf[j + 1] = hex2char(frame[i]);
    }
    buf[j] = 0xd;
    buf[++j] = 0xa;
    buf[++j] = 0;
    buf_len = j;
return trp->Write(buf, buf_len);
} // End coderMB_ASC
//  -------------------------------------------------------------------------
int Driver::requestMB_RTU(unsigned char adr,unsigned char *data, int len){ // MODBUS_RTU
    buf[0] = adr;     // ADR
    if(len)memcpy(buf + 1, data, len);
    len++;
    unsigned short ks = ksumCRC(buf, len);
	buf[len + 1 ] = (ks >> 8) & 0xff;
	buf[len + 0 ] = ks & 0xff;
    buf_len = len+2;
return trp->Write(buf, buf_len);
} // End coderMB_RTU
//  -------------------------------------------------------------------------
int Driver::requestXXX(unsigned char adr,unsigned char *data, int len){
//#ifdef DEBUG
//cout << endl << "CMD_HEX:";
//for(i=0; i < buf_len; i++) printf(" %2x", buf[i]);
//cout << endl << "CMD_SIM:";
//for(i=0; i < buf_len; i++) printf(" %2c", buf[i]);
//cout << endl;
//#endif
    buf_len = len;
return trp->Write(data, buf_len);
} // End
//  -------------------------------------------------------------------------
/*************************************************************/
//  -------------------------------------------------------------------------
//              Разбор ответа
//  -------------------------------------------------------------------------
int Driver::unpakELEMER(unsigned char adr){	// распаковка ELEMER
    unsigned char *ptr = buf;
    if((ptr[0] != '!')||(ptr[0] != ':')) ptr++;	// !!!!!!!!!!!!!!!!!!!!!!!!!
//  --
    if((buf_len < 7) || (buf_len >= MAXBUF)) return E_DN;
    int i;
    for(i = buf_len-1;(ptr[i] != ';') && (i > 3);i--){};
    if (i < 4) return E_DN;
    unsigned short tm_ks = ksumCRC(ptr+1, i );
//  --
    i++;
    int a, b;
    char str[7];
    for( a = 1, b = 0;(ptr[a] >= '0' ) && (ptr[a] <= '9'); a++, b++)str[b] = ptr[a];
    str[b] = '\0';
//cout << "ADR=" << str << " - " << atoi(str) << endl;
    if( adr != (unsigned char)atoi(str)) return E_CP;
//  --
    int temp = i + 1;
    int j = 0;
    for(;i < buf_len; i++, j++)str[j] = ptr[i];
    str[j-1] = '\0';
//  --
    if(tm_ks != atoi(str)) return E_KS;
//  --
    j=0;
    for( i = a+2; i < temp; i++, j++)buf[j] = buf[i];
    buf[j] = 0;
    buf_len = j-1;
    return 0;
}// End decoderELEMER
//  -------------------------------------------------------------------------
int Driver::unpakDCON(unsigned char adr){	// распаковка DCON
    unsigned short tm_ks = 0;
    if((buf_len < 5) || (buf_len >= MAXBUF)) return E_DN;
    buf[7] = 0;
    tm_ks = ksumDCON(buf, 5);
    unsigned char eks = char2hex( buf[5] );
    eks = (eks << 4) + char2hex( buf[6] );
//printf("eKS: %x\n", eks);
//  --
    if(tm_ks != eks) return E_KS;
//  --  --
    unsigned char x = char2hex( buf[1] );
    x = (x << 4) | char2hex( buf[2] );
    buf[0] = x;
    x = char2hex( buf[3] );
    x = (x << 4) | char2hex( buf[4] );
    buf[1] = x;
    buf[2] = 0;
    buf_len = 2;
//  --
return 0;
}// End parseMB110_16D_DCON
//  -------------------------------------------------------------------------
int Driver::unpakOWEN(unsigned char adr){	// распаковка OWEN
//cout << "unpakOWEN: buf_len = " << buf_len << endl;
    if((buf_len < 16) || (buf_len >= MAXBUF)) return E_DN;
//cout << "прверить начало '#' и конец '0xD' в buf_r" << endl;
    if(( buf[0] != 0x23 ) || ( buf[buf_len-1] != 0xD ))return E_DN;
// заполнить owen_frame и прверить пердел символов 'G ... V'.
	int i, j;
    unsigned char frame[21];
//cout <<  "прверить перделы символов (G ... V)" << endl;
	for (i = 1, j = 0; i < buf_len-2;  i += 2, ++j){
		if(( 'G' > buf[i] ) && ( buf[i] > 'V' )) break;
		if(( 'G' > buf[i+1] ) && ( buf[i+1] > 'V' )) break;
		frame[j] = (buf[i] - 'G') << 4 | (buf[i+1] - 'G');
	}
    buf_len--;
    if( i != buf_len ) return E_DN;
    buf_len = (buf_len - 1) / 2;
//  --
// проверка адреса
	if(frame[0] != adr)  return E_CP;	//чужой пакет - неверный адресс
	if((frame[1] & 0x10) != 0) return E_CP;	//чужой пакет - признак запроса
// прверить hash ?  unsigned int hash = (frame[2] << 8) | frame[3];
//printf ("Hash: %x\n", hash);
// проверить KS
    buf_len--;
	unsigned short crc;
	crc = (frame[buf_len-1] << 8) | frame[buf_len];
	if( crc != ksumOwen(frame, buf_len-1)) return E_KS; // ошибка KS
//  --
	int dataSize = frame[1] & 0x0F;
    if( dataSize == 0 ) return E_WNO; // недоступны
//	memcpy(buf, frame + 4, dataSize);

    for(i = 0; i < dataSize; i++)buf[i] = frame[i + 4];
    buf[i] = 0;
    buf_len = i;
//  --
return 0;
}// End parseMB110_16D_OWEN
//  -------------------------------------------------------------------------
int Driver::unpakMB_ASC(unsigned char adr){	// распаковка MODBUS_ASCII
    if((buf_len < 11) || (buf_len >= MAXBUF)) return E_DN;
//cout << "прверить начало ':' и конец '0xD' 'oxA' в buf_r" << endl;
    if(( buf[0] != 0x3A ) || (buf[buf_len-1] != 0xA ) || (buf[buf_len-2] != 0xD ))return E_DN;
//cout << "Пакет - ОК.\n";
    buf_len = (buf_len - 3) / 2;
//cout << "Count: " << buf_len << endl;
    unsigned char frame[buf_len];
    for (int i = 0; i < buf_len; i++){
        unsigned char x = char2hex( buf[ i * 2 +1 ] );
        frame[i] = (x << 4) + char2hex( buf[ i * 2 + 2 ] );
    }
//printf("КS %x\n", frame[buf_len-1]);
//printf("КSx %x\n", ksumLRC(frame, buf_len-1));
    if( frame[buf_len-1] != ksumLRC(frame, buf_len-1)) return E_KS; // ошибка KS
//printf("Fr1 %x\n",frame[1] );
    if((frame[1] == 0X83) || (frame[1] == 0X84)) return E_WNO;
//printf("Fr2 %x\n",frame[2] );
    if((frame[2] < 1) || (frame[2] > MAXBUF)) return E_DN;   // Количество данных !!!!!!
//  --
    int i;
    for(i = 0; i < frame[2]; i++){
        buf[i] = frame[i + 3];
    }
    buf[i] = 0;
    buf_len = i;
//  --
return 0;
 }// End unpacMB_ASC
//  -------------------------------------------------------------------------
int Driver::unpakMB_ASC_B(unsigned char adr){	// распаковка MODBUS_ASCII_бозна
//cout << endl << "проверка корректности ответа" << endl;
    if((buf_len < 11) || (buf_len >= MAXBUF)) return E_DN;
//cout << "прверить начало ':' и конец '0xD' 'oxA' в buf_r" << endl;
    if(( buf[0] != 0x3A ) || (buf[buf_len-1] != 0xA ))return E_DN;
//cout << "Пакет - ОК.\n";
    buf_len = (buf_len - 3) / 2;
//cout << "Count: " << buf_len << endl;
    unsigned char frame[buf_len];
    for (int i = 0; i < buf_len; i++){
        unsigned char x = char2hex( buf[ i * 2 +1 ] );
        frame[i] = (x << 4) + char2hex( buf[ i * 2 + 2 ] );
//printf("%x ", frame[i]);
    }
//cout << endl;
//  --
//printf("КS %x\n", frame[buf_len-1]);
//printf("КSx %x\n", ksumLRC(frame, buf_len-1));
    if(frame[buf_len-1] != ksumLRC(frame, buf_len-1)) return E_KS; // ошибка KS
    if(frame[1] != 0X1D) return E_DN;
    if((frame[4] < 1) || (frame[4] > MAXBUF)) return E_DN;   // Количество данных !!!!!!
//  --
    int i;
    for(i = 0; i < frame[4]; i++){
        buf[i] = frame[i + 5];
//printf("Buf %x", buf[i]);
    }
//cout << endl;
    buf[i] = 0;
    buf_len = i;
//  --
return 0;
 }// End unpacMB_ASC_B
//  -------------------------------------------------------------------------
int Driver::unpakMB_RTU(unsigned char adr){	// распаковка MB_RTU
    buf_len--;
    unsigned short ks = (buf[buf_len] << 8) | buf[buf_len-1];
    if( ks != ksumCRC(buf, buf_len-1)) return E_KS; // ошибка KS
    if(buf[1] >= 0X80) return E_WNO;
//  --
    if((buf[2] < 1) || (buf[2] > MAXBUF)) return E_DN;   // Количество данных !!!!!!
    int i;
    for(i = 0; i < buf[2]; i++){
        buf[i] = buf[i + 3];
    }
    buf[i] = 0;
    buf_len = i;
return 0;
}// End unpacMB_RTU
//  -------------------------------------------------------------------------
int Driver::unpakXXX(unsigned char adr){	// заглушка
return 0;
}// End decoderXXX
//  -------------------------------------------------------------------------
/*************************************************************/

//  -------------------------------------------------------------------------
//              Вспомогательные функции
//  -------------------------------------------------------------------------
unsigned short Driver::ksumCRC(unsigned char *msg, int len){ // Контрольная сумма CRC16
    unsigned short ks = 0xFFFF; //KSUM
    int j = 0, k = 0;
    while(k < len){
        ks = ks ^ (short) msg[k];
        for( j=0; j < 8; j++){
            if(ks & 0x0001) ks = (ks >> 1) ^ 0xA001;
            else ks = ks >> 1;
        }
    k++;
    }
    return ks;
}// End ksumCRC
//  -------------------------------------------------------------------------
unsigned char Driver::ksumDCON(unsigned char *msg, int len){ // Контрольная сумма протокола DCON
    unsigned char ks = 0; //KSUM
    int j = 0;
    for(; j < len; j++ )ks = ks +  msg[j];
    return ks;
}// End ksum
//  -------------------------------------------------------------------------
unsigned char Driver::ksumLRC(unsigned char *msg, int len){ // Контрольная сумма LRC
    unsigned char ks = 0; //KSUM
    while(len>0){
        ks += *msg++;
        len--;
    }
return ((~ks) + 1);
}// End ksumLRC
//  -------------------------------------------------------------------------
unsigned short Driver::ksumOwen(unsigned char *msg, int len){// Контрольная сумма протокола OWEN
    int i, j;
    unsigned short crc;
//    assert(msg);
    crc = 0;
    for (i = 0; i < len; ++i){
        unsigned char b = msg[i];
        for (j = 0; j < 8; ++j, b <<= 1){
            if ((b ^ (crc >> 8)) & 0x80){
                crc <<= 1;
                crc ^= 0x8F57;
            } else crc <<= 1;
        }
    }
return crc;
}//  End ksumOwen
//  -------------------------------------------------------------------------
char Driver::char2hex(char c){
char rez = c - 0x30;
    if (rez > 9) rez = rez - 0x7;
    if (rez > 16) rez = rez - 0x20;
    if (rez > 16) rez = 0;
return rez;
}// End char2hex
//  -------------------------------------------------------------------------
char Driver::hex2char(char c){
    c = (c & 0x0f) + 0x30;
    if (c > '9' )c = c + 0x7;
return c;
}// End hex2char
//  -------------------------------------------------------------------------
/*************************************************************/
Driver::~Driver(){
//    delete[] idata;
}// End
