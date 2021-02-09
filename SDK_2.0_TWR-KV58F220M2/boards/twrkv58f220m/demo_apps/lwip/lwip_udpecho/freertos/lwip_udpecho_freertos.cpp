/*
* Copyright (c) 2016, Freescale Semiconductor, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lwip/opt.h"
#include "progFlash.h"
#if LWIP_NETCONN

#include "udpecho/udpecho.h"
#include "lwip/tcpip.h"
#include "netif/ethernet.h"
#include "ethernetif.h"

#include "board.h"
#include "fsl_flexcan.h"

#include "fsl_flexcan_edma.h"
#include "fsl_uart_edma.h"
#include "fsl_dma_manager.h"

#include "fsl_common.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_ftm.h"
#include "fsl_uart.h"
#include "fsl_edma.h"

/*******************************************************************************
 * Definitions CAN
 ******************************************************************************/
#define BUS_CAN CAN0
#define BUS_CAN_CLKSRC kCLOCK_FastPeriphClk
#define BUS_CAN_DMA_CHANNEL 3
#define BUS_CAN_DMA_REQUEST kDmaRequestMux0CAN0
#define TX_MESSAGE_BUFFER_NUM (9)
#define RX_MESSAGE_BUFFER_NUM (9)

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define EXAMPLE_ENET ENET




/* The Flextimer base address/channel used for board */
/* The Flextimer instance/channel used for board */
#define BOARD_FTM_BASEADDR FTM1
#define BOARD_FTM_CHANNEL kFTM_Chnl_0
/* Interrupt number and interrupt handler for the FTM instance used */
#define FTM_INTERRUPT_NUMBER FTM1_IRQn
#define FTM_LED_HANDLER FTM1_IRQHandler

/* Interrupt to enable and flag to read; depends on the FTM channel used */
#define FTM_CHANNEL_INTERRUPT_ENABLE kFTM_Chnl6InterruptEnable
#define FTM_CHANNEL_FLAG kFTM_Chnl1Flag

/* Get source clock for FTM driver */
#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_FastPeriphClk)
/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
 * Variables CAN
 ******************************************************************************/
   xTaskHandle xHandle_tx_uart;
volatile bool txComplete = false;
volatile bool rxComplete = false;
flexcan_handle_t flexcanHandle;
flexcan_edma_handle_t flexcanEdmaHandle;
edma_handle_t flexcanRxFifoEdmaHandle;

flexcan_fifo_transfer_t rxFifoXfer;
flexcan_frame_t txFrame, rxFrame;
extern flexcan_frame_t txFrame1;
  flexcan_frame_t rxFrame1;
   unsigned char can_data[8]={0};
/*******************************************************************************
 * Prototypes UART
 ******************************************************************************/

/* UART user callback */
void UART_UserCallback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData);


/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile uint8_t updatedDutycycle = 50U;
portCHAR bufer_trance[200];


/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief FlexCAN Call Back function
 */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    /* Process FlexCAN Tx event. */
    if ((kStatus_FLEXCAN_TxIdle == status) && (TX_MESSAGE_BUFFER_NUM == result))
    {
        txComplete = true;
    }
}

/*!
 * @brief FlexCAN DMA Call Back function
 */
static void flexcan_dma_callback(CAN_Type *base, flexcan_edma_handle_t *handle, status_t status, void *userData)
{
   

    /* Process FlexCAN Rx event. */
    if (kStatus_FLEXCAN_RxFifoIdle == status)
    {
                
                
                can_data[0]=rxFrame1.dataByte0;
                can_data[1]=rxFrame1.dataByte1;
                can_data[2]=rxFrame1.dataByte2;
                can_data[3]=rxFrame1.dataByte3;
                
                can_data[4]=rxFrame1.dataByte4;
                can_data[5]=rxFrame1.dataByte5;
                can_data[6]=rxFrame1.dataByte6;
                can_data[7]=rxFrame1.dataByte7;
                portBASE_TYPE xYieldRequired;
                // ¬озобновл€ем приостановленную задачу
                ///xYieldRequired = xTaskResumeFromISR(xHandle_tx_uart);
               
                rxFifoXfer.frame = &rxFrame1;
                FLEXCAN_TransferReceiveFifoEDMA(BUS_CAN, &flexcanEdmaHandle, &rxFifoXfer);
               xTaskResumeFromISR(xHandle_tx_uart);
                portEND_SWITCHING_ISR(xHandle_tx_uart);
                 
     
    }
}






int ct_index=0;
void Trance( void *pvParameters )
{

  while (1){
    
  //  
  //  taskENTER_CRITICAL();
  //  progF();
  //  taskEXIT_CRITICAL();
     vTaskList(bufer_trance);
     while (bufer_trance[ct_index] !=0)
      {
     PRINTF("%c", bufer_trance[ct_index]);
     ct_index++;
     }
     ct_index=0;
     
    // g=xPortGetFreeHeapSize();
    // PRINTF("%u", g);
     vTaskDelay(5000); 
     
     }
  
}
/*!
 * @brief Main function
 */
