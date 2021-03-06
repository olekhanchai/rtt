#include <rtthread.h>
#include <board.h>
#include <components.h>
#define COMMON_REG_MR			0x0000
#define COMMON_REG_GAR0			0x0001
#define COMMON_REG_GAR1			0x0002
#define COMMON_REG_GAR2			0x0003
#define COMMON_REG_GAR3			0x0004
#define COMMON_REG_SUBR0		0x0005
#define COMMON_REG_SUBR1		0x0006
#define COMMON_REG_SUBR2		0x0007
#define COMMON_REG_SUBR3		0x0008
#define COMMON_REG_SHAR0		0x0009
#define COMMON_REG_SHAR1		0x000A
#define COMMON_REG_SHAR2		0x000B
#define COMMON_REG_SHAR3		0x000C
#define COMMON_REG_SHAR4		0x000D
#define COMMON_REG_SHAR5		0x000E
#define COMMON_REG_SIPR0		0x000F
#define COMMON_REG_SIPR1		0x0010
#define COMMON_REG_SIPR2		0x0011
#define COMMON_REG_SIPR3		0x0012
#define COMMON_REG_IR			0x0015
#define COMMON_REG_IMR			0x0016
#define COMMON_REG_RTR0			0x0017
#define COMMON_REG_RTR1			0x0018
#define COMMON_REG_RCR			0x0019
#define COMMON_REG_RMSR			0x001A
#define COMMON_REG_TMSR			0x001B
#define COMMON_REG_UIPR0		0x002A
#define COMMON_REG_UIPR1		0x002B
#define COMMON_REG_UIPR2		0x002C
#define COMMON_REG_UIPR3		0x002D
#define COMMON_REG_UPORT0		0x002E
#define COMMON_REG_UPORT1		0x002F
#define C_MR_RST				(1<<7)
#define C_MR_PING_ENABLE		(1<<4)
#define C_MR_AI_ENABLE			(1<<1)
#define C_MR_IND_ENABLE			(1<<0)
#define C_IR_CONFLICT_IP		(1<<7)
#define C_IR_UNREACH_IP			(1<<6)
#define C_IR_S3_INT				(1<<3)
#define C_IR_S2_INT				(1<<2)
#define C_IR_S1_INT				(1<<1)
#define C_IR_S0_INT				(1<<0)
#define C_IMR_CONFLICT_IP		(1<<7)
#define C_IMR_UNREACH_IP		(1<<6)
#define C_IMR_S3_INT			(1<<3)
#define C_IMR_S2_INT			(1<<2)
#define C_IMR_S1_INT			(1<<1)
#define C_IMR_S0_INT			(1<<0)
#define C_IMSR_S0_1KB			0x00
#define C_IMSR_S0_2KB			0x01
#define C_IMSR_S0_4KB			0x02
#define C_IMSR_S0_8KB			0x03
#define C_IMSR_S1_1KB			(0x00<<2)
#define C_IMSR_S1_2KB			(0x01<<2)
#define C_IMSR_S1_4KB			(0x02<<2)
#define C_IMSR_S1_8KB			(0x03<<2)
#define C_IMSR_S2_1KB			(0x00<<4)
#define C_IMSR_S2_2KB			(0x01<<4)
#define C_IMSR_S2_4KB			(0x02<<4)
#define C_IMSR_S2_8KB			(0x03<<4)
#define C_IMSR_S3_1KB			(0x00<<6)
#define C_IMSR_S3_2KB			(0x01<<6)
#define C_IMSR_S3_4KB			(0x02<<6)
#define C_IMSR_S3_8KB			(0x03<<6)

