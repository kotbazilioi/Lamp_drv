#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_hsadc.h"
#include "stdint.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_xbara.h"
#include "fsl_pwm.h"
#include "board.h"
#include "fsl_flexcan.h"
#include  "stdlib.h"
#include "fsl_flexcan_edma.h"
#include "fsl_uart_edma.h"
#include "fsl_dma_manager.h"
#include "frame_decod.h"
#include "fsl_common.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_ftm.h"
#include "fsl_uart.h"
#include "fsl_edma.h"
#include "fsl_debug_console.h"
#include "fsl_flash.h"
#include "fsl_cmp.h"
#include "udpecho/udpecho.h"
#include "lwip/tcpip.h"
#include "netif/ethernet.h"
#include "ethernetif.h"
#include "app.h"
#include "task.h"
#include "fsl_mpu.h"
#include "fsl_dspi.h"
#include "fsl_dspi_edma.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include "ssd1306.h"
#include "fsl_port.h"
#include "hard_init.h"
#include "fsl_gpio.h"
#include "fsl_dspi.h"
#include "MKV58F24.h"
#include "fsl_i2c.h"
#include "fsl_i2c_edma.h"
#include "fsl_dmamux.h"
#include "heap_3.h"
#include <stdio.h>
  /*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SPI0_DSPI_MASTER_BASEADDR SPI0
#define SPI0_DSPI_MASTER_DMA_MUX_BASE DMAMUX_BASE
#define SPI0_DSPI_MASTER_DMA_BASE DMA_BASE
#define SPI0_DSPI_MASTER_DMA_RX_REQUEST_SOURCE kDmaRequestMux0SPI0Rx
#define SPI0_DSPI_MASTER_DMA_TX_REQUEST_SOURCE kDmaRequestMux0SPI0Tx
#define DSPI_MASTER_CLK_SRC DSPI0_CLK_SRC
//#define SPI0_DSPI_MASTER_PCS_FOR_INIT kDSPI_Pcs1
//#define SPI0_DSPI_MASTER_PCS_FOR_TRANSFER kDSPI_MasterPcs1


#define SPI0_DSPI_MASTER_DMA_MUX_BASEADDR ((DMAMUX_Type *)(SPI0_DSPI_MASTER_DMA_MUX_BASE))
#define SPI0_DSPI_MASTER_DMA_BASEADDR ((DMA_Type *)(SPI0_DSPI_MASTER_DMA_BASE))



//#define SPI0_SIZE 500U        /*! Transfer dataSize */
#define SPI0_BAUDRATE 10000U /*! Transfer baudrate - 500k */

#define SPI1_DSPI_MASTER_BASEADDR SPI1
#define SPI1_DSPI_MASTER_DMA_MUX_BASE DMAMUX_BASE
#define SPI1_DSPI_MASTER_DMA_BASE DMA_BASE
#define SPI1_DSPI_MASTER_DMA_RX_REQUEST_SOURCE kDmaRequestMux0Group1SPI1Rx
#define SPI1_DSPI_MASTER_DMA_TX_REQUEST_SOURCE kDmaRequestMux0Group1SPI1Tx
#define DSPI1_MASTER_CLK_SRC DSPI1_CLK_SRC
//#define SPI0_DSPI_MASTER_PCS_FOR_INIT kDSPI_Pcs1
//#define SPI0_DSPI_MASTER_PCS_FOR_TRANSFER kDSPI_MasterPcs1


#define SPI1_DSPI_MASTER_DMA_MUX_BASEADDR ((DMAMUX_Type *)(SPI1_DSPI_MASTER_DMA_MUX_BASE))
#define SPI1_DSPI_MASTER_DMA_BASEADDR ((DMA_Type *)(SPI1_DSPI_MASTER_DMA_BASE))



//#define SPI0_SIZE 500U        /*! Transfer dataSize */
#define SPI1_BAUDRATE 20000U /*! Transfer baudrate - 500k */
#define SPI1_BAUDRATE_LOW 100000U /*! Transfer baudrate - 500k */


/* The converter A's channels to sample. */
#define ADC0_CH0A 0U
#define ADC0_CH0A_MUX_NUM 0U
#define ADC0_CH0A_ENABLE_DIFF false

#define ADC0_CH2A 2U
#define ADC0_CH2A_MUX_NUM 0U
#define ADC0_CH2A_ENABLE_DIFF false

#define ADC0_CH3A 3U
#define ADC0_CH3A_MUX_NUM 0U
#define ADC0_CH3A_ENABLE_DIFF false


#define ADC0_CH7A 7U
#define ADC0_CH7A_MUX_NUM 0U
#define ADC0_CH7A_ENABLE_DIFF false

#define ADC0_CH10A 6U
#define ADC0_CH10A_MUX_NUM 2U
#define ADC0_CH10A_ENABLE_DIFF false

#define ADC0_CH11A 7U
#define ADC0_CH11A_MUX_NUM 2U
#define ADC0_CH11A_ENABLE_DIFF false


#define ADC0_CH12A 6U
#define ADC0_CH12A_MUX_NUM 3U
#define ADC0_CH12A_ENABLE_DIFF false



/* The converter B's channels to sample. */
#define ADC0_CH0B 0U
#define ADC0_CH0B_MUX_NUM 0U
#define ADC0_CH0B_ENABLE_DIFF false 


#define ADC0_CH8B 6U
#define ADC0_CH8B_MUX_NUM 1U
#define ADC0_CH8B_ENABLE_DIFF false

#define ADC0_CH10B 6U
#define ADC0_CH10B_MUX_NUM 2U
#define ADC0_CH10B_ENABLE_DIFF false

#define ADC0_CH11B 7U
#define ADC0_CH11B_MUX_NUM 2U
#define ADC0_CH11B_ENABLE_DIFF false


/// HSADC1 config data define
//**************A canal **************
#define ADC1_CH3A 3U
#define ADC1_CH3A_MUX_NUM 0U
#define ADC1_CH3A_ENABLE_DIFF false


//-************B canal ***************

#define ADC1_CH0B 0U
#define ADC1_CH0B_MUX_NUM 0U
#define ADC1_CH0B_ENABLE_DIFF false

#define ADC1_CH1B 1U
#define ADC1_CH1B_MUX_NUM 0U
#define ADC1_CH1B_ENABLE_DIFF false

#define ADC1_CH2B 2U
#define ADC1_CH2B_MUX_NUM 0U
#define ADC1_CH2B_ENABLE_DIFF false

#define ADC1_CH3B 3U
#define ADC1_CH3B_MUX_NUM 0U
#define ADC1_CH3B_ENABLE_DIFF false

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/
    uint16_t HSADC0_sampleMask;
    uint16_t HSADC1_sampleMask;
    uint16_t ct_hsadc0,ct_hsadc1;
    uint8_t flag_front_strob[6]={0};
    
   
    edma_transfer_config_t hsadc0_dma_transferConfig_4;
    edma_config_t hsadc0_dma_Config_4;
    uint32_t transferDone = false;
    edma_handle_t g_EDMA_Handle_hsadc0_4;

    edma_transfer_config_t hsadc0_dma_transferConfig_5;
    edma_config_t hsadc0_dma_Config_5;
    edma_handle_t g_EDMA_Handle_hsadc0_5;
    
     edma_transfer_config_t hsadc0_dma_transferConfig_6;
    edma_config_t hsadc0_dma_Config_6;
    edma_handle_t g_EDMA_Handle_hsadc0_6;
    
    edma_transfer_config_t hsadc0_dma_transferConfig_7;
    edma_config_t hsadc0_dma_Config_7;
    edma_handle_t g_EDMA_Handle_hsadc0_7;
    
    edma_transfer_config_t hsadc0_dma_transferConfig_8;
    edma_config_t hsadc0_dma_Config_8;
    edma_handle_t g_EDMA_Handle_hsadc0_8;
    
    edma_transfer_config_t hsadc0_dma_transferConfig_16;
    edma_config_t hsadc0_dma_Config_16;
    edma_handle_t g_EDMA_Handle_hsadc0_16;
    
    edma_transfer_config_t hsadc0_dma_transferConfig_17;
    edma_config_t hsadc0_dma_Config_17;
    edma_handle_t g_EDMA_Handle_hsadc0_17;
    
    edma_transfer_config_t hsadc0_dma_transferConfig_18;
    edma_config_t hsadc0_dma_Config_18;
    edma_handle_t g_EDMA_Handle_hsadc0_18;
   
    
    
    
    uint16_t data_hsadc0_buf1[50]; 
    uint16_t data_hsadc0_buf2[50];
    uint16_t data_hsadc0_buf3[50];
    uint16_t data_hsadc0_buf4[128]; 
    uint16_t data_hsadc0_buf5[50]; 
    uint16_t data_hsadc0_buf6[50];
    uint16_t data_hsadc0_buf7[50];
    uint16_t data_hsadc0_buf8[50];
    uint32_t flag_start_adc[8]={0};     
    uint32_t ct_shortcur[6]={0};


    DCDC_data_t Hi_DCDC;
    lamp_state_t lamp_state;
    FW_data_t FW_data;
    
          
  // __root uint16_t data_hsadc0_buf[8][256]@0x2001A540;
    uint32_t data_transfert_hsadc0=0;
    
    
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* DSPI user callback */
void DSPI1_MasterUserCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData);
void DSPI1_SlaveUserCallback(SPI_Type *base, dspi_slave_edma_handle_t *handle, status_t status, void *userData);
void DSPI0_MasterUserCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData);
void DSPI0_SlaveUserCallback(SPI_Type *base, dspi_slave_edma_handle_t *handle, status_t status, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
//uint8_t masterRxData[TRANSFER_SIZE] = {0};
//uint8_t masterTxData[TRANSFER_SIZE] = {0};
//uint8_t slaveRxData[TRANSFER_SIZE] = {0};
//uint8_t slaveTxData[TRANSFER_SIZE] = {0};

dspi_master_edma_handle_t g_dspi_edma_m_handle;
edma_handle_t dspiEdmaMasterRxRegToRxDataHandle;
edma_handle_t dspiEdmaMasterTxDataToIntermediaryHandle;
edma_handle_t dspiEdmaMasterIntermediaryToTxRegHandle;

dspi_slave_edma_handle_t g_dspi_edma_s_handle;
edma_handle_t dspiEdmaSlaveRxRegToRxDataHandle;
edma_handle_t dspiEdmaSlaveTxDataToTxRegHandle;

dspi_master_edma_handle_t g_dspi1_edma_m_handle;
edma_handle_t dspi1EdmaMasterRxRegToRxDataHandle;
edma_handle_t dspi1EdmaMasterTxDataToIntermediaryHandle;
edma_handle_t dspi1EdmaMasterIntermediaryToTxRegHandle;

dspi_slave_edma_handle_t g_dspi1_edma_s_handle;
edma_handle_t dspi1EdmaSlaveRxRegToRxDataHandle;
edma_handle_t dspi1EdmaSlaveTxDataToTxRegHandle;

 bool isTransferCompleted0 = false;
 bool isTransferCompleted1 = false;
 dspi_master_config_t masterConfig;
 dspi_master_config_t masterConfig1;
 dspi_master_config_t masterConfig2;
 uint32_t srcClock_Hz;
 uint8_t can_tx_addr=ID_TX_DEF;  
 uint8_t can_rx_addr = ID_RX0_DEF;
 edma_config_t edmaConfig;
 flexcan_config_t flexcanConfig;
 flexcan_rx_fifo_config_t rxFifoConfig;
 uint32_t rxFifoFilter[1] = {0};
volatile bool txComplete = false;
volatile bool rxComplete = false;

  uint32_t ct_can_frame=0;
  uint8_t flag_end_can_frame=0;

  flexcan_frame_t rxFrame1;
  flexcan_mb_transfer_t  rxXfer;
  char can_data[8]={0};
flexcan_edma_handle_t flexcanEdmaHandle;
edma_handle_t flexcanRxFifoEdmaHandle;
  flexcan_handle_t flexcanHandle;
flexcan_fifo_transfer_t rxFifoXfer;
flexcan_frame_t txFrame;//, rxFrame;
extern flexcan_frame_t txFrame1;

volatile uint8_t updatedDutycycle = 50U;
portCHAR bufer_trance[200];
xSemaphoreHandle xBinarySemaphore_CAN;

flexcan_rx_mb_config_t mbConfig;

unsigned char flag_err[6]={0};
xTaskHandle xHandle_start_convert;
//xTaskHandle xHandle_start_ethernet;
xTaskHandle xHandle_reload_io;


uint8_t g_master_txBuff[I2C_DATA_LENGTH];
uint8_t g_master_rxBuff[I2C_DATA_LENGTH];
i2c_master_edma_handle_t g_m_dma_handle;
edma_handle_t edmaHandle;
volatile bool g_MasterCompletionFlag = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
void HSADC0_CCA_IRQHandler(void)
{
  
 /* if(ct_hsadc0<256)          
          
          {
           cur_adc_data[3][ct_hsadc0]=(HSADC_GetSampleResultValue(HSADC0, 0U)>>3);
           cur_adc_data[0][ct_hsadc0]=(HSADC_GetSampleResultValue(HSADC0, 1U)>>3);
           cur_adc_data[4][ct_hsadc0]=(HSADC_GetSampleResultValue(HSADC0, 2U)>>3);           
                   
           bus_adc_data[ct_hsadc0] =((HSADC_GetSampleResultValue(HSADC0, 5U)>>3)<<1)-
                          (HSADC_GetSampleResultValue(HSADC0, 4U)>>3);
           ct_hsadc0++;
          }
      else
          {
            ct_hsadc0=0;
          }*/

        HSADC_ClearStatusFlags(HSADC0, kHSADC_ConverterAEndOfScanFlag);
    

}

void EDMA_Callback_hsadc0_4 (struct _edma_handle *handle, void *userData, bool transferDone, uint32_t tcds)
{ 
  uint32_t temp_data_bus=0;
  uint32_t i;
  
 if (EDMA_GetChannelStatusFlags(DMA0,4)&kEDMA_DoneFlag==kEDMA_DoneFlag)
 {
    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_4, &hsadc0_dma_transferConfig_4) == kStatus_Success)
      {
          flag_start_adc[0]=1;
           for (i=0;i<10;i++)
          {
            temp_data_bus = temp_data_bus + ((data_hsadc0_buf1[i])>>3);
          }
         temp_data_bus=temp_data_bus/10;
         
          Hi_DCDC.curr_raw=temp_data_bus;
          Hi_DCDC.curr_ma=((Hi_DCDC.curr_raw*3780)/1000)-2058;
          
          
          EDMA_ClearChannelStatusFlags(DMA0,4,kEDMA_DoneFlag);
      } 
    else 
      {
          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");          
      }
    EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_4);
 }
}

