/***************************************************************************
 *  Simple Server_SD (SCADA)                                               *
 *  Copyright  (C)  2010-2013  by  Oleg Shirokov    olgshir@gmail.com      *
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; version 2 of the License, or             *
 *  (at your option) any later version.                                    *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//  --
#include <memory.h>
#include <assert.h>
//  --
#include "errors.h"
#include "devices.h"
//  -------------------------------------------------------------------------
unsigned short ksumCRC( unsigned char *msg, int len ); // Контрольная сумма CRC16
unsigned char ksumDCON( unsigned char *msg, int len ); // Контрольная сумма протокола DCON
unsigned char ksumLRC( unsigned char *msg, int len ); // Контрольная сумма LRC
unsigned short ksumOwen( unsigned char *msg, int len );// Контрольная сумма протокола OWEN
char char2hex( char c );
char hex2char( char c );
float get_floatOwen( unsigned char *pack, int len ); // ИЗ OWEN
float get_floatMB( unsigned char *pack, int len ); // ИЗ MODBUS
/***************************************************************************/
/*     Кодирование фреймов для передачи   */
//  -------------------------------------------------------------------------
static int coder_XXX( unsigned char adr, unsigned char *data, int len, unsigned char *cmd ){
return 0;
}//  End request_XXX()
//  -------------------------------------------------------------------------
static int coder_ELEMER( unsigned char adr, unsigned char *data, int len, unsigned char *cmd ){ // ELEMER
//printf( "coder_ELEMER: %d\n", len );
    int i = sprintf( (char*)cmd,":%d;", adr );
    if( len ) memcpy( cmd + i, data, len );
    i += len;
    i += sprintf( (char*)cmd + i, "%d", ksumCRC( cmd + 1, i -1 ));
    cmd[i] = 0xD;
    int cmd_len = i + 1;
    cmd[cmd_len] = 0;
return cmd_len;
} // End coder_ELEMER
//  -------------------------------------------------------------------------
static int coder_DCON( unsigned char adr, unsigned char *data, int len, unsigned char *cmd ){ // DCON
//printf( "coder_DCON: %d\n", len );
    if( adr < 0x10 ){
        sprintf( (char*)cmd,"@0%X", adr );
    } else{
        sprintf( (char*)cmd,"@%X", adr );
    }
    int i = 3;
    i += sprintf( (char*)cmd + i, "%X", ksumDCON( cmd, i ));
    cmd[i] = 0x0D;
    int cmd_len = i + 1;
    cmd[cmd_len] = 0;
return cmd_len;
} // End coder_DCON
//  -------------------------------------------------------------------------
static int coder_OWEN( unsigned char adr, unsigned char *data, int len, unsigned char *cmd ){ // OWEN
//printf( "coder_OWEN: %d\n", len );
// максимальная длина пакета согласно протоколу ОВЕН  = 21
    unsigned char frame[len + 3];
    frame[0] = adr;
    if( len ) memcpy( frame+1, data, len );
    unsigned short crc = ksumOwen( frame, len + 1 );
    frame[len+1] = ( crc >> 8 ) & 0xff;
    frame[len+2] = crc & 0xff;
    int frameSize = len + 3;
// максимальная длина ASCII-пакета включая маркеры и символ '\0' = 45;
    int i, j;
    cmd[0] = '#';
    for( i = 0, j = 1; i < frameSize; ++i, j += 2 ){
        cmd[j] = 'G' + (( frame[i] >> 4 ) & 0x0f );
        cmd[j + 1] = 'G' + ( frame[i] & 0x0f );
    }
    cmd[frameSize * 2 + 1] = '\xD';
    cmd[frameSize * 2 + 2] = '\0';
return (frameSize * 2 + 2);
} // End coder_OWEN
//  -------------------------------------------------------------------------
static int coder_MB_ASC( unsigned char adr, unsigned char *data, int len, unsigned char *cmd ){ // MODBUS_ASCII
//printf( "coder_MB_ASC: %d\n", len );
    unsigned char frame[len + 2 ];
    frame[0] = adr;
    if( len ) memcpy( frame + 1, data, len );
    len++;
    frame[len] = ksumLRC( frame, len );
    len++;
//  --
    int i, j;
    cmd[0] = ':';
    for( i = 0, j = 1; i < len; ++i, j += 2 ){
        cmd[j] = hex2char( frame[i] >> 4 );
        cmd[j + 1] = hex2char( frame[i] );
    }
    cmd[j] = 0xd;
    cmd[++j] = 0xa;
    cmd[++j] = 0;
return j;
} // End coder_MB_ASC
//  -------------------------------------------------------------------------
static int coder_MB_RTU( unsigned char adr, unsigned char *data, int len, unsigned char *cmd ){ // MODBUS_RTU
//printf( "coder_MB_RTU: %d\n", len );
    cmd[0] = adr;     // ADR
    if( len ) memcpy( cmd + 1, data, len );
    len++;
    unsigned short ks = ksumCRC( cmd, len );
    cmd[len + 1 ] = ( ks >> 8 ) & 0xff;
    cmd[len + 0 ] = ks & 0xff;
    int cmd_len = len + 2;
return cmd_len;
} // End coder_MB_RTU()
//  -------------------------------------------------------------------------
/***************************************************************************/