#define SOCKET_BASE					0x0400
#define SOCKET_REG_S0_MR			(SOCKET_BASE+0x0000)
#define SOCKET_REG_S0_CR			(SOCKET_BASE+0x0001)
#define SOCKET_REG_S0_IR			(SOCKET_BASE+0x0002)
#define SOCKET_REG_S0_SR			(SOCKET_BASE+0x0003)
#define SOCKET_REG_S0_PROT0			(SOCKET_BASE+0x0004)
#define SOCKET_REG_S0_PROT1			(SOCKET_BASE+0x0005)
#define SOCKET_REG_S0_DHAR0			(SOCKET_BASE+0x0006)
#define SOCKET_REG_S0_DHAR1			(SOCKET_BASE+0x0007)
#define SOCKET_REG_S0_DHAR2			(SOCKET_BASE+0x0008)
#define SOCKET_REG_S0_DHAR3			(SOCKET_BASE+0x0009)
#define SOCKET_REG_S0_DHAR4			(SOCKET_BASE+0x000A)
#define SOCKET_REG_S0_DHAR5			(SOCKET_BASE+0x000B)
#define SOCKET_REG_S0_DIPR0			(SOCKET_BASE+0x000C)
#define SOCKET_REG_S0_DIPR1			(SOCKET_BASE+0x000D)
#define SOCKET_REG_S0_DIPR2			(SOCKET_BASE+0x000E)
#define SOCKET_REG_S0_DIPR3			(SOCKET_BASE+0x000F)
#define SOCKET_REG_S0_DPORT0		(SOCKET_BASE+0x0010)
#define SOCKET_REG_S0_DPORT1		(SOCKET_BASE+0x0011)
#define SOCKET_REG_S0_MSSR0			(SOCKET_BASE+0x0012)
#define SOCKET_REG_S0_MSSR1			(SOCKET_BASE+0x0013)
#define SOCKET_REG_S0_PROTO			(SOCKET_BASE+0x0014)
#define SOCKET_REG_S0_TOS			(SOCKET_BASE+0x0015)
#define SOCKET_REG_S0_TTL			(SOCKET_BASE+0x0016)
#define SOCKET_REG_S0_TX_FSR0		(SOCKET_BASE+0x0020)
#define SOCKET_REG_S0_TX_FSR1		(SOCKET_BASE+0x0021)
#define SOCKET_REG_S0_TX_RD0		(SOCKET_BASE+0x0022)
#define SOCKET_REG_S0_TX_RD1		(SOCKET_BASE+0x0023)
#define SOCKET_REG_S0_TX_WR0		(SOCKET_BASE+0x0024)
#define SOCKET_REG_S0_TX_WR1		(SOCKET_BASE+0x0025)
#define SOCKET_REG_S0_RX_RSR0		(SOCKET_BASE+0x0026)
#define SOCKET_REG_S0_RX_RSR1		(SOCKET_BASE+0x0027)
#define SOCKET_REG_S0_RX_RD0		(SOCKET_BASE+0x0028)
#define SOCKET_REG_S0_RX_RD1		(SOCKET_BASE+0x0029)
#define SOCKET_REG_S0_RX_WR0		(SOCKET_BASE+0x002A)
#define SOCKET_REG_S0_RX_WR1		(SOCKET_BASE+0x002B)