void UART_transmit  (void *pvParameters)
{
  
  while (1){
  
      uart_send(can_data,sizeof(can_data));
       while (!txOnGoing)
                  {
                    taskYIELD();                    
                  }
               txOnGoing = false;
       
    
     vTaskSuspend( NULL );

  }
}
void  CAN_transmit (void *pvParameters)
{
flexcan_mb_transfer_t txXfer;
  

  while (1){
  //  if (FLEXCAN_Send ==1)
  /// {
    
            txFrame1.format = kFLEXCAN_FrameFormatExtend;
            txFrame1.type = kFLEXCAN_FrameTypeData;
            txFrame1.id = FLEXCAN_ID_EXT(ID_TX_DEF);
            txFrame1.length = 8;
            txComplete = true;
        
            txXfer.frame = &txFrame1;
            txXfer.mbIdx = 8;
            txFrame1.dataByte0 = data_uart[0];
            txFrame1.dataByte1 = data_uart[1];
            txFrame1.dataByte2 = data_uart[2];
            txFrame1.dataByte3 = data_uart[3];
    
            txFrame1.dataByte4 = data_uart[4];
            txFrame1.dataByte5 = data_uart[5];
            txFrame1.dataByte6 = data_uart[6];
            txFrame1.dataByte7 = data_uart[7];
      
            FLEXCAN_TransferSendNonBlocking(BUS_CAN, &flexcanHandle, &txXfer);
              while (!txComplete)
                  {
                    taskYIELD();
                    
                  }
               txComplete = false;
         //   FLEXCAN_Send=0;
  //  }
   
   // taskYIELD();
     vTaskSuspend( NULL );
}

};
void CAN_Receive(void *pvParameters)
{
   while (1)
                  {
                    taskYIELD();
                    
                  }
        /*        
                PRINTF("Receive Msg  0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x \r\n",
                       can_data[0],can_data[1],can_data[2],can_data[3]
                       ,can_data[4],can_data[5],can_data[6],can_data[7]);
                  
                  taskYIELD();
              }*/
}

