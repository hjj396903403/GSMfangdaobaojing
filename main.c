#include<reg52.h> 
#include"18b20.h"
#define uchar unsigned char 
#define uint unsigned int 
uchar tabel1[]={~0x3f,~0x06,~0x5b,~0x4f,~0x66,~0x6d,~0x7d,~0x07,~0x7f,~0x6f,~0x40,~0x00};
uchar tabel2[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar keyflag,tp,bp,shi,ge,dingwen,bflag,qq,pppp;
uint temp,temp2;
uchar d[4],dd[4]; 
sbit beep=P3^7;
sbit key1=P1^4;
sbit key2=P1^5;
sbit key3=P1^6;
sbit key4=P1^7;


void delay1(uint a)
{
	uint i;
	for(;a>0;a--)
	for(i=6245;i>0;i--);
}
void delay2(uint a)
{
	uchar i;
	for(;a>0;a--)
	for(i=19;i>0;i--);
}

void uart1send(uchar c)
{
	SBUF=c;
	while(!TI);//等待发送完成信号（TI=1）出现
	TI=0;	
}
void uartsend(uchar *str)
{
	while(*str!='\0')
	{
		SBUF=*str;
		while(!TI);
		TI=0;
		str++;
	}
}
void display()
{
	uchar i;
	d[0]=temp/1000;
	d[1]=(temp%1000)/100;
	d[2]=(temp%100)/10;
	d[3]=temp%10;
	dd[0]=shi;
	dd[1]=ge;
	dd[2]=11;
	dd[3]=bp;
	for(i=0;i<4;i++)
	{
		if(keyflag==0)
		{
			P0=tabel1[d[i]];
			if(i==1)
			{
				P0=P0&0x7f;
			}
		}
		if((keyflag==1)||(keyflag==2))
		{
		 P0=tabel1[dd[i]];
		 }
		P2=tabel2[i];
		if((tp==0)&&(keyflag==1))
		{
			P2=P2|0x01;
		}
		if((tp==0)&&(keyflag==2))
		{
			P2=P2|0x02;
		}
		delay2(15);
//		P2=0xff;
	}	
}
void key()
{
	if(key1==0)
	{
		delay2(1);
		if(key1==0)
		{
			display();
			bflag=1;
			uartsend("at\r\n");
			delay1(5);
			uartsend("at+cmgf=1\r\n");
			delay1(5);
			uartsend("at+csmp=17,167,2,25\r\n");
			delay1(5);
			uartsend("at+cscs=\"UCS2\"\r\n");	
			delay1(5);
			uartsend("at+cmgs=\"00310038003800370034003200320034003200390034\"\r\n");
			delay1(5);
			uartsend("5BB691CC67655C0F50775566FF0C5FEB56DE5BB6");
			delay1(5);
			uart1send(0x1a);
			
			delay1(50);
			uartsend("at\r\n");
			delay1(5);
			uartsend("at\r\n");
			delay1(1);
			uartsend("ate1\r\n");
			delay1(5);			
			uartsend("at+colp=1\r\n");
			delay1(5);
			uartsend("atd18874224294;\r\n");	
			while(!key1)display();
		}
	}
	if(key2==0)
	{
		delay2(10);
		if(key2==0)
		{
			keyflag++;
			if(keyflag==3)
			{
				keyflag=0;
			}
			while(!key2)display();
		}
	}
	if(keyflag==1)
	{
		if(key3==0)
		{
			delay2(1);
			if(key3==0)
			{
				shi++;
				if(shi==10)
				{
					shi=0;
				}
				while(!key3)display();
			}
		}	
	}
	if(keyflag==2)
	{
		if(key3==0)
		{
			delay2(1);
			if(key3==0)
			{
				ge++;
				if(ge==10)
				{
					ge=0;
				}
				while(!key3)display();
			}
		}	
	}
	if(bflag==1)
	{
		if(key4==0)
		{
			delay2(1);
			if(key4==0)
			{
				bflag=0;
				while(!key4)display();
			}
		}
	}
	if(bflag==0)
	{
		if(key4==0)
		{
			delay2(1);
			if(key4==0)
			{
				bp++;
				if(bp==1)
				{
					pppp=0;
				}
				if(bp==2)
				{
					bp=0;
				}
				while(!key4)display();
			}
		}
	}
}

void main()
{	
	
	TMOD=0x21;
	TH1=253;
	TL1=253;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	ES=1;
	EA=1;
	SCON=0x50;
	TR1=1;
	ET0=1;
	TR0=1;
	while(1)
	{	
		dingwen=shi*10+ge;
		temp2=(uint)(temp/100);	
		temp=read_temp(); 
		key();
   		display();
			
   		if((pppp==0)&&(temp2>=dingwen))
		{
			pppp++;
			uartsend("at\r\n");
			delay1(5);
			uartsend("at+cmgf=1\r\n");
			delay1(5);
			uartsend("at+csmp=17,167,2,25\r\n");
			delay1(5);
			uartsend("at+cscs=\"UCS2\"\r\n");	
			delay1(5);
			uartsend("at+cmgs=\"00310038003800370034003200320034003200390034\"\r\n");
			delay1(5);
			uartsend("5BB691CC53D1751F706B707E4E86FF01FF01FF01FF01FF01FF01");
			delay1(5);
			uart1send(0x1a);
		}

		if((bflag==1)||((bp==1)&&(temp2>=dingwen)))
		{
			display();
			beep=0;
			delay1(1);
			beep=1;
		}		
	//	uartsend("at+cmgf=1\r");	
	}
}
void time() interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	qq++;
	if(qq==10)
	{
		qq=0;
		tp=~tp;
	}
}
void chuan() interrupt 4
{
	RI=0;
}