#define SOCKET_REG_S1_MR			(SOCKET_BASE+0x0100)
#define SOCKET_REG_S1_CR			(SOCKET_BASE+0x0101)
#define SOCKET_REG_S1_IR			(SOCKET_BASE+0x0102)
#define SOCKET_REG_S1_SR			(SOCKET_BASE+0x0103)
#define SOCKET_REG_S1_PROT0			(SOCKET_BASE+0x0104)
#define SOCKET_REG_S1_PROT1			(SOCKET_BASE+0x0105)
#define SOCKET_REG_S1_DHAR0			(SOCKET_BASE+0x0106)
#define SOCKET_REG_S1_DHAR1			(SOCKET_BASE+0x0107)
#define SOCKET_REG_S1_DHAR2			(SOCKET_BASE+0x0108)
#define SOCKET_REG_S1_DHAR3			(SOCKET_BASE+0x0109)
#define SOCKET_REG_S1_DHAR4			(SOCKET_BASE+0x010A)
#define SOCKET_REG_S1_DHAR5			(SOCKET_BASE+0x010B)
#define SOCKET_REG_S1_DIPR0			(SOCKET_BASE+0x010C)
#define SOCKET_REG_S1_DIPR1			(SOCKET_BASE+0x010D)
#define SOCKET_REG_S1_DIPR2			(SOCKET_BASE+0x010E)
#define SOCKET_REG_S1_DIPR3			(SOCKET_BASE+0x010F)
#define SOCKET_REG_S1_DPORT0		(SOCKET_BASE+0x0110)
#define SOCKET_REG_S1_DPORT1		(SOCKET_BASE+0x0111)
#define SOCKET_REG_S1_MSSR0			(SOCKET_BASE+0x0112)
#define SOCKET_REG_S1_MSSR1			(SOCKET_BASE+0x0113)
#define SOCKET_REG_S1_PROTO			(SOCKET_BASE+0x0114)
#define SOCKET_REG_S1_TOS			(SOCKET_BASE+0x0115)
#define SOCKET_REG_S1_TTL			(SOCKET_BASE+0x0116)
#define SOCKET_REG_S1_TX_FSR0		(SOCKET_BASE+0x0120)
#define SOCKET_REG_S1_TX_FSR1		(SOCKET_BASE+0x0121)
#define SOCKET_REG_S1_TX_RD0		(SOCKET_BASE+0x0122)
#define SOCKET_REG_S1_TX_RD1		(SOCKET_BASE+0x0123)
#define SOCKET_REG_S1_TX_WR0		(SOCKET_BASE+0x0124)
#define SOCKET_REG_S1_TX_WR1		(SOCKET_BASE+0x0125)
#define SOCKET_REG_S1_RX_RSR0		(SOCKET_BASE+0x0126)
#define SOCKET_REG_S1_RX_RSR1		(SOCKET_BASE+0x0127)
#define SOCKET_REG_S1_RX_RD0		(SOCKET_BASE+0x0128)
#define SOCKET_REG_S1_RX_RD1		(SOCKET_BASE+0x0129)
#define SOCKET_REG_S1_RX_WR0		(SOCKET_BASE+0x012A)
#define SOCKET_REG_S1_RX_WR1		(SOCKET_BASE+0x012B)