int main(void)
{
  
    
   // Enet var 
    static struct netif fsl_netif0;
    ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
   //FTM var  
    ftm_config_t ftmInfo;
    ftm_chnl_pwm_signal_param_t ftmParam;
    ftm_pwm_level_select_t pwmLevel = kFTM_LowTrue;

    /* Configure ftm params with frequency 25MHZ */
    ftmParam.chnlNumber = BOARD_FTM_CHANNEL;
    ftmParam.level = pwmLevel;
    ftmParam.dutyCyclePercent = updatedDutycycle;
    ftmParam.firstEdgeDelayPercent = 0U;
// CAN var 
     flexcan_rx_mb_config_t mbConfig;
  
     
    edma_config_t edmaConfig;
    flexcan_config_t flexcanConfig;
    flexcan_rx_fifo_config_t rxFifoConfig;
    uint32_t rxFifoFilter[] = {
        FLEXCAN_RX_FIFO_EXT_FILTER_TYPE_A(ID_RX0_DEF, 0, 1),
        FLEXCAN_RX_FIFO_EXT_FILTER_TYPE_A(ID_RX0_DEF, 1, 1),
        FLEXCAN_RX_FIFO_EXT_FILTER_TYPE_A(ID_RX1_DEF, 0, 1),
        FLEXCAN_RX_FIFO_EXT_FILTER_TYPE_A(ID_RX1_DEF, 1, 1)
    
    };
       
   
  //******************************************************************************************************
    
    MPU_Type *base = MPU;
    //Board init 
    BOARD_InitPins();
    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();
   
    UART_dma_init ();
    
    
    
    
    //Ftm init
    
    FTM_GetDefaultConfig(&ftmInfo);
    /* Initialize FTM module */
    FTM_Init(BOARD_FTM_BASEADDR, &ftmInfo);

    FTM_SetupPwm(BOARD_FTM_BASEADDR, &ftmParam, 1U, kFTM_CenterAlignedPwm, 25000000U, FTM_SOURCE_CLOCK);

    FTM_StartTimer(BOARD_FTM_BASEADDR, kFTM_SystemClock);
    //  Ethernet init and PHY 
       
     Ethernet_pin_init ();
       
    delay(10000);
    
    GPIO_PinInit (GPIOB,PIN9_IDX,&config_out);
    GPIO_ClearPinsOutput(GPIOB, 1u << PIN9_IDX);
     
    delay(10000);

    GPIO_SetPinsOutput(GPIOB, 1u << PIN9_IDX);
    
    delay(5000000);
     //xPortStartScheduler() ;
    //fLASH 
    //progF();
    
    //Ethernet
    
    base->CESR &= ~MPU_CESR_VLD_MASK;
    SysTick_Config(configCPU_CLOCK_HZ/configTICK_RATE_HZ);
    IP4_ADDR(&fsl_netif0_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&fsl_netif0_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&fsl_netif0_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);

    tcpip_init(NULL, NULL);

    netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, NULL, ethernetif_init, tcpip_input);

    netif_set_default(&fsl_netif0);
    netif_set_up(&fsl_netif0);
    
  ///  http_server_netconn_init();

 ///   udpecho_init();
 
    PRINTF("\r\n************************************************\r\n");
    PRINTF(" HTTP Server example\r\n");
    PRINTF("************************************************\r\n");
    PRINTF(" IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_ipaddr)[0], ((u8_t *)&fsl_netif0_ipaddr)[1],
           ((u8_t *)&fsl_netif0_ipaddr)[2], ((u8_t *)&fsl_netif0_ipaddr)[3]);
    PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_netmask)[0], ((u8_t *)&fsl_netif0_netmask)[1],
           ((u8_t *)&fsl_netif0_netmask)[2], ((u8_t *)&fsl_netif0_netmask)[3]);
    PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_gw)[0], ((u8_t *)&fsl_netif0_gw)[1],
           ((u8_t *)&fsl_netif0_gw)[2], ((u8_t *)&fsl_netif0_gw)[3]);
     PRINTF(" CAN Adress     : TX=0x%X\r\n", ID_TX_DEF);
     PRINTF(" CAN Adress     : RX0=0x%X\r\n", ID_RX0_DEF);
     PRINTF(" CAN Adress     : RX1=0x%X\r\n",ID_RX1_DEF);
    PRINTF("************************************************\r\n");
    
    // CAN init 
    
    /* Init FlexCAN module. */
    /*
     * flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
     * flexcanConfig.baudRate = 125000U;
     * flexcanConfig.maxMbNum = 16;
     * flexcanConfig.enableLoopBack = false;
     * flexcanConfig.enableSelfWakeup = false;
     * flexcanConfig.enableIndividMask = false;
     * flexcanConfig.enableDoze = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    flexcanConfig.clkSrc = kFLEXCAN_ClkSrcPeri;
    flexcanConfig.baudRate =125000;
    flexcanConfig.enableIndividMask=true;
    flexcanConfig.enableLoopBack = false;
    
    FLEXCAN_Init(BUS_CAN, &flexcanConfig, CLOCK_GetFreq(BUS_CAN_CLKSRC));

    /* Configure DMA. */
    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, BUS_CAN_DMA_CHANNEL, BUS_CAN_DMA_REQUEST);
    DMAMUX_EnableChannel(DMAMUX0, BUS_CAN_DMA_CHANNEL);

    /*
     * edmaConfig.enableRoundRobinArbitration = false;
     * edmaConfig.enableHaltOnError = true;
     * edmaConfig.enableContinuousLinkMode = false;
     * edmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(DMA0, &edmaConfig);

    /* Create EDMA handle. */
    EDMA_CreateHandle(&flexcanRxFifoEdmaHandle, DMA0, BUS_CAN_DMA_CHANNEL);
    NVIC_SetPriority (DMA3_DMA19_IRQn, 6);
  
    /* Setup Tx Message Buffer. */
    FLEXCAN_SetTxMbConfig(BUS_CAN, TX_MESSAGE_BUFFER_NUM, true);

    /* Setup Rx FIFO. */
    rxFifoConfig.idFilterTable = rxFifoFilter;
    rxFifoConfig.idFilterType = kFLEXCAN_RxFifoFilterTypeA;
    rxFifoConfig.idFilterNum = sizeof(rxFifoFilter) / sizeof(rxFifoFilter[0]);
    rxFifoConfig.priority = kFLEXCAN_RxFifoPrioHigh;
    FLEXCAN_SetRxFifoConfig(BUS_CAN, &rxFifoConfig, true);

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(BUS_CAN, &flexcanHandle, flexcan_callback, NULL);

    /* Create FlexCAN EDMA handle structure and set call back function. */
    FLEXCAN_TransferCreateHandleEDMA(BUS_CAN, &flexcanEdmaHandle, flexcan_dma_callback, NULL,
                                     &flexcanRxFifoEdmaHandle);

    // Send first message through Tx Message Buffer. 
    txFrame.format = kFLEXCAN_FrameFormatExtend;
    txFrame.type = kFLEXCAN_FrameTypeData;
    txFrame.id = FLEXCAN_ID_EXT(ID_TX_DEF);
    txFrame.length = 8;
    txComplete = true;
    rxFifoXfer.frame = &rxFrame1;
    FLEXCAN_TransferReceiveFifoEDMA(BUS_CAN, &flexcanEdmaHandle, &rxFifoXfer);
    
    // Start FREERTOS
    xTaskCreate( Trance, "Trance", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );
 //   CAN_transmit
   
  //  xTaskCreate( CAN_Receive, "CAN_Receive", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );
    xTaskCreate( CAN_transmit, "CAN_transmit", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, &xHandle_tx_flexcan );
    vTaskSuspend( xHandle_tx_flexcan );
    xTaskCreate( UART_transmit, "UART_transmit", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, &xHandle_tx_uart );
    vTaskSuspend( xHandle_tx_uart );
    vTaskStartScheduler();
    
    /* Will not get here unless a task calls vTaskEndScheduler ()*/
    while (1){};
    return 0;
}
#endif
