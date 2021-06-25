#include "board.h"
#include "fsl_flexcan.h"

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
#include "pflash.h"
#include "task.h"
#include "hard_init.h"
#include "fsl_dspi.h"
#include "fsl_port.h"
#include "fsl_xbara.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "app.h"
uint8_t flag_global_save_data=0;
uint8_t flag_global_load_def=0;
uint8_t SPI0_masterRxData[all_modul+1] = {0U};
uint8_t SPI0_masterTxData[all_modul+1] = {0U};

uint8_t SPI1_masterRxData[64*all_modul] = {0U};
uint8_t SPI1_masterTxData[64*all_modul] = {0U};
//////uint8_t led_data[all_modul*4*16]={0};
//////uint8_t led_data_r[all_modul*4*4]={0};
//////uint8_t led_data_g[all_modul*4*4]={0};
//////uint8_t led_data_b[all_modul*4*4]={0};
//////uint8_t led_data_ir[all_modul*4*2]={0};
////=127-127*COS(4*??()*A1/16)

//__root const uint8_t call_r[all_modul*16]@a_call_data ;//= [0x1b];
//__root const uint8_t call_g[all_modul*16]@(a_call_data+1024);// = [0x1b];
//__root const uint8_t call_b[all_modul*16]@(a_call_data+2*1024);// = [0x1b];
//__root const uint8_t call_ir[all_modul*16]@(a_call_data+3*1024);// = [0x1b];


void  constr_addr (uint8_t* addr_mass, uint8_t modul_sel,uint8_t device_sel,uint8_t addr)
{  
  uint8_t i;
  for (i=0;i<all_modul;i++)
  {
      addr_mass[i]=0;
  }
  switch (device_sel)
   {
   case T_sensor:
    {
      for (i=0;i<all_modul;i++)
         {
           addr_mass[i]=0x07<<3;
          }
      addr_mass[all_modul-modul_sel-1] = (addr&0x0e)<<2;
    }
     break; 
     case Dac_chip:
    {
      
       addr_mass[all_modul-modul_sel-1] = 0x07;
    }
     break; 
     case mode_set:
    {
       for (i=0;i<modul_sel+1;i++)
         {
           addr_mass[i]=1<<0x07;
          }
      
    }
     break; 
     case xlat_set:
    {
       for (i=0;i<modul_sel;i++)
         {
           addr_mass[i]=1<<0x06;
          }
    }
     break; 
     case xlat_mode_set:
    {
       for (i=0;i<modul_sel;i++)
         {
           addr_mass[i]=3<<0x06;
          }
      
    }
    break;
          case clear:
    {
       for (i=0;i<all_modul+1;i++)
         {
           addr_mass[i]=0;
          }
    }
     break; 
  
   default:
           PRINTF("Error command");
           while(1){};
           break;
   }
}
void  set_adress_drv( uint8_t modul,uint8_t device_sel,uint8_t addr)
{

      constr_addr(SPI0_masterTxData,modul,device_sel,addr);
      isTransferCompleted0=false;
      SPI0_TX(SPI0_masterTxData,0,all_modul,0);
    
      while (isTransferCompleted0==false)
      {
       vTaskDelay(speed_grade);
      };
      
}
void  set_xlat_drv( void )
{
   GPIO_SetPinsOutput(BOARD_INITPINS_DRV_XLAT_GPIO,1<<BOARD_INITPINS_DRV_XLAT_PIN);
   vTaskDelay(10*speed_grade);
   GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_XLAT_GPIO,1<<BOARD_INITPINS_DRV_XLAT_PIN);
      
}


void  set_xlat_mode_drv( void )
{

       set_adress_drv(all_modul,mode_set,0);
       vTaskDelay(20);
       set_xlat_drv();
       vTaskDelay(20);
        set_adress_drv(all_modul,clear,0);
}