void EDMA_Callback_hsadc0_5 (struct _edma_handle *handle, void *userData, bool transferDone, uint32_t tcds)
{
  uint32_t temp_data_bus=0;
  uint32_t i;
  
  if (EDMA_GetChannelStatusFlags(DMA0,5)&kEDMA_DoneFlag==kEDMA_DoneFlag)
 {
    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_5, &hsadc0_dma_transferConfig_5) == kStatus_Success)
      {
          flag_start_adc[1]=1;
           for (i=0;i<10;i++)
          {
            temp_data_bus = temp_data_bus + ((data_hsadc0_buf2[i])>>3);
          }
         temp_data_bus=temp_data_bus/10;
          Hi_DCDC.power_bus_volt=465+((68234*temp_data_bus)/10000);
          
          
          EDMA_ClearChannelStatusFlags(DMA0,5,kEDMA_DoneFlag);
      } 
    else 
      {
          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");          
      }
    EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_5);
 }
}


void EDMA_Callback_hsadc0_6 (struct _edma_handle *handle, void *userData, bool transferDone, uint32_t tcds)
{
 if (EDMA_GetChannelStatusFlags(DMA0,6)&kEDMA_DoneFlag==kEDMA_DoneFlag)
 {
    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_6, &hsadc0_dma_transferConfig_6) == kStatus_Success)
      {
          flag_start_adc[2]=1;
          EDMA_ClearChannelStatusFlags(DMA0,6,kEDMA_DoneFlag);
      } 
    else 
      {
          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");          
      }
    EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_6);
 }
}


void EDMA_Callback_hsadc0_7 (struct _edma_handle *handle, void *userData, bool transferDone, uint32_t tcds)
{
  if (EDMA_GetChannelStatusFlags(DMA0,7)&kEDMA_DoneFlag==kEDMA_DoneFlag)
 {
    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_7, &hsadc0_dma_transferConfig_7) == kStatus_Success)
      {
         flag_start_adc[6]=1;
          EDMA_ClearChannelStatusFlags(DMA0,7,kEDMA_DoneFlag);
      } 
    else 
      {
          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");          
      }
    EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_7);
 }
} 

void EDMA_Callback_hsadc0_8 (struct _edma_handle *handle, void *userData, bool transferDone, uint32_t tcds)
{
  if (EDMA_GetChannelStatusFlags(DMA0,8)&kEDMA_DoneFlag==kEDMA_DoneFlag)
 {
    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_8, &hsadc0_dma_transferConfig_8) == kStatus_Success)
      {
          flag_start_adc[7]=1;
          EDMA_ClearChannelStatusFlags(DMA0,8,kEDMA_DoneFlag);
      } 
    else 
      {
          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");          
      }
    EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_8);
 }
}

void EDMA_Callback_hsadc0_16 (struct _edma_handle *handle, void *userData, bool transferDone, uint32_t tcds)
{
  uint32_t temp_data_bus=0;
  uint32_t i;
  
  if (EDMA_GetChannelStatusFlags(DMA0,16)&kEDMA_DoneFlag==kEDMA_DoneFlag)
 {
    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_16, &hsadc0_dma_transferConfig_16) == kStatus_Success)
      {
         flag_start_adc[3]=1;
         for (i=0;i<128;i++)
          {
            temp_data_bus = temp_data_bus + ((data_hsadc0_buf4[i])>>3);
          }
         temp_data_bus=temp_data_bus/1219;////560;
         Hi_DCDC.dc_bus_volt=temp_data_bus;
        
         
         EDMA_ClearChannelStatusFlags(DMA0,16,kEDMA_DoneFlag);
      } 
    else 
      {
          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");          
      }
    EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_16);
 } 
}

void EDMA_Callback_hsadc0_17 (struct _edma_handle *handle, void *userData, bool transferDone, uint32_t tcds)
{
  uint32_t temp_data_bus=0;
  uint32_t i;
  
  
  if (EDMA_GetChannelStatusFlags(DMA0,17)&kEDMA_DoneFlag==kEDMA_DoneFlag)
 {
    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_17, &hsadc0_dma_transferConfig_17) == kStatus_Success)
      {
         flag_start_adc[3]=1;
         for (i=0;i<128;i++)
          {
            temp_data_bus = temp_data_bus + ((data_hsadc0_buf5[i])>>3);
          }
         temp_data_bus=temp_data_bus/1219;
          Hi_DCDC.power_bus_volt=temp_data_bus;
       if (Hi_DCDC.start_flag==1) 
       {
         if ( Hi_DCDC.power_bus_volt>800)
         {
          if ( Hi_DCDC.power>4)
           {
              Hi_DCDC.power--;
              load_power( Hi_DCDC.power);
           }
         }
         else           
         {
           if ( Hi_DCDC.power<80)
           {
////             if (Hi_DCDC.start_flag==1)
////             {
               Hi_DCDC.power++;
               load_power( Hi_DCDC.power);
////             }             
           }
         }
       }
         
         
         flag_start_adc[4]=1;
          EDMA_ClearChannelStatusFlags(DMA0,17,kEDMA_DoneFlag);
      } 
    else 
      {
          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");          
      }
    EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_17);
 } 
}
void EDMA_Callback_hsadc0_18 (struct _edma_handle *handle, void *userData, bool transferDone, uint32_t tcds)
{
  if (EDMA_GetChannelStatusFlags(DMA0,18)&kEDMA_DoneFlag==kEDMA_DoneFlag)
 {
    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_18, &hsadc0_dma_transferConfig_18) == kStatus_Success)
      {
         flag_start_adc[5]=1;
          EDMA_ClearChannelStatusFlags(DMA0,18,kEDMA_DoneFlag);
      } 
    else 
      {
          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");          
      }
    EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_18);
 } 
}

void HSADC0_init (void)  
{
    hsadc_config_t hsadcConfigStruct;
    hsadc_converter_config_t hsadcConverterConfigStruct;
    hsadc_sample_config_t hsadcSampleConfigStruct;
    /* Initialize the HSADC common digital control.
    * "kHSADC_DualConverterWorkAsTriggeredSequential" is used in this case. A long conversion sequence which contails
    * at most 16 sample slots would be executed by both converter. The sequence would be controlled by converter A's
    * control logic such as start, stop, DMA enable, sync input etc.
    */
    HSADC_GetDefaultConfig(&hsadcConfigStruct);
    hsadcConfigStruct.dualConverterScanMode = kHSADC_DualConverterWorkAsLoopSequential;//kHSADC_DualConverterWorkAsTriggeredSequential;

    HSADC_Init(HSADC0, &hsadcConfigStruct);
    
    
    HSADC0->CTRL1 |=HSADC_CTRL1_HLMTIE(1);
  //  HSADC0->CTRL1 |=HSADC_CTRL1_EOSIEA(1);
   // NVIC_SetPriority (HSADC0_CCA_IRQn, 1);
    NVIC_SetPriority (HSADC_ERR_IRQn, 1);
    
    
    /* Configure each converter. */
    HSADC_GetDefaultConverterConfig(&hsadcConverterConfigStruct);
    hsadcConverterConfigStruct.clockDivisor = 63;
    HSADC_SetConverterConfig(HSADC0, kHSADC_ConverterA | kHSADC_ConverterB, &hsadcConverterConfigStruct);
    /* Enable the power for each converter. */
    HSADC_EnableConverterPower(HSADC0, kHSADC_ConverterA | kHSADC_ConverterB, true);
    while (
        (kHSADC_ConverterAPowerDownFlag | kHSADC_ConverterBPowerDownFlag) ==
        ((kHSADC_ConverterAPowerDownFlag | kHSADC_ConverterBPowerDownFlag) & HSADC_GetStatusFlags(HSADC0)))
    {
    }
    /* Make each converter exit stop mode. */
    HSADC_EnableConverter(HSADC0, kHSADC_ConverterA | kHSADC_ConverterB, true);


    /* Configure the samples. */
    HSADC_GetDefaultSampleConfig(&hsadcSampleConfigStruct);
    /* For converter A. */
    hsadcSampleConfigStruct.channelNumber = ADC0_CH0A ; // 10 Pin
    hsadcSampleConfigStruct.channel67MuxNumber = ADC0_CH0A_MUX_NUM ;
    hsadcSampleConfigStruct.enableDifferentialPair = ADC0_CH0A_ENABLE_DIFF ;
    HSADC_SetSampleConfig(HSADC0, 0U, &hsadcSampleConfigStruct);
    
////    hsadcSampleConfigStruct.channelNumber = ADC0_CH0B+8U ; //12 pin 
////    hsadcSampleConfigStruct.channel67MuxNumber = ADC0_CH0B_MUX_NUM ; 
////    hsadcSampleConfigStruct.enableDifferentialPair = ADC0_CH0B_ENABLE_DIFF ;
////    HSADC_SetSampleConfig(HSADC0, 8U, &hsadcSampleConfigStruct);
    
    hsadcSampleConfigStruct.channelNumber = ADC0_CH3A ; //19 pin 
    hsadcSampleConfigStruct.channel67MuxNumber = ADC0_CH3A_MUX_NUM ; 
    hsadcSampleConfigStruct.enableDifferentialPair = ADC0_CH3A_ENABLE_DIFF ;
    HSADC_SetSampleConfig(HSADC0, 1U, &hsadcSampleConfigStruct);
    
    
////    hsadcSampleConfigStruct.channelNumber = ADC0_CH0B+8U ; //12 pin 
////    hsadcSampleConfigStruct.channel67MuxNumber = ADC0_CH0B_MUX_NUM ; 
////    hsadcSampleConfigStruct.enableDifferentialPair = ADC0_CH0B_ENABLE_DIFF ;
////    HSADC_SetSampleConfig(HSADC0, 9U, &hsadcSampleConfigStruct);
    
    
   
////    hsadcSampleConfigStruct.channelNumber = ADC0_CH12A ; //28 Pin
////    hsadcSampleConfigStruct.channel67MuxNumber = ADC0_CH12A_MUX_NUM ;
////    hsadcSampleConfigStruct.enableDifferentialPair = ADC0_CH12A_ENABLE_DIFF ;
////    HSADC_SetSampleConfig(HSADC0,1U, &hsadcSampleConfigStruct);
////    
    
    /* For converter B.
     * In HSADC_SetSampleConfig(), the channel number 0~7 represents input 0~7 of converter A and channel number 8~15
     * represents input 0~7 of converter B.
     */
    
////    hsadcSampleConfigStruct.channelNumber = (ADC0_CH8B +8U ); //70 pin
////    hsadcSampleConfigStruct.channel67MuxNumber = ADC0_CH8B_MUX_NUM ;
////    hsadcSampleConfigStruct.enableDifferentialPair = ADC0_CH8B_ENABLE_DIFF ;
////    HSADC_SetSampleConfig(HSADC0, 5U, &hsadcSampleConfigStruct);
////   
////    hsadcSampleConfigStruct.channelNumber = (ADC0_CH11B+8u); // 4 Pin
////    hsadcSampleConfigStruct.channel67MuxNumber = ADC0_CH11B_MUX_NUM ;
////    hsadcSampleConfigStruct.enableDifferentialPair = ADC0_CH11B_ENABLE_DIFF ;
////    HSADC_SetSampleConfig(HSADC0,2U, &hsadcSampleConfigStruct);    
    
    /* Enable the sample slot.
     * The conversion sequence for converter A includes sample 0 and sample 1, while the sequence for converter B
     * includes sample 2 and 3. Sample slot 0~15 can reference to any input 0~7 of converter A and converter B in
     * sequential mode.
     */
    HSADC0_sampleMask =  
                  HSADC_SAMPLE_MASK(0U)   
                | HSADC_SAMPLE_MASK(1U)  
////                | HSADC_SAMPLE_MASK(8U)              
////                | HSADC_SAMPLE_MASK(9U) 
//////                | HSADC_SAMPLE_MASK(4U)
//////                | HSADC_SAMPLE_MASK(5U)  
               ; 
 
    HSADC_EnableSample(HSADC0, HSADC0_sampleMask, true);
    HSADC_EnableSample(HSADC0, (uint16_t)(~HSADC0_sampleMask), false); /* Disable other sample slots. */

//////    /* Calibrate the converter after power up period. */
//////    HSADC_DoAutoCalibration(HSADC0, (kHSADC_ConverterA | kHSADC_ConverterB),
//////                            (kHSADC_CalibrationModeSingleEnded | kHSADC_CalibrationModeDifferential));
//////    /* Wait the calibration process complete. Only converter A's End of Scan flag will be set after calibration
//////    process in sequential mode. */
//////    while ((kHSADC_ConverterAEndOfScanFlag | kHSADC_ConverterAEndOfCalibrationFlag |
//////            kHSADC_ConverterBEndOfCalibrationFlag) !=
//////           ((kHSADC_ConverterAEndOfScanFlag | kHSADC_ConverterAEndOfCalibrationFlag |
//////             kHSADC_ConverterBEndOfCalibrationFlag) &
//////            HSADC_GetStatusFlags(HSADC0)))
//////    {
//////    }
//////    HSADC_ClearStatusFlags(HSADC0, kHSADC_ConverterAEndOfScanFlag | kHSADC_ConverterAEndOfCalibrationFlag |
//////                                                    kHSADC_ConverterBEndOfCalibrationFlag);
 //  HSADC_Init(HSADC0, &hsadcConfigStruct);
    
    
   //  NVIC_EnableIRQ(HSADC0_CCA_IRQn);
     NVIC_EnableIRQ(HSADC_ERR_IRQn);
////     
////     HSADC0->HILIM[0] =HSADC_HILIM_HLMT(0x7FF8);
////     HSADC0->HILIM[1] =HSADC_HILIM_HLMT(0x7FF8);
////     HSADC0->HILIM[8] =HSADC_HILIM_HLMT(0x7FF8);
////     HSADC0->HILIM[9] =HSADC_HILIM_HLMT(0x7FF8);
     HSADC_EnableConverterDMA(HSADC0,kHSADC_ConverterA|kHSADC_ConverterB,true);
          
  
     
     
    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, 4, kDmaRequestMux0HSADC0A);
    DMAMUX_EnableChannel(DMAMUX0, 4);
     
    EDMA_GetDefaultConfig(&hsadc0_dma_Config_4);
    EDMA_Init(DMA0, &hsadc0_dma_Config_4);
    EDMA_CreateHandle(&g_EDMA_Handle_hsadc0_4, DMA0, 4);
    EDMA_SetCallback(&g_EDMA_Handle_hsadc0_4, EDMA_Callback_hsadc0_4, NULL);
    EDMA_PrepareTransfer(&hsadc0_dma_transferConfig_4, (void*)&HSADC0->RSLT[0], (uint32_t)2, data_hsadc0_buf1,(uint32_t)2,
    2, 20, kEDMA_PeripheralToMemory); 
    
    NVIC_SetPriority (DMA4_DMA20_IRQn, 3);
    NVIC_EnableIRQ(DMA4_DMA20_IRQn);
    
    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_4, &hsadc0_dma_transferConfig_4) == kStatus_Success) {
        //  PRINTF("\r\nEDMA Submit Transfer: Success\r\n");
     } else {
          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");
     }

     EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_4);
    
    
