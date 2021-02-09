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

typedef struct DCDC_data
    {
    uint8_t V_IP_config[4];
    uint8_t V_MASK_config[4];
    uint8_t V_GET_config[4];
    uint8_t V_MAC_config[8];
    uint16_t dc_bus_volt ;
    uint16_t power_bus_volt ;
    uint16_t curr_raw;
    uint16_t curr_ma;
    uint8_t fault_bus ;
    uint8_t power;
    uint8_t start_flag;
    uint8_t stop_flag;
    
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
 Modul_data_t modul_state[all_modul]; 
 uint8_t led_onoff[all_modul*4*16];
 uint8_t led_data_r[all_modul*4*4];
 uint8_t led_data_g[all_modul*4*4];
 uint8_t led_data_b[all_modul*4*4];
 uint8_t led_data_ir[all_modul*4*4];
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
//extern xTaskHandle xHandle_start_ethernet;
extern xTaskHandle xHandle_reload_io;
extern uint8_t g_master_txBuff[I2C_DATA_LENGTH];
extern uint8_t g_master_rxBuff[I2C_DATA_LENGTH];
void SPI0_TX (uint8_t *SPI_masterTxData,uint8_t *SPI_masterRxData,uint32_t data_size,uint8_t psc);
void SPI1_TX (uint8_t *SPI_masterTxData,uint8_t *SPI_masterRxData,uint32_t data_size,uint8_t psc);
void hard_config (void);
void  CAN_transmit (void *pvParameters);
uint8_t  load_power(int pwmValue);
void start_DCDC(void );
void stop_DCDC(void);
void I2C_run (i2c_direction_t dir, uint8_t* data, uint8_t adress, uint8_t command, uint8_t LENGTH);
void Dac_run (i2c_direction_t dir, uint8_t* data, uint8_t adress, uint8_t command, uint8_t LENGTH);
void SPI1_TX_7bit (uint8_t *SPI_masterTxData,uint8_t *SPI_masterRxData,uint32_t data_size,uint8_t psc);