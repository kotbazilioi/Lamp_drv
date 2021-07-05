#define DSPI1_TRANSFER_SIZE 1U        /*! Transfer dataSize */ 
/* Get source clock for FTM driver */
#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_FastPeriphClk)
#include "fsl_ftm.h"
#include "FreeRTOS.h"
#include "udpecho/udpecho.h"
#include "lwip/tcpip.h"
#include "netif/ethernet.h"
#include "ethernetif.h"
#include "app.h"
#include "fsl_i2c.h"
#include "fsl_flexcan.h"
/*******************************************************************************
 * Definitions CAN
 ******************************************************************************/
#define BUS_CAN CAN0
#define BUS_CAN_CLKSRC kCLOCK_FastPeriphClk
#define BUS_CAN_DMA_CHANNEL 3
#define BUS_CAN_DMA_REQUEST kDmaRequestMux0CAN0


/* The Flextimer base address/channel used for board */
/* The Flextimer instance/channel used for board */
#define BOARD_FTM_BASEADDR FTM0
#define BOARD_FTM_CHANNEL kFTM_Chnl_1
/* Interrupt number and interrupt handler for the FTM instance used */
#define FTM_INTERRUPT_NUMBER FTM1_IRQn
#define FTM_LED_HANDLER FTM1_IRQHandler

/* Interrupt to enable and flag to read; depends on the FTM channel used */
#define FTM_CHANNEL_INTERRUPT_ENABLE kFTM_Chnl6InterruptEnable
#define FTM_CHANNEL_FLAG kFTM_Chnl1Flag

/* Get source clock for FTM driver */
#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_FastPeriphClk)
#define TX_MESSAGE_BUFFER_NUM (8)
#define RX_MESSAGE_BUFFER_NUM (9)
/* I2C source clock */
#define I2C_MASTER_CLK_SRC          I2C0_CLK_SRC
#define I2C_MASTER_CLK_FREQ         CLOCK_GetFreq(I2C0_CLK_SRC)
#define I2C_MASTER_BASEADDR I2C0
#define DMA_REQUEST_SRC             kDmaRequestMux0I2C0
#define I2C_DMAMUX_BASEADDR DMAMUX0
#define I2C_DMA_BASEADDR    DMA0
#define I2C_DMA_CHANNEL             9

//#define I2C_MASTER_SLAVE_ADDR_7BIT 0x78U
#define I2C_BAUDRATE              10000U
#define I2C_DATA_LENGTH            32U

typedef struct 
{ 
uint16_t  V_CRC_APP  ;
////#define A_CRC_APP 0x8004000
uint16_t  V_CRC_BOOT  ;
////#define A_CRC_DATA_BOOT 0x8004002
//////*******************************
uint8_t V_IP_CONFIG[4] ;
////#define A_IP_ADRESS 0x8004004
uint8_t V_IP_MASK[4] ;
////#define A_IP_MASK 0x8004008
uint8_t V_IP_GET[4] ;
////#define A_GET_ADRESS 0x800400c
uint8_t V_FW1_VER[4] ;
////#define A_FW_VER 0x8004010
uint32_t V_FW1_LEN ;
////#define A_FW_LEN 0x8004014
uint32_t V_BOOT_VER ;
////#define A_BOOT_VER 0x8004018


//*******************************
uint16_t V_CRC_DATA;
////#define A_CRC_DATA 0x800401C

uint8_t V_DHCP;
////#define A_DHCP 0x800401C
char V_LOGIN[16];
////#define A_LOGIN 0x8004021
char V_PASSWORD[16];
////#define A_PASSWORD 0x8004031
uint8_t V_IP_DNS[4] ;
////#define A_DNS 0x8004041
uint16_t V_WEB_PORT;
////#define A_WEB_PORT 0x8004045
uint16_t V_SNMP;
////#define A_SNMP 0x8004047
uint16_t V_HTTP_IP; 
////#define A_HTTP_IP 0x8004049
char V_ON_MESS[32];
////#define A_ON_MESS 0x800404B
char V_OFF_MESS[32];
////#define A_OFF_MESS 0x800406B
uint8_t V_FLAG_EN_MAN;
////#define A_FLAG_EN_MAN 0x800408B
uint8_t V_FLAG_EN_WEB;
////#define A_FLAG_EN_WEB 0x800408C
uint8_t V_FLAG_EN_RASP;
////#define A_FLAG_EN_RASP 0x800408D
uint8_t V_FLAG_EN_WATCHMAN;
////#define A_FLAG_EN_WATCHMAN 0x800408E
uint8_t V_EMAIL_ERR[32];
////#define A_EMAIL_ERR 0x800408F
uint16_t V_D_TIME[10][2];
////#define A_D1_TIME 0x80040AF
////#define A_D2_TIME 0x80040B3
////#define A_D3_TIME 0x80040B7
////#define A_D4_TIME 0x80040BB
////#define A_D5_TIME 0x80040BF
////#define A_D6_TIME 0x80040C3
////#define A_D7_TIME 0x80040C7
////#define A_D8_TIME 0x80040CB
////#define A_D9_TIME 0x80040CF
////#define A_D10_TIME 0x80040D3
uint16_t V_RD_TIME[3][2];
////#define A_RD1_TIME 0x80040D7
////#define A_RD2_TIME 0x80040DB
////#define A_RD3_TIME 0x80040DF
uint16_t V_IP_PING_TIME;
////#define A_IP_PING_TIME 0x80040E3
uint16_t V_TIME_SEND;
////#define A_TIME_SEND 0x80040E5
uint16_t V_TIME_READ;
////#define A_TIME_READ 0x80040E7
uint16_t V_N_RERID;
////#define A_N_RERID 0x80040E9
uint16_t V_TIME_RESET;
////#define A_TIME_RESET 0x80040EB
uint16_t V_DELAY_PING;
////#define A_DELAY_PING 0x80040ED
uint8_t V_SOST_RESET;
////#define A_SOST_RESET 0x80040EF
uint8_t V_N_PING;
////#define A_N_PING 0x80040F0
uint8_t V_IP_NTP1[4];
////#define A_IP_NTP1 0x80040F1
uint8_t V_IP_NTP2[4];
////#define A_IP_NTP2 0x80040F5
uint16_t V_PORT_NTP;
////#define A_PORT_NTP 0x80040F9
char V_NAME_SMTP[16];
////#define A_NAME_SNMP 0x80040FB
uint16_t V_PORT_SNMP;
////#define A_PORT_SNMP 0x800411B
char V_LOGIN_SMTP[16];
////#define A_LOGIN_SNMP 0x800411D
char V_PASSWORD_SMTP[16];
////#define A_PASSWORD_SNMP 0x800412B
char V_GEOM_NAME[32];
uint8_t V_ID_MAC[8];
char V_Name_dev[16];
char V_CALL_DATA[16];
uint8_t V_IP_SNMP[4];
signed char V_NTP_CIRCL;
uint8_t V_TYPE_OUT;
uint8_t V_resv[1655];

////#define A_RESV 0x800413B
////////////////////////////////////logs_t V_logs_struct;

}FW_data_t;   
   
