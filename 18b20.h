#ifndef __18B20_H__
#define __18B20_H__
#define uchar unsigned char
#define uint unsigned int
void delay(uint a);
void init();
void write_byte(uchar dat);
uint read_byte();
uint read_temp();
#endif