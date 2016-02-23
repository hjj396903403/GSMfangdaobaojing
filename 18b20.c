#include<reg52.h>
#include"18b20.h"
#include"intrins.h"
#define uchar unsigned char
#define uint unsigned int
sbit DQ=P3^3;

void delay(uint a)				//10us
{
	uchar i;
	for(i=0;i<a;i++)
	{
		_nop_();		
	}	
}
void init()
{
	DQ=1;
	delay(1);
	DQ=0;
	delay(60);
	DQ=1;
	delay(15);
	while(DQ)
	{
		delay(200);
		break;
	}
		
}
void write_byte(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
	 DQ=1;
	 _nop_();
	 DQ=0;
	 
	 DQ=dat&0x01;
	 delay(5);
	 DQ=1;
	 dat=dat>>1;
	 
	 }
}
/*
uint read_byte()
{
	unsigned char r_value,i,j;
	for(j=0;j<8;j++)
	{	
	  DQ=1;
		for(i=0;i<0;i++);
	  DQ=0;
		for(i=0;i<0;i++);
		DQ=1;
		r_value>>=1;
		if(DQ)
		{
				r_value|=0x80;
		}
		for(i=0;i<8;i++); 
	}
	return r_value;
}
*/
uint read_byte()
{
	uchar i,dat,byte;
	dat=dat<<7;
	for(i=0;i<8;i++)
	{
		DQ=1;
		_nop_();
		DQ=0;
		delay(1);
		DQ=1;	
		byte=DQ;
		dat=(dat>>1)|(byte<<7);
		delay(5);		
	}
	return dat;
}

uint read_temp()
{
	uint temph,templ,tempdat,retemp;
	float temp;
	init();
	write_byte(0xcc);
	write_byte(0x44);
	while(!DQ);
	init();
	write_byte(0xcc);
	write_byte(0xbe);
	templ=read_byte();
	temph=read_byte();
	tempdat=temph<<8;
	tempdat=tempdat | templ;
	temp=tempdat;
	temp=temp*0.0625;
	retemp=(uint)(temp*100);
	return retemp;
}