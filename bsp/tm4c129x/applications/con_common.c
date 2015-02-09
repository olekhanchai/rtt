#include "con_socket.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "board.h"

#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#if 0
void IntGpioD()
{
	if(MAP_GPIOIntStatus(GPIO_PORTD_BASE, true)&GPIO_PIN_2)
	{			
		MAP_GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_2);
		if(MAP_GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2)!=GPIO_PIN_2)
			MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0);
		else
			MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);
		//ind[3]=((MAP_GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2)&(GPIO_PIN_2))==0)?RT_TRUE:RT_FALSE;
		rt_kprintf("gpiod 2 int \r\n");
	}	
}

#else
unsigned char config_local_ip[4+7]				={0xF5,0x8A,0x00,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*local ip*/
unsigned char config_socket0_local_port[2+7]	={0xF5,0x8A,0x01,0xff,0xff,0x26,0xfa,0x00,0x00};/*local port0*/
unsigned char config_socket1_local_port[2+7]	={0xF5,0x8A,0x02,0xff,0xff,0x26,0xfa,0x00,0x00};/*local port1*/
unsigned char config_socket2_local_port[2+7]	={0xF5,0x8A,0x03,0xff,0xff,0x26,0xfa,0x00,0x00};/*local port2*/
unsigned char config_socket3_local_port[2+7]	={0xF5,0x8A,0x04,0xff,0xff,0x26,0xfa,0x00,0x00};/*local port3*/
unsigned char config_sub_msk[4+7]				={0xF5,0x8A,0x05,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*sub msk*/
unsigned char config_gw[4+7]					={0xF5,0x8A,0x06,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*gw*/
unsigned char config_mac[6+7]					={0xF5,0x8A,0x07,0xff,0xff,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*mac*/
unsigned char config_socket0_ip[4+7]			={0xF5,0x8A,0x08,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 0 ip*/
unsigned char config_socket1_ip[4+7]			={0xF5,0x8A,0x09,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 1 ip*/
unsigned char config_socket2_ip[4+7]			={0xF5,0x8A,0x0a,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 2 ip*/
unsigned char config_socket3_ip[4+7]			={0xF5,0x8A,0x0b,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 3 ip*/
unsigned char config_socket0_ip6[64+7]			={0xF5,0x8A,0x0c,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 0 ip6*/
unsigned char config_socket1_ip6[64+7]			={0xF5,0x8A,0x0d,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 1 ip6*/
unsigned char config_socket2_ip6[64+7]			={0xF5,0x8A,0x0e,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 2 ip6*/
unsigned char config_socket3_ip6[64+7]			={0xF5,0x8A,0x0f,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 3 ip6*/
unsigned char config_socket0_remote_port[2+7]	={0xF5,0x8A,0x10,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 0 port*/
unsigned char config_socket1_remote_port[2+7]	={0xF5,0x8A,0x11,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 1 port*/
unsigned char config_socket2_remote_port[2+7]	={0xF5,0x8A,0x12,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 2 port*/
unsigned char config_socket3_remote_port[2+7]	={0xF5,0x8A,0x13,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 3 port*/
unsigned char config_net_protol0[1+7]			={0xF5,0x8A,0x14,0xff,0x26,0xfa,0x00,0x00};/*protol0*/
unsigned char config_net_protol1[1+7]			={0xF5,0x8A,0x15,0xff,0x26,0xfa,0x00,0x00};/*protol1*/
unsigned char config_net_protol2[1+7]			={0xF5,0x8A,0x16,0xff,0x26,0xfa,0x00,0x00};/*protol2*/
unsigned char config_net_protol3[1+7]			={0xF5,0x8A,0x17,0xff,0x26,0xfa,0x00,0x00};/*protol3*/
unsigned char config_socket_mode0[1+7]			={0xF5,0x8A,0x18,0xff,0x26,0xfa,0x00,0x00};/*server mode0*/
unsigned char config_socket_mode1[1+7]			={0xF5,0x8A,0x19,0xff,0x26,0xfa,0x00,0x00};/*server mode1*/
unsigned char config_socket_mode2[1+7]			={0xF5,0x8A,0x1a,0xff,0x26,0xfa,0x00,0x00};/*server mode2*/
unsigned char config_socket_mode3[1+7]			={0xF5,0x8A,0x1b,0xff,0x26,0xfa,0x00,0x00};/*server mode3*/
unsigned char config_uart_baud0[1+7]			={0xF5,0x8A,0x1c,0xff,0x26,0xfa,0x00,0x00};/*uart baud*/
unsigned char config_uart_baud1[1+7]			={0xF5,0x8A,0x1d,0xff,0x26,0xfa,0x00,0x00};/*uart baud*/
unsigned char config_uart_baud2[1+7]			={0xF5,0x8A,0x1e,0xff,0x26,0xfa,0x00,0x00};/*uart baud*/
unsigned char config_uart_baud3[1+7]			={0xF5,0x8A,0x1f,0xff,0x26,0xfa,0x00,0x00};/*uart baud*/
unsigned char config_local_ip6[64+7]			={0xF5,0x8A,0x20,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*local ip6*/
unsigned char config_tcp0[1+7]					={0xF5,0x8A,0x21,0xff,0x26,0xfa,0x00,0x00};/*protol0*/
unsigned char config_tcp1[1+7]					={0xF5,0x8A,0x22,0xff,0x26,0xfa,0x00,0x00};/*protol1*/
unsigned char config_tcp2[1+7]					={0xF5,0x8A,0x23,0xff,0x26,0xfa,0x00,0x00};/*protol2*/
unsigned char config_tcp3[1+7]					={0xF5,0x8A,0x24,0xff,0x26,0xfa,0x00,0x00};/*protol3*/
#define COMMAND_FAIL "Command crc fail"
#define COMMAND_OK "Command exec OK"
void print_config();

unsigned char get_config[35];//			={0xF5,0x8B,0x0f,0xff,0xff,0xff,0xff,0x27,0xfa,0x00,0x00};/*get config 0xf5,0x8b,********0x27,0xfa,0x00,0x00*/
rt_thread_t tid_common_w[4]={RT_NULL,RT_NULL,RT_NULL,RT_NULL},tid_common_r[4]={RT_NULL,RT_NULL,RT_NULL,RT_NULL};
rt_device_t common_dev[4] = {RT_NULL,RT_NULL,RT_NULL,RT_NULL};
bool ind[4]={RT_TRUE,RT_TRUE,RT_TRUE,RT_TRUE};
bool phy_link=false;
enum STATE_OP{
	GET_F5,
	GET_8A_8B,
	GET_LEN,
	GET_DATA,
	GET_26,
	GET_FA,
	GET_CHECSUM
};
struct rt_semaphore rx_sem[4];

int which_common_dev(rt_device_t *dev,rt_device_t dev2)
{
	int i=0;
	for(i=0;i<4;i++)
		if(dev[i]==dev2)
		{
			//DBG("Uart %d 's setting\r\n",i);
			break;
		}
	return i;
}
void IntGpioD()
{
	if(MAP_GPIOIntStatus(GPIO_PORTD_BASE, true)&GPIO_PIN_2)
	{		
		MAP_GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_2);
		ind[0]=((MAP_GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2)&(GPIO_PIN_2))==GPIO_PIN_2)?RT_TRUE:RT_FALSE;
		rt_kprintf("gpiod 2 int %d\r\n",ind[0]);
		
	}	
}/*
void IntGpioB()
{
	if(MAP_GPIOIntStatus(GPIO_PORTB_BASE, true)&GPIO_PIN_4)
	{
		MAP_GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_4);
		ind[3]=((MAP_GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_4)&(GPIO_PIN_4))==0)?RT_TRUE:RT_FALSE;
		rt_kprintf("gpiob 4 int %d\r\n",ind[3]);
	}	
}
*/
/*get config data to global config zone, or get socket data to buffer*/
void default_config()
{
	struct netif * netif=netif_list;
	g_conf.config[0]=CONFIG_IPV6|CONFIG_TCP;
	g_conf.config[1]=CONFIG_TCP;
	g_conf.config[2]=CONFIG_TCP;
	g_conf.config[3]=CONFIG_TCP;
	g_conf.local_port[0]=1234;
	g_conf.local_port[1]=1235;
	g_conf.local_port[2]=1236;
	g_conf.local_port[3]=1237;
	memset(g_conf.remote_ip[0],'\0',16);
	strcpy(g_conf.remote_ip[0],"192.168.1.6");
	memset(g_conf.remote_ip[1],'\0',16);
	strcpy(g_conf.remote_ip[1],"192.168.1.6");
	memset(g_conf.remote_ip[2],'\0',16);
	strcpy(g_conf.remote_ip[2],"192.168.1.6");
	memset(g_conf.remote_ip[3],'\0',16);
	strcpy(g_conf.remote_ip[3],"192.168.1.6");
	memset(g_conf.remote_ip6[0],'\0',64);
	strcpy(g_conf.remote_ip6[0],"fe80::5867:8730:e9e6:d5c5%11");
	memset(g_conf.remote_ip6[1],'\0',64);
	strcpy(g_conf.remote_ip6[1],"fe80::5867:8730:e9e6:d5c5%11");
	memset(g_conf.remote_ip6[2],'\0',64);
	strcpy(g_conf.remote_ip6[2],"fe80::5867:8730:e9e6:d5c5%11");
	memset(g_conf.remote_ip6[3],'\0',64);
	strcpy(g_conf.remote_ip6[3],"fe80::5867:8730:e9e6:d5c5%11");
	
	memset(g_conf.local_ip,'\0',16);
	strcpy(g_conf.local_ip,"192.168.1.32");	
	memset(g_conf.gw,'\0',16);
	strcpy(g_conf.gw,"192.168.1.1");	
	memset(g_conf.sub_msk,'\0',16);
	strcpy(g_conf.sub_msk,"255.255.255.0");
	memset(g_conf.mac,'\0',16);
	rt_sprintf(g_conf.mac,"%02x:%02x:%02x:%02x:%02x:%02x",netif->hwaddr[0],netif->hwaddr[1],netif->hwaddr[2],netif->hwaddr[3],netif->hwaddr[4],netif->hwaddr[5]);
	g_conf.remote_port[0]=1234;
	g_conf.remote_port[1]=1235;
	g_conf.remote_port[2]=1236;
	g_conf.remote_port[3]=1237;
	set_if6("e0","fe80::1");
	set_if("e0",g_conf.local_ip,g_conf.sub_msk,g_conf.gw);
}
int set_config(rt_uint8_t *data,int ipv6_len,int dev)//0 no change ,1 local socket need reconfig ,2 all socket need reconfig
{
	bool ipv6_changed=false,ipv4_changed=false;
	int need_reconfig=0;
	int i;
	switch(data[0])
	{
		case 0:
		{
			//set local ipv4 ip
			ipv4_changed=true;
			rt_memset(g_conf.local_ip,'\0',16);
			rt_sprintf(g_conf.local_ip,"%d.%d.%d.%d",data[1],data[2],data[3],data[4]);
			need_reconfig=2;
		}
		break;
		case 1:
		case 2:
		case 3:
		case 4:
		{
			//set local port
			if((data[0]-1)==dev)
			{
				g_conf.local_port[data[0]-1]=(data[1]<<8|data[2]);
				need_reconfig=1;
			}
		}
		break;
		case 5:
		{
			//set sub msk
			ipv4_changed=true;
			rt_memset(g_conf.sub_msk,'\0',16);
			rt_sprintf(g_conf.sub_msk,"%d.%d.%d.%d",data[1],data[2],data[3],data[4]);
			need_reconfig=2;
		}
		break;
		case 6:
		{
			//set gateway
			ipv4_changed=true;
			rt_memset(g_conf.gw,'\0',16);
			rt_sprintf(g_conf.gw,"%d.%d.%d.%d",data[1],data[2],data[3],data[4]);
			need_reconfig=2;
		}
		break;
		case 7:
		{
			//set mac
			
		}
		break;
		case 8:
		case 9:
		case 10:
		case 11:
		{
			//set remote ipv4 ip
			if((data[0]-8)==dev)
			{
				rt_memset(g_conf.remote_ip[data[0]-8],'\0',16);
				rt_sprintf(g_conf.remote_ip[data[0]-8],"%d.%d.%d.%d",data[1],data[2],data[3],data[4]);
				need_reconfig=1;
			}
		}
		break;
		case 12:
		case 13:			
		case 14:
		case 15:
		{
			//set remote ipv6 ip
			if((data[0]-12)==dev)
			{
				rt_memset(g_conf.remote_ip6[data[0]-12],'\0',64);
				for(i=0;i<ipv6_len;i++)
					g_conf.remote_ip6[data[0]-12][i]=data[i+1];
				need_reconfig=1;
			}
		}
		break;
		case 16:
		case 17:
		case 18:
		case 19:
		{
			//set remote port
			if((data[0]-16)==dev)
			{
				g_conf.remote_port[data[0]-16]=(data[1]<<8|data[2]);
				need_reconfig=1;
			}
		}
		break;
		case 20:
		case 21:
		case 22:
		case 23:
		{
			//set net protol ipv4 or ipv6
			if((data[0]-20)==dev)
			{
				if(data[1])
					g_conf.config[data[0]-20]|=CONFIG_IPV6;
				else
					g_conf.config[data[0]-20]&=~CONFIG_IPV6;
				need_reconfig=1;
			}
		}
		break;
		case 24:
		case 25:
		case 26:
		case 27:
		{
			//set server or client mode
			if((data[0]-24)==dev)
			{
				if(data[1])
					g_conf.config[data[0]-24]|=CONFIG_SERVER;
				else
					g_conf.config[data[0]-24]&=~CONFIG_SERVER;
				need_reconfig=1;
			}
		}
		break;
		case 28:
		case 29:
		case 30:
		case 31:
		{
			//set uart baud
			if((data[0]-28)==dev)
			{
				g_conf.config[data[0]-28]=((g_conf.config[data[0]-20]&0x07)|(data[1]<<3));
				need_reconfig=1;
			}
		}
		break;
		case 32:
		{
			//set local ipv6 ip
			ipv6_changed=true;
			rt_memset(g_conf.local_ip6,'\0',64);
			for(i=0;i<ipv6_len;i++)
				g_conf.local_ip6[i]=data[i+1];
			need_reconfig=2;
		}
		break;
		case 33:
		case 34:
		case 35:
		case 36:
		{//set tcp or udp
		if((data[0]-33)==dev)
			{
				if(data[1])
					g_conf.config[data[0]-33]|=CONFIG_TCP;
				else
					g_conf.config[data[0]-33]&=~CONFIG_TCP;
				need_reconfig=1;
			}
		}
		break;
		default:
			need_reconfig=false;
	}
	if(ipv4_changed)
		set_if("e0",g_conf.local_ip,g_conf.sub_msk,g_conf.gw);
	if(ipv6_changed)
		set_if6("e0",g_conf.local_ip6);
	print_config();
	rt_kprintf("set_config %d\n",need_reconfig);
	return need_reconfig;
}
int common_rw_config(int dev)
{

	static rt_uint8_t buf[65];
	rt_uint8_t i=0;	
	static int data_len,crc_len,longlen=0;
	static unsigned char crc[2];
	char ch;	
	static rt_uint8_t len=0,param;
	static enum STATE_OP state=GET_F5;
	DBG("enter common_rw_config\r\n");
	rt_uint8_t *ptr=(rt_uint8_t *)buf;
	int need_reconfig=0;
	rt_thread_delay(10);
	while((rt_device_read(common_dev[dev], 0, &ch, 1) == 1))
	{
		if(ch==0xf5 && state==GET_F5)
		{
			DBG("GET_F5\n");
			state=GET_8A_8B;
		}
		else if(ch==0x8a && state==GET_8A_8B)
		{
			DBG("GET_8A\n");
			data_len=0;
			longlen=0;
			state=GET_DATA;
		}
		else if(ch==0x8b && state==GET_8A_8B)
		{
			DBG("GET_8B\n");
			break;
		}
		else if(state==GET_DATA)
		{	
			DBG("GET_DATA %2x\n",ch);
			*(ptr+data_len)=ch;
			if(data_len==0)
			{
				if(ch==0x0c || ch==0x0d || ch==0x0e || ch==0x0f || ch==0x20)
					state=GET_LEN;
 			}
			else
			{
				if(ch==0x26&&(data_len>longlen))
					state=GET_FA;
 			}
			
			data_len++;
		}
		else if(state==GET_LEN)
		{
			DBG("GET_LEN %2x\n",ch);
			longlen=ch;
			state=GET_DATA;
		}
		else if(ch==0xfa && state==GET_FA)
		{
			DBG("GET_FA\n");
			data_len--;
			crc_len=0;
			state=GET_CHECSUM;
		}
		else if(state==GET_CHECSUM)
		{
			if(crc_len!=2)
			{
				crc[crc_len++]=ch;
			}

			if(crc_len==2)
			{
				//verify checksum
				int verify=0xf5+0x8a+0xfa+0x26+longlen;
				rt_kprintf("command is \n");
				for(i=0;i<data_len;i++)
				{
					rt_kprintf("%2x ",ptr[i]);	
					verify+=ptr[i];
				}
				rt_kprintf("crc is %2x %2x\n",crc[0],crc[1]);
				if(verify!=(crc[0]<<8|crc[1]))
				{
					rt_kprintf("verify %d crc error\n",verify);
					need_reconfig=-1;
				}
				else
				{
					rt_kprintf("Command OK\n");
					need_reconfig|=set_config(ptr,longlen,dev);
					//rt_device_write(common_dev[dev], 0, (void *)COMMAND_OK, strlen(COMMAND_OK));
				}
				state=GET_F5;
			}
		}
	}
	rt_kprintf("common_rw_config %d\n",need_reconfig);
	return need_reconfig;
}
static bool flag_cnn[4]={false,false,false,false};
void all_cut()
{
	MAP_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_PIN_4);	
	MAP_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0); 
	MAP_GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_2,0); 
	flag_cnn[0]=false;
	flag_cnn[1]=false;
	flag_cnn[2]=false;
	flag_cnn[3]=false;
}
void cnn_out(int index,int level)
{
	
	rt_kprintf("dev %d , level %d, phy_link %d\n",index,level,phy_link);
	if(phy_link)
	{
		if(level&&(flag_cnn[index]==false))
			flag_cnn[index]=true;
		else if(!level&&(flag_cnn[index]==true))
			flag_cnn[index]=false;
		else
			return;
		switch(index)
		{
			case 0:
				if(level)
					MAP_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,0);
				else
					MAP_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_PIN_4);
				break;
			case 1:
				if(level)
					MAP_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,GPIO_PIN_5);
				else
					MAP_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0);	
				break;
			case 2:
				if(level)
					MAP_GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_2,GPIO_PIN_2);
				else
					MAP_GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_2,0);	
				break;
			case 3:
				if(level)
				;//	MAP_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_PIN_4);
				else
				;//	MAP_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,0);	
				break;
			default:
				break;
		}
	}
	else
	{
		MAP_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_PIN_4);	
		MAP_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0);	
		MAP_GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_2,0);	
		flag_cnn[0]=false;
		flag_cnn[1]=false;
		flag_cnn[2]=false;
		flag_cnn[3]=false;
		//MAP_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,0);	
	}
}
int baud(int type)
{
	switch(type)
	{
		case 0:
			return 115200;
		case 1:
			return 460800;
		case 2:
			return 921600;
		case 3:
			return 1000000;
		case 4:
			return 2000000;
		case 5:
			return 4000000;
		case 6:
			return 6000000;
		}
	return 0;
}
void print_config()
{
	rt_kprintf("\n============================================================================>\n");
	rt_kprintf("local_ip %s\n",g_conf.local_ip);
	rt_kprintf("local_port0 %d\n",g_conf.local_port[0]);
	rt_kprintf("local_port1 %d\n",g_conf.local_port[1]);
	rt_kprintf("local_port2 %d\n",g_conf.local_port[2]);
	rt_kprintf("local_port3 %d\n",g_conf.local_port[3]);
	rt_kprintf("sub_msk %s\n",g_conf.sub_msk);
	rt_kprintf("gw %s\n",g_conf.gw);
	rt_kprintf("mac %s\n",g_conf.mac);
	rt_kprintf("remote_ip0 %s\n",g_conf.remote_ip[0]);
	rt_kprintf("remote_ip1 %s\n",g_conf.remote_ip[1]);
	rt_kprintf("remote_ip2 %s\n",g_conf.remote_ip[2]);
	rt_kprintf("remote_ip3 %s\n",g_conf.remote_ip[3]);
	rt_kprintf("remote_ip60 %s\n",g_conf.remote_ip6[0]);
	rt_kprintf("remote_ip61 %s\n",g_conf.remote_ip6[1]);
	rt_kprintf("remote_ip62 %s\n",g_conf.remote_ip6[2]);
	rt_kprintf("remote_ip63 %s\n",g_conf.remote_ip6[3]);
	rt_kprintf("remote_port0 %d\n",g_conf.remote_port[0]);
	rt_kprintf("remote_port1 %d\n",g_conf.remote_port[1]);
	rt_kprintf("remote_port2 %d\n",g_conf.remote_port[2]);
	rt_kprintf("remote_port3 %d\n",g_conf.remote_port[3]);
	rt_kprintf("IP socket0 %s socket1 %s socket2 %s socket3 %s\n",((g_conf.config[0]&0x01)==0)?"IPV4":"IPV6",((g_conf.config[1]&0x01)==0)?"IPV4":"IPV6",((g_conf.config[2]&0x01)==0)?"IPV4":"IPV6",((g_conf.config[3]&0x01)==0)?"IPV4":"IPV6");
	rt_kprintf("protol socket0 %s socket1 %s socket2 %s socket3 %s\n",((g_conf.config[0]&0x02)==0x02)?"TCP":"UDP",((g_conf.config[1]&0x02)==0x02)?"TCP":"UDP",((g_conf.config[2]&0x02)==0x02)?"TCP":"UDP",((g_conf.config[3]&0x02)==0x02)?"TCP":"UDP");
	rt_kprintf("mode socket0 %s socket1 %s socket2 %s socket3 %s\n",((g_conf.config[0]&0x04)==0x04)?"SERVER":"CLIENT",((g_conf.config[1]&0x04)==0x04)?"SERVER":"CLIENT",((g_conf.config[2]&0x04)==0x04)?"SERVER":"CLIENT",((g_conf.config[3]&0x04)==0x04)?"SERVER":"CLIENT");
	rt_kprintf("baud %d.%d.%d.%d\n",baud((g_conf.config[0]&0xf8)>>3),baud((g_conf.config[1]&0xf8)>>3),baud((g_conf.config[2]&0xf8)>>3),baud((g_conf.config[3]&0xf8)>>3));
	rt_kprintf("\n============================================================================>\n");
}
int common_w_socket(int dev)
{	
	int len;
	rt_uint8_t common_buf[1024],*ptr;
	ptr=common_buf;
	len=rt_device_read(common_dev[dev], 0, ptr, 1024);
	if(phy_link&&(len>0))
		rt_data_queue_push(&g_data_queue[dev*2], ptr, len, RT_WAITING_FOREVER);
	return 0;
}
static rt_err_t common_rx_ind(rt_device_t dev, rt_size_t size)
{
    /* release semaphore to let finsh thread rx data */
	//DBG("dev %d common_rx_ind %d\r\n",which_common_dev(common_dev,dev),size);
    rt_sem_release(&(rx_sem[which_common_dev(common_dev,dev)]));
    return RT_EOK;
}
extern struct rt_object_information rt_object_container[];

static long _list_thread(struct rt_list_node *list)
{
    struct rt_thread *thread;
    struct rt_list_node *node;
    rt_uint8_t *ptr;

    rt_kprintf(" thread  pri  status      sp     stack size max used   left tick  error\n");
    rt_kprintf("-------- ---- ------- ---------- ---------- ---------- ---------- ---\n");
    for (node = list->next; node != list; node = node->next)
    {
        thread = rt_list_entry(node, struct rt_thread, list);
        rt_kprintf("%-8.*s 0x%02x", RT_NAME_MAX, thread->name, thread->current_priority);

        if (thread->stat == RT_THREAD_READY)        rt_kprintf(" ready  ");
        else if (thread->stat == RT_THREAD_SUSPEND) rt_kprintf(" suspend");
        else if (thread->stat == RT_THREAD_INIT)    rt_kprintf(" init   ");
        else if (thread->stat == RT_THREAD_CLOSE)   rt_kprintf(" close  ");

        ptr = (rt_uint8_t*)thread->stack_addr;
        while (*ptr == '#')ptr ++;

        rt_kprintf(" 0x%08x 0x%08x 0x%08x 0x%08x %03d\n",
            thread->stack_size + ((rt_uint32_t)thread->stack_addr - (rt_uint32_t)thread->sp),
            thread->stack_size,
            thread->stack_size - ((rt_uint32_t) ptr - (rt_uint32_t)thread->stack_addr),
            thread->remaining_tick,
            thread->error);
    }
	
    return 0;
}

long list_thread(void)
{
    return _list_thread(&rt_object_container[RT_Object_Class_Thread].object_list);
}

void common_w(void* parameter)
{
	int dev=((int)parameter)/2;
	static int flag=0,need_reconfig=0;
	DBG("common_w %d Enter\r\n",dev);
	while (1)
	{
		/* wait receive */
		if (rt_sem_take(&(rx_sem[dev]), RT_WAITING_FOREVER) != RT_EOK) continue;
		//DBG("to read in_low %d\r\n",ind_low(dev));
		if(ind[dev])
		{	
			DBG("dev %d in socket data flag %d\n",dev,flag);
			if(flag==1)
			{
				print_config();
				flag=0;
				rt_kprintf("%d common_w %d\n",dev,need_reconfig);
				if(need_reconfig==1)
					socket_ctl(RT_TRUE,dev);
				else if(need_reconfig==2||need_reconfig==3)
				{
					socket_ctl(RT_TRUE,0);
					socket_ctl(RT_TRUE,1);
					socket_ctl(RT_TRUE,2);
					socket_ctl(RT_TRUE,3);
				}
				need_reconfig=0;
				rt_thread_delay(10);
				list_thread();
				//list_tcps1();
				list_mem1();
			}
			
			/*socket data transfer,use dma*/
			common_w_socket(dev);
		}
		else
		{
			DBG("dev %d in config data flag %d\n",dev,flag);					
			int tmp=0;
			tmp=common_rw_config(dev);	
			if(tmp!=-1)
			{
				need_reconfig|=tmp;
				if(flag==0)
				{
					flag=1;
					socket_ctl(RT_FALSE,dev);
					/*config data parser*/
					if(need_reconfig==2||need_reconfig==3)
					{	
						socket_ctl(RT_FALSE,0);
						socket_ctl(RT_FALSE,1);
						socket_ctl(RT_FALSE,2);
						socket_ctl(RT_FALSE,3);
					}
					rt_thread_delay(10);
					list_thread();
					list_tcps1();
					list_mem1();
				}
				rt_device_write(common_dev[dev], 0, (void *)COMMAND_OK, strlen(COMMAND_OK));
			}
			else
				rt_device_write(common_dev[dev], 0, (void *)COMMAND_FAIL, strlen(COMMAND_FAIL));
		}
	}
}
static void common_r(void* parameter)
{
    rt_size_t data_size;
    const void *last_data_ptr;
	int dev=(int)parameter;
	while(1)
	{
		rt_data_queue_pop(&g_data_queue[dev], &last_data_ptr, &data_size, RT_WAITING_FOREVER);
		if(data_size!=0 && last_data_ptr)
		{			
			rt_device_write(common_dev[(dev-1)/2], 0, last_data_ptr, data_size);
		}
	}
}

/*init common1,2,3,4 for 4 socket*/
int common_init(int dev)//0 uart , 1 parallel bus, 2 usb
{
	/*init common device*/
	rt_err_t result;
	rt_uint8_t common[10];
	int i;
	/*read config data from internal flash*/
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	MAP_SysCtlDelay(1);
	//config select
	MAP_GPIOIntDisable(GPIO_PORTD_BASE, GPIO_PIN_2);
	MAP_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_2);//ind0
	MAP_GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_BOTH_EDGES);
	MAP_IntEnable(INT_GPIOD);
	MAP_GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_2);
	int ui32Status = MAP_GPIOIntStatus(GPIO_PORTD_BASE, true);
	MAP_GPIOIntClear(GPIO_PORTD_BASE, ui32Status);
	//connect ind
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);//CNN1
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_2);//CNN2
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_3);//CNN3
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4);//CNN4
	MAP_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_PIN_4);	
	MAP_GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0);	
	MAP_GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_2,0);	
	default_config();

	for(i=3;i<67;i++)
	{
		config_socket0_ip6[i]=0xff;
		config_socket1_ip6[i]=0xff;
		config_socket2_ip6[i]=0xff;
		config_socket3_ip6[i]=0xff;
		config_local_ip6[i]=0xff;
	}
	config_socket0_ip6[67]=0x26;
	config_socket1_ip6[67]=0x26;
	config_socket2_ip6[67]=0x26;
	config_socket3_ip6[67]=0x26;
	config_local_ip6[67]=0x26;
	config_socket0_ip6[68]=0xfa;
	config_socket1_ip6[68]=0xfa;
	config_socket2_ip6[68]=0xfa;
	config_socket3_ip6[68]=0xfa;
	config_local_ip6[68]=0xfa;
	for(i=0;i<4;i++)
	{
		//config sem
		rt_sprintf(common,"common_%d_rx",i);
		rt_sem_init(&(rx_sem[i]), common, 0, 0);
		if(dev==DEV_UART)
		{
			if(i==1)
				rt_sprintf(common,"uart%d",4);
			else
				rt_sprintf(common,"uart%d",i);
		}
		else if(dev==DEV_BUS)
		{

		}
		else
		{

		}
		DBG("To open ==>%s\n",common);
		//open uart ,parallel ,usb
		common_dev[i] = rt_device_find(common);
		if (common_dev[i] == RT_NULL)
		{
			DBG("app_common: can not find device: %s\n",common);
			return 0;
		}
		if (rt_device_open(common_dev[i], RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX) == RT_EOK)
		{
			//create common_w,r thread to read data from socket write to uart,parallel,usb
			//or read from uart,parallel,usb write to socket
			rt_sprintf(common,"common_wx%d",i);
			tid_common_w[i] = rt_thread_create(common,common_w, (void *)(i*2),2048, 20, 10);
			rt_sprintf(common,"common_rx%d",i);
			tid_common_r[i] = rt_thread_create(common,common_r, (void *)(i*2+1),2048, 20, 10);

			rt_device_set_rx_indicate(common_dev[i], common_rx_ind);

			if(tid_common_w[i]!=RT_NULL)
				rt_thread_startup(tid_common_w[i]);			
			if(tid_common_r[i]!=RT_NULL)
				rt_thread_startup(tid_common_r[i]);
		}
	}
	for(i=0;i<4;i++)
	socket_ctl(RT_TRUE,i);
	list_mem1();
	DBG("common_init ok\n");
	return 1;
}
#endif