/***************************************************************************/
/*         Разбор ответа         */
//  -------------------------------------------------------------------------
static int decoder_XXX( unsigned char adr, unsigned char *buf, int len ){
return 0;
}// End decoder_XXX
//  -------------------------------------------------------------------------
static int decoder_ELEMER( unsigned char adr, unsigned char *buf, int len ){// распаковка ELEMER
    if( len < 7 ) return E_DRIVER;
    unsigned char *ptr = buf;
    ptr++;	// !!!!!!!!!!!!!!!!!!!!!!!!!
//fprintf(stderr, "%s\n", ptr );
    if( !( ptr[0] == '!' )) return E_DRIVER; // неответ
//  --
    int i;
    for( i = len - 1; ( ptr[i] != ';' ) && ( i > 3 ); i-- ){};
    if ( i < 4 ) return E_DRIVER;
    unsigned short tm_ks = ksumCRC( ptr + 1, i );
//  --
    i++;
    if(( len - i ) > 7 ) return E_DRIVER;
    char str[9];
//  --
    int a = 1, b = 0;
    memset( str, 0, sizeof( str ));
    for(; ( ptr[a] >= '0' ) && ( ptr[a] <= '9' ); a++, b++ ) str[b] = ptr[a];
    if( adr != (unsigned char)atoi( str )) return E_DRV_ID;
//  --
    int temp = i + 1;
    int j = 0;
    memset( str, 0, sizeof( str ));
    for( ;i < len; i++, j++ ) str[j] = ptr[i];
//  --
    if( tm_ks != atoi( str )) return E_DRV_KS;
//  --
    j = 0;
    for( i = a + 2; i < temp; i++, j++ ) buf[j] = buf[i];
    buf[j] = 0;
return (j - 1);
}// End decoder_ELEMER
//  -------------------------------------------------------------------------
static int decoder_DCON( unsigned char adr, unsigned char *buf, int len ){// распаковка DCON
    unsigned short tm_ks = 0;
    if( len < 5 ) return E_DRIVER;
    buf[7] = 0;
    tm_ks = ksumDCON( buf, 5 );
    unsigned char eks = char2hex( buf[5] );
    eks = ( eks << 4 ) + char2hex( buf[6] );
//printf("eKS: %x\n", eks);
//  --
    if( tm_ks != eks ) return E_DRV_KS;
//  --  --
    unsigned char x = char2hex( buf[1] );
    x = ( x << 4 ) | char2hex( buf[2] );
    buf[0] = x;
    x = char2hex( buf[3] );
    x = ( x << 4 ) | char2hex( buf[4] );
    buf[1] = x;
    buf[2] = 0;
return 2;
}// End decoder_DCON
//  -------------------------------------------------------------------------
static int decoder_OWEN( unsigned char adr, unsigned char *buf, int len ){// распаковка OWEN
//cout << "unpakOWEN: buf_len = " << buf_len << " byte. >> ";
    if( len < 16 ) return E_DRIVER;
//printf("Проверить начало '#'= %x  и завершение '0xD'= %x\n", buf[0], buf[buf_len-1]);
    if( buf[0] != 0x23 ) return E_DRIVER;
    if( !(( buf[len-1] == 0xA ) || ( buf[len-1] == 0xD ))) return E_DRIVER;
// заполнить owen_frame и прверить пердел символов 'G ... V'.
    int i, j;
    unsigned char frame[21];
//cout <<  "прверить пределы символов (G ... V)" << endl;
    for ( i = 1, j = 0; i < len - 2;  i += 2, ++j ){
        if(( 'G' > buf[i] ) && ( buf[i] > 'V' )) break;
        if(( 'G' > buf[i + 1] ) && ( buf[i + 1] > 'V' )) break;
        frame[j] = ( buf[i] - 'G' ) << 4 | ( buf[i + 1] - 'G' );
//printf("%x ", frame[j]);
    }
    len--;
    if( i != len ) return E_DRIVER;
    len = ( len - 1 ) / 2;
//  --
// проверка адреса
//printf("\nПроверка адреса  %x hex,  %d dec.\n", frame[0], frame[0]);
	if( frame[0] != adr )  return E_DRV_ID;	//чужой пакет - неверный адресс
	if(( frame[1] & 0x10 ) != 0 ) return E_DRV_FR;	//- признак запроса
// прверить hash ?  unsigned int hash = (frame[2] << 8) | frame[3];
//printf ("Hash: %x\n", hash);
// проверить KS
    len--;
    unsigned short crc;
    crc = ( frame[len - 1] << 8 ) | frame[len];
    if( crc != ksumOwen( frame, len - 1 )) return E_DRV_KS; // ошибка KS
//  --
    int dataSize = frame[1] & 0x0F;
    if( dataSize == 0 ) return E_WNO; // недоступны
    memcpy( buf, frame + 4, dataSize );

    for( i = 0; i < dataSize; i++ ) buf[i] = frame[i + 4];
    buf[i] = 0;
return i;
}// End decoder_OWEN
//  -------------------------------------------------------------------------
static int decoder_MB_ASC( unsigned char adr, unsigned char *buf, int len ){// распаковка MODBUS_ASCII
    if( len < 11 ) return E_DRIVER;
//cout << "прверить начало ':' и конец '0xD' 'oxA' в buf_r" << endl;
    if(( buf[0] != 0x3A ) || ( buf[len-1] != 0xA ) || ( buf[len-2] != 0xD )) return E_DRIVER;
//cout << "Пакет - ОК.\n";
    len = ( len - 3 ) / 2;
//cout << "Count: " << buf_len << endl;
    int i;
    unsigned char frame[len];
    for( i = 0; i < len; i++ ){
        unsigned char x = char2hex( buf[i * 2 + 1] );
        frame[i] = ( x << 4 ) + char2hex( buf[i * 2 + 2] );
    }
//printf("КS %x\n", frame[buf_len-1]);
//printf("КSx %x\n", ksumLRC(frame, buf_len-1));
    if( frame[len-1] != ksumLRC( frame, len - 1 )) return E_DRV_KS; // ошибка KS
//printf("Fr1 %x\n",frame[1] );
    if(( frame[1] == 0X83 ) || ( frame[1] == 0X84 )) return E_WNO;
//printf("Fr2 %x\n",frame[2] );
    if( frame[2] < 1 ) return E_DRV_FR;   // Количество данных !!!!!!
//  --
    for( i = 0; i < frame[2]; i++ ){
        buf[i] = frame[i + 3];
    }
    buf[i] = 0;
return i;
}// End decoder_MB_ASC
//  -------------------------------------------------------------------------
static int decoder_MB_ASC_B( unsigned char adr, unsigned char *buf, int len ){// распаковка MODBUS_ASCII_бозна
//cout << endl << "проверка корректности ответа" << endl;
    if( len < 11 ) return E_DRIVER;
//cout << "прверить начало ':' и конец '0xD' 'oxA' в buf_r" << endl;
    if(( buf[0] != 0x3A ) || ( buf[len-1] != 0xA )) return E_DRIVER;
//cout << "Пакет - ОК.\n";
    len = ( len - 3 ) / 2;
//cout << "Count: " << buf_len << endl;
    int i;
    unsigned char frame[len];
    for( i = 0; i < len; i++ ){
        unsigned char x = char2hex( buf[ i * 2 +1 ] );
        frame[i] = ( x << 4 ) + char2hex( buf[ i * 2 + 2 ] );
//printf("%x ", frame[i]);
    }
//cout << endl;
//  --
//printf("КS %x\n", frame[buf_len-1]);
//printf("КSx %x\n", ksumLRC(frame, buf_len-1));
    if( frame[len-1] != ksumLRC( frame, len-1 )) return E_DRV_KS; // ошибка KS
    if( frame[1] != 0X1D ) return E_DRV_FR;
    if( frame[4] < 1 ) return E_DRV_FR;   // Количество данных !!!!!!
//  --
    for( i = 0; i < frame[4]; i++ ){
        buf[i] = frame[i + 5];
//printf("Buf %x", buf[i]);
    }
//cout << endl;
    buf[i] = 0;
return i;
}// End decoder_MB_ASC_B
//  -------------------------------------------------------------------------
static int decoder_MB_RTU( unsigned char adr, unsigned char *buf, int len ){// распаковка MB_RTU
    len--;
    unsigned short ks = ( buf[len] << 8 ) | buf[len-1];
    if( ks != ksumCRC( buf, len - 1 )) return E_DRV_KS; // ошибка KS
    if( buf[1] >= 0X80 ) return E_WNO;
//  --
    if( buf[2] < 1 ) return E_DRV_FR;   // Количество данных !!!!!!
    int i;
    for( i = 0; i < buf[2]; i++ ){
        buf[i] = buf[i + 3];
    }
    buf[i] = 0;
return i;
}// End decoder_MB_RTU
//  -------------------------------------------------------------------------