////    
   // DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, 5, kDmaRequestMux0HSADC0A);
    DMAMUX_EnableChannel(DMAMUX0, 5);
    
    EDMA_GetDefaultConfig(&hsadc0_dma_Config_5);
    EDMA_Init(DMA0, &hsadc0_dma_Config_5);
    EDMA_CreateHandle(&g_EDMA_Handle_hsadc0_5, DMA0, 5);
    EDMA_SetCallback(&g_EDMA_Handle_hsadc0_5, EDMA_Callback_hsadc0_5, NULL);
    EDMA_PrepareTransfer(&hsadc0_dma_transferConfig_5,(void*)&HSADC0->RSLT[1], (uint32_t)2,data_hsadc0_buf2,(uint32_t)2,
    2, 20, kEDMA_PeripheralToMemory); 
    NVIC_SetPriority (DMA5_DMA21_IRQn, 3);
    NVIC_EnableIRQ(DMA5_DMA21_IRQn);
    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_5, &hsadc0_dma_transferConfig_5) == kStatus_Success) {
         // PRINTF("\r\nEDMA Submit Transfer: Success\r\n");
     } else {
          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");
     }

     EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_5);


   
//////////////////////     
//////////////////////      DMAMUX_Init(DMAMUX0);
//////////////////////    DMAMUX_SetSource(DMAMUX0, 6, kDmaRequestMux0HSADC0A);
//////////////////////    DMAMUX_EnableChannel(DMAMUX0, 6);
//////////////////////    
//////////////////////    EDMA_GetDefaultConfig(&hsadc0_dma_Config_6);
//////////////////////    EDMA_Init(DMA0, &hsadc0_dma_Config_6);
//////////////////////    EDMA_CreateHandle(&g_EDMA_Handle_hsadc0_6, DMA0, 6);
//////////////////////    EDMA_SetCallback(&g_EDMA_Handle_hsadc0_6, EDMA_Callback_hsadc0_6, NULL);
//////////////////////    EDMA_PrepareTransfer(&hsadc0_dma_transferConfig_6,(void*)&HSADC0->RSLT[8] , (uint32_t)2,data_hsadc0_buf3,(uint32_t)2,
//////////////////////    2, 80, kEDMA_PeripheralToMemory); 
//////////////////////    NVIC_SetPriority (DMA6_DMA22_IRQn, 3);
//////////////////////    NVIC_EnableIRQ(DMA6_DMA22_IRQn);
//////////////////////    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_6, &hsadc0_dma_transferConfig_6) == kStatus_Success) {
//////////////////////        //  PRINTF("\r\nEDMA Submit Transfer: Success\r\n");
//////////////////////     } else {
//////////////////////          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");
//////////////////////     }
//////////////////////
//////////////////////     EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_6);
//////////////////////
//////////////////////
//////////////////////   
//////////////////////     
//////////////////////    DMAMUX_Init(DMAMUX0);
//////////////////////    DMAMUX_SetSource(DMAMUX0, 7, kDmaRequestMux0HSADC0A);
//////////////////////    DMAMUX_EnableChannel(DMAMUX0, 7);
//////////////////////    
//////////////////////    EDMA_GetDefaultConfig(&hsadc0_dma_Config_7);
//////////////////////    EDMA_Init(DMA0, &hsadc0_dma_Config_7);
//////////////////////    EDMA_CreateHandle(&g_EDMA_Handle_hsadc0_7, DMA0, 7);
//////////////////////    EDMA_SetCallback(&g_EDMA_Handle_hsadc0_7, EDMA_Callback_hsadc0_7, NULL);
//////////////////////    EDMA_PrepareTransfer(&hsadc0_dma_transferConfig_7,(void*)&HSADC0->RSLT[9], (uint32_t)2,data_hsadc0_buf7,(uint32_t)2,
//////////////////////    2, 80, kEDMA_PeripheralToMemory); 
//////////////////////    NVIC_SetPriority (DMA7_DMA23_IRQn, 3);
//////////////////////    NVIC_EnableIRQ(DMA7_DMA23_IRQn);
//////////////////////    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_7, &hsadc0_dma_transferConfig_7) == kStatus_Success) {
//////////////////////       //   PRINTF("\r\nEDMA Submit Transfer: Success\r\n");
//////////////////////     } else {
//////////////////////          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");
//////////////////////     }
//////////////////////
//////////////////////    EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_7);
////////////////////// 

     
     
   


        
   
           
}



void HSADC1_CCA_IRQHandler(void)
{
       
   /*   if (ct_hsadc1<32)
        {
           
          cur_adc_data[5][ct_hsadc1]=(HSADC_GetSampleResultValue(HSADC1, 1U)>>3);
          cur_adc_data[2][ct_hsadc1]=(HSADC_GetSampleResultValue(HSADC1, 2U)>>3);           
          cur_adc_data[1][ct_hsadc1]=(HSADC_GetSampleResultValue(HSADC1, 3U)>>3);
          ct_hsadc1++;
        }
      else
        {
          ct_hsadc1=0;
        }
   */
    
        HSADC_ClearStatusFlags(HSADC1, kHSADC_ConverterAEndOfScanFlag);
   //     HSADC_DoSoftwareTriggerConverter(HSADC1, kHSADC_ConverterA|kHSADC_ConverterB);

}

void HSADC_ERR_IRQHandler (void)
{
  
if (((HSADC0->STAT)&HSADC_STAT_HLMTI_MASK)>0)
 {
  if (((HSADC0->HILIMSTAT)&(1<<0))>0)
    {
    
      ct_shortcur[3]++;
      flag_err[3]=3;
    }
   if (((HSADC0->HILIMSTAT)&(1<<2))>0)
    {
      
      ct_shortcur[4]++;
      flag_err[4]=3;
    }
   if (((HSADC0->HILIMSTAT)&(1<<1))>0)
    {
    
      ct_shortcur[0]++;
      flag_err[0]=3;
    }
  HSADC_ClearStatusFlags(HSADC0, kHSADC_HighLimitFlag);
 }
else
 {
   if (((HSADC1->HILIMSTAT)&(1<<3))>0)
    {
    
      ct_shortcur[1]++;
      flag_err[1]=3;
    }
   if (((HSADC1->HILIMSTAT)&(1<<2))>0)
    {
     
      ct_shortcur[2]++;
      flag_err[2]=3;
    }
   if (((HSADC1->HILIMSTAT)&(1<<1))>0)
    {
     
      ct_shortcur[5]++;
      flag_err[5]=3;
    }
  HSADC_ClearStatusFlags(HSADC1, kHSADC_HighLimitFlag);
 }  
}


void HSADC1_init (void)
  