void  set_data_drv( uint8_t modul,uint8_t mode,uint8_t addr,uint8_t* data)
{
  //    uint16_t c;
     GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_XLAT_GPIO,1<<BOARD_INITPINS_DRV_XLAT_PIN);
       vTaskDelay(10*speed_grade);
      if (mode==mode_set)
      {
        GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
        vTaskDelay(10*speed_grade);
        set_adress_drv(modul,mode_set,0);
       
        isTransferCompleted1=false;
        if ((16*4*all_modul)>500)
        {
          SPI1_TX_7bit(data,0,16*2*all_modul,0);
    
        while (isTransferCompleted1==false)
          {
          // vTaskDelay(2*speed_grade);   
          }
        SPI1_TX_7bit((uint8_t*)(data+16*2*all_modul),0,16*2*all_modul,0);
    
        while (isTransferCompleted1==false)
          {
          // vTaskDelay(2*speed_grade);   
          }
        }
        else
        {
        SPI1_TX_7bit(data,0,16*4*all_modul,0);
    
        while (isTransferCompleted1==false)
          {
          // vTaskDelay(2*speed_grade);   
          }
        }
        set_xlat_mode_drv();
        set_adress_drv(modul,clear,0);
      }
      else
      {
       GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_XLAT_GPIO,1<<BOARD_INITPINS_DRV_XLAT_PIN);
        vTaskDelay(10*speed_grade);
        set_adress_drv(modul,clear,0); // mode in low
        vTaskDelay(10*speed_grade);
      
//////        for (c=0;c<4*2*all_modul;c++)
//////          {           
//////                   SPI1_masterTxData[c]=data[c];
//////                   
//////          }
      
        isTransferCompleted1=false;
        SPI1_TX(data,0,8*all_modul,0);

    
         while (isTransferCompleted1==false)
          {
           vTaskDelay(speed_grade);   
          }
         
       
            GPIO_SetPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
        vTaskDelay(10*speed_grade);
   
   
        
        set_xlat_drv();
        vTaskDelay(speed_grade);
        GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
         vTaskDelay(10*speed_grade);
      };
      
}