#define SOCKET_REG_S2_MR			(SOCKET_BASE+0x0200)
#define SOCKET_REG_S2_CR			(SOCKET_BASE+0x0201)
#define SOCKET_REG_S2_IR			(SOCKET_BASE+0x0202)
#define SOCKET_REG_S2_SR			(SOCKET_BASE+0x0203)
#define SOCKET_REG_S2_PROT0			(SOCKET_BASE+0x0204)
#define SOCKET_REG_S2_PROT1			(SOCKET_BASE+0x0205)
#define SOCKET_REG_S2_DHAR0			(SOCKET_BASE+0x0206)
#define SOCKET_REG_S2_DHAR1			(SOCKET_BASE+0x0207)
#define SOCKET_REG_S2_DHAR2			(SOCKET_BASE+0x0208)
#define SOCKET_REG_S2_DHAR3			(SOCKET_BASE+0x0209)
#define SOCKET_REG_S2_DHAR4			(SOCKET_BASE+0x020A)
#define SOCKET_REG_S2_DHAR5			(SOCKET_BASE+0x020B)
#define SOCKET_REG_S2_DIPR0			(SOCKET_BASE+0x020C)
#define SOCKET_REG_S2_DIPR1			(SOCKET_BASE+0x020D)
#define SOCKET_REG_S2_DIPR2			(SOCKET_BASE+0x020E)
#define SOCKET_REG_S2_DIPR3			(SOCKET_BASE+0x020F)
#define SOCKET_REG_S2_DPORT0		(SOCKET_BASE+0x0210)
#define SOCKET_REG_S2_DPORT1		(SOCKET_BASE+0x0211)
#define SOCKET_REG_S2_MSSR0			(SOCKET_BASE+0x0212)
#define SOCKET_REG_S2_MSSR1			(SOCKET_BASE+0x0213)
#define SOCKET_REG_S2_PROTO			(SOCKET_BASE+0x0214)
#define SOCKET_REG_S2_TOS			(SOCKET_BASE+0x0215)
#define SOCKET_REG_S2_TTL			(SOCKET_BASE+0x0216)
#define SOCKET_REG_S2_TX_FSR0		(SOCKET_BASE+0x0220)
#define SOCKET_REG_S2_TX_FSR1		(SOCKET_BASE+0x0221)
#define SOCKET_REG_S2_TX_RD0		(SOCKET_BASE+0x0222)
#define SOCKET_REG_S2_TX_RD1		(SOCKET_BASE+0x0223)
#define SOCKET_REG_S2_TX_WR0		(SOCKET_BASE+0x0224)
#define SOCKET_REG_S2_TX_WR1		(SOCKET_BASE+0x0225)
#define SOCKET_REG_S2_RX_RSR0		(SOCKET_BASE+0x0226)
#define SOCKET_REG_S2_RX_RSR1		(SOCKET_BASE+0x0227)
#define SOCKET_REG_S2_RX_RD0		(SOCKET_BASE+0x0228)
#define SOCKET_REG_S2_RX_RD1		(SOCKET_BASE+0x0229)
#define SOCKET_REG_S2_RX_WR0		(SOCKET_BASE+0x022A)
#define SOCKET_REG_S2_RX_WR1		(SOCKET_BASE+0x022B)

#define SOCKET_REG_S3_MR			(SOCKET_BASE+0x0300)
#define SOCKET_REG_S3_CR			(SOCKET_BASE+0x0301)
#define SOCKET_REG_S3_IR			(SOCKET_BASE+0x0302)
#define SOCKET_REG_S3_SR			(SOCKET_BASE+0x0303)
#define SOCKET_REG_S3_PROT0			(SOCKET_BASE+0x0304)
#define SOCKET_REG_S3_PROT1			(SOCKET_BASE+0x0305)
#define SOCKET_REG_S3_DHAR0			(SOCKET_BASE+0x0306)
#define SOCKET_REG_S3_DHAR1			(SOCKET_BASE+0x0307)
#define SOCKET_REG_S3_DHAR2			(SOCKET_BASE+0x0308)
#define SOCKET_REG_S3_DHAR3			(SOCKET_BASE+0x0309)
#define SOCKET_REG_S3_DHAR4			(SOCKET_BASE+0x030A)
#define SOCKET_REG_S3_DHAR5			(SOCKET_BASE+0x030B)
#define SOCKET_REG_S3_DIPR0			(SOCKET_BASE+0x030C)
#define SOCKET_REG_S3_DIPR1			(SOCKET_BASE+0x030D)
#define SOCKET_REG_S3_DIPR2			(SOCKET_BASE+0x030E)
#define SOCKET_REG_S3_DIPR3			(SOCKET_BASE+0x030F)
#define SOCKET_REG_S3_DPORT0		(SOCKET_BASE+0x0310)
#define SOCKET_REG_S3_DPORT1		(SOCKET_BASE+0x0311)
#define SOCKET_REG_S3_MSSR0			(SOCKET_BASE+0x0312)
#define SOCKET_REG_S3_MSSR1			(SOCKET_BASE+0x0313)
#define SOCKET_REG_S3_PROTO			(SOCKET_BASE+0x0314)
#define SOCKET_REG_S3_TOS			(SOCKET_BASE+0x0315)
#define SOCKET_REG_S3_TTL			(SOCKET_BASE+0x0316)
#define SOCKET_REG_S3_TX_FSR0		(SOCKET_BASE+0x0320)
#define SOCKET_REG_S3_TX_FSR1		(SOCKET_BASE+0x0321)
#define SOCKET_REG_S3_TX_RD0		(SOCKET_BASE+0x0322)
#define SOCKET_REG_S3_TX_RD1		(SOCKET_BASE+0x0323)
#define SOCKET_REG_S3_TX_WR0		(SOCKET_BASE+0x0324)
#define SOCKET_REG_S3_TX_WR1		(SOCKET_BASE+0x0325)
#define SOCKET_REG_S3_RX_RSR0		(SOCKET_BASE+0x0326)
#define SOCKET_REG_S3_RX_RSR1		(SOCKET_BASE+0x0327)
#define SOCKET_REG_S3_RX_RD0		(SOCKET_BASE+0x0328)
#define SOCKET_REG_S3_RX_RD1		(SOCKET_BASE+0x0329)
#define SOCKET_REG_S3_RX_WR0		(SOCKET_BASE+0x032A)
#define SOCKET_REG_S3_RX_WR1		(SOCKET_BASE+0x032B)

