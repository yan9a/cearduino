/////////////////////////////////////////////////////////////////////////////
// Name:         ceModbus.cpp
// Description:  Byte stuffing- sending and receiving frames
//  Modbus RTU frame format (primarily used on asynchronous serial data lines like RS-485/EIA-485)
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com
/////////////////////////////////////////////////////////////////////////////
#include<stdio.h>
#include <stdint.h>
#include "ceModbus.h"

int TxN;//number of transmitting bytes
int RxN;//number of receiving bytes
char tb[CE_FRAME_TX_BUF_SIZE];//transmit buffer
char rb[CE_FRAME_RX_BUF_SIZE];//receiving data
int _count; // receiving count
int _frameSize;// frame size
int _tick_n;
int _reset_tick;
//-----------------------------------------------------------------------------
void ceFrame()
{
  TxN=0;
  RxN=0;
  _count = 0;
  _frameSize = 8;
  _tick_n = 0;
  _reset_tick = 10;
}
//-----------------------------------------------------------------------------
char* ceFraGetTxBuf() {
  return tb;
}
//-----------------------------------------------------------------------------
char* ceFraGetRxBuf() {
  return rb;
}
//-----------------------------------------------------------------------------
//Prepare transmitting frame
int ceFraSet(char* d, int n)
{
  if (n > (CE_FRAME_TX_BUF_SIZE - 2)) return 0;// size error
  unsigned int txcrc = 0xFFFF;//initialize crc
  char c;
  int i = 0, j = 0;
  for (j = 0; j < n; j++) {
    c = d[j];
    tb[i++] = c;
  }
  txcrc = ceFraCRC16(d, n, txcrc);//calculate crc
  tb[i++] = txcrc & 0xFF;
  tb[i++] = (txcrc >> 8) & 0xFF;
  TxN = i;
  return TxN;
}
//-----------------------------------------------------------------------------
//Inputs
//s : pointer to input char string
//len: string len (maximum 255)
//crc: initial CRC value

//Output
//Returns calculated CRC
unsigned int ceFraCRC16(char* s, unsigned char len, unsigned int crc)
{
  //CRC Order: 16
  //CRC Poly: 0x8005 <=> A001
  //Operational initial value:  0xFFFF
  //Final xor value: 0
  unsigned char i, j;
  for (i = 0; i < len; i++, s++) {
    crc ^= ((unsigned int)(*s)) & 0xFF;
    for (j = 0; j < 8; j++){
      if (crc & 0x0001) crc = (crc >> 1) ^ 0xA001;
      else crc >>= 1;
    }
  }
  return (crc & 0xFFFF);//truncate last 16 bit
}
//-----------------------------------------------------------------------------
//get number of transmitting bytes
int ceFraGetTxN()
{
  return TxN;
}
//-----------------------------------------------------------------------------
//get number of transmitting bytes
int ceFraGetRxN()
{
  return RxN;
}
//-----------------------------------------------------------------------------
// process receiving char
// return RXN if a frame is successfully received, else retrun 0
int ceFraProcess(char c)
{
  _tick_n = 0;
  RxN = 0;
  rb[_count++] = c;
//  if (_count == 3) {
//    // check function
//    if (rb[1] == 0x06) {
//      _frameSize = 8; // reply for control cmd
//    }
//    else if (rb[1] == 0x03) {
//      uint8_t len = (uint8_t)rb[2];
//      _frameSize = 5 + len; // reply for read cmd
//    }
//  }

  // instead use frame size for particular frame
  if (_count >= _frameSize) {
    rb[_count] = 0;//null termination
    RxN = _count;
    _count = 0;
    _frameSize = 8;// default
    _tick_n = 0; // will clear RxN when time out
    // for (int i = 0; i < RxN; i++) printf("%02X ", (unsigned int)rb[i] & 0xFF);
    // uint16_t rxcrc = ((uint16_t)rb[RxN-1] << 8 | ((uint16_t)rb[RxN - 2] & 0xFF)) & 0xFFFF;//get received crc
    uint16_t computed_crc = 0xFFFF;//initialize CRC
    computed_crc = ceFraCRC16(rb, RxN, computed_crc);//calculate crc
    // printf("\nComputed crc: %02X \n",computed_crc);
    //if(RxN==8) { 
    if (computed_crc == 0) { 
      // RxN -= 2; 
      return (RxN); 
    }//if crc is correct return bytes // excluding crc           
    else { RxN = 0; }//discard the frame
  }
  return 0;  
}

int Tick() 
{
  if (_tick_n <= _reset_tick) {
    _tick_n++;
  }
  else {
    _count = 0;
    _frameSize = 8;// default
    RxN = 0;
  }
  return _tick_n; 
}