{
    hsadc_config_t hsadcConfigStruct;
    hsadc_converter_config_t hsadcConverterConfigStruct;
    hsadc_sample_config_t hsadcSampleConfigStruct;
    /* Initialize the HSADC common digital control.
    * "kHSADC_DualConverterWorkAsTriggeredSequential" is used in this case. A long conversion sequence which contails
    * at most 16 sample slots would be executed by both converter. The sequence would be controlled by converter A's
    * control logic such as start, stop, DMA enable, sync input etc.
    */
    HSADC_GetDefaultConfig(&hsadcConfigStruct);
    hsadcConfigStruct.dualConverterScanMode = kHSADC_DualConverterWorkAsTriggeredParallel;
 
    HSADC_Init(HSADC1, &hsadcConfigStruct);
  
    HSADC1->CTRL1 |=HSADC_CTRL1_HLMTIE(1);
  //  HSADC1->CTRL1 |=HSADC_CTRL1_EOSIEA(1);
 //   NVIC_SetPriority (HSADC1_CCA_IRQn, 1);
    NVIC_SetPriority (HSADC_ERR_IRQn, 5);
   
     
    /* Configure each converter. */
    HSADC_GetDefaultConverterConfig(&hsadcConverterConfigStruct);
    hsadcConverterConfigStruct.clockDivisor = 63;
    HSADC_SetConverterConfig(HSADC1, kHSADC_ConverterA | kHSADC_ConverterB, &hsadcConverterConfigStruct);
    /* Enable the power for each converter. */
    HSADC_EnableConverterPower(HSADC1, kHSADC_ConverterA | kHSADC_ConverterB, true);
    while (
        (kHSADC_ConverterAPowerDownFlag | kHSADC_ConverterBPowerDownFlag) ==
        ((kHSADC_ConverterAPowerDownFlag | kHSADC_ConverterBPowerDownFlag) & HSADC_GetStatusFlags(HSADC1)))
    {
    }
    /* Make each converter exit stop mode. */
    HSADC_EnableConverter(HSADC1, kHSADC_ConverterA | kHSADC_ConverterB, true);


    /* Configure the samples. */
    HSADC_GetDefaultSampleConfig(&hsadcSampleConfigStruct);
    /* For converter A. */
    hsadcSampleConfigStruct.channelNumber = ADC1_CH3A ;
    hsadcSampleConfigStruct.channel67MuxNumber = ADC1_CH3A_MUX_NUM ;
    hsadcSampleConfigStruct.enableDifferentialPair = ADC1_CH3A_ENABLE_DIFF ;
    HSADC_SetSampleConfig(HSADC1,0U, &hsadcSampleConfigStruct);
    
  
    
    /* For converter B.
     * In HSADC_SetSampleConfig(), the channel number 0~7 represents input 0~7 of converter A and channel number 8~15
     * represents input 0~7 of converter B.
     */
    
    hsadcSampleConfigStruct.channelNumber = ADC1_CH2B+8u ;
    hsadcSampleConfigStruct.channel67MuxNumber = ADC1_CH2B_MUX_NUM ;
    hsadcSampleConfigStruct.enableDifferentialPair = ADC1_CH2B_ENABLE_DIFF ;
    HSADC_SetSampleConfig(HSADC1,8U, &hsadcSampleConfigStruct);
    
//////    hsadcSampleConfigStruct.channelNumber = ADC1_CH2B+8u ;
//////    hsadcSampleConfigStruct.channel67MuxNumber = ADC1_CH2B_MUX_NUM ;
//////    hsadcSampleConfigStruct.enableDifferentialPair = ADC1_CH2B_ENABLE_DIFF ;
//////    HSADC_SetSampleConfig(HSADC1,2U, &hsadcSampleConfigStruct);
//////    
//////    hsadcSampleConfigStruct.channelNumber = ADC1_CH3B+8u ;
//////    hsadcSampleConfigStruct.channel67MuxNumber = ADC1_CH3B_MUX_NUM ;
//////    hsadcSampleConfigStruct.enableDifferentialPair = ADC1_CH3B_ENABLE_DIFF ;
//////    HSADC_SetSampleConfig(HSADC1,3U, &hsadcSampleConfigStruct);
//////    

    
    
    /* Enable the sample slot.
     * The conversion sequence for converter A includes sample 0 and sample 1, while the sequence for converter B
     * includes sample 2 and 3. Sample slot 0~15 can reference to any input 0~7 of converter A and converter B in
     * sequential mode.
     */
    HSADC1_sampleMask =  
                  HSADC_SAMPLE_MASK(0U)   
                | HSADC_SAMPLE_MASK(8U)  
////                | HSADC_SAMPLE_MASK(2U)              
////                | HSADC_SAMPLE_MASK(3U)                
               ; 
 
    HSADC_EnableSample(HSADC1, HSADC1_sampleMask, true);
    HSADC_EnableSample(HSADC1, (uint16_t)(~HSADC1_sampleMask), false); /* Disable other sample slots. */

    
    
//////    
    /* Calibrate the converter after power up period. */
    HSADC_DoAutoCalibration(HSADC1, (kHSADC_ConverterA | kHSADC_ConverterB),
                            (kHSADC_CalibrationModeSingleEnded|kHSADC_CalibrationModeDifferential ));
    /* Wait the calibration process complete. Only converter A's End of Scan flag will be set after calibration
    process in sequential mode. */
    while ((kHSADC_ConverterAEndOfScanFlag | kHSADC_ConverterAEndOfCalibrationFlag |
            kHSADC_ConverterBEndOfCalibrationFlag) !=
           ((kHSADC_ConverterAEndOfScanFlag | kHSADC_ConverterAEndOfCalibrationFlag |
             kHSADC_ConverterBEndOfCalibrationFlag) &
            HSADC_GetStatusFlags(HSADC1)))
    {
    }
    HSADC_ClearStatusFlags(HSADC1, kHSADC_ConverterAEndOfScanFlag | kHSADC_ConverterAEndOfCalibrationFlag |
                                                    kHSADC_ConverterBEndOfCalibrationFlag);
    
    hsadcConfigStruct.dualConverterScanMode = kHSADC_DualConverterWorkAsLoopParallel;//kHSADC_DualConverterWorkAsLoopSequential;// kHSADC_DualConverterWorkAsTriggeredSequential;
 
    HSADC_Init(HSADC1, &hsadcConfigStruct);
    
    
   //  NVIC_EnableIRQ(HSADC1_CCA_IRQn);
     NVIC_EnableIRQ(HSADC_ERR_IRQn);
     
     HSADC1->HILIM[1] =HSADC_HILIM_HLMT(0x7FF8);
     HSADC1->HILIM[2] =HSADC_HILIM_HLMT(0x7FF8);
////     HSADC1->HILIM[3] =HSADC_HILIM_HLMT(0x800);
     
     
    HSADC_EnableConverterDMA(HSADC1,kHSADC_ConverterA|kHSADC_ConverterB,true);
    
   
  //  DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0,16 , kDmaRequestMux0Group1HSADC1A);
    DMAMUX_EnableChannel(DMAMUX0, 16);
    
    EDMA_GetDefaultConfig(&hsadc0_dma_Config_16);
    EDMA_Init(DMA0, &hsadc0_dma_Config_16);
    EDMA_CreateHandle(&g_EDMA_Handle_hsadc0_16, DMA0, 16);
    EDMA_SetCallback(&g_EDMA_Handle_hsadc0_16, EDMA_Callback_hsadc0_16, NULL);
    EDMA_PrepareTransfer(&hsadc0_dma_transferConfig_16,(void*)&HSADC1->RSLT[0], (uint32_t)2,(void*)data_hsadc0_buf4,(uint32_t)2,
    2, 256, kEDMA_PeripheralToMemory); 
    
    
    
    NVIC_SetPriority (DMA0_DMA16_IRQn, 3);
    NVIC_EnableIRQ(DMA0_DMA16_IRQn);
    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_16, &hsadc0_dma_transferConfig_16) == kStatus_Success) {
        //  PRINTF("\r\nEDMA Submit Transfer: Success\r\n");
     } else {
          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");
     }

     EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_16);    
    
   
   
    
 //   DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, 17, kDmaRequestMux0Group1HSADC1A);
    DMAMUX_EnableChannel(DMAMUX0, 17);
    
    EDMA_GetDefaultConfig(&hsadc0_dma_Config_17);
    EDMA_Init(DMA0, &hsadc0_dma_Config_17);
    EDMA_CreateHandle(&g_EDMA_Handle_hsadc0_17, DMA0, 17);
    EDMA_SetCallback(&g_EDMA_Handle_hsadc0_17, EDMA_Callback_hsadc0_17, NULL);
    EDMA_PrepareTransfer(&hsadc0_dma_transferConfig_17,(void*)&HSADC1->RSLT[8], (uint32_t)2,(void*)data_hsadc0_buf5,(uint32_t)2,
    2, 80, kEDMA_PeripheralToMemory); 
    NVIC_SetPriority (DMA1_DMA17_IRQn, 3);
    NVIC_EnableIRQ(DMA1_DMA17_IRQn);
    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_17, &hsadc0_dma_transferConfig_17) == kStatus_Success) {
        //  PRINTF("\r\nEDMA Submit Transfer: Success\r\n");
     } else {
          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");
     }

     EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_17);
    
    
    
  
     
    
//////////    DMAMUX_Init(DMAMUX0);
//////////    DMAMUX_SetSource(DMAMUX0, 18, kDmaRequestMux0Group1HSADC1A);
//////////    DMAMUX_EnableChannel(DMAMUX0,18);
//////////    
//////////    EDMA_GetDefaultConfig(&hsadc0_dma_Config_18);
//////////    EDMA_Init(DMA0, &hsadc0_dma_Config_18);
//////////    EDMA_CreateHandle(&g_EDMA_Handle_hsadc0_18, DMA0, 18);
//////////    EDMA_SetCallback(&g_EDMA_Handle_hsadc0_18, EDMA_Callback_hsadc0_18, NULL);
//////////    EDMA_PrepareTransfer(&hsadc0_dma_transferConfig_18,(void*)&HSADC1->RSLT[3], (uint32_t)2,(void*)data_hsadc0_buf6,(uint32_t)2,
//////////    2, 512, kEDMA_PeripheralToMemory); 
//////////    NVIC_SetPriority (DMA2_DMA18_IRQn, 3);
//////////    NVIC_EnableIRQ(DMA2_DMA18_IRQn);
//////////    if (EDMA_SubmitTransfer(&g_EDMA_Handle_hsadc0_18, &hsadc0_dma_transferConfig_18) == kStatus_Success) {
//////////        //  PRINTF("\r\nEDMA Submit Transfer: Success\r\n");
//////////     } else {
//////////          PRINTF("\r\nEDMA Submit Transfer: Failed\r\n");
//////////     }

  //   EDMA_StartTransfer(&g_EDMA_Handle_hsadc0_18);
    
    

    
    
    
 //HSADC_DoSoftwareTriggerConverter(HSADC1, kHSADC_ConverterA|kHSADC_ConverterB);
     
      
} 
void start_hsadc (void)
{
//HSADC_DoSoftwareTriggerConverter(HSADC1, kHSADC_ConverterA|kHSADC_ConverterB);
     
HSADC_DoSoftwareTriggerConverter(HSADC0, kHSADC_ConverterA|kHSADC_ConverterB);
 
}
////void start_HSADC1(void )
////  {
////   //  HSADC_DoSoftwareTriggerConverter(HSADC1, kHSADC_ConverterA|kHSADC_ConverterB);
////  }





  
/*******************************************************************************
 * Code
 ******************************************************************************/
void spi_delay_slot (unsigned char flag)
{
 if (flag ==0)
 {   
    /* PORTE16 (pin 18) is configured as GPIO */
    PORT_SetPinMux(PORTE, 16U, kPORT_MuxAsGpio); 
    GPIO_PinInit (GPIOE,PIN16_IDX,&config_out);
    GPIO_SetPinsOutput(GPIOE,1<<PIN16_IDX);
 }
 else
 {
    
    GPIO_ClearPinsOutput(GPIOE,1<<PIN16_IDX);
    GPIO_PinInit (GPIOE,PIN16_IDX,&config_in);
    /* PORTE16 (pin 18) is configured as SPI0_PCS0 */
    PORT_SetPinMux(PORTE, 16U, kPORT_MuxAlt2); 
 }

 }


void DSPI0_MasterUserCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        //PRINTF("This is DSPI master edma transfer completed callback. \r\n\r\n");
      isTransferCompleted0 = true;
    }
     
}

void SPI0_init (void)
{


  
    /* DMA Mux setting and EDMA init */
    uint32_t masterRxChannel, masterIntermediaryChannel, masterTxChannel;
 /////   uint32_t slaveRxChannel, slaveTxChannel;
    edma_config_t userConfig;

    masterRxChannel = 6U;
    masterIntermediaryChannel = 7U;
    masterTxChannel = 8U;

  

    /* DMA MUX init */
    DMAMUX_Init(SPI0_DSPI_MASTER_DMA_MUX_BASEADDR);

#if (defined SPI0_DSPI_MASTER_DMA_MUX_BASE) && (defined SPI0_DSPI_SLAVE_DMA_MUX_BASE) && \
    (SPI0_DSPI_MASTER_DMA_MUX_BASE != SPI0_DSPI_SLAVE_DMA_MUX_BASE)
    DMAMUX_Init(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR);
#endif

    DMAMUX_SetSource(SPI0_DSPI_MASTER_DMA_MUX_BASEADDR, masterRxChannel, SPI0_DSPI_MASTER_DMA_RX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(SPI0_DSPI_MASTER_DMA_MUX_BASEADDR, masterRxChannel);

#if (defined SPI0_DSPI_MASTER_DMA_TX_REQUEST_SOURCE)
    DMAMUX_SetSource(SPI0_DSPI_MASTER_DMA_MUX_BASEADDR, masterTxChannel, SPI0_DSPI_MASTER_DMA_TX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(SPI0_DSPI_MASTER_DMA_MUX_BASEADDR, masterTxChannel);
#endif

  
#if (defined EXAMPLE_DSPI_SLAVE_DMA_TX_REQUEST_SOURCE)
    DMAMUX_SetSource(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR, slaveTxChannel, EXAMPLE_DSPI_SLAVE_DMA_TX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR, slaveTxChannel);
#endif

    /* EDMA init*/
    /*
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableContinuousLinkMode = false;
     * userConfig.enableDebugMode = false;
     */
   EDMA_GetDefaultConfig(&userConfig);

    EDMA_Init(SPI0_DSPI_MASTER_DMA_BASEADDR, &userConfig);

#if (defined EXAMPLE_DSPI_SLAVE_DMA_BASE) && (defined EXAMPLE_DSPI_MASTER_DMA_BASE) && \
    (EXAMPLE_DSPI_SLAVE_DMA_BASE != EXAMPLE_DSPI_MASTER_DMA_BASE)
    EDMA_Init(EXAMPLE_DSPI_SLAVE_DMA_BASEADDR, &userConfig);
#endif

    /*DSPI init*/

   //// uint32_t errorCount;
   //// uint32_t i;

  ////  dspi_slave_config_t slaveConfig;
    dspi_transfer_t masterXfer;
    dspi_transfer_t slaveXfer;

    /*Master config*/
    masterConfig.whichCtar = kDSPI_Ctar0;
    masterConfig.ctarConfig.baudRate = SPI0_BAUDRATE;
    masterConfig.ctarConfig.bitsPerFrame = 8U;
    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
    masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
    masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / SPI0_BAUDRATE;
    masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / SPI0_BAUDRATE;
    masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / SPI0_BAUDRATE;



  

    isTransferCompleted0 = false;

        /* Set up dspi master */
    memset(&(dspiEdmaMasterRxRegToRxDataHandle), 0, sizeof(dspiEdmaMasterRxRegToRxDataHandle));
    memset(&(dspiEdmaMasterTxDataToIntermediaryHandle), 0, sizeof(dspiEdmaMasterTxDataToIntermediaryHandle));
    memset(&(dspiEdmaMasterIntermediaryToTxRegHandle), 0, sizeof(dspiEdmaMasterIntermediaryToTxRegHandle));

    EDMA_CreateHandle(&(dspiEdmaMasterRxRegToRxDataHandle), SPI0_DSPI_MASTER_DMA_BASEADDR, masterRxChannel);
    EDMA_CreateHandle(&(dspiEdmaMasterTxDataToIntermediaryHandle), SPI0_DSPI_MASTER_DMA_BASEADDR,masterIntermediaryChannel);
    EDMA_CreateHandle(&(dspiEdmaMasterIntermediaryToTxRegHandle), SPI0_DSPI_MASTER_DMA_BASEADDR, masterTxChannel);

    DSPI_MasterTransferCreateHandleEDMA(SPI0_DSPI_MASTER_BASEADDR, &g_dspi_edma_m_handle, DSPI0_MasterUserCallback,
                                        NULL, &dspiEdmaMasterRxRegToRxDataHandle,
                                        &dspiEdmaMasterTxDataToIntermediaryHandle,
                                        &dspiEdmaMasterIntermediaryToTxRegHandle);

    
}

void SPI0_TX (uint8_t *SPI_masterTxData,uint8_t *SPI_masterRxData,uint32_t data_size,uint8_t psc)
{
  
    dspi_transfer_t masterXfer;      
//    masterXfer.txData = SPI_masterTxData;
//    masterXfer.rxData = SPI_masterRxData;
//    masterXfer.dataSize = data_size;
//    masterXfer.configFlags = kDSPI_MasterCtar0 | PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;  
//    
//    DSPI_MasterTransferBlocking(SPI0, &masterXfer);
    
    
   
    /* Start master transfer */
    masterXfer.txData = SPI_masterTxData;
    masterXfer.rxData = SPI_masterRxData;
    masterXfer.dataSize = data_size;
////    if (psc==0)
////    {
////       /* PORTE16 (pin 18) is configured as SPI0_PCS0 */
////      PORT_SetPinMux(PORTE, 16U, kPORT_MuxAlt2);
////      /* PORTC3 (pin 106) is configured as SPI0_PCS1 */
////      PORT_SetPinMux(PORTC, 3U, kPORT_MuxAsGpio);
      masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
      masterConfig.whichPcs = kDSPI_Pcs0;//SPI0_DSPI_MASTER_PCS_FOR_INIT;
      masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

      masterConfig.enableContinuousSCK = false;
      masterConfig.enableRxFifoOverWrite = false;
      masterConfig.enableModifiedTimingFormat = false;
      masterConfig.samplePoint = kDSPI_SckToSin0Clock;

      srcClock_Hz = CLOCK_GetFreq(DSPI_MASTER_CLK_SRC);
      DSPI_MasterInit(SPI0_DSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);
////    }
////    else
////    {
////      /* PORTC3 (pin 106) is configured as SPI0_PCS1 */
////      PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt2);
////       /* PORTE16 (pin 18) is configured as SPI0_PCS0 */
////      PORT_SetPinMux(PORTE, 16U, kPORT_MuxAsGpio);
////      masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;
////      masterConfig.whichPcs = kDSPI_Pcs1;//SPI0_DSPI_MASTER_PCS_FOR_INIT;
////      masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;
////
////      masterConfig.enableContinuousSCK = false;
////      masterConfig.enableRxFifoOverWrite = false;
////      masterConfig.enableModifiedTimingFormat = false;
////      masterConfig.samplePoint = kDSPI_SckToSin0Clock;
////
////      srcClock_Hz = CLOCK_GetFreq(DSPI_MASTER_CLK_SRC);
////      DSPI_MasterInit(SPI0_DSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);
////      masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;
////    }

    if (kStatus_Success != DSPI_MasterTransferEDMA(SPI0_DSPI_MASTER_BASEADDR, &g_dspi_edma_m_handle, &masterXfer))
    {
        PRINTF("There is error when start DSPI_MasterTransferEDMA \r\n ");
    }


}


void DSPI1_MasterUserCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        //PRINTF("This is DSPI master edma transfer completed callback. \r\n\r\n");
      isTransferCompleted1 = true;
    }
     
}