typedef struct DCDC_data
    {
//    uint8_t V_IP_config[4];
//    uint8_t V_MASK_config[4];
//    uint8_t V_GET_config[4];
//    uint8_t V_MAC_config[8];
    uint16_t dc_bus_volt ;
    uint16_t power_bus_volt ;
    uint16_t curr_raw;
    uint16_t curr_ma;
    uint8_t fault_bus ;
    uint8_t power;
    uint8_t start_flag;
    uint8_t stop_flag;
    uint8_t stop_off;
    }DCDC_data_t;

typedef struct Modul_data_t
  {
    uint8_t temp; 
    uint16_t power;
  ///  uint8_t k_mech[58];
    
      
  }Modul_data_t;

typedef struct lamp_state_t
{
 uint16_t lamp_power ;
 uint16_t lamp_power_req;
 Modul_data_t modul_state[all_modul+1]; 
 uint8_t led_onoff[all_modul*4*16+32];
 uint8_t led_data_r[all_modul*4*4+32];
 uint8_t led_data_g[all_modul*4*4+32];
 uint8_t led_data_b[all_modul*4*4+32];
 uint8_t led_data_ir[all_modul*4*4+32];
 uint8_t led_data_r_req;
 uint8_t led_data_g_req;
 uint8_t led_data_b_req;
 uint8_t led_data_ir_req;
 uint16_t pixel_edit_index;
 uint8_t color_index;
 uint8_t reload_dot;
 uint8_t onoff_pix;
}lamp_state_t;

extern  bool isTransferCompleted0;

extern  bool isTransferCompleted1;


extern char can_data[8];
extern volatile bool txComplete ;
extern volatile bool rxComplete;
extern flexcan_handle_t flexcanHandle;
extern  uint32_t ct_can_frame;
extern  uint8_t flag_end_can_frame;
extern volatile uint8_t updatedDutycycle;
extern portCHAR bufer_trance[200];
extern xSemaphoreHandle xBinarySemaphore_CAN;
extern  flexcan_frame_t rxFrame1;
extern  flexcan_mb_transfer_t  rxXfer;
extern DCDC_data_t Hi_DCDC;
extern  lamp_state_t lamp_state;
extern xTaskHandle xHandle_start_convert;
extern uint8_t flag_set_ip;
extern struct netif fsl_netif0;
//extern xTaskHandle xHandle_start_ethernet;
extern xTaskHandle xHandle_reload_io;
extern uint8_t g_master_txBuff[I2C_DATA_LENGTH];
extern uint8_t g_master_rxBuff[I2C_DATA_LENGTH];
extern FW_data_t FW_data;
extern ip4_addr_t ipdns;
extern uint8_t flag_save_param;
 void save_settings (void);
void SPI0_TX (uint8_t *SPI_masterTxData,uint8_t *SPI_masterRxData,uint32_t data_size,uint8_t psc);
void SPI1_TX (uint8_t *SPI_masterTxData,uint8_t *SPI_masterRxData,uint32_t data_size,uint8_t psc);
void hard_config (void);
void  CAN_transmit (void *pvParameters);
uint8_t  load_power(int pwmValue);
void start_DCDC(void );
void stop_DCDC(void);
status_t  I2C_run (i2c_direction_t dir, uint8_t* data, uint8_t adress, uint8_t command, uint8_t LENGTH);
void Dac_run (i2c_direction_t dir, uint8_t* data, uint8_t adress, uint8_t command, uint8_t LENGTH);
void SPI1_TX_7bit (uint8_t *SPI_masterTxData,uint8_t *SPI_masterRxData,uint32_t data_size,uint8_t psc);
void save_settings (void);
void set_color_power (uint8_t r,uint8_t g,uint8_t b, uint8_t ir);