void load_dot_correct(uint8_t* data_mass_white,uint8_t* data_mass_g,uint8_t* data_mass_b,uint8_t* data_mass_ir)
{
uint32_t ct_modul=0;
uint16_t ct_drv=0;
uint8_t mass_drv[4]={0};
uint16_t max_cur = max_dot_curr_white;
//DAC_slow_swich(0);

for (ct_modul=0;ct_modul<64*all_modul;ct_modul++)
{
  SPI1_masterTxData[ct_modul]=0;
}
#if (lamp_sys)
for (ct_modul=0;ct_modul<all_modul;ct_modul++)
  {
 
    for (ct_drv=0;ct_drv<4;ct_drv++)
      {
        if (data_mass_white[ct_modul*16+ct_drv*4]<max_cur)
          {
             mass_drv[0]= data_mass_white[ct_modul*16+ct_drv*4];
          }
        else
          {
             mass_drv[0]= max_cur-1;
          }
        
         if (data_mass_white[ct_modul*16+ct_drv*4+1]<max_cur)
          {
             mass_drv[1]= data_mass_white[ct_modul*16+ct_drv*4+1];
          }
        else
          {
             mass_drv[1]= max_cur-1;
          }
        
         if (data_mass_white[ct_modul*16+ct_drv*4+2]<max_cur)
          {
             mass_drv[2]= data_mass_white[ct_modul*16+ct_drv*4+2];
          }
        else
          {
             mass_drv[2]= max_cur-1;
          }
        
         if (data_mass_white[ct_modul*16+ct_drv*4+3]<max_cur)
          {
             mass_drv[3]= data_mass_white[ct_modul*16+ct_drv*4+3];
          }
        else
          {
             mass_drv[3]= max_cur-1;
          }
        
        
       
       
        //remap_canal_lamp_white(mass_drv,ct_modul,ct_drv);  
        remap_canal_lamp_white(mass_drv,ct_modul,ct_drv,SPI1_masterTxData); 
      }
  }


max_cur = max_dot_curr_ir;
for (ct_modul=0;ct_modul<all_modul;ct_modul++)
  {
    for (ct_drv=0;ct_drv<4;ct_drv++)
      {
         if (data_mass_ir[ct_modul*16+ct_drv*4]<max_cur)
          {
             mass_drv[0]= data_mass_ir[ct_modul*16+ct_drv*4];
          }
        else
          {
             mass_drv[0]= max_cur-1;
          }
        
         if (data_mass_ir[ct_modul*16+ct_drv*4+1]<max_cur)
          {
             mass_drv[1]= data_mass_ir[ct_modul*16+ct_drv*4+1];
          }
        else
          {
             mass_drv[1]= max_cur-1;
          }
        

        //remap_canal_lamp_ir(mass_drv,ct_modul,ct_drv);  
        remap_canal_lamp_ir(mass_drv,ct_modul,ct_drv,SPI1_masterTxData); 
      }
  }

 
#else
max_cur = max_dot_curr_r;
for (ct_modul=0;ct_modul<all_modul;ct_modul++)
  {
 
    for (ct_drv=0;ct_drv<4;ct_drv++)
      {
        if (data_mass_white[ct_modul*16+ct_drv*4]<max_cur)
          {
             mass_drv[0]= data_mass_white[ct_modul*16+ct_drv*4];
          }
        else
          {
             mass_drv[0]= max_cur-1;
          }
        
         if (data_mass_white[ct_modul*16+ct_drv*4+1]<max_cur)
          {
             mass_drv[1]= data_mass_white[ct_modul*16+ct_drv*4+1];
          }
        else
          {
             mass_drv[1]= max_cur-1;
          }
        
         if (data_mass_white[ct_modul*16+ct_drv*4+2]<max_cur)
          {
             mass_drv[2]= data_mass_white[ct_modul*16+ct_drv*4+2];
          }
        else
          {
             mass_drv[2]= max_cur-1;
          }
        
         if (data_mass_white[ct_modul*16+ct_drv*4+3]<max_cur)
          {
             mass_drv[3]= data_mass_white[ct_modul*16+ct_drv*4+3];
          }
        else
          {
             mass_drv[3]= max_cur-1;
          }
        
        
       
       
        if (ct_modul==1)
         {
         if(ct_drv==0)
         {
           remap_canal_lamp_r(mass_drv,ct_modul,3,SPI1_masterTxData);   
         }
         if(ct_drv==1)
         {
           remap_canal_lamp_r(mass_drv,ct_modul,2,SPI1_masterTxData);   
         }
         if(ct_drv==2)
         {
           remap_canal_lamp_r(mass_drv,ct_modul,1,SPI1_masterTxData);   
         }
         if(ct_drv==3)
         {
           remap_canal_lamp_r(mass_drv,ct_modul,0,SPI1_masterTxData);   
         }
        
       }
       else
       {
        remap_canal_lamp_r(mass_drv,ct_modul,ct_drv,SPI1_masterTxData);  
       }
      }
  }


max_cur = max_dot_curr_g;
for (ct_modul=0;ct_modul<all_modul;ct_modul++)
  {
 
    for (ct_drv=0;ct_drv<4;ct_drv++)
      {
        if (data_mass_g[ct_modul*16+ct_drv*4]<max_cur)
          {
             mass_drv[0]= data_mass_g[ct_modul*16+ct_drv*4];
          }
        else
          {
             mass_drv[0]= max_cur-1;
          }
        
         if (data_mass_g[ct_modul*16+ct_drv*4+1]<max_cur)
          {
             mass_drv[1]= data_mass_g[ct_modul*16+ct_drv*4+1];
          }
        else
          {
             mass_drv[1]= max_cur-1;
          }
        
         if (data_mass_g[ct_modul*16+ct_drv*4+2]<max_cur)
          {
             mass_drv[2]= data_mass_g[ct_modul*16+ct_drv*4+2];
          }
        else
          {
             mass_drv[2]= max_cur-1;
          }
        
         if (data_mass_g[ct_modul*16+ct_drv*4+3]<max_cur)
          {
             mass_drv[3]= data_mass_g[ct_modul*16+ct_drv*4+3];
          }
        else
          {
             mass_drv[3]= max_cur-1;
          }
        
        
       
if (ct_modul==1)
         {
         if(ct_drv==0)
         {
           remap_canal_lamp_g(mass_drv,ct_modul,3,SPI1_masterTxData);   
         }
         if(ct_drv==1)
         {
           remap_canal_lamp_g(mass_drv,ct_modul,2,SPI1_masterTxData);   
         }
         if(ct_drv==2)
         {
           remap_canal_lamp_g(mass_drv,ct_modul,1,SPI1_masterTxData);   
         }
         if(ct_drv==3)
         {
           remap_canal_lamp_g(mass_drv,ct_modul,0,SPI1_masterTxData);   
         }
        
       }
       else
       {
        remap_canal_lamp_g(mass_drv,ct_modul,ct_drv,SPI1_masterTxData);  
       }
      }
  }



max_cur = max_dot_curr_b;
for (ct_modul=0;ct_modul<all_modul;ct_modul++)
  {
 
    for (ct_drv=0;ct_drv<4;ct_drv++)
      {
        if (data_mass_b[ct_modul*16+ct_drv*4]<max_cur)
          {
             mass_drv[0]= data_mass_b[ct_modul*16+ct_drv*4];
          }
        else
          {
             mass_drv[0]= max_cur-1;
          }
        
         if (data_mass_b[ct_modul*16+ct_drv*4+1]<max_cur)
          {
             mass_drv[1]= data_mass_b[ct_modul*16+ct_drv*4+1];
          }
        else
          {
             mass_drv[1]= max_cur-1;
          }
        
         if (data_mass_b[ct_modul*16+ct_drv*4+2]<max_cur)
          {
             mass_drv[2]= data_mass_b[ct_modul*16+ct_drv*4+2];
          }
        else
          {
             mass_drv[2]= max_cur-1;
          }
        
         if (data_mass_b[ct_modul*16+ct_drv*4+3]<max_cur)
          {
             mass_drv[3]= data_mass_b[ct_modul*16+ct_drv*4+3];
          }
        else
          {
             mass_drv[3]= max_cur-1;
          }
        
        
       
       
       if (ct_modul==1)
         {
         if(ct_drv==0)
         {
           remap_canal_lamp_b(mass_drv,ct_modul,3,SPI1_masterTxData);   
         }
         if(ct_drv==1)
         {
           remap_canal_lamp_b(mass_drv,ct_modul,2,SPI1_masterTxData);   
         }
         if(ct_drv==2)
         {
           remap_canal_lamp_b(mass_drv,ct_modul,1,SPI1_masterTxData);   
         }
         if(ct_drv==3)
         {
           remap_canal_lamp_b(mass_drv,ct_modul,0,SPI1_masterTxData);   
         }
        
       }
       else
       {
        remap_canal_lamp_b(mass_drv,ct_modul,ct_drv,SPI1_masterTxData);  
       }
      }
  }



max_cur = max_dot_curr_irf;
for (ct_modul=0;ct_modul<all_modul;ct_modul++)
  {
 
    for (ct_drv=0;ct_drv<4;ct_drv++)
      {
        if (data_mass_ir[ct_modul*16+ct_drv*4]<max_cur)
          {
             mass_drv[0]= data_mass_ir[ct_modul*16+ct_drv*4];
          }
        else
          {
             mass_drv[0]= max_cur-1;
          }
        
         if (data_mass_ir[ct_modul*16+ct_drv*4+1]<max_cur)
          {
             mass_drv[1]= data_mass_ir[ct_modul*16+ct_drv*4+1];
          }
        else
          {
             mass_drv[1]= max_cur-1;
          }
        
         if (data_mass_ir[ct_modul*16+ct_drv*4+2]<max_cur)
          {
             mass_drv[2]= data_mass_ir[ct_modul*16+ct_drv*4+2];
          }
        else
          {
             mass_drv[2]= max_cur-1;
          }
        
         if (data_mass_ir[ct_modul*16+ct_drv*4+3]<max_cur)
          {
             mass_drv[3]= data_mass_ir[ct_modul*16+ct_drv*4+3];
          }
        else
          {
             mass_drv[3]= max_cur-1;
          }
        
        
       
       
       if (ct_modul==1)
         {
         if(ct_drv==0)
         {
           remap_canal_lamp_irf(mass_drv,ct_modul,3,SPI1_masterTxData);   
         }
         if(ct_drv==1)
         {
           remap_canal_lamp_irf(mass_drv,ct_modul,2,SPI1_masterTxData);   
         }
         if(ct_drv==2)
         {
           remap_canal_lamp_irf(mass_drv,ct_modul,1,SPI1_masterTxData);   
         }
         if(ct_drv==3)
         {
           remap_canal_lamp_irf(mass_drv,ct_modul,0,SPI1_masterTxData);   
         }
        
       }
       else
       {
        remap_canal_lamp_irf(mass_drv,ct_modul,ct_drv,SPI1_masterTxData);  
       }
      }
  }



#endif
set_data_drv(all_modul,mode_set,0,SPI1_masterTxData);
//RSet_lamp_power();
}
// wh1 - out2_3
// wh2 - out4_5
// wh3 - out8_9
// wh4 - out10_11

 void remap_canal_lamp_white (uint8_t* data_mass,uint16_t modul,uint8_t addr,uint8_t* TxData)
  {                  
            
     //   SPI1_masterTxData[all_modul*64-modul*64-16*addr-16+0]=0x00;          //15
     //   SPI1_masterTxData[all_modul*64-modul*64-16*addr-16+1]=0x00;          //14
        
     //   SPI1_masterTxData[all_modul*64-modul*64-16*addr-16+2]= data_mass[5];   //13
     //   SPI1_masterTxData[all_modul*64-modul*64-16*addr-16+3]= data_mass[5];  //12
        
        TxData[all_modul*64-modul*64-16*addr-16+4]= data_mass[3];   //11
        TxData[all_modul*64-modul*64-16*addr-16+5]= data_mass[3];  //10
        
       
        
        
        TxData[all_modul*64-modul*64-16*addr-16+6]= data_mass[2];   //9
        TxData[all_modul*64-modul*64-16*addr-16+7]=data_mass[2];    //8
         
    //    SPI1_masterTxData[all_modul*64-modul*64-16*addr-16+8]= 0x00;          //7
    //    SPI1_masterTxData[all_modul*64-modul*64-16*addr-16+9]= data_mass[4];  //6
    
        TxData[all_modul*64-modul*64-16*addr-16+10]= data_mass[1];  //5
        TxData[all_modul*64-modul*64-16*addr-16+11]= data_mass[1];  //4
            
        TxData[all_modul*64-modul*64-16*addr-16+12]= data_mass[0];  //3
        TxData[all_modul*64-modul*64-16*addr-16+13]= data_mass[0];  //2
        
   //     SPI1_masterTxData[all_modul*64-modul*64-16*addr-16+14]=0x00; //1
     //   SPI1_masterTxData[all_modul*64-modul*64-16*addr-16+15]=0x00;  //0
              

       
  }




