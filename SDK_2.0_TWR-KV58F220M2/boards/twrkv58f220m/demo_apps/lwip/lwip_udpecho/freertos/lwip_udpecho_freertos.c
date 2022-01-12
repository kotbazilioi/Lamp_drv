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
#include "pflash.h"
#include "frame_decod.h"
#include "fsl_flash.h"
#include "heap_3.h"
#include "pflash.h"
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
#include "fsl_pwm.h"
#include "fsl_common.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_ftm.h"
#include "fsl_uart.h"
#include "fsl_edma.h"
#include "fsl_sim.h"
#include "hard_init.h"
#include "httpserver-netconn.h"
#include "fsl_port.h"
#include "fsl_i2c.h"
#include "fsl_i2c_edma.h"
#include "fsl_dmamux.h"
#include "ssd1306.h"
#include "app.h"
#include <stdio.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define EXAMPLE_ENET ENET


/*******************************************************************************
* Prototypes
******************************************************************************/
 xTaskHandle xHandle_tx_uart;


/*******************************************************************************
 * Variables CAN
 ******************************************************************************/


/*******************************************************************************
 * Prototypes UART
 ******************************************************************************/

/* UART user callback */
void UART_UserCallback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData);


/*******************************************************************************
 * Variables
 ******************************************************************************/




/*******************************************************************************
 * Code
 ******************************************************************************/


/*!
 * @brief FlexCAN DMA Call Back function
 */

const char on[]="On";
const char off[]="Off";
const char run[]="Run";
const char err[]="Err";
const char stop[]="Stop";




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
       ///   PRINTF("%c", bufer_trance[ct_index]);
          ct_index++;
     }
     
     ct_index=0;

    // g=xPortGetFreeHeapSize();
    // PRINTF("%u", g);
     vTaskDelay(50000);

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