/***************************************************************************/
static int get_data_XXX( int poz, unsigned char *buf, int len ){	// XXX
return E_DRV_NO;
}// End det_data_XXX
//  -------------------------------------------------------------------------
static int get_data_TM513X( int poz, unsigned char *buf, int len ){	// TM513X
//printf( "get_data_TM513X\n" );
    int xss = E_DEVICE;	// 21
    char st[10];
    st[0] = 'x';
    st[1] = '\0';

    int i = 0, j = 0;
    for( ; i <= len; i++ ){
        if( buf[i]==';' ) j++;
        if( j == poz ) break;
    }
    i++;
    if( poz == 0 ) i = 0;
    if( i >= len ) return E_DEV_DN;	//20

    j = 0;
    for( ; (( buf[i] >= '0' ) && ( buf[i] <= '9' )) || ( buf[i] == '.' ) || ( buf[i] == '$' ) || ( buf[i] == '-' ); i++, j++ ){
        if( i >= len )break;
        st[j] = buf[i];
    }
    st[j] = '\0';
//printf( "%s\n", st );
    if(( st[0] == '-' ) || (( st[0] >= '0' ) && ( st[0] <= '9' ))) xss = (int)( atof( st ) * 100 );
    if(st[0] == '$'){
        if( st[1] == '0' ) xss = E_ZER; //неисправен 
        if( st[1] == '1' ) xss = E_KZ; 
        if( st[1] == '3' ) xss = E_NO;  //нет канала
        if( st[1] == '6' ) xss = E_WNO; //недоступны
        if( st[1] == '8' ) xss = E_OBR;
        if( st[1] == '9' ) xss = E_ZER; //неисправен
    }
return xss;
}// End det_data_TM513X
//  -------------------------------------------------------------------------
static int get_data_TRM101( int poz, unsigned char *buf, int len ){	// TRM101
    if( len != 3 ) return E_DEVICE;	// можно идентифицировать ошибку устройства
//  --
    float fx = get_floatOwen( buf, len );
return (int)( fx * 100 );
}// End det_data_TRM101
//  -------------------------------------------------------------------------
static int get_data_TRM138( int poz, unsigned char *buf, int len ){	// TRM138
//int i;
//for( i = 0; i < len; i++ ) printf(" %x", buf[i]);
//printf("\n");
//  --
    if( len > 6 ) return E_DEVICE;	// можно идентифицировать ошибку устройства
//  --
    float fx = get_floatOwen( buf, 3 );
return (int)( fx * 100 );
}// End det_data_TRM138
//  -------------------------------------------------------------------------
static int get_data_NORD_Z3M( int poz, unsigned char *buf, int len ){	//НОРД-З3М
    float fx = get_floatMB( &buf[( poz * 4 )], 4 );
return (int)( fx * 100 );
}// End det_data_NORD_Z3M
//  -------------------------------------------------------------------------
static int get_data_MB110_16D( int poz, unsigned char *buf, int len ){
    if( len != 0x2 ) return E_DEVICE;   // Количество данных !!!!!!
    unsigned short x = buf[0];
    x = ( x << 8 ) | buf[1];
    x = x >> poz;
return (int)( x & 0x0001 );
}// End det_data_MB110_16D
//  -------------------------------------------------------------------------
static int get_data_MB110_16D_dcon( int poz, unsigned char *buf, int len ){
return 0x1 ^ get_data_MB110_16D( poz, buf, len ); // если DCON то инвертируем
}// End det_data_MB110_16D
//  -------------------------------------------------------------------------
//static int get_data_( int poz, unsigned char *buf, int len ){
//return 0;
//}// End det_data_
//  -------------------------------------------------------------------------


