#include <rtthread.h>
#include "stm32f10x.h"
#include "numfont.h"
#include "LM096.h"
#include "led.h"
#define SSD1306_LCDWIDTH                    128
#define SSD1306_LCDHEIGHT                   64
#define _BV(bit) (1<<(bit))
I2C_InitTypeDef  I2C_InitStructure;
void pin_init()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	 //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	 //GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
	 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOD, &GPIO_InitStructure);
	 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
	 GPIO_Init(GPIOE, &GPIO_InitStructure);
	 I2C_SoftwareResetCmd(I2C1,ENABLE);
	 I2C_SoftwareResetCmd(I2C1,DISABLE);
	 I2C_Cmd(I2C1, ENABLE);
	 I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	 I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	 I2C_InitStructure.I2C_OwnAddress1 = 0x79;
	 I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	 I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	 I2C_InitStructure.I2C_ClockSpeed = 100000;
	 I2C_Init(I2C1, &I2C_InitStructure);
}

void ssd1306_send_byte_cmd(uint8_t data)
{
	 I2C_GenerateSTART(I2C1, ENABLE);
	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	 I2C_Send7bitAddress(I2C1, 0x78, I2C_Direction_Transmitter);

	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	 I2C_SendData(I2C1,0x00);
	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	 I2C_SendData(I2C1,data);
	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	 I2C_GenerateSTOP(I2C1, ENABLE);
}

void ssd1306_fill_frame_buffer()
{
	 int i=0;

	 I2C_GenerateSTART(I2C1, ENABLE);
	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	 I2C_Send7bitAddress(I2C1, 0x78, I2C_Direction_Transmitter);
	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));


	 I2C_SendData(I2C1,0x40);
	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	 for(i=0;i<sizeof(buffer)/sizeof(uint8_t);i++)
	 {

		  I2C_SendData(I2C1,buffer[i]);
		  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	 } 
	 I2C_GenerateSTOP(I2C1, ENABLE);

}

void display(void) 
{
	ssd1306_fill_frame_buffer();
}

void device_rst()
{
	 GPIO_ResetBits(GPIOD, GPIO_Pin_2);	//config iic address
	 GPIO_SetBits(GPIOE, GPIO_Pin_4);	//rst
	 rt_thread_delay(10);
	 GPIO_ResetBits(GPIOE, GPIO_Pin_4);
	 rt_thread_delay(10);
	 GPIO_SetBits(GPIOE, GPIO_Pin_4);
}    

void clear(void) 
{
	 rt_memset(buffer, 0, (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8));
}

void ssd1306_init() 
{
	 int i;
	 pin_init();
	 device_rst();
	 for(i=0;i<sizeof(init_reg);i++)
	 {
		  ssd1306_send_byte_cmd(init_reg[i]);
	 }
	 clear();
	 for(i=0;i<sizeof(draw_reg);i++)
	 {						
		  ssd1306_send_byte_cmd(draw_reg[i]);
	 }
}

void setpixel(uint8_t x, uint8_t y,uint8_t clear) {
	 if ((x >= SSD1306_LCDWIDTH) || (y >= SSD1306_LCDHEIGHT))
		  return;

	 if(clear)
		  buffer[x+ (y/8)*SSD1306_LCDWIDTH] = 0;  
	 else
		  buffer[x+ (y/8)*SSD1306_LCDWIDTH] |= _BV((y%8));  
}

void drawline(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) 
{
	 uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
	 uint8_t dx, dy,tmp;
	 int8_t err;
	 int8_t ystep;
	 if (steep) 
	 {
		  tmp=x0;
		  x0=y0;
		  y0=tmp;
		  tmp=x1;
		  x1=y1;
		  y1=tmp;
	 }
	 if (x0 > x1) 
	 {
		  tmp=x0;
		  x0=x1;
		  x1=tmp;
		  tmp=y0;
		  y0=y1;
		  y1=tmp;
	 }

	 dx = x1 - x0;
	 dy = abs(y1 - y0);

	 err = dx / 2;
	 if (y0 < y1) {
		  ystep = 1;
	 } else {
		  ystep = -1;}

	 for (; x0<x1; x0++) {
		  if (steep) {
			   setpixel(y0, x0,0);
		  } else {
			   setpixel(x0, y0,0);
		  }
		  err -= dy;
		  if (err < 0) {
			   y0 += ystep;
			   err += dx;
		  }
	 }
}

void fillrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h,uint8_t clear) 
{
	 uint8_t i,j;
	 for (i=x; i<x+w; i++) {
		  for (j=y; j<y+h; j++) {
			   setpixel(i, j,clear);
		  }
	 }
}
void draw_num(int *pos,int *l,char *num)
{
	char *p=num;
	int j=0;
	int x=*pos;
	while(*p!=0)
	{
		if(*p=='.')
			*p=58;
		x=*pos;
		for(j=0;j<32;j++)
		{				
			buffer[x+((*l)*128)]=font32[(*p-48)*128+j];
			x++;				
		}
		for(j=0;j<32;j++)
		{				
			buffer[x+((*l+1)*128)-32]=font32[(*p-48)*128+32+j];
			x++;				
		}
		for(j=0;j<32;j++)
		{				
			buffer[x+((*l+2)*128)-64]=font32[(*p-48)*128+64+j];
			x++;				
		}
		for(j=0;j<32;j++)
		{				
			buffer[x+((*l+3)*128)-96]=font32[(*p-48)*128+96+j];
			x++;				
		}		  
		*pos=*pos+31;
		if(*pos+31>SSD1306_LCDWIDTH)
		{
			*pos=0;
			*l=*l+4;
			break;
		}
		p++;
	}
}
void draw(char *co2,char *co1)
{
   int x=0,line=0,x1=0,j;
   for(j=0;j<8;j++)
   {
		buffer[x]=hzdot[j];
		x++;				
   }
   for(j=0;j<8;j++)
   {				
		buffer[x+128-8]=hzdot[j+8];
		x++;				
   }   
   x1+=8;
   x=x1;
   for(j=0;j<8;j++)
   {				
		buffer[x]=hzdot[16+j];
		x++;				
   }
   for(j=0;j<8;j++)
   {				
		buffer[x+128-8]=hzdot[16+j+8];
		x++;				
   }
   x1+=8;
    x=x1;
   for(j=0;j<8;j++)
   {				
		buffer[x]=hzdot[6*8+j];
		x++;				
   }
   for(j=0;j<8;j++)
   {				
		buffer[x+128-8]=hzdot[6*8+j+8];
		x++;				
   }
   x1+=8;
   draw_num(&x1,&line,co2);
   
   x=0;
   for(j=0;j<8;j++)
   {				
		buffer[x+((line)*128)]=hzdot[j];
		x++;				
   }
   for(j=0;j<8;j++)
   {				
		buffer[x+((line+1)*128)-8]=hzdot[j+8];
		x++;				
   }  
   x1+=8;
   x=x1;
   for(j=0;j<8;j++)
   {				
		buffer[x+((line)*128)]=hzdot[8*8+j];
		x++;				
   }
   for(j=0;j<8;j++)
   {				
		buffer[x+((line+1)*128)-8]=hzdot[8*8+j+8];
		x++;				
   } 
   x1+=8;
   x=x1;
   for(j=0;j<8;j++)
   {				
		buffer[x+((line)*128)]=hzdot[6*8+j];
		x++;				
   }
   for(j=0;j<8;j++)
   {				
		buffer[x+((line+1)*128)-8]=hzdot[6*8+j+8];
		x++;				
   }
   x1+=8;
   x=x1;
   for(j=0;j<8;j++)
   {				
		buffer[x+((line)*128)]=hzdot[2*8+j];
		x++;				
   }
   for(j=0;j<8;j++)
   {				
		buffer[x+((line+1)*128)-8]=hzdot[2*8+j+8];
		x++;				
   }
   x1+=8;   
   draw_num(&x1,&line,co1);
}

void draw1(uint8_t bat1,uint8_t bat2,char *c)
{
	 int x=0,line=0,x1=0,i,j;
	 if(bat1>100||bat1<0||bat2>100||bat2<0||rt_strlen(c)!=7)
		  return ;
	 if(bat1==1)
	 {
		  fillrect(2,31,28,15,1);
	 }
	 if(bat2==1)
	 {
		  fillrect(2,48,28,15,1);
	 }
	 drawline(1,32,28,32);
	 drawline(28,32,28,36);
	 drawline(28,36,32,36);
	 drawline(32,36,32,43);
	 drawline(32,43,28,43);
	 drawline(28,43,28,47);
	 drawline(28,47,1,47);
	 drawline(1,47,1,32);
	 fillrect(1,32,(uint8_t)(bat1*28/100),15,0);
	 drawline(1,49,28,49);
	 drawline(28,49,28,52);
	 drawline(28,52,32,52);
	 drawline(32,52,32,59);
	 drawline(32,59,28,59);
	 drawline(28,59,28,63);
	 drawline(28,63,1,63);
	 drawline(1,63,1,49);
	 fillrect(1,49,(uint8_t)(bat2*28/100),15,0);
	 while(c[0]!=0)
	 {
		  if(c[0]!='.')
		  {
			   x=x1;
			   for(j=0;j<32;j++)
			   {				
					buffer[x+((line)*128)]=font32[(c[0]-48)*128+j];
					x++;				
			   }
			   for(j=0;j<32;j++)
			   {				
					buffer[x+((line+1)*128)-32]=font32[(c[0]-48)*128+32+j];
					x++;				
			   }
			   for(j=0;j<32;j++)
			   {				
					buffer[x+((line+2)*128)-64]=font32[(c[0]-48)*128+64+j];
					x++;				
			   }
			   for(j=0;j<32;j++)
			   {				
					buffer[x+((line+3)*128)-96]=font32[(c[0]-48)*128+96+j];
					x++;				
			   }
		  }
		  else
		  {
			   x=x1;
			   for(j=0;j<32;j++)
			   {				
					buffer[x+((line)*128)]=font32[1280+j];
					x++;				
			   }
			   for(j=0;j<32;j++)
			   {				
					buffer[x+((line+1)*128)-32]=font32[1280+32+j];
					x++;				
			   }
			   for(j=0;j<32;j++)
			   {				
					buffer[x+((line+2)*128)-64]=font32[1280+64+j];
					x++;				
			   }
			   for(j=0;j<32;j++)
			   {				
					buffer[x+((line+3)*128)-96]=font32[1280+96+j];
					x++;				
			   }
		  }

		  x1=x1+31;
		  if(x1+31>SSD1306_LCDWIDTH)
		  {
			   x1=33;
			   line=line+4;
		  }

		  c++;

	 }	
}
