#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int

sbit k2=P3^2;           //����K2
sbit beep=P3^7;
sbit A1=P2^1;           // A1��A2��A3��A4�ֱ��Ӧ�ĸ������
sbit A2=P2^2;
sbit A3=P2^3;
sbit A4=P2^4;
sbit Trig=P1^0;
sbit Echo=P1^1;

uchar DisplayData[4]={0xBF,0xBF,0xBF,0xBF};  //��ʼ��ʾ
uchar digcode[11]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF};
uchar digcode_doc[11]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0xFF};
uchar a[5];
uint time, flag;
float dis;

/**********��������************/
void keyboard_delay(uchar k_i);        //�԰�����ʱ�����ĺ�������
void Display();                        //���������ʾ�ĺ�������
void refresh();                        //�������ˢ�»������ĺ�������
void delay_20us();                     //��20usTTL������ʱ�ĺ�������
/******************************/

/************������************/
void main()
{
	while(1)          
	{
		Display();
		if(k2==0)            //��ⰴ���Ƿ���
		{
			keyboard_delay(20);		 //����
			if(k2==0)
			{
				while(!k2);	    //�ȴ������ɿ�
				beep=0;         //���÷�������Ϊ�������е���ʾ
				Trig=0;
				TMOD=0x01;
				EA=1;
				ET0=1;
				TH0=0;
				TL0=0;
				Trig=1;
				delay_20us();
				Trig=0;
				while(Echo==0);  //�ȴ�Echo������ߵ�ƽ
				TR0=1;           //������ʱ��0
				while(Echo==1);  //�ȴ�Echo�˸ߵ�ƽ����
				TR0=0;           //�رն�ʱ��0
				EA=0;
				time=TH0*256+TL0;
				TH0=0;
				TL0=0;
				dis=time*0.0172;   //�������
				beep=1;
			}
			refresh();
		}
	}
}

/*****************************/

/********������ʱ����*******/
void keyboard_delay(uchar k_i)    
{
	uchar k_j,k_k;
	for(k_j=k_i;k_j>0;k_j--)
	for(k_k=125;k_k>0;k_k--);
}
/***************************/

/********��־��뺯��********/         //����þ���١�ʮ����λ�Ȳ��
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

/******����ܻ�����ˢ��******/
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

/**********�������ʾ********/
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
		P0=0xFF; //����
	}
}
/*****************************/

/********20usTTL������ʱ******/
void delay_20us()
{
	uchar ttl;
	for(ttl=0;ttl<100;ttl++);
}
/*****************************/

/************����ж�*********/
void T0_time() interrupt 1
{
	flag=1;
}
/*****************************/