// ir1 - pin 4 - out 13
// ir2 - pin 2 - out 7
// ir3 - pin 1 - out 15
// ir4 - pin 24 - out 3
  

  void remap_canal_lamp_irf (uint8_t* data_mass,uint16_t modul,uint8_t addr,uint8_t* TxData )
  {                  
            
////        SPI1_masterTxData[all_modul*64-modul*64-16*addr-16+0]=data_mass[2];          //15
////        
////        
////        
////        SPI1_masterTxData[all_modul*64-modul*64-16*addr-16+2]= data_mass[0];   //13
////        
////
////         
////        SPI1_masterTxData[all_modul*64-modul*64-16*addr-16+8]= data_mass[1];          //7
////        
////        
////
////            
////        SPI1_masterTxData[all_modul*64-modul*64-16*addr-16+12]= data_mass[3];  //3
        

        TxData[all_modul*64-modul*64-16*addr-16+0]=data_mass[2];          //15
        
        
        
        TxData[all_modul*64-modul*64-16*addr-16+2]= data_mass[0];   //13
        

         
        TxData[all_modul*64-modul*64-16*addr-16+8]= data_mass[1];          //7
        
        

            
        TxData[all_modul*64-modul*64-16*addr-16+12]= data_mass[3];  //3
  
              

       
  }
  