#define S_MR_MULTI_ENABLE			(1<<7)
#define S_MR_NO_DELAY_ACK			(1<<6)
#define S_MR_PROTO_CLOSE			0x00
#define S_MR_PROTO_TCP				0x01
#define S_MR_PROTO_UDP				0x02
#define S_MR_PROTO_IPRAW			0x03
#define S_CR_COMMAND_OPEN			0x01
#define S_CR_COMMAND_LISTEN			0x02
#define S_CR_COMMAND_CONNECT		0x04
#define S_CR_COMMAND_DISCON			0x08
#define S_CR_COMMAND_CLOSE			0x10
#define S_CR_COMMAND_SEND			0x20
#define S_CR_COMMAND_SENDMAC		0x21
#define S_CR_COMMAND_SENDKEEP		0x22
#define S_CR_COMMAND_RECV			0x40
#define S_IR_SEND_OK				(1<<4)
#define S_IR_TIMEOUT				(1<<3)
#define S_IR_RECV					(1<<2)
#define S_IR_DISCON					(1<<1)
#define S_IR_CON					(1<<0)
#define S_SR_SOCK_CLOSED			0x00
#define S_SR_SOCK_INIT				0x13
#define S_SR_SOCK_LISTEN			0x14
#define S_SR_SOCK_SOCK_ESB			0x17
#define S_SR_SOCK_CLOSE_WAIT		0x1C
#define S_SR_SOCK_UDP				0x22
#define S_SR_SOCK_IPRAW				0x32
#define S_SR_SOCK_MACRAW			0x42
#define S_SR_SOCK_PPPOE				0x5F
#define S_SR_SOCK_SYSENT			0x15
#define S_SR_SOCK_SYNRECV			0x16
#define S_SR_SOCK_FIN_WAIT			0x18
#define S_SR_SOCK_CLOSING			0x1a
#define S_SR_SOCK_TIME_WAIT			0x1b
#define S_SR_SOCK_LAST_ACK			0x1d
#define S_SR_SOCK_ARP1				0x11
#define S_SR_SOCK_ARP2				0x21
#define S_SR_SOCK_ARP3				0x31

