/////////////////////////////////////////////////////////////////////////////
// Name:         ceModbus.h
// Description:  Sending and receiving modbus frames
//  Modbus RTU frame format (primarily used on asynchronous serial data lines like RS-485/EIA-485)
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com
/////////////////////////////////////////////////////////////////////////////

#ifndef CE_MODBUS_H
#define CE_MODBUS_H
#include <stdio.h>

//-----------------------------------------------------------------------------

#define STX 0x02
#define ETX 0x03
#define DLE 0x10

#define CE_FRAME_TX_BUF_SIZE 128
#define CE_FRAME_RX_BUF_SIZE 128

//-----------------------------------------------------------------------------

void ceFrame();
int ceFraSet(char* d, int n);
unsigned int ceFraCRC16(char* s, unsigned char len, unsigned int crc);
int ceFraGetTxN();
int ceFraGetRxN();
int ceFraProcess(char c);//get receiving frame from received char
char* ceFraGetTxBuf();
char* ceFraGetRxBuf();
int Tick();

#endif // CE_MODBUS_H