//  g1 - pin 31 - out 1
//  g2 - pin 20 - out 5
//  g3 - pin 12 - out 9
//  g4 - pin 6 - out 12
  

void remap_canal_lamp_g (uint8_t* data_mass,uint16_t modul,uint8_t addr,uint8_t* TxData)
  {                  
                
    
        TxData[all_modul*64-modul*64-16*addr-16+3]= data_mass[3];  //12
              
        
       TxData[all_modul*64-modul*64-16*addr-16+6]= data_mass[2];   //9

    
        TxData[all_modul*64-modul*64-16*addr-16+10]= data_mass[1];  //5

        
        TxData[all_modul*64-modul*64-16*addr-16+14]=data_mass[0]; //1

              

       
  }
//  b1 - pin 26 - out 2
//  b2 - pin 18 - out 6
//  b3 - pin 10 - out 10
//  b4 - pin 3 - out 14
void remap_canal_lamp_b (uint8_t* data_mass,uint16_t modul,uint8_t addr,uint8_t* TxData)
  {                  
            
  
        TxData[all_modul*64-modul*64-16*addr-16+1]=data_mass[3];    //14
        

        TxData[all_modul*64-modul*64-16*addr-16+5]= data_mass[2];  //10
        

        TxData[all_modul*64-modul*64-16*addr-16+9]= data_mass[1];  //6
    

        TxData[all_modul*64-modul*64-16*addr-16+13]= data_mass[0];  //2
        
              

       
  }

