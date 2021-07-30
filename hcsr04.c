#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int

sbit k2=P3^2;           //按键K2
sbit beep=P3^7;
sbit A1=P2^1;           // A1、A2、A3、A4分别对应四个数码管
sbit A2=P2^2;
sbit A3=P2^3;
sbit A4=P2^4;
sbit Trig=P1^0;
sbit Echo=P1^1;

uchar DisplayData[4]={0xBF,0xBF,0xBF,0xBF};  //初始显示
uchar digcode[11]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF};
uchar digcode_doc[11]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0xFF};
uchar a[5];
uint time, flag;
float dis;

/**********声明部分************/
void keyboard_delay(uchar k_i);        //对按键延时函数的函数声明
void Display();                        //对数码管显示的函数声明
void refresh();                        //对数码管刷新缓存区的函数声明
void delay_20us();                     //对20usTTL脉冲延时的函数声明
/******************************/

/************主函数************/
void main()
{
	while(1)          
	{
		Display();
		if(k2==0)            //检测按键是否按下
		{
			keyboard_delay(20);		 //消抖
			if(k2==0)
			{
				while(!k2);	    //等待按键松开
				beep=0;         //采用蜂鸣器作为测距过程中的提示
				Trig=0;
				TMOD=0x01;
				EA=1;
				ET0=1;
				TH0=0;
				TL0=0;
				Trig=1;
				delay_20us();
				Trig=0;
				while(Echo==0);  //等待Echo端输出高电平
				TR0=1;           //开启定时器0
				while(Echo==1);  //等待Echo端高电平结束
				TR0=0;           //关闭定时器0
				EA=0;
				time=TH0*256+TL0;
				TH0=0;
				TL0=0;
				dis=time*0.0172;   //计算距离
				beep=1;
			}
			refresh();
		}
	}
}

/*****************************/

/********按键延时函数*******/
void keyboard_delay(uchar k_i)    
{
	uchar k_j,k_k;
	for(k_j=k_i;k_j>0;k_j--)
	for(k_k=125;k_k>0;k_k--);
}
/***************************/

/********拆分距离函数********/         //将测得距离百、十、个位等拆分
void chaifen(float Dis)
{
  Dis=(int)Dis;
	a[0]=Dis/100;
	a[1]=(Dis-a[0]*100)/10;
	a[2]=Dis-a[0]*100-a[1]*10;
	a[3]=(dis-Dis)*10;
	if (a[0]==0)
	{
		a[0]=10;
		if(a[1]==0)
			a[1]=10;
	}
}
/****************************/

/******数码管缓存区刷新******/
void refresh()
{
	uchar r_i;
	chaifen(dis);
	if((dis>400)||(dis<2)||(flag==1))
	{
		flag=0;
	  DisplayData[0]=0x86;	   //E
	  DisplayData[1]=0x86;	   //E
	  DisplayData[2]=0x88;	   //R
	  DisplayData[3]=0xC0;	   //O
	}
	else
	{
	for(r_i=0;r_i<4;r_i++)
	{
		if (r_i==2)
			DisplayData[r_i]=digcode_doc[a[r_i]];
		else
			DisplayData[r_i]=digcode[a[r_i]];
  }
  }
}
/****************************/

/**********数码管显示********/
void Display()
{
	uchar a_i;
	unsigned int a_j;
	for(a_i=0;a_i<4;a_i++)
	{
		switch(a_i)
		{
			case 0 :
				A1=0;A2=1;A3=1;A4=1; break;
			case 1 :
				A1=1;A2=0;A3=1;A4=1; break;
			case 2 :
				A1=1;A2=1;A3=0;A4=1; break;
			case 3 :
				A1=1;A2=1;A3=1;A4=0; break;
		}
		P0=DisplayData[a_i];
		a_j=10;
		while(a_j--);	
		P0=0xFF; //消隐
	}
}
/*****************************/

/********20usTTL脉冲延时******/
void delay_20us()
{
	uchar ttl;
	for(ttl=0;ttl<100;ttl++);
}
/*****************************/

/************溢出中断*********/
void T0_time() interrupt 1
{
	flag=1;
}
/*****************************/