#include <reg52.h>
#define u8 unsigned char
#define u16 unsigned int
#define PASSWORD_BIT 6
#define MAX_FAILURE_TIMES 3

#define key P1		   //按键io口
sbit fen=P3^0;		   //蜂鸣器io口
sbit rs=P3^1;        //LCD控制口
sbit rw=P3^2;
sbit e=P3^3;
sbit led=P3^4;       //LED灯io口

//bit 位变量声明：定义一个位变量的值
u8 keyz=17,j=0,biao1=0,a=10,b=10,c=10,d=10;
bit biao=0;
u16 zh;

u8 shu[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
          0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
u8 zi[]={"lnput password:"};
u8 num[]={"0123456789ABCDEF"};

void Initialize();//lcd设置
void InputPassword();//输入密码
bit checkPassword();//验证密码
char Getoptions();//功能选择
void Unlock();//开锁
void ChangePassword();//修改密码
void Alert();//报警

void delay(uint c)
{
	uchar a,b;
	for(;c>0;c--)
	{
		for(b=199;b>0;b--)
		{
			for(a=1;a>0;a--);
		}
	}
}

/*蜂鸣器*/
void Fen(u8 a,b)   //蜂鸣器发声函数
{
	u8 i,j;	
	for(i=0;i<a;i++)
	{
		for(j=0;j<100;j++)
		 {fen=~fen;delay(b);}
	 }
}

/*lcd*/
void writecom(u8 com)	//LCD写指令
{
	 rs=0;
	 rw=0;
	 e=0;
	 P0=com;
	 delay(5);
	 e=1;
	 e=0;
}
void writedat(u8 dat)		//LCD写数据
{ 
   rs=1;
	 rw=0;
	 e=0;
	 P0=dat;
	 delay(5);
	 e=1;
	 e=0;
}
void initlcd()			   //LCD初始化函数
{ 
    writecom(0x38);   //显示模式设置
  	writecom(0x0c);
	  writecom(0x06);

}
void xianshi()        //LCD显示数字函数
{
	
}

/*键盘*/
#define key P1
u8 keyz=17;
bit biao=0;
void key()
{ 
	u8 a=0;
	if(key!=0x0f)delay(500);
	if(key!=0x0f)
	{
		key=0x0f;
		switch(key)
		{
			case 0x07:keyz=0;biao=1;break;
			case 0x0b:keyz=1;biao=1;break;
			case 0x0d:keyz=2;biao=1;break;
			case 0x0e:keyz=3;biao=1;break;
		}
		key=0xf0;
		switch(key)
		{
			case 0x70:keyz=keyz;biao=1;break;
			case 0xb0:keyz=keyz+4;biao=1;break;
			case 0xd0:keyz=keyz+8;biao=1;break;
			case 0xe0:keyz=key+12;biao=1;break;
		}
		while((a<50)&&(key!=0xf0)){a++;delay(10000);}
	}
}


	
void main()
{
	u8 _failures;
	while(1)
	{
		Initialize();
		do
		{
			InputPassword();
			if(checkPassword())
			{
				switch(Getoptions())
				{
					case eUnlock:Unlock();break;
					case eChangePassword():ChangePassword();break;
					default;break;
				}
				Initialize();
			}
			else
			{
				_failures+=1;
			}
		}while(_failures<=Max_FAILURE_TIMES);
		Alert();
	}
}