void SPI1_init (void)
{


  
    /* DMA Mux setting and EDMA init */
    uint32_t masterRxChannel, masterIntermediaryChannel, masterTxChannel;
 /////   uint32_t slaveRxChannel, slaveTxChannel;
    edma_config_t userConfig;

    masterRxChannel = 25;
    masterIntermediaryChannel = 26;
    masterTxChannel = 27;

  

    /* DMA MUX init */
    DMAMUX_Init(SPI1_DSPI_MASTER_DMA_MUX_BASEADDR);

#if (defined SPI1_DSPI_MASTER_DMA_MUX_BASE) && (defined SPI1_DSPI_SLAVE_DMA_MUX_BASE) && \
    (SPI1_DSPI_MASTER_DMA_MUX_BASE != SPI1_DSPI_SLAVE_DMA_MUX_BASE)
    DMAMUX_Init(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR);
#endif

    DMAMUX_SetSource(SPI1_DSPI_MASTER_DMA_MUX_BASEADDR, masterRxChannel, SPI1_DSPI_MASTER_DMA_RX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(SPI1_DSPI_MASTER_DMA_MUX_BASEADDR, masterRxChannel);

#if (defined SPI1_DSPI_MASTER_DMA_TX_REQUEST_SOURCE)
    DMAMUX_SetSource(SPI1_DSPI_MASTER_DMA_MUX_BASEADDR, masterTxChannel, SPI1_DSPI_MASTER_DMA_TX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(SPI1_DSPI_MASTER_DMA_MUX_BASEADDR, masterTxChannel);
#endif

  
#if (defined EXAMPLE_DSPI_SLAVE_DMA_TX_REQUEST_SOURCE)
    DMAMUX_SetSource(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR, slaveTxChannel, EXAMPLE_DSPI_SLAVE_DMA_TX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DSPI_SLAVE_DMA_MUX_BASEADDR, slaveTxChannel);
#endif

    /* EDMA init*/
    /*
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableContinuousLinkMode = false;
     * userConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&userConfig);

    EDMA_Init(SPI1_DSPI_MASTER_DMA_BASEADDR, &userConfig);

#if (defined EXAMPLE_DSPI_SLAVE_DMA_BASE) && (defined EXAMPLE_DSPI_MASTER_DMA_BASE) && \
    (EXAMPLE_DSPI_SLAVE_DMA_BASE != EXAMPLE_DSPI_MASTER_DMA_BASE)
    EDMA_Init(EXAMPLE_DSPI_SLAVE_DMA_BASEADDR, &userConfig);
#endif

    /*DSPI1 init*/

   //// uint32_t errorCount;
   //// uint32_t i;

  ////  dspi_slave_config_t slaveConfig;
  //  dspi_transfer_t masterXfer;
    dspi_transfer_t slaveXfer;

    /*Master config*/
    masterConfig1.whichCtar = kDSPI_Ctar0;
    masterConfig1.ctarConfig.baudRate = SPI1_BAUDRATE;
    masterConfig1.ctarConfig.bitsPerFrame = 8U;
    masterConfig1.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
    masterConfig1.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    masterConfig1.ctarConfig.direction = kDSPI_MsbFirst;
    masterConfig1.ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / SPI1_BAUDRATE;
    masterConfig1.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / SPI1_BAUDRATE;
    masterConfig1.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / SPI1_BAUDRATE;
  

  

    isTransferCompleted1 = false;


    /* Set up dspi master */
    memset(&(dspi1EdmaMasterRxRegToRxDataHandle), 0, sizeof(dspi1EdmaMasterRxRegToRxDataHandle));
    memset(&(dspi1EdmaMasterTxDataToIntermediaryHandle), 0, sizeof(dspi1EdmaMasterTxDataToIntermediaryHandle));
    memset(&(dspi1EdmaMasterIntermediaryToTxRegHandle), 0, sizeof(dspi1EdmaMasterIntermediaryToTxRegHandle));

    EDMA_CreateHandle(&(dspi1EdmaMasterRxRegToRxDataHandle), SPI1_DSPI_MASTER_DMA_BASEADDR, masterRxChannel);
    EDMA_CreateHandle(&(dspi1EdmaMasterTxDataToIntermediaryHandle), SPI1_DSPI_MASTER_DMA_BASEADDR,
                      masterIntermediaryChannel);
    EDMA_CreateHandle(&(dspi1EdmaMasterIntermediaryToTxRegHandle), SPI1_DSPI_MASTER_DMA_BASEADDR, masterTxChannel);

    DSPI_MasterTransferCreateHandleEDMA(SPI1_DSPI_MASTER_BASEADDR, &g_dspi1_edma_m_handle, DSPI1_MasterUserCallback,
                                        NULL, &dspi1EdmaMasterRxRegToRxDataHandle,
                                        &dspi1EdmaMasterTxDataToIntermediaryHandle,
                                        &dspi1EdmaMasterIntermediaryToTxRegHandle);

     
    
}

void SPI1_TX (uint8_t *SPI_masterTxData,uint8_t *SPI_masterRxData,uint32_t data_size,uint8_t psc)
{
  
    dspi_transfer_t masterXfer;      
//    masterXfer.txData = SPI_masterTxData;
//    masterXfer.rxData = SPI_masterRxData;
//    masterXfer.dataSize = data_size;
//    masterXfer.configFlags = kDSPI_MasterCtar0 | PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;  
//    
//    DSPI_MasterTransferBlocking(SPI1, &masterXfer);
    
    /*Master config*/
    masterConfig2.whichCtar = kDSPI_Ctar0;
    masterConfig2.ctarConfig.baudRate = SPI1_BAUDRATE_LOW;
    masterConfig2.ctarConfig.bitsPerFrame = 8U;
    masterConfig2.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
    masterConfig2.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    masterConfig2.ctarConfig.direction = kDSPI_MsbFirst;
    masterConfig2.ctarConfig.pcsToSckDelayInNanoSec = 200;// 1000000000U / SPI1_BAUDRATE_LOW;
    masterConfig2.ctarConfig.lastSckToPcsDelayInNanoSec = 200;//1000000000U / SPI1_BAUDRATE_LOW;
    masterConfig2.ctarConfig.betweenTransferDelayInNanoSec = 0;//1000000000U / SPI1_BAUDRATE_LOW;
   
    /* Start master transfer */
    masterXfer.txData = SPI_masterTxData;
    masterXfer.rxData = SPI_masterRxData;
    masterXfer.dataSize = data_size;
//////    if (psc==0)
//////    {
//////       /* PORTE16 (pin 18) is configured as SPI1_PCS0 */
//////      PORT_SetPinMux(PORTE, 16U, kPORT_MuxAlt2);
//////      /* PORTC3 (pin 106) is configured as SPI1_PCS1 */
//////      PORT_SetPinMux(PORTC, 3U, kPORT_MuxAsGpio);
      masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
      masterConfig2.whichPcs =  kDSPI_Pcs0;//SPI1_DSPI_MASTER_PCS_FOR_INIT;
      masterConfig2.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

      masterConfig2.enableContinuousSCK = false;
      masterConfig2.enableRxFifoOverWrite = false;
      masterConfig2.enableModifiedTimingFormat = false;
      masterConfig2.samplePoint = kDSPI_SckToSin2Clock;

      srcClock_Hz = CLOCK_GetFreq(DSPI1_MASTER_CLK_SRC);
      DSPI_MasterInit(SPI1_DSPI_MASTER_BASEADDR, &masterConfig2, srcClock_Hz);
////    }
////    else
////    {
////      /* PORTC3 (pin 106) is configured as SPI1_PCS1 */
////      PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt2);
////       /* PORTE16 (pin 18) is configured as SPI1_PCS0 */
////      PORT_SetPinMux(PORTE, 16U, kPORT_MuxAsGpio);
////      masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;
////      masterConfig.whichPcs = kDSPI_Pcs1;//SPI1_DSPI_MASTER_PCS_FOR_INIT;
////      masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;
////
////      masterConfig.enableContinuousSCK = false;
////      masterConfig.enableRxFifoOverWrite = false;
////      masterConfig.enableModifiedTimingFormat = false;
////      masterConfig.samplePoint = kDSPI_SckToSin0Clock;
////
////      srcClock_Hz = CLOCK_GetFreq(DSPI_MASTER_CLK_SRC);
////      DSPI_MasterInit(SPI1_DSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);
////      masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;
////    }

    if (kStatus_Success != DSPI_MasterTransferEDMA(SPI1_DSPI_MASTER_BASEADDR, &g_dspi1_edma_m_handle, &masterXfer))
    {
        PRINTF("There is error when start DSPI_MasterTransferEDMA \r\n ");
    }


}

void SPI1_TX_7bit (uint8_t *SPI_masterTxData,uint8_t *SPI_masterRxData,uint32_t data_size,uint8_t psc)
{
  
    dspi_transfer_t masterXfer;      
//    masterXfer.txData = SPI_masterTxData;
//    masterXfer.rxData = SPI_masterRxData;
//    masterXfer.dataSize = data_size;
//    masterXfer.configFlags = kDSPI_MasterCtar0 | PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;  
//    
//    DSPI_MasterTransferBlocking(SPI1, &masterXfer);
    
    /*Master config*/
    masterConfig1.whichCtar = kDSPI_Ctar0;
    masterConfig1.ctarConfig.baudRate = SPI1_BAUDRATE;
    masterConfig1.ctarConfig.bitsPerFrame = 7U;
    masterConfig1.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
    masterConfig1.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    masterConfig1.ctarConfig.direction = kDSPI_MsbFirst;
    masterConfig1.ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / SPI1_BAUDRATE;
    masterConfig1.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / SPI1_BAUDRATE;
    masterConfig1.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / SPI1_BAUDRATE;
   

    
    /* Start master transfer */
    masterXfer.txData = SPI_masterTxData;
    masterXfer.rxData = SPI_masterRxData;
    masterXfer.dataSize = data_size;
//////    if (psc==0)
//////    {
//////       /* PORTE16 (pin 18) is configured as SPI1_PCS0 */
//////      PORT_SetPinMux(PORTE, 16U, kPORT_MuxAlt2);
//////      /* PORTC3 (pin 106) is configured as SPI1_PCS1 */
//////      PORT_SetPinMux(PORTC, 3U, kPORT_MuxAsGpio);
      masterXfer.configFlags = kDSPI_MasterCtar0| kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
      masterConfig1.whichPcs = kDSPI_Pcs0;//SPI1_DSPI_MASTER_PCS_FOR_INIT;
      masterConfig1.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

      masterConfig1.enableContinuousSCK = false;
      masterConfig1.enableRxFifoOverWrite = false;
      masterConfig1.enableModifiedTimingFormat = false;
      masterConfig1.samplePoint = kDSPI_SckToSin2Clock;

      srcClock_Hz = CLOCK_GetFreq(DSPI1_MASTER_CLK_SRC);
      DSPI_MasterInit(SPI1_DSPI_MASTER_BASEADDR, &masterConfig1, srcClock_Hz);
////    }
////    else
////    {
////      /* PORTC3 (pin 106) is configured as SPI1_PCS1 */
////      PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt2);
////       /* PORTE16 (pin 18) is configured as SPI1_PCS0 */
////      PORT_SetPinMux(PORTE, 16U, kPORT_MuxAsGpio);
////      masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;
////      masterConfig.whichPcs = kDSPI_Pcs1;//SPI1_DSPI_MASTER_PCS_FOR_INIT;
////      masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;
////
////      masterConfig.enableContinuousSCK = false;
////      masterConfig.enableRxFifoOverWrite = false;
////      masterConfig.enableModifiedTimingFormat = false;
////      masterConfig.samplePoint = kDSPI_SckToSin0Clock;
////
////      srcClock_Hz = CLOCK_GetFreq(DSPI_MASTER_CLK_SRC);
////      DSPI_MasterInit(SPI1_DSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);
////      masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;
////    }

    if (kStatus_Success != DSPI_MasterTransferEDMA(SPI1_DSPI_MASTER_BASEADDR, &g_dspi1_edma_m_handle, &masterXfer))
    {
        PRINTF("There is error when start DSPI_MasterTransferEDMA \r\n ");
    }


}


uint8_t can_data_out[128];
uint32_t len_out_can=0;
uint32_t can_ct_data =0;
/*!
 * @brief FlexCAN Call Back function
 */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
  static portBASE_TYPE xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdTRUE;
    /* Process FlexCAN Tx event. */
    if ((kStatus_FLEXCAN_TxIdle == status) && (TX_MESSAGE_BUFFER_NUM == result))
    {
        txComplete = true;
        if (flag_end_can_frame==1)
        {
      //    vTaskSuspend( xHandle_tx_flexcan );
       //   NVIC_EnableIRQ(DMA3_DMA19_IRQn); 
          flag_end_can_frame=0;
        }
        
    }
     if ((kStatus_FLEXCAN_RxIdle == status) && (RX_MESSAGE_BUFFER_NUM == result))
    {
        rxComplete = true;
          ct_can_frame++;
                can_data[0]=rxFrame1.dataByte0;
                can_data[1]=rxFrame1.dataByte1;
                can_data[2]=rxFrame1.dataByte2;
                can_data[3]=rxFrame1.dataByte3;

                can_data[4]=rxFrame1.dataByte4;
                can_data[5]=rxFrame1.dataByte5;
                can_data[6]=rxFrame1.dataByte6;
                can_data[7]=rxFrame1.dataByte7;
       ///         NVIC_DisableIRQ( CAN0_ORed_Message_buffer_IRQn); 
                // Возобновляем приостановленную задачу
                ///xYieldRequired = xTaskResumeFromISR(xHandle_tx_uart);

                 rxXfer.frame = &rxFrame1;
                  rxXfer.mbIdx = RX_MESSAGE_BUFFER_NUM;
                  FLEXCAN_TransferReceiveNonBlocking(BUS_CAN, &flexcanHandle, &rxXfer);
            //   xTaskResumeFromISR(xHandle_tx_flexcan);
           ///    portEND_SWITCHING_ISR(xHandle_tx_flexcan);
              
               xSemaphoreGiveFromISR(xBinarySemaphore_CAN,&xHigherPriorityTaskWoken);
               if( xHigherPriorityTaskWoken == pdTRUE )
                {
     /* Выдача семафора разблокирует задачу, и приоритет разблокированной
        задачи выше, чем у текущей выполняющейся задачи - поэтому контекст
        выполнения переключается принудительно в разблокированную (с более
        высоким приоритетом) задачу.
        ВНИМАНИЕ: макрос, реально используемый для переключения контекста
        из ISR, зависит от конкретного порта FreeRTOS. Здесь указано
        имя макроса, корректное для порта Open Watcom DOS. Другие порты
        FreeRTOS могут использовать другой синтаксис. Для определения
        используемого синтаксиса обратитесь к примерам, предоставленным
        вместе с портом FreeRTOS. */
                 taskYIELD();
              }
        status=0;

    }
}
void  CAN_transmit (void *pvParameters)
{
flexcan_mb_transfer_t txXfer;
//BaseType_t status_can =0;


  while (1){
xSemaphoreTake(xBinarySemaphore_CAN,portMAX_DELAY);
 
   decoding_frame(can_data,8,1,can_data_out,&len_out_can);
    if (len_out_can!=0)
      {

        for (can_ct_data=0;can_ct_data<(len_out_can);can_ct_data=can_ct_data+8)
          {
            txFrame1.format = kFLEXCAN_FrameFormatExtend;
            txFrame1.type = kFLEXCAN_FrameTypeData;
            txFrame1.id = FLEXCAN_ID_EXT(can_tx_addr);
            txFrame1.length = 8;
            txComplete = false;

            txXfer.frame = &txFrame1;
            txXfer.mbIdx = 8;
            
            txFrame1.dataByte0 = can_data_out[can_ct_data+0];
            txFrame1.dataByte1 = can_data_out[can_ct_data+1];
            txFrame1.dataByte2 = can_data_out[can_ct_data+2];
            txFrame1.dataByte3 = can_data_out[can_ct_data+3];

            txFrame1.dataByte4 = can_data_out[can_ct_data+4];
            txFrame1.dataByte5 = can_data_out[can_ct_data+5];
            txFrame1.dataByte6 = can_data_out[can_ct_data+6];
            txFrame1.dataByte7 = can_data_out[can_ct_data+7];
            FLEXCAN_TransferSendNonBlocking(BUS_CAN, &flexcanHandle, &txXfer);
            txComplete = false;
            if (can_ct_data+8>=len_out_can)
            {
              flag_end_can_frame=1;
            }
           //  NVIC_EnableIRQ( CAN0_ORed_Message_buffer_IRQn);
            while (!txComplete)
              {
              
              }
         ///   vTaskDelay(100);
          }  
           len_out_can=0;
            can_ct_data=0;
             
       ///     vTaskSuspend( NULL );
      }
    else
    {
         len_out_can=0;
          can_ct_data=0;
      //  NVIC_EnableIRQ(DMA3_DMA19_IRQn);
    ///    vTaskSuspend( NULL );
    }
         //   FLEXCAN_Send=0;
  //  }

   // taskYIELD();
// NVIC_EnableIRQ(CAN0_ORed_Message_buffer_IRQn);
     
}
};

///PWM1 controll



void restart_pwm (void)
{
 PWM1->OUTEN |=PWM_OUTEN_PWMA_EN(0x1);
 PWM1->OUTEN |=PWM_OUTEN_PWMB_EN(0x1);

 PWM_ClearStatusFlags(PWM1,kPWM_Module_0,kPWM_Fault1Flag); 
  
 PWM_SetPwmLdok(PWM1, kPWM_Control_Module_0, true);
}



uint8_t  load_power(int pwmValue)
{
  if ((PWM_GetStatusFlags(PWM1,kPWM_Module_0)&kPWM_Fault0Flag) != kPWM_Fault0Flag)
  {
    PWM_UpdatePwmDutycycle(PWM1, kPWM_Module_0, kPWM_PwmA, kPWM_SignedCenterAligned, pwmValue  );
    PWM_UpdatePwmDutycycle(PWM1, kPWM_Module_0, kPWM_PwmB, kPWM_SignedCenterAligned, pwmValue  );
    PWM_SetPwmLdok(PWM1, kPWM_Control_Module_0, true);
    return 0;
  }
  else
  {
    return 1;
  }

}


void PWM1_FAULT_IRQHandler(void)
{
/////////////*
////////////if (((PWM1->FSTS)&PWM_FSTS_FFLAG(8))>0)
////////////  {
////////////    __NOP();
////////////    __NOP();
////////////    __NOP();
////////////    __NOP();
////////////    __NOP();
////////////   if ((PORTE->ISFR&(1<<24))>0)
////////////      {
////////////        flag_err[4]=3;
////////////        PWM1->OUTEN &=~PWM_OUTEN_PWMA_EN(0xA);
////////////        PWM1->OUTEN &=~PWM_OUTEN_PWMB_EN(0xA);
////////////        PORTE->ISFR |=(1<<24);
////////////       // PWM_ClearStatusFlags(PWM1,kPWM_Module_3,kPWM_Fault3Flag); 
////////////        restart_pwm_ch5();
////////////        restart_pwm_ch3();
////////////      ///  PWM1->FSTS|=PWM_FSTS_FFLAG(8);    
////////////      }
////////////   else
////////////      {
////////////        if ((PORTE->ISFR&(1<<21))>0)
////////////          {
////////////            flag_err[5]=3;
////////////            PWM1->OUTEN &=~PWM_OUTEN_PWMA_EN(0xC); // Canal 5 fault
////////////            PWM1->OUTEN &=~PWM_OUTEN_PWMB_EN(0xC);
////////////            PORTE->ISFR|=(1<<21);
////////////           // PWM_ClearStatusFlags(PWM1,kPWM_Module_3,kPWM_Fault3Flag); 
////////////            restart_pwm_ch3();
////////////            restart_pwm_ch4();
////////////           /// PWM1->FSTS|=PWM_FSTS_FFLAG(8);    
////////////          }
////////////        else
////////////          {
////////////            flag_err[3]=3;
////////////            PWM1->OUTEN &=~PWM_OUTEN_PWMA_EN(0x9);
////////////            PWM1->OUTEN &=~PWM_OUTEN_PWMB_EN(0x9);
////////////          //  PWM_ClearStatusFlags(PWM1,kPWM_Module_3,kPWM_Fault3Flag); 
////////////            restart_pwm_ch4();
////////////            restart_pwm_ch5();
////////////           /// PWM1->FSTS|=PWM_FSTS_FFLAG(8);          
////////////          }
////////////      }*/
   if (((PORTE->ISFR&(1<<21))==0)&&((PORTE->ISFR&(1<<24))==0))
      {
       flag_err[3]=3;
       PWM1->OUTEN &=~PWM_OUTEN_PWMA_EN(0x9);
  //     PWM1->OUTEN &=~PWM_OUTEN_PWMB_EN(0x9);
        
        GPIO_ClearPinsOutput(BOARD_INITPINS_BUS_kontaktor_GPIO,1<<BOARD_INITPINS_BUS_kontaktor_PIN);
        Hi_DCDC.fault_bus=1;
        restart_pwm();
      
      }
  PWM1->FSTS|=PWM_FSTS_FFLAG(8);
 // }
  
}
static void PWM1_DRV_Init3PhPwm(void)
{
    uint16_t deadTimeVal;
    pwm_signal_param_t pwmSignal[2];
    uint32_t pwmSourceClockInHz;
    uint32_t pwmFrequencyInHz = 150000;

    pwmSourceClockInHz = CLOCK_GetFreq(kCLOCK_FastPeriphClk);

    /* Set deadtime count, we set this to about 650ns */
    deadTimeVal = ((uint64_t)pwmSourceClockInHz * 250) / 1000000000;

    pwmSignal[0].pwmChannel = kPWM_PwmA;
    pwmSignal[0].level = kPWM_HighTrue;
    pwmSignal[0].dutyCyclePercent = 25; /* 1 percent dutycycle */
    pwmSignal[0].deadtimeValue = deadTimeVal;
    pwmSignal[0].faultState       = kPWM_PwmFaultState0;

    pwmSignal[1].pwmChannel = kPWM_PwmB;
    pwmSignal[1].level = kPWM_HighTrue;
    /* Dutycycle field of PWM B does not matter as we are running in PWM A complementary mode */
    pwmSignal[1].dutyCyclePercent = 25;
    pwmSignal[1].deadtimeValue = deadTimeVal;
    pwmSignal[1].faultState       = kPWM_PwmFaultState0;

    /*********** PWMA_SM0 - phase A, configuration, setup 2 channel as an example ************/
    PWM_SetupPwm(PWM1, kPWM_Module_0, pwmSignal, 2, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);


//////////////////    PWM1->SM[0].DISMAP[0] =0x088;//??????? ?????????? ???????? ? ?? ??????? ???????
//////////////////    PWM1->SM[1].DISMAP[0] =0x088;
//////////////////    PWM1->SM[2].DISMAP[0] =0x088;
//////////////////    PWM1->SM[3].DISMAP[0] =0x000;
//////////////////    PWM1->FCTRL |=PWM_FCTRL_FLVL(0x00);
//////////////////    PWM1->FSTS |=PWM_FSTS_FFULL(8);//?????????? ??? ? ?????? ????? 
    
    
}

void data_load_config (ip4_addr_t* netif0_ipaddr,ip4_addr_t* netif0_netmask,ip4_addr_t* netif0_gw)
{
    uint32_t crc_symm[8]={0};
    uint32_t crc_flash[8]={0};
    
    crc_flash[7] =  *((uint32_t*)(IP_CONFIG_A+28));
    crc_flash[6] =  *((uint32_t*)(IP_CONFIG_A+24));
    crc_flash[5] =  *((uint32_t*)(IP_CONFIG_A+20));
    crc_flash[4] =  *((uint32_t*)(IP_CONFIG_A+16));
    crc_flash[3] =  *((uint32_t*)(IP_CONFIG_A+12));
    crc_flash[2] =  *((uint32_t*)(IP_CONFIG_A+8));
    crc_flash[1] =  *((uint32_t*)(IP_CONFIG_A+4));
    crc_flash[0] =  *((uint32_t*)(IP_CONFIG_A));
    

     //Р—Р°РіСЂСѓР·РєР° Р°РґСЂРµСЃРѕРІ CAN Рё IP
    
    InitCrc16(CRC0, 0xFFFFFFFFU);
    FMC->PFB0CR |= 1<<19;
    CRC_WriteData(CRC0, (const uint8_t *)crc_flash, 28);
    crc_symm[0] = CRC_Get16bitResult(CRC0);
    
    if ((crc_flash[7]==crc_symm[0]))
    {
              netif0_ipaddr->addr=crc_flash[0];
              netif0_netmask->addr=crc_flash[3];
              netif0_gw->addr=crc_flash[4];
              
    }
    else
    {      
              
              IP4_ADDR(netif0_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);   
              IP4_ADDR(netif0_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
              IP4_ADDR(netif0_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);         
       
       
              crc_flash[0]=netif0_ipaddr->addr;
              crc_flash[1]=ID_TX_DEF;
              crc_flash[2]=ID_RX0_DEF;
              crc_flash[3]=netif0_netmask->addr;
              crc_flash[4]=netif0_gw->addr;
              crc_flash[5]=configIP_ADDR0;
              crc_flash[6]=configIP_ADDR0;
              crc_flash[7]=configIP_ADDR0;
         
              InitCrc16(CRC0, 0xFFFFFFFFU);
              CRC_WriteData(CRC0, (const uint8_t *)crc_flash, 28);
              crc_flash[7] = CRC_Get16bitResult(CRC0);
              
             if (progSector( IP_CONFIG_A, (uint32_t*)crc_flash, 32))
                       {
                          PRINTF("Error prog LEN FW2 \r\n"); 
                        }

                //taskEXIT_CRITICAL();
//                
       

    
    }  
    
    FW_data.V_IP_CONFIG[0]=crc_flash[0];
    FW_data.V_IP_CONFIG[1]=crc_flash[0]>>8;
    FW_data.V_IP_CONFIG[2]=crc_flash[0]>>16;
    FW_data.V_IP_CONFIG[3]=crc_flash[0]>>24;
    
    FW_data.V_IP_MASK[0]=(uint32_t)crc_flash[3];
    FW_data.V_IP_MASK[1]=(uint32_t)crc_flash[3]>>8;
    FW_data.V_IP_MASK[2]=(uint32_t)crc_flash[3]>>16;
    FW_data.V_IP_MASK[3]=(uint32_t)crc_flash[3]>>24;
    
    FW_data.V_IP_GET[0]=(uint32_t)crc_flash[4];
    FW_data.V_IP_GET[1]=(uint32_t)crc_flash[4]>>8;
    FW_data.V_IP_GET[2]=(uint32_t)crc_flash[4]>>16;
    FW_data.V_IP_GET[3]=(uint32_t)crc_flash[4]>>24;
}
/*
 * @brief Main function
 */
void start_DCDC(void )
  {
    load_power(5);
    Hi_DCDC.power=5;
     PWM_StartTimer(PWM1, kPWM_Control_Module_0 );
     vTaskDelay(5000);
     Hi_DCDC.start_flag=1;
     PORT_SetPinMux(PORTB, 23U, kPORT_MuxAlt6);
////////       vTaskDelay(50000);
////////     
////////     GPIO_ClearPinsOutput(GPIOB, 1u << PIN9_IDX);
////////    GPIO_ClearPinsOutput(GPIOC, 1u << PIN15_IDX);
//////// 
////////
////////    vTaskDelay(500);
////////
////////    GPIO_SetPinsOutput(GPIOB, 1u << PIN9_IDX);    
////////    GPIO_SetPinsOutput(GPIOC, 1u << PIN15_IDX);
//     vTaskDelay(5000);
//      netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, NULL, ethernetif_init, tcpip_input);
//
//      netif_set_default(&fsl_netif0);
//      netif_set_up(&fsl_netif0);
   
    
  }
void stop_DCDC(void)
{
  load_power(0);
     PWM_StopTimer(PWM1, kPWM_Control_Module_0 );
     Hi_DCDC.start_flag=0;
     PORT_SetPinMux(PORTB, 23U, kPORT_MuxAsGpio);
      
}
int PWM1_init(void)
{
    gpio_pin_config_t HI_SWICH_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTC14 (pin 86)  */
    GPIO_PinInit(BOARD_INITPINS_HI_SWICH_GPIO, BOARD_INITPINS_HI_SWICH_PIN, &HI_SWICH_config);
    
    gpio_pin_config_t LO_SWICH_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTC15 (pin 87)  */
    GPIO_PinInit(BOARD_INITPINS_LO_SWICH_GPIO, BOARD_INITPINS_LO_SWICH_PIN, &LO_SWICH_config);
    
    
    /* Structure of initialize PWM */
    pwm_config_t pwmConfig;
    pwm_fault_param_t faultConfig;


 

    /* Set the PWM Fault inputs to a low value */
//////////////////////////    XBARA_Init(XBARA);
//////////////////////////    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn3, kXBARA_OutputPwm0Fault3);


   
    /*
     * pwmConfig.enableDebugMode = false;
     * pwmConfig.enableWait = false;
     * pwmConfig.reloadSelect = kPWM_LocalReload;
     * pwmConfig.faultFilterCount = 0;
     * pwmConfig.faultFilterPeriod = 0;
     * pwmConfig.clockSource = kPWM_BusClock;
     * pwmConfig.prescale = kPWM_Prescale_Divide_1;
     * pwmConfig.initializationControl = kPWM_Initialize_LocalSync;
     * pwmConfig.forceTrigger = kPWM_Force_Local;
     * pwmConfig.reloadFrequency = kPWM_LoadEveryOportunity;
     * pwmConfig.reloadLogic = kPWM_ReloadImmediate;
     * pwmConfig.pairOperation = kPWM_Independent;
     */
    PWM_GetDefaultConfig(&pwmConfig);

    /* Use full cycle reload */
    pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle;
    /* PWM A & PWM B form a complementary PWM pair */
    pwmConfig.pairOperation = kPWM_ComplementaryPwmA;
    pwmConfig.enableDebugMode = true;
   
    /* Initialize submodule 0 */
    pwmConfig.clockSource    = kPWM_BusClock;
////    pwmConfig.initializationControl = kPWM_Initialize_MasterSync;
    if (PWM_Init(PWM1, kPWM_Module_0, &pwmConfig) == kStatus_Fail)
    {
        PRINTF("PWM initialization failed\n");
        return 1;
    }
   PWM_FaultDefaultConfig(&faultConfig);
 
  /* Sets up the PWM fault protection */
   faultConfig.faultLevel =1;
   faultConfig.faultClearingMode = kPWM_Automatic;
   
    PWM_SetupFaults(PWM1, kPWM_Fault_1, &faultConfig);
  

    PWM1->SM[0].DISMAP[0] =0x002;
    /* Call the init function with demo configuration */
    PWM1_DRV_Init3PhPwm();

    /* Set the load okay bit for all submodules to load registers from their buffer */
    PWM_SetPwmLdok(PWM1, kPWM_Control_Module_0 , true);

    /* Start the PWM generation from Submodules 0, 1 and 2 */

    
    PWM1->FFILT |=PWM_FFILT_FILT_CNT(0);
    PWM1->FFILT |=PWM_FFILT_FILT_PER(0);
    PWM1->FFILT |=PWM_FFILT_GSTR(0);
    NVIC_SetPriority (PWM1_FAULT_IRQn, 1);
    PWM1->FCTRL |=PWM_FCTRL_FIE(2);
    NVIC_EnableIRQ(PWM1_FAULT_IRQn);
 //   PWM_StartTimer(PWM1, kPWM_Control_Module_0 );
   
    
   
      
    restart_pwm();
   
      return 0;
}


//**************** CMP2 CUR_BOOST  CMP3 CUR_BUS***************//
void CMP_init (void)
{
    cmp_config_t mCmpConfigStruct;
    cmp_dac_config_t mCmpDacConfigStruct;
/*
     * mCmpConfigStruct.enableCmp = true;
     * mCmpConfigStruct.hysteresisMode = kCMP_HysteresisLevel0;
     * mCmpConfigStruct.enableHighSpeed = false;
     * mCmpConfigStruct.enableInvertOutput = false;
     * mCmpConfigStruct.useUnfilteredOutput = false;
     * mCmpConfigStruct.enablePinOut = false;
     * mCmpConfigStruct.enableTriggerMode = false;
     */
    CMP_GetDefaultConfig(&mCmpConfigStruct);
    mCmpConfigStruct.enableHighSpeed = 0;
    mCmpConfigStruct.hysteresisMode = kCMP_HysteresisLevel0;
    mCmpConfigStruct.enablePinOut = 1;
    mCmpConfigStruct.enableInvertOutput = 0;
    /* Init the CMP comparator. */
    CMP_Init(CMP3, &mCmpConfigStruct);

    /* Configure the DAC channel. */
    mCmpDacConfigStruct.referenceVoltageSource = kCMP_VrefSourceVin2; /* VCC. */
    mCmpDacConfigStruct.DACValue               = 63U;                 /* Half voltage of logic high level. */
    CMP_SetDACConfig(CMP3, &mCmpDacConfigStruct);
    CMP_SetInputChannels(CMP3, 5, 7);

    
   
  //  CMP_EnableInterrupts(CMP2, kCMP_OutputRisingInterruptEnable | kCMP_OutputFallingInterruptEnable);

}
////static void i2c_master_callback(I2C_Type *base, i2c_master_edma_handle_t *handle, status_t status, void *userData)
////{
////    /* Signal transfer success when received success status. */
////    if (status == kStatus_Success)
////    {
////        g_MasterCompletionFlag = true;
////    }
////}

void I2C_init (void)
{
  
  i2c_master_config_t masterConfig;
  uint32_t sourceClock;

    
    

    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = I2C_BAUDRATE;

    sourceClock = I2C_MASTER_CLK_FREQ;

    I2C_MasterInit(I2C_MASTER_BASEADDR, &masterConfig, sourceClock);

 
    
////////////////////////////////////    i2c_master_config_t masterConfig;
////////////////////////////////////    uint32_t sourceClock;
////////////////////////////////////    
////////////////////////////////////    edma_config_t config;
////////////////////////////////////
////////////////////////////////////
////////////////////////////////////
////////////////////////////////////    /*Init EDMA for example*/
////////////////////////////////////    DMAMUX_Init(I2C_DMAMUX_BASEADDR);
////////////////////////////////////    EDMA_GetDefaultConfig(&config);
////////////////////////////////////    EDMA_Init(I2C_DMA_BASEADDR, &config);
////////////////////////////////////
////////////////////////////////////   /// PRINTF("\r\nI2C board2board EDMA example -- Master transfer.\r\n");
////////////////////////////////////
////////////////////////////////////    /* Set up i2c master to send data to slave*/
////////////////////////////////////    /* First data in txBuff is data length of the transmiting data. */
////////////////////////////////////    g_master_txBuff[0] = I2C_DATA_LENGTH - 1U;
////////////////////////////////////    for (uint32_t i = 1U; i < I2C_DATA_LENGTH; i++)
////////////////////////////////////    {
////////////////////////////////////        g_master_txBuff[i] = i - 1;
////////////////////////////////////    }
////////////////////////////////////
////////////////////////////////////////////    PRINTF("Master will send data :");
////////////////////////////////////////////    for (uint32_t i = 0U; i < I2C_DATA_LENGTH - 1U; i++)
////////////////////////////////////////////    {
////////////////////////////////////////////        if (i % 8 == 0)
////////////////////////////////////////////        {
////////////////////////////////////////////            PRINTF("\r\n");
////////////////////////////////////////////        }
////////////////////////////////////////////        PRINTF("0x%2x  ", g_master_txBuff[i + 1]);
////////////////////////////////////////////    }
////////////////////////////////////////////    PRINTF("\r\n\r\n");
////////////////////////////////////
////////////////////////////////////    /*
////////////////////////////////////     * masterConfig->baudRate_Bps = 100000U;
////////////////////////////////////     * masterConfig->enableStopHold = false;
////////////////////////////////////     * masterConfig->glitchFilterWidth = 0U;
////////////////////////////////////     * masterConfig->enableMaster = true;
////////////////////////////////////     */
////////////////////////////////////    I2C_MasterGetDefaultConfig(&masterConfig);
////////////////////////////////////    masterConfig.baudRate_Bps = I2C_BAUDRATE;
////////////////////////////////////
////////////////////////////////////    sourceClock = I2C_MASTER_CLK_FREQ;
////////////////////////////////////
////////////////////////////////////    I2C_MasterInit(I2C_MASTER_BASEADDR, &masterConfig, sourceClock);
////////////////////////////////////
////////////////////////////////////    memset(&g_m_dma_handle, 0, sizeof(g_m_dma_handle));
////////////////////////////////////
////////////////////////////////////    
////////////////////////////////////    
////////////////////////////////////   DMAMUX_SetSource(I2C_DMAMUX_BASEADDR, I2C_DMA_CHANNEL, DMA_REQUEST_SRC);
////////////////////////////////////   EDMA_CreateHandle(&edmaHandle,I2C_DMA_BASEADDR, I2C_DMA_CHANNEL);
////////////////////////////////////   DMAMUX_EnableChannel(I2C_DMAMUX_BASEADDR, I2C_DMA_CHANNEL);
   
   
}
void Dac_run (i2c_direction_t dir, uint8_t* data, uint8_t adress, uint8_t command, uint8_t LENGTH)
{
  
////  void ssd1306_I2C_WriteMulti(I2C_Type* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {

        i2c_master_transfer_t masterXfer;
   memset(&masterXfer, 0, sizeof(masterXfer));

    /* subAddress = 0x01, data = g_master_txBuff - write to slave.
      start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
    uint8_t deviceAddress     = command;
    masterXfer.slaveAddress   = adress>>1;
    masterXfer.direction      = dir;
    masterXfer.subaddress     = (uint32_t)deviceAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = data;
    masterXfer.dataSize       = LENGTH;
    masterXfer.flags          = kI2C_TransferDefaultFlag;
    I2C0->FLT|=0x0f;

    I2C_MasterTransferBlocking(I2C_MASTER_BASEADDR, &masterXfer);
        
}
void I2C_run (i2c_direction_t dir, uint8_t* data, uint8_t adress, uint8_t command, uint8_t LENGTH)
{
  
////  void ssd1306_I2C_WriteMulti(I2C_Type* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {

        i2c_master_transfer_t masterXfer;
   memset(&masterXfer, 0, sizeof(masterXfer));

    /* subAddress = 0x01, data = g_master_txBuff - write to slave.
      start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
    uint8_t deviceAddress     = command;
    masterXfer.slaveAddress   = adress>>1;
    masterXfer.direction      = dir;
    masterXfer.subaddress     = (uint32_t)deviceAddress;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = data;
    masterXfer.dataSize       = LENGTH;
    masterXfer.flags          = kI2C_TransferDefaultFlag;
    I2C0->FLT|=0x0f;

    I2C_MasterTransferBlocking(I2C_MASTER_BASEADDR, &masterXfer);
        
        
////}



//////////////////////////////////////////////////  i2c_master_transfer_t masterXfer;
//////////////////////////////////////////////////  memset(&masterXfer, 0, sizeof(masterXfer));
//////////////////////////////////////////////////  
//////////////////////////////////////////////////  
//////////////////////////////////////////////////  if (dir == kI2C_Write)
//////////////////////////////////////////////////  {
//////////////////////////////////////////////////   
//////////////////////////////////////////////////    /* subAddress = 0x01, data = g_master_txBuff - write to slave.
//////////////////////////////////////////////////      start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/   
//////////////////////////////////////////////////
//////////////////////////////////////////////////   
//////////////////////////////////////////////////    
//////////////////////////////////////////////////    uint8_t deviceAddress     = 0x01U;
//////////////////////////////////////////////////    masterXfer.slaveAddress   = adress>>1;
//////////////////////////////////////////////////    masterXfer.direction      = kI2C_Write;
//////////////////////////////////////////////////    masterXfer.subaddress     = (uint32_t)command;
//////////////////////////////////////////////////    masterXfer.subaddressSize = 1;
//////////////////////////////////////////////////    if (command == 0 )
//////////////////////////////////////////////////    {
//////////////////////////////////////////////////      masterXfer.subaddressSize = 0;
//////////////////////////////////////////////////    }
//////////////////////////////////////////////////    masterXfer.data           = data;
//////////////////////////////////////////////////    masterXfer.dataSize       = LENGTH;
//////////////////////////////////////////////////    masterXfer.flags          = kI2C_TransferDefaultFlag;
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////    I2C_MasterCreateEDMAHandle(I2C_MASTER_BASEADDR, &g_m_dma_handle, i2c_master_callback, NULL, &edmaHandle);
//////////////////////////////////////////////////    I2C_MasterTransferEDMA(I2C_MASTER_BASEADDR, &g_m_dma_handle, &masterXfer);
//////////////////////////////////////////////////
//////////////////////////////////////////////////    /*  Wait for transfer completed. */
//////////////////////////////////////////////////    while (!g_MasterCompletionFlag)
//////////////////////////////////////////////////    {
//////////////////////////////////////////////////       vTaskDelay(100);
//////////////////////////////////////////////////    }
//////////////////////////////////////////////////    g_MasterCompletionFlag = false;
//////////////////////////////////////////////////    
//////////////////////////////////////////////////    
//////////////////////////////////////////////////  }
//////////////////////////////////////////////////  else
//////////////////////////////////////////////////  {
//////////////////////////////////////////////////
//////////////////////////////////////////////////  ///  PRINTF("Receive sent data from slave :");
//////////////////////////////////////////////////
//////////////////////////////////////////////////    /* subAddress = 0x01, data = g_master_rxBuff - read from slave.
//////////////////////////////////////////////////      start + slaveaddress(w) + subAddress + repeated start + slaveaddress(r) + rx data buffer + stop */
//////////////////////////////////////////////////    masterXfer.slaveAddress   = adress>>1;
//////////////////////////////////////////////////    masterXfer.direction      = kI2C_Read;
//////////////////////////////////////////////////    masterXfer.subaddress     = (uint32_t)command;
//////////////////////////////////////////////////    masterXfer.subaddressSize = 1;
//////////////////////////////////////////////////    if (command == 0 )
//////////////////////////////////////////////////    {
//////////////////////////////////////////////////      masterXfer.subaddressSize = 0;
//////////////////////////////////////////////////    }
//////////////////////////////////////////////////    masterXfer.data           = data;
//////////////////////////////////////////////////    masterXfer.dataSize       = LENGTH ;
//////////////////////////////////////////////////    masterXfer.flags          = kI2C_TransferDefaultFlag;
//////////////////////////////////////////////////    masterXfer.flags          = kI2C_TransferDefaultFlag;
////////////////////////////////////////////////// I2C_MasterCreateEDMAHandle(I2C_MASTER_BASEADDR, &g_m_dma_handle, i2c_master_callback, NULL, &edmaHandle);
//////////////////////////////////////////////////    I2C_MasterTransferEDMA(I2C_MASTER_BASEADDR, &g_m_dma_handle, &masterXfer);
//////////////////////////////////////////////////
//////////////////////////////////////////////////    /*  Reset master completion flag to false. */
//////////////////////////////////////////////////    g_MasterCompletionFlag = false;
//////////////////////////////////////////////////
//////////////////////////////////////////////////    /*  Wait for transfer completed. */
//////////////////////////////////////////////////    while (!g_MasterCompletionFlag)
//////////////////////////////////////////////////    {
//////////////////////////////////////////////////    ///   vTaskDelay(100);
//////////////////////////////////////////////////    }
//////////////////////////////////////////////////    g_MasterCompletionFlag = false;
//////////////////////////////////////////////////
//////////////////////////////////////////////////  }
}

void hard_config (void)
{
//  delay(10000000);
    init_system_heap(); //Инициализация динамического выделения памяти 
    SIM_GetUniqueId(&ID_STR); //Считывание серийный номер 
    long long Ser;
          Ser = ID_STR.ML;
          Ser = Ser <<32;
          Ser |= ID_STR.L;
      char* p = (char*)&Ser;
        for (int i = 0; i <= 7; i++)
          {
            Serial[i] = *(p + i);
            FW_data.V_ID_MAC[i]=*(p + i);
          }

   // Enet var
    static struct netif fsl_netif0;
    ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
   //FTM var
    ftm_config_t ftmInfo;
    ftm_chnl_pwm_signal_param_t ftmParam;
    ftm_pwm_level_select_t pwmLevel = kFTM_LowTrue;

    /* Configure ftm params with frequency 25MHZ */
    //Таймер тактирования PHY
    ftmParam.chnlNumber = BOARD_FTM_CHANNEL;
    ftmParam.level = pwmLevel;
    ftmParam.dutyCyclePercent = updatedDutycycle;
    ftmParam.firstEdgeDelayPercent = 0U;
     
  //******************************************************************************************************
  //**************Инициализация переферии**********************************
    MPU_Type *base = MPU;
    //Board init
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

  ///  UART_dma_init ();
    progFLASH_init();//Init flash controller
    
  //  SPI1_init();

    //Ftm init

    FTM_GetDefaultConfig(&ftmInfo);
    /* Initialize FTM module */
    FTM_Init(BOARD_FTM_BASEADDR, &ftmInfo);

    FTM_SetupPwm(BOARD_FTM_BASEADDR, &ftmParam, 1U, kFTM_CenterAlignedPwm, 25000000U, FTM_SOURCE_CLOCK);

    FTM_StartTimer(BOARD_FTM_BASEADDR, kFTM_SystemClock);

    
    //  Ethernet init and PHY

     Ethernet_pin_init ();


    delay(100000);

    GPIO_PinInit (GPIOB,PIN9_IDX,&config_out);
    GPIO_SetPinsOutput(GPIOB, 1u << PIN9_IDX);    
    GPIO_PinInit (GPIOC,PIN15_IDX,&config_out);
    GPIO_SetPinsOutput(GPIOC, 1u << PIN15_IDX);

    delay(100000);
     
////    
    GPIO_ClearPinsOutput(GPIOB, 1u << PIN9_IDX);
    GPIO_ClearPinsOutput(GPIOC, 1u << PIN15_IDX);
 

    delay(100000);

    GPIO_SetPinsOutput(GPIOB, 1u << PIN9_IDX);    
    GPIO_SetPinsOutput(GPIOC, 1u << PIN15_IDX);

    //Ethernet
    taskENTER_CRITICAL();
    base->CESR &= ~MPU_CESR_VLD_MASK;
    SysTick_Config(configCPU_CLOCK_HZ/configTICK_RATE_HZ);    



   

    data_load_config(&fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw);
    
  

    tcpip_init(NULL, NULL);

    netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, NULL, ethernetif_init, tcpip_input);

    netif_set_default(&fsl_netif0);
    netif_set_up(&fsl_netif0);

    http_server_netconn_init();

    udpecho_init();
//
//    PRINTF("\r\n************************************************\r\n");
//    PRINTF(" Csort boot loader start \r\n");
//    PRINTF("************************************************\r\n");
//    PRINTF(" IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_ipaddr)[0], ((u8_t *)&fsl_netif0_ipaddr)[1],
//           ((u8_t *)&fsl_netif0_ipaddr)[2], ((u8_t *)&fsl_netif0_ipaddr)[3]);
//    PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_netmask)[0], ((u8_t *)&fsl_netif0_netmask)[1],
//           ((u8_t *)&fsl_netif0_netmask)[2], ((u8_t *)&fsl_netif0_netmask)[3]);
//    PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_gw)[0], ((u8_t *)&fsl_netif0_gw)[1],
//           ((u8_t *)&fsl_netif0_gw)[2], ((u8_t *)&fsl_netif0_gw)[3]);
//    PRINTF(" CAN Adress     : TX=0x%X\r\n", can_tx_addr);
//    PRINTF(" CAN Adress     : RX0=0x%X\r\n", can_rx_addr);
//    PRINTF("************************************************\r\n");

///delay(10000000);
    
      SPI1_init();
//////      isTransferCompleted1=false;
//////      SPI1_TX(SPI1_masterTxData,0,all_modul,0);
//////      while (isTransferCompleted1==false){}
    
      SPI0_init();
//////      isTransferCompleted0=false;
//////      SPI0_TX(SPI0_masterTxData,0,all_modul,0);
//////      while (isTransferCompleted0==false){};
      
      
      I2C_init();
      
     HSADC0_init();
 ////  HSADC1_init();
     start_hsadc();
//////////////////////     CMP_init();
//////////////////////     PWM1_init();  
     
   //  SSD1306_Init();       
    // SPI1_init();
  //  SSD1306_DrawRectangle(10,10,30,30,1);

   
}