//  r1 - pin 33 - out 0
//  r2 - pin 22 - out 4
//  r3 - pin 14 - out 8
//  r4 - pin 8  - out 11
void remap_canal_lamp_r (uint8_t* data_mass,uint16_t modul,uint8_t addr,uint8_t* TxData)
  {                  
            
  
        
        TxData[all_modul*64-modul*64-16*addr-16+4]= data_mass[3];   //11
    
       TxData[all_modul*64-modul*64-16*addr-16+7]=data_mass[2];    //8
         
       
        TxData[all_modul*64-modul*64-16*addr-16+11]= data_mass[1];  //4
            
    
        TxData[all_modul*64-modul*64-16*addr-16+15]=data_mass[0];  //0
              


  }




 // ir1 - out6
// ir2 - out12_13

void remap_canal_lamp_ir (uint8_t* data_mass,uint16_t modul,uint8_t addr,uint8_t* TxData)
  {                  
            
  
        
        TxData[all_modul*64-modul*64-16*addr-16+2]= data_mass[1];   //13
        TxData[all_modul*64-modul*64-16*addr-16+3]= data_mass[1];  //12
        
     
        TxData[all_modul*64-modul*64-16*addr-16+9]= data_mass[0];  //6
    
      

       
  }
//func for set up and  set down power led,  read Term sensor 
void DAC_slow_swich (uint16_t power)
  {
        uint16_t ct_mod;
        signed int  ct_mass;
        
     ///   lamp_state.lamp_power_req=power;
  if (power>max_power_dac)
    {
     power=max_power_dac;
    }
    for (ct_mod=0;ct_mod<all_modul;ct_mod++)
    {
        //vTaskDelay(speed_grade);
        set_adress_drv(ct_mod,T_sensor,addr_term);
        vTaskDelay(speed_grade);
        g_master_txBuff[0]=0;
        I2C_run(kI2C_Write,g_master_txBuff,addr_term,0,1);
        //vTaskDelay(speed_grade);
        I2C_run(kI2C_Read,g_master_txBuff,addr_term,0,2);
        lamp_state.modul_state[ct_mod].temp= g_master_txBuff[0];

    }   
        
        
 if (lamp_state.lamp_power!=power)
 {
  if (lamp_state.lamp_power<power)
    {    
        
     for (ct_mass=lamp_state.lamp_power;ct_mass<power;ct_mass=ct_mass+0xff)
     {
        for (ct_mod=0;ct_mod<all_modul;ct_mod=ct_mod+1)
          {
           //  vTaskDelay(speed_grade);
            set_adress_drv(ct_mod,Dac_chip,addr_dac);
            vTaskDelay(speed_grade);
            g_master_txBuff[0]=((max_power_dac-ct_mass)&0xff00)>>8;
            g_master_txBuff[1]=((max_power_dac-ct_mass)&0x00ff);
            Dac_run(kI2C_Write,g_master_txBuff,addr_dac,0x2f,2);
            
          }    
      //  vTaskDelay(speed_grade);
    }
  }
  else
  {
     for (ct_mass=lamp_state.lamp_power;ct_mass>power;ct_mass=ct_mass-0xff)
     {
       for (ct_mod=0;ct_mod<all_modul;ct_mod=ct_mod-1)
         {
         // vTaskDelay(speed_grade);
          set_adress_drv(ct_mod,Dac_chip,addr_dac);
          vTaskDelay(speed_grade);
          g_master_txBuff[0]=((max_power_dac-ct_mass)&0xff00)>>8;
          g_master_txBuff[1]=((max_power_dac-ct_mass)&0x00ff);
          Dac_run(kI2C_Write,g_master_txBuff,addr_dac,0x2f,2);
            
         }
     //  vTaskDelay(speed_grade*3);   
    }
    
  }
    lamp_state.lamp_power=power;
 }
  }