typedef struct {
	rt_uint8_t mr;			/*0x0000*/
	rt_uint8_t gar0;			/*0x0001*/
	rt_uint8_t gar1;			/*0x0002*/
	rt_uint8_t gar2;			/*0x0003*/
	rt_uint8_t gar3;			/*0x0004*/
	rt_uint8_t subr0;			/*0x0005*/
	rt_uint8_t subr1;			/*0x0006*/
	rt_uint8_t subr2;			/*0x0007*/
	rt_uint8_t subr3;			/*0x0008*/
	rt_uint8_t shar0;			/*0x0009*/
	rt_uint8_t shar1;			/*0x000A*/
	rt_uint8_t shar2;			/*0x000B*/
	rt_uint8_t shar3;			/*0x000C*/
	rt_uint8_t shar4;			/*0x000D*/
	rt_uint8_t shar5;			/*0x000E*/
	rt_uint8_t sipr0;			/*0x000F*/
	rt_uint8_t sipr1;			/*0x0010*/
	rt_uint8_t sipr2;			/*0x0011*/
	rt_uint8_t sipr3;			/*0x0012*/
	rt_uint8_t ir;			/*0x0015*/
	rt_uint8_t imr;			/*0x0016*/
	rt_uint8_t rtr0;			/*0x0017*/
	rt_uint8_t rtr1;			/*0x0018*/
	rt_uint8_t rcr;			/*0x0019*/
	rt_uint8_t rmsr;			/*0x001A*/
	rt_uint8_t tmsr;			/*0x001B*/
	rt_uint8_t uipr0;			/*0x002A*/
	rt_uint8_t uipr1;			/*0x002B*/
	rt_uint8_t uipr2;			/*0x002C*/
	rt_uint8_t uipr3;			/*0x002D*/
	rt_uint8_t uport0;		/*0x002E*/
	rt_uint8_t uport1;		/*0x002F*/
}tm4c_lwip_common_reg,*ptm4c_lwip_common_reg;

typedef struct {
	rt_uint8_t mr;				/*0x0401*/
	rt_uint8_t cr;				/*0x0402*/
	rt_uint8_t ir;				/*0x0403*/
	rt_uint8_t sr;				/*0x0404*/
	rt_uint8_t port0;				/*0x0405*/
	rt_uint8_t port1;				/*0x0406*/
	rt_uint8_t dhar0;				/*0x0407*/
	rt_uint8_t dhar1;				/*0x0408*/
	rt_uint8_t dhar2;				/*0x0409*/
	rt_uint8_t dhar3;				/*0x040A*/
	rt_uint8_t dhar4;				/*0x040B*/
	rt_uint8_t dhar5;				/*0x040C*/
	rt_uint8_t dipr0;				/*0x040D*/
	rt_uint8_t dipr1;				/*0x040E*/
	rt_uint8_t dipr2;				/*0x040F*/
	rt_uint8_t dipr3;				/*0x0410*/
	rt_uint8_t dport0;			/*0x0411*/
	rt_uint8_t dport1;			/*0x0412*/
	rt_uint8_t mssr0;				/*0x0413*/
	rt_uint8_t mssr1;				/*0x0414*/
	rt_uint8_t proto;				/*0x0415*/	
	rt_uint8_t tos;				/*0x0416*/
	rt_uint8_t ttl;				/*0x0420*/
	rt_uint8_t txfsr0;			/*0x0421*/
	rt_uint8_t txfsr1;			/*0x0422*/
	rt_uint8_t txrd0;				/*0x0423*/
	rt_uint8_t txrd1;				/*0x0424*/
	rt_uint8_t txwr0;				/*0x0425*/
	rt_uint8_t txwr1;				/*0x0425*/
	rt_uint8_t rxrsr0;			/*0x0426*/
	rt_uint8_t rxrsr1;			/*0x0427*/
	rt_uint8_t rxrd0;				/*0x0428*/
	rt_uint8_t rxrd1;				/*0x0429*/	
	rt_uint8_t rxwr0;				/*0x042A*/
	rt_uint8_t rxwr1;				/*0x042B*/
}tm4c_lwip_socket_reg,*ptm4c_lwip_socket_reg;

typedef struct{
	tm4c_lwip_socket_reg *socket[4];
	tm4c_lwip_common_reg *common;
}lwip_app,*plwip_app;
ALIGN(RT_ALIGN_SIZE)
plwip_app g_lwip_app;