/***************************************************************************/
void init_device( Device *d, char *name, int id, int adr ){
    d->err = E_DEVICE;
    d->name = name;
    d->id = id;
    d->adr = adr;
//    d->modif = 100;
//  --
// зависит от устройства
    if( strcasecmp( name, "EL_TM5132" ) == 0 ){			// ЭЛЕМЕР ТМ5132
        d->reg = 4;
        d->frm_len = sprintf( (char*)d->frm,"6;0;%d;", d->reg );
        d->coder_frm = &coder_ELEMER;
        d->decoder = &decoder_ELEMER;
        d->get_data = &get_data_TM513X;
//  --
    } else if( strcasecmp( name, "EL_TM5133" ) == 0 ){		// ЭЛЕМЕР ТМ5133
        d->reg = 8;
        d->frm_len = sprintf( (char*)d->frm,"6;0;%d;", d->reg );
        d->coder_frm = &coder_ELEMER;
        d->decoder = &decoder_ELEMER;
        d->get_data = &get_data_TM513X;
//  --
    } else if( strcasecmp( name, "OW_TRM101" ) == 0 ){		// ОВЕН ТРМ101
        d->reg = 1;
        d->frm[0] = 0;		// внутренний адресс 4-бита
        d->frm[0] |= 0x10;	// признак запроса
        d->frm[0] |= 0;		// dataSize;
        d->frm[1] = 0xB8;	// (hash >> 8) & 0xff;
        d->frm[2] = 0xDF;	// hash & 0xff;
        d->frm_len = 3; 
        d->coder_frm = &coder_OWEN;
        d->decoder = &decoder_OWEN;
        d->get_data = &get_data_TRM101;
//  --
    } else if( strcasecmp( name, "OW_TRM138" ) == 0 ){		// ОВЕН ТРМ138
        d->reg = 1;
        d->frm[0] = 0;		// внутренний адресс 4-бита
        d->frm[0] |= 0x10;	// признак запроса
        d->frm[0] |= 0;		// dataSize;
//    hash = 0x8784 - "rEAd" | 0x2D26 - "r.Cn" | 0x2517 - "r.Cou" 
        d->frm[1] = 0x87;	// (hash >> 8) & 0xff;
        d->frm[2] = 0x84;	// hash & 0xff;
        d->frm_len = 3; 
        d->coder_frm = &coder_OWEN;
        d->decoder = &decoder_OWEN;
        d->get_data = &get_data_TRM138;
//  --
    } else if( strcasecmp( name, "OW_TRM138_MB" ) == 0 ){	// ОВЕН ТРМ138 МОДБАС
        d->reg = 8;
        d->frm[0] = 0x4;	// Func 03|04;
        d->frm[1] = 0x0;	// hi reg;
        d->frm[2] = 0x0;	// lo reg
        d->frm[3] = 0;		// hi;
        d->frm[4] = 27;		// lo;
        d->frm_len = 5;
        d->coder_frm = &coder_MB_ASC;
        d->decoder = &decoder_MB_ASC;
        d->get_data = &get_data_TRM138;
//  --
    } else if( strcasecmp( name, "NORD_Z3M" ) == 0 ){		// НОРД-З3М
        d->reg = 2;
        d->frm[0] = 0x1D;	// Func
        d->frm[1] = 0x24;	// hi reg;
        d->frm[2] = 0x00;	// lo reg
        d->frm[3] = 4 * d->reg;	// lo;
        d->frm_len = 4;
        d->coder_frm = &coder_MB_ASC;
        d->decoder = &decoder_MB_ASC_B;
        d->get_data = &get_data_NORD_Z3M;
//  --
    } else if( strcasecmp( name, "OW_MB110_16D" ) == 0 ){		// MB110_16D
        d->reg = 16;
        d->frm[0] = 0;		// внутренний адресс 4-бита
        d->frm[0] |= 0x10;		// признак запроса
        d->frm[0] |= 0;		// dataSize;
////    hash = 0x2D26;		// 0x2D26 - "r.Cn" | 0x2517 - "r.Cou" 
        d->frm[1] = 0x2D;		// 2d (hash >> 8) & 0xff;
        d->frm[2] = 0x26;		// 26  hash & 0xff;
//	if ( dataSize ) memcpy( frame+4, data, dataSize ); - данные не передаём
        d->frm_len = 3; 
        d->coder_frm = &coder_OWEN;
        d->decoder = &decoder_OWEN;
        d->get_data = &get_data_MB110_16D;
//  --
    } else if( strcasecmp( name, "OW_MB110_16D_MB" ) == 0 ){		// MB110_16D ModBus
        d->reg = 16;
        d->frm[0] = 0x3;	// Func 03|04;
        d->frm[1] = 0x0;	// hi reg;
        d->frm[2] = 0x33;	// lo reg
        d->frm[3] = 0;		// hi;
        d->frm[4] = 1;		// lo;
        d->frm_len = 5;
        d->coder_frm = &coder_MB_ASC;
        d->decoder = &decoder_MB_ASC;
        d->get_data = &get_data_MB110_16D;
//  --
    } else if( strcasecmp( name, "OW_MB110_16D_DC" ) == 0 ){		// MB110_16D DCON
        d->reg = 16;
        d->frm[0] = 0;
        d->frm_len = 0;
        d->coder_frm = &coder_DCON;
        d->decoder = &decoder_DCON;
        d->get_data = &get_data_MB110_16D_dcon;
//  --
//    } else if( strcasecmp( name, "" ) == 0 ){
//        d->reg = ;
//        d->cmd_len = ;
//        d->coder_frm = &coder_;
//        d->decoder = &decoder_;
//        d->get_data = &get_data_;
//  --
    } else {
        d->reg = 0;
        d->frm[0] = 0;
        d->frm_len = 0;
        d->coder_frm = &coder_XXX;
        d->decoder = &decoder_XXX;
        d->get_data = &get_data_XXX;
//  --
    }
}// End init_device()
/***************************************************************************/