void Set_lamp_power (uint16_t power)
{
  lamp_state.lamp_power_req=power;
   DAC_slow_swich(lamp_state.lamp_power_req);
}
void RSet_lamp_power (void)
{
   if (Hi_DCDC.start_flag==1)
      {

         DAC_slow_swich(100);      
         vTaskDelay(speed_grade);
         set_data_drv(all_modul,mode_clear,0,lamp_state.led_onoff); 
         vTaskDelay(speed_grade);
         GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
          DAC_slow_swich(lamp_state.lamp_power_req);
      
         vTaskDelay(speed_grade);
         Hi_DCDC.stop_flag=0;
         Hi_DCDC.start_flag=0;
       
      }
      vTaskDelay(speed_grade);
   
   
    if (Hi_DCDC.stop_flag==1)
      {
         vTaskDelay(speed_grade);
         DAC_slow_swich(100);   
         GPIO_SetPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
         vTaskDelay(speed_grade);
         Hi_DCDC.stop_flag=0;
         Hi_DCDC.start_flag=0;
      }
    vTaskDelay(speed_grade);
    
    
    if (lamp_state.reload_dot==1)
    {
       DAC_slow_swich(100);
     // vTaskDelay(speed_grade);
      GPIO_SetPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
      vTaskDelay(speed_grade);
      Hi_DCDC.start_flag=0;   
     load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir);//((uint8_t*)data,(uint8_t*)(data+all_modul*16),(uint8_t*)(data+all_modul*32),(uint8_t*)data+all_modul*48);
     lamp_state.reload_dot=0;
     GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
    // vTaskDelay(speed_grade);
     Hi_DCDC.stop_flag=0;
   //  lamp_state.lamp_power_req=32000;
      DAC_slow_swich(lamp_state.lamp_power_req);
      vTaskDelay(speed_grade);
   
    }
      
      DAC_slow_swich(lamp_state.lamp_power_req);
      vTaskDelay(speed_grade);
     
    if (lamp_state.onoff_pix==1)
    {
    lamp_state.onoff_pix=0;
    set_data_drv(all_modul,mode_clear,0,lamp_state.led_onoff);    
    }
      vTaskDelay(speed_grade);
    
}
void lamp_init (uint16_t power)
{
   uint32_t ct_mass;
    lamp_state.lamp_power=32768;
  
    vTaskDelay(10*speed_grade);
   DAC_slow_swich(power_lamp_0);  
    vTaskDelay(speed_grade);
 GPIO_SetPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
 
  lamp_state.lamp_power_req=power;
   for (ct_mass=0;ct_mass<all_modul*4*4;ct_mass++)
         {
           lamp_state.led_data_r[ct_mass]=0xff;
           lamp_state.led_data_g[ct_mass]=0xff;
           lamp_state.led_data_b[ct_mass]=0xff;
           lamp_state.led_data_ir[ct_mass]=0xff;
          }
     lamp_state.led_data_r_req=0xff;
     lamp_state.led_data_g_req=0xff;
     lamp_state.led_data_b_req=0xff;
     lamp_state.led_data_ir_req=0xff;
    load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir);
  
   
    for (ct_mass=0;ct_mass<all_modul*2*4;ct_mass++)
        {
          lamp_state.led_onoff[ct_mass]=0xff;
        }
   set_data_drv(all_modul,mode_clear,0,lamp_state.led_onoff);    
  // vTaskDelay(500);
   DAC_slow_swich(power);  
   GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
   vTaskDelay(100);
 //  vTaskDelay(500);
 //  lamp_state.lamp_power_req=power_lamp_10;
   
   // Hi_DCDC.start_flag=1;
}
void move_dot(uint8_t sost)
{
     uint32_t ct_mass;
 for (ct_mass=0;ct_mass<all_modul*2*4;ct_mass++)
        {
          lamp_state.led_onoff[ct_mass]=sost;
        }
   set_data_drv(all_modul,mode_clear,0,lamp_state.led_onoff);    
}
void stoboskop (void)
{
      uint32_t ct_mass;
 Set_lamp_power(power_lamp_10);
         for (ct_mass=all_modul*4;ct_mass<all_modul*16;ct_mass++)
          {           
           lamp_state.led_data_r[ct_mass]=0x7f;
          }
     
           load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir);
         
         for (ct_mass=0;ct_mass<all_modul;ct_mass++)
          {           
           GPIO_SetPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dy*speed_grade);
           GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dp*speed_grade);
          }
         
       //     vTaskDelay(25*speed_grade);
       
            
            
        for (ct_mass=0;ct_mass<all_modul*4;ct_mass++)
          {
           
           lamp_state.led_data_g[ct_mass]=0x7f;


          }
         
            load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir); 
           
          for (ct_mass=0;ct_mass<all_modul;ct_mass++)
          {           
           GPIO_SetPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dy*speed_grade);
           GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dp*speed_grade);
          }
        
        
        for (ct_mass=all_modul*4;ct_mass<all_modul*8;ct_mass++)
          {
           
           lamp_state.led_data_b[ct_mass]=0x7f;

           
;
          }
    
    load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir);
  ///   vTaskDelay(255*speed_grade);
      for (ct_mass=0;ct_mass<all_modul;ct_mass++)
          {           
           GPIO_SetPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dy*speed_grade);
           GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dp*speed_grade);
          }
      
     for (ct_mass=0;ct_mass<all_modul*16;ct_mass++)
          {
           
           lamp_state.led_data_b[ct_mass]=0x00;
            lamp_state.led_data_r[ct_mass]=0x00;
             lamp_state.led_data_g[ct_mass]=0x00;
              lamp_state.led_data_ir[ct_mass]=0x00;
          }
           load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir);
       
         for (ct_mass=all_modul*4;ct_mass<all_modul*16;ct_mass++)
          {           
           lamp_state.led_data_b[ct_mass]=0x7f;
          }
     
           load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir);
         
           for (ct_mass=0;ct_mass<all_modul;ct_mass++)
          {           
           GPIO_SetPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dy*speed_grade);
           GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dp*speed_grade);
          }
        //    vTaskDelay(25*speed_grade);
       
            
            
        for (ct_mass=0;ct_mass<all_modul*4;ct_mass++)
          {
           
           lamp_state.led_data_r[ct_mass]=0x7f;


          }
         
            load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir); 
           
          for (ct_mass=0;ct_mass<all_modul;ct_mass++)
          {           
           GPIO_SetPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dy*speed_grade);
           GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dp*speed_grade);
          }
        
        
        for (ct_mass=all_modul*4;ct_mass<all_modul*8;ct_mass++)
          {
           
           lamp_state.led_data_g[ct_mass]=0x7f;

           
;
          }
    
    load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir);
   //  vTaskDelay(255*speed_grade);
      for (ct_mass=0;ct_mass<all_modul;ct_mass++)
          {           
           GPIO_SetPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dy*speed_grade);
           GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dp*speed_grade);
          }
     for (ct_mass=0;ct_mass<all_modul*16;ct_mass++)
          {
           
           lamp_state.led_data_b[ct_mass]=0x00;
            lamp_state.led_data_r[ct_mass]=0x00;
             lamp_state.led_data_g[ct_mass]=0x00;
              lamp_state.led_data_ir[ct_mass]=0x00;
          }
           load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir);
     
       for (ct_mass=all_modul*4;ct_mass<all_modul*16;ct_mass++)
          {           
           lamp_state.led_data_g[ct_mass]=0x7f;
          }
     
           load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir);
         
           for (ct_mass=0;ct_mass<all_modul;ct_mass++)
          {           
           GPIO_SetPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dy*speed_grade);
           GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dp*speed_grade);
          }
         //   vTaskDelay(25*speed_grade);
       
            
            
        for (ct_mass=0;ct_mass<all_modul*4;ct_mass++)
          {
           
           lamp_state.led_data_b[ct_mass]=0x7f;


          }
         
            load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir); 
           
          for (ct_mass=0;ct_mass<all_modul;ct_mass++)
          {           
           GPIO_SetPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dy*speed_grade);
           GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dp*speed_grade);
          }
        
        
        for (ct_mass=all_modul*4;ct_mass<all_modul*8;ct_mass++)
          {
           
           lamp_state.led_data_r[ct_mass]=0x7f;

           
;
          }
    
    load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir);
 //    vTaskDelay(255*speed_grade);
      for (ct_mass=0;ct_mass<all_modul;ct_mass++)
          {           
           GPIO_SetPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dy*speed_grade);
           GPIO_ClearPinsOutput(BOARD_INITPINS_DRV_OFF_GPIO,1<<BOARD_INITPINS_DRV_OFF_PIN);
           vTaskDelay(dp*speed_grade);
          }
     for (ct_mass=0;ct_mass<all_modul*16;ct_mass++)
          {
           
           lamp_state.led_data_b[ct_mass]=0x00;
            lamp_state.led_data_r[ct_mass]=0x00;
             lamp_state.led_data_g[ct_mass]=0x00;
              lamp_state.led_data_ir[ct_mass]=0x00;
          }
           load_dot_correct(lamp_state.led_data_r,lamp_state.led_data_g,lamp_state.led_data_b,lamp_state.led_data_ir);
}