void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
 /*   ( void ) pcTaskName;
    ( void ) pxTask;*/
  PRINTF("Error %s StackOverflowHook\r\n",pcTaskName);

    for( ;; );
}
void led_blink (void *pvParameters)
{

     
 
  while (1){
    vTaskDelay(2000);


      GPIO_TogglePinsOutput(GPIOE,1<<PIN30_IDX);
      
      GPIO_TogglePinsOutput(GPIOC,1<<PIN13_IDX);
      
      
 

     }
}
void oled_diplay (void *pvParameters)
{
  uint8_t contr=0;
    SSD1306_DrawFilledCircle(19,28,3,SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    vTaskDelay(1500);
    SSD1306_DrawCircle(16,32,16,SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    vTaskDelay(1500);
    SSD1306_GotoXY(35,25);
    char mess1[]="Csort";
    SSD1306_Puts (mess1,&Font_16x26,SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    for (;contr<255;contr++)
    {
      set_contrast(contr);
      vTaskDelay(25);
    }
    vTaskDelay(1500);
      for (;contr>0;contr--)
    {
      set_contrast(contr);
      vTaskDelay(25);
    }
////     for (;contr<255;contr++)
////    {
////      set_contrast(contr);
////      vTaskDelay(25);
////    }
////    vTaskDelay(1500);
////      for (;contr>0;contr--)
////    {
////      set_contrast(contr);
////      vTaskDelay(25);
////    }
////     for (;contr<255;contr++)
////    {
////      set_contrast(contr);
////      vTaskDelay(25);
////    }
////    vTaskDelay(1500);
////      for (;contr>0;contr--)
////    {
////      set_contrast(contr);
////      vTaskDelay(25);
////    }
 //   vTaskDelay(500);
    
 // char* mess;
  SSD1306_Fill(SSD1306_COLOR_BLACK);
  SSD1306_UpdateScreen();
   vTaskDelay(1500);
set_contrast(255);
  while (1){

 SSD1306_Fill(SSD1306_COLOR_BLACK);
 SSD1306_DrawFilledCircle(3,3,3,SSD1306_COLOR_WHITE); 

 put_int(Hi_DCDC.dc_bus_volt,0,16,SSD1306_COLOR_WHITE);
 put_int(Hi_DCDC.power_bus_volt,0,28,SSD1306_COLOR_WHITE);
 put_int(Hi_DCDC.power,0,40,SSD1306_COLOR_WHITE);
    SSD1306_GotoXY(32,16);
 if (Hi_DCDC.start_flag == 0)
 {
  SSD1306_Puts ((char*)on,&Font_7x10,SSD1306_COLOR_WHITE);
 }
 else
 {
  SSD1306_Puts ((char*)off,&Font_7x10,SSD1306_COLOR_WHITE);
 }
 
 SSD1306_GotoXY(10,SSD1306_COLOR_WHITE);
 if (Hi_DCDC.fault_bus == 0)
 {
   SSD1306_Puts ((char*)err,&Font_7x10,SSD1306_COLOR_WHITE);
 }
 else
 {
    SSD1306_Puts ((char*)err,&Font_7x10,SSD1306_COLOR_BLACK);
 }
 
  put_int(lamp_state.modul_state[0].temp,32,28,SSD1306_COLOR_WHITE);
  put_int(lamp_state.modul_state[1].temp,32,40,SSD1306_COLOR_WHITE);
 
 
   SSD1306_UpdateScreen();
     vTaskDelay(1000);
     SSD1306_DrawFilledCircle(3,3,3,SSD1306_COLOR_BLACK);      
      SSD1306_UpdateScreen();
      vTaskDelay(1000);
  
     }
}

void reload_io (void *pvParameters)
{
  
vTaskDelay(100*speed_grade);


#if (lamp_sys)
lamp_state.led_data_b_req=0x00;
lamp_state.led_data_r_req=0xff;
lamp_state.led_data_g_req=0x00;
lamp_state.led_data_ir_req=0x00;
#else
lamp_state.led_data_b_req=0xff;
lamp_state.led_data_r_req=0x00;
lamp_state.led_data_g_req=0x00;
lamp_state.led_data_ir_req=0x00;
#endif


  while (1){
    RSet_lamp_power();
    if (flag_save_param == 1)
    {
    save_settings();
    flag_save_param=0;
    }
    
    // stoboskop ();
       if ((fsl_netif0.ip_addr.addr!=0)&&(flag_set_ip!=1))
      {
       FW_data.V_IP_CONFIG[0]=fsl_netif0.ip_addr.addr&0x000000ff;
       FW_data.V_IP_CONFIG[1]=(fsl_netif0.ip_addr.addr&0x0000ff00)>>8;
       FW_data.V_IP_CONFIG[2]=(fsl_netif0.ip_addr.addr&0x00ff0000)>>16;
       FW_data.V_IP_CONFIG[3]=(fsl_netif0.ip_addr.addr&0xff000000)>>24;
       FW_data.V_IP_MASK[0]=(fsl_netif0.netmask.addr&0x000000ff);
       FW_data.V_IP_MASK[1]=(fsl_netif0.netmask.addr&0x0000ff00)>>8;
       FW_data.V_IP_MASK[2]=(fsl_netif0.netmask.addr&0x00ff0000)>>16;
       FW_data.V_IP_MASK[3]=(fsl_netif0.netmask.addr&0xff000000)>>24;
       FW_data.V_IP_GET[0]=(fsl_netif0.gw.addr&0x000000ff);
       FW_data.V_IP_GET[1]=(fsl_netif0.gw.addr&0x0000ff00)>>8;
       FW_data.V_IP_GET[2]=(fsl_netif0.gw.addr&0x00ff0000)>>16;
       FW_data.V_IP_GET[3]=(fsl_netif0.gw.addr&0xff000000)>>24;
       flag_set_ip=1;
       
       
       
    //      ipdns.addr=fsl_netif0.dhcp.server_ip_addr.addr;
 
     //   dns_setserver (0,&ipdns);
  /* Start DHCP negotiation for a network interface (IPv4) */
  ////dhcp_start(&gnetif);
       
       
       
//       status_dns=dns_gethostbyname ("drv_fon1.csort.local", &ipdns, smtp_serverFound, NULL);
//       status_dns++;
  
       
       
       }
       
       dns_tmr();
       vTaskDelay(50*speed_grade);
       
     }
}

void start_convert (void *pvParameters)
{
  
  
  while (1){

    

    
   //vTaskDelay(100);
   //lamp_init(power_lamp_0);
  //  vTaskDelay(1*configTICK_RATE_HZ);
    
     lamp_init(power_lamp_1);
     http_server_netconn_init();
    
   //  lamp_init(power_lamp_5);
     udpecho_init();
     
    vTaskDelay(configTICK_RATE_HZ*speed_grade/10);
    xTaskCreate( reload_io, "reload_io", 512, NULL, tskIDLE_PRIORITY+1, &xHandle_reload_io );
  //  vTaskResume(xHandle_reload_io);  
    vTaskDelay(configTICK_RATE_HZ*speed_grade/10);
    #if (lamp_sys)
       lamp_state.lamp_power_req=15000;
    #else
       lamp_state.lamp_power_req=30000;
    #endif
    set_color_power(lamp_state.led_data_r_req,lamp_state.led_data_g_req,lamp_state.led_data_b_req,0);
    lamp_state.reload_dot=1;
    vTaskDelay(configTICK_RATE_HZ*speed_grade/10);
    Hi_DCDC.start_flag=1;  
      vTaskDelay(configTICK_RATE_HZ*speed_grade/10);
     set_color_power(lamp_state.led_data_r_req,lamp_state.led_data_g_req,lamp_state.led_data_b_req,0);
    lamp_state.reload_dot=1;
     vTaskDelay(configTICK_RATE_HZ*speed_grade/10);
    Hi_DCDC.start_flag=1;  
     vTaskDelay(configTICK_RATE_HZ*speed_grade/10);
     set_color_power(lamp_state.led_data_r_req,lamp_state.led_data_g_req,lamp_state.led_data_b_req,0);
    lamp_state.reload_dot=1;
      vTaskDelay(configTICK_RATE_HZ*speed_grade/10);
    Hi_DCDC.start_flag=1;  
      vTaskDelay(configTICK_RATE_HZ*speed_grade/10);
     
     
    vTaskSuspend( xHandle_start_convert );
      
     }
}
 


void BUS_controll (void *pvParameters)
{

  while (1){

if ((Hi_DCDC.dc_bus_volt>100)&&(Hi_DCDC.dc_bus_volt<400))
{
  if (Hi_DCDC.fault_bus == 0)
  {
  //   if (Hi_DCDC.start_flag==1)
  //   {
    //  GPIO_SetPinsOutput(BOARD_INITPINS_BUS_kontaktor_GPIO,1<<BOARD_INITPINS_BUS_kontaktor_PIN);
 //    }
  }
  
}
else
{
      GPIO_ClearPinsOutput(BOARD_INITPINS_BUS_kontaktor_GPIO,1<<BOARD_INITPINS_BUS_kontaktor_PIN);
}
  

     vTaskDelay(50);

     }
}
void ct_timeout (void *pvParameters)
{
  while (1){

     if (flag_frame_run_can==1)
      {
        ct_timeout_frame++;
        vTaskDelay(1);
      }
     else
      {
        ct_timeout_frame=0;
        vTaskSuspend( NULL );
      
      }

     

     }
}
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
//const int Serial @ 0x40048024;
uint32_t read_int_flash (char * addr)
  {
    uint32_t data=0;
  //  uint32_t* addr = (uint32_t*)&fw_sm;
    data |=*(addr);
    data |=(*(addr+1))<<8;
    data |=(*(addr+2))<<16;
    data |=(*(addr+3))<<24;
    return data;
  }

   



int main(void)
{


  //main_par();
    
    hard_config();
                // xTaskCreate( oled_diplay, "oled_diplay", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
               // xTaskCreate( BUS_controll, "BUS_controll", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );
    xTaskCreate( start_convert, "start_convert", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &xHandle_start_convert );
               // xTaskCreate( start_ethernet, "start_ethernet", 512, NULL, tskIDLE_PRIORITY+1, &xHandle_start_ethernet );
  //  xTaskCreate( reload_io, "reload_io", 512, NULL, tskIDLE_PRIORITY+1, &xHandle_reload_io );
 //   vTaskSuspend( xHandle_reload_io );
                   //  vTaskSuspend( xHandle_start_ethernet );
    
   // Start FREERTOS
  //   vSemaphoreCreateBinary( xBinarySemaphore_CAN );
////     xTaskCreate( ct_timeout, "ct_timeout", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &xHandle_timeout_frame );
////     vTaskSuspend( xHandle_timeout_frame );
    
  //  xTaskCreate( Trance, "Trance", 2*configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );
    xTaskCreate( led_blink, "led_blink", 256, NULL, tskIDLE_PRIORITY, NULL );
   //Can transmit app
 //   xTaskCreate( CAN_transmit, "CAN_transmit", 2000, NULL, tskIDLE_PRIORITY+2, NULL );
 //   xSemaphoreTake(xBinarySemaphore_CAN,portMAX_DELAY);
    // UART tramsmit 
 //   xTaskCreate( UART_transmit, "UART_transmit", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, &xHandle_tx_uart );
 //   vTaskSuspend( xHandle_tx_uart );

 
   // vTaskSuspend( xHandle_tx_udp );
    vTaskStartScheduler();

    /* Will not get here unless a task calls vTaskEndScheduler ()*/
    while (1){};
   
}
#endif
