//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//����Ӳ������Ƭ��STC12C5A60S2,����30M  ��Ƭ��������ѹ3.3V
//QDtech-TFTҺ������ for C51
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/7/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
//********************************************************************************
//=========================================��Դ����================================================//
//5V��DC 5V��Դ
//GND�ӵ�
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������ΪSPI
//Һ����ģ��            ��Ƭ��
// SDI(MOSI)     ��       P15        //SPIд�ź�
// SDO(MISO)     ��       P16        //SPI���źţ��������Ҫ�����ܣ��˹ܽſɲ���
//=======================================Һ���������߽���==========================================//
//Һ����ģ��            ��Ƭ��
//  CS           ��       P13       //Ƭѡ�����ź�
//  RESET        ��       P33       //��λ�ź�
//  DC           ��       P12       //����/����ѡ������ź�
//  SCK          ��       P17       //SPIʱ���ź�
//  LED          ��       P32       //��������źţ����ʹ��STC89C52RC��Ƭ�����߲���Ҫ���ƣ���3.3V
//=========================================����������=========================================//
//STC89C52RC��Ƭ��ROM̫С���޷����д��������ܵĳ������Դ���������Ҫ����
//��ʹ�ô�������ģ�鱾��������������ɲ�����
//������ʹ�õ�������������ΪSPI
//������ģ��            ��Ƭ��
//  T_CLK        ��       P36       //������SPIʱ���ź�
//  T_CS         ��       P37       //������Ƭѡ�����ź�
//  T_DIN        ��       P34       //������SPIд�ź�
//  T_DO         ��       P35       //������SPI���ź�
//  T_IRQ        ��       P40       //��������Ӧ����źţ��絥Ƭ����P4�飬�����и�����������IO���޸Ĵ��붨��
//**************************************************************************************************/	
#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"

/////////////////////////////////////�û�������///////////////////////////////////	 
//֧�ֺ��������ٶ����л�
#define USE_HORIZONTAL  	0 //����Һ����˳ʱ����ת���� 	0-0����ת��1-90����ת��2-180����ת��3-270����ת
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////	  
//����LCD�ĳߴ�
#define LCD_W 320
#define LCD_H 480

//IO����
sbit LCD_RS = P1^2;  		 //����/�����л�
sbit LCD_SDI = P1^5;		  //SPIд
sbit LCD_SDO = P1^6;		     //SPI��
sbit LCD_CS = P1^3;		//Ƭѡ	
sbit LCD_CLK = P1^7;   //SPIʱ��
sbit LCD_RESET = P3^3;	      //��λ 
sbit LCD_BL=P3^2;		//������ƣ��������Ҫ���ƣ���3.3V

//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ����
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	 wramcmd;		//��ʼдgramָ��
	u16  setxcmd;		//����x����ָ��
	u16  setycmd;		//����y����ָ��	 
}_lcd_dev; 	

//����Ӳ��ƽ̨��STC12C5A60S2/STC12LE5A60S2
//��Ƶ��12MHZ
//��Ƭ��IO����������ö���
#define MCU_STC12//�����ʹ�õĵ�Ƭ������STC12ϵ��(��STC89C52)�����δ˺궨��
#define		MAIN_Fosc	12000000L	//����ʱ��
#ifdef MCU_STC12
//sfr P3M1  = 0xB1;	//P3M1.n,P3M0.n 	=00--->Standard,	01--->push-pull
//sfr P3M0  = 0xB2;	//					=10--->pure input,	11--->open drain
sfr AUXR1 = 0xA2;
#endif
//-----------------------------SPI ��������--------------------------------------//


//STC��Ƭ��Ӳ��SPI�Ĵ�������
sfr SPSTAT = 0xCD;	//STC12C5A60S2ϵ��
sfr SPCTL  = 0xCE;	//STC12C5A60S2ϵ��
sfr SPDAT  = 0xCF;	//STC12C5A60S2ϵ��
//sfr SPCTL   = 0x85;	SPI���ƼĴ���
//   7       6       5       4       3       2       1       0    	Reset Value
//	SSIG	SPEN	DORD	MSTR	CPOL	CPHA	SPR1	SPR0		0x00
#define	SSIG		1	//1: ����SS�ţ���MSTRλ�����������Ǵӻ�		0: SS�����ھ������ӻ���
#define	SPEN		1	//1: ����SPI��								0����ֹSPI������SPI�ܽž�Ϊ��ͨIO
#define	DORD		0	//1��LSB�ȷ���								0��MSB�ȷ�
#define	MSTR		1	//1����Ϊ����								0����Ϊ�ӻ�
#define	CPOL		1	//1: ����ʱSCLKΪ�ߵ�ƽ��					0������ʱSCLKΪ�͵�ƽ
#define	CPHA		1	//
#define	SPR1		0	//SPR1,SPR0   00: fosc/4,     01: fosc/16
#define	SPR0		0	//            10: fosc/64,    11: fosc/128
#define	SPEED_4		0	// fosc/4
#define	SPEED_16	1	// fosc/16
#define	SPEED_64	2	// fosc/64
#define	SPEED_128	3	// fosc/128

//sfr SPSTAT  = 0xCD;	//SPI״̬�Ĵ���
//   7       6      5   4   3   2   1   0    	Reset Value
//	SPIF	WCOL	-	-	-	-	-	-
#define	SPIF	0x80		//SPI������ɱ�־��д��1��0��
#define	WCOL	0x40		//SPIд��ͻ��־��д��1��0��


//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����

void LCD_Init(void); 
void LCD_Clear(u16 Color);
void LCD_WR_DATA(u8 Data);
void LCD_WR_REG(u8 Reg);
void LCD_SetCursor(u16 Xpos, u16 Ypos);//���ù��λ��
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);//������ʾ����
void LCD_DrawPoint(u16 x,u16 y);//����
void LCD_WriteRAM_Prepare(void);
void LCD_direction(u8 direction );
void LCD_WR_DATA_16Bit(u16 Data);


//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


					  		 
#endif  
	 
	 