//  -------------------------------------------------------------------------
//              Вспомогательные функции
//  -------------------------------------------------------------------------
unsigned short ksumCRC( unsigned char *msg, int len ){ // Контрольная сумма CRC16
    unsigned short ks = 0xFFFF; //KSUM
    int j = 0, k = 0;
    while( k < len ){
        ks = ks ^ (short)msg[k];
        for( j = 0; j < 8; j++ ){
            if( ks & 0x0001 ) ks = ( ks >> 1 ) ^ 0xA001;
            else ks = ks >> 1;
        }
    k++;
    }
    return ks;
}// End ksumCRC
//  -------------------------------------------------------------------------
unsigned char ksumDCON( unsigned char *msg, int len ){ // Контрольная сумма протокола DCON
    unsigned char ks = 0; //KSUM
    int j = 0;
    for(; j < len; j++ ) ks = ks +  msg[j];
    return ks;
}// End ksum
//  -------------------------------------------------------------------------
unsigned char ksumLRC( unsigned char *msg, int len ){ // Контрольная сумма LRC
    unsigned char ks = 0; //KSUM
    while( len > 0 ){
        ks += *msg++;
        len--;
    }
return (( ~ks ) + 1 );
}// End ksumLRC
//  -------------------------------------------------------------------------
unsigned short ksumOwen( unsigned char *msg, int len ){// Контрольная сумма протокола OWEN
    int i, j;
    unsigned short crc;
    crc = 0;
    for( i = 0; i < len; ++i ){
        unsigned char b = msg[i];
        for( j = 0; j < 8; ++j, b <<= 1 ){
            if(( b ^ ( crc >> 8 )) & 0x80 ){
                crc <<= 1;
                crc ^= 0x8F57;
            } else crc <<= 1;
        }
    }
return crc;
}//  End ksumOwen
//  -------------------------------------------------------------------------
char char2hex( char c ){
    char rez = c - 0x30;
    if ( rez > 9 ) rez = rez - 0x7;
    if ( rez > 16 ) rez = rez - 0x20;
    if ( rez > 16 ) rez = 0;
return rez;
}// End char2hex
//  -------------------------------------------------------------------------
char hex2char( char c ){
    c = ( c & 0x0f ) + 0x30;
    if ( c > '9' ) c = c + 0x7;
return c;
}// End hex2char
//  -------------------------------------------------------------------------
float get_floatOwen( unsigned char *pack, int len ){ // ИЗ OWEN
    if(( len < 3 ) || ( len > 4 ) || ( pack == NULL )) return E_DEV_DN;
    unsigned int x = pack[0];
    int i;
    for( i = 1; i < len; i++ ) x = ( x << 8 ) | pack[i];
    if( len == 3 )x = ( x << 8 ) | 0;
//  --
    float *pfx = (float*)&x;
    float fx = *pfx;
    return fx;
}// End det_floatOwen()
//  -------------------------------------------------------------------------
float get_floatMB( unsigned char *pack, int len ){ // ИЗ MODBUS
    if(( len < 1 ) || ( len > 4 ) || ( pack == NULL )) return E_DEV_DN;
    len--;
    unsigned int x = pack[len];
    for( ; len >= 0; len-- ) x = ( x << 8 ) | pack[len];
//  --
    float *pfx = (float*)&x;
    float fx = *pfx;
return fx;
}// End det_floatMB()
//  -------------------------------------------------------------------------
/*char Device::getParam(int poz){
    if(poz < 1)return ';';
    int i, j = 0;
    for(i = 0; i <= (int)param.size(); i++){
        if(param[i]==';') j++;
        if(j == poz)break;
    }
    if( i >= (int)param.size() )return ';';
    return param[i-1];
}// End getParam */
/***************************************************************************/
