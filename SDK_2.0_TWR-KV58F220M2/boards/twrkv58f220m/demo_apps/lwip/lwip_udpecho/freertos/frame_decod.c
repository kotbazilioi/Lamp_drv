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

////
////__root const char  FW1_CRC[32] @ FW1_CRC_A={0,0,0,0,0,0,0,0
////                                          ,0,0,0,0,0,0,0,0
////                                          ,0,0,0,0,0,0,0,0
////                                          ,0,0,0,0,0,0,0,0};

__root const char FW1_VER[32] @ FW1_VER_A ={0,0,0,0,0,0,0,0
                                          ,0,0,0,0,0,0,0,1
                                          ,0,0,0,0,0,0,0,0
                                          ,0,0,0,0,0,0,0,0};

//////__root const char FW1_LEN[32] @ FW1_LEN_A={0xff,0x1f,0,0,0,0,0,0
//////                                          ,0,0,0,0,0,0,0,0
//////                                          ,0,0,0,0,0,0,0,0
//////                                          ,0,0,0,0,0,0,0,0};
//////
//////
//////__root const char  FW2_CRC[32] @ FW2_CRC_A={0,0,0,0,0,0,0,0
//////                                          ,0,0,0,0,0,0,0,0
//////                                          ,0,0,0,0,0,0,0,0
//////                                          ,0,0,0,0,0,0,0,0};
//////
//////__root const char FW2_VER[32] @ FW2_VER_A={0,0,0,0,0,0,0,0
//////                                          ,0,0,0,0,0,0,0,0
//////                                          ,0,0,0,0,0,0,0,0
//////                                          ,0,0,0,0,0,0,0,0};
//////
//////__root const char  FW2_LEN[32] @ FW2_LEN_A ={0,0,0,0,0,0,0,0
//////                                          ,0,0,0,0,0,0,0,0
//////                                          ,0,0,0,0,0,0,0,0
//////                                          ,0,0,0,0,0,0,0,0};




////__root const char FW_LOADER_VER[32] @ FW_LOADER_VER_A={0,0,0,01,04,17,0,0
////                                          ,0,0,0,0,0,0,0,0
////                                          ,0,0,0,0,0,0,0,0
////                                          ,0,0,0,0,0,0,0,0};
////__root const char IP_config[32] @ IP_CONFIG_A={192,168,0,102
//                                                 ,0x21,0,0,0
////                                               ,0x22,0,0,0
//                                                 ,0,0,0,0
////                                               ,0,0,0,0
//                                                 ,0,0,0,0
////                                               ,0,0,0,0
//                                                 ,0,0,0,0};


/*
__root const char FW1_data[393216]@ FW1;
__root const char FW2_data[393216]@ FW2;*/
CRC_Type *base = CRC0;
int frame_ct_udp=0;
int frame_ct_can=0;
bool flag_frame_run_udp=0;
bool flag_frame_run=0;
bool flag_frame_run_can=0;
uint32_t ct_timeout_frame=0;
SCommands_t frame;
int number_frame=0;
//uint32_t sector_buffer[2048];
xTaskHandle xHandle_tx_udp;
///int begin_b_store ;
int data_size_store ;
int len_can_frame = 1037;
int frame_run(int id, int data_size, uint32_t* data,int begin_b,int len,int *out_len_data, uint8_t* data_tx_out);



/*!
 * @brief Init for CRC-16/MAXIM.
 * @details Init CRC peripheral module for CRC-16/MAXIM protocol.
 *          width=16 poly=0x8005 init=0x0000 refin=true refout=true xorout=0xffff check=0x44c2 name="CRC-16/MAXIM"
 *          http://reveng.sourceforge.net/crc-catalogue/
 */
void InitCrc16(CRC_Type *base, uint32_t seed)
{
    crc_config_t config;

    config.polynomial = 0x8005;
    config.seed = seed;
    config.reflectIn = true;
    config.reflectOut = true;
    config.complementChecksum = true;
    config.crcBits = kCrcBits16;
    config.crcResult = kCrcFinalChecksum;

    CRC_Init(base, &config);
}

 void InitCrc32(CRC_Type *base, uint32_t seed)
{
    crc_config_t config;

    config.polynomial = 0x04C11DB7U;
    config.seed = seed;
    config.reflectIn = true;
    config.reflectOut = true;
    config.complementChecksum = false;
    config.crcBits = kCrcBits32;
    config.crcResult = kCrcFinalChecksum;

    CRC_Init(base, &config);
}

int crc_compar32 (uint32_t testData,uint32_t* len,uint32_t* checkCrc32)
  {
    int checksum32;
    InitCrc32(base, 0xFFFFFFFFU);
    CRC_WriteData(base, (uint8_t *)testData, *len);
    checksum32 = CRC_Get32bitResult(base);


    if ((checksum32 == *checkCrc32)||((*checkCrc32)==0))
    {

        return 0;
    }
    else
    {
      PRINTF("...Check fail. Expected: 0x%x\r\n", *checkCrc32);
      return 1;
    }

  }
/*!
 * @brief Init for CRC-16/KERMIT.
 * @details Init CRC peripheral module for CRC-16/KERMIT protocol.
 *          width=16 poly=0x1021 init=0x0000 refin=true refout=true xorout=0x0000 check=0x2189 name="KERMIT"
 *          http://reveng.sourceforge.net/crc-catalogue/
 */
void InitCrc16_Kermit(CRC_Type *base, uint32_t seed)
{
    crc_config_t config;

    config.polynomial = 0x8005;
    config.seed = seed;
    config.reflectIn = true;
    config.reflectOut = true;
    config.complementChecksum = false;
    config.crcBits = kCrcBits16;
    config.crcResult = kCrcFinalChecksum;

    CRC_Init(base, &config);
}

void crc_init (void)
{


     InitCrc16(base, 0xFFFFU);
}
status_t crc_compar (char *mass,int in_crc, int len_mass)
  {

//    CRC_Type *base = CRC0;
    uint16_t checksum16;
   InitCrc16_Kermit(base, 0xFFFFU);
    CRC_WriteData(base, (const uint8_t *)mass, len_mass);//sizeof(mass) - 3);
    checksum16 = CRC_Get16bitResult(base);
  //   checksum16 = crc16( mass, 1408);
    if (checksum16 != in_crc)
    {
//////      PRINTF("CRC-16 CCIT FALSE: 0x%x\r\n", checksum16);
//////      PRINTF("...Check fail. Expected: 0x%x\r\n", in_crc);

        return 1;
    }
    else
    {

        return 0;
    }
  }
////int   crc_fw1_compar(void)
////{
////
////////  if (crc_compar32(FW1,(uint32_t *)FW1_LEN_A,(uint32_t *)FW1_CRC_A))
////////  {
////////    PRINTF("Error prog FW1 CRC Compar  \r\n"); //Ошибка
////////    return 1;
////////  }
////////  else
////////    {
////////      return 0;
////////    }
////
////};
//data_coding_frame (SPI0_masterTxData ,id,SPI0_SIZE-13,data,begin_b,SPI0_SIZE);
 void data_coding_frame (uint8_t * frame ,int id, int data_size, uint8_t* data,int begin_b,int len)
 {
   /* frame.id=mass[1]|(mass[0]<<8);
    frame.len=mass[4]|(mass[3]<<8)|(mass[2]<<16);
    frame.begin_b=mass[7]|(mass[6]<<8)|(mass[5]<<16);
    frame.data_size=mass[10]|(mass[9]<<8)|(mass[8]<<16);
    frame.data = (uint32_t*)&mass[11];
    frame.crc = mass[len-1]|(mass[len-2]<<8);*/
   int ct_for_datareload;
   frame[1] = 0x000000ff&id;
   frame[0] = (0x0000ff00&id)>>8;

   frame[4] = 0x000000ff&len;
   frame[3] = (0x0000ff00&len)>>8;
   frame[2] = (0x00ff0000&len)>>16;

   frame[7] = 0x000000ff&begin_b;
   frame[6] = (0x0000ff00&begin_b)>>8;
   frame[5] = (0x00ff0000&begin_b)>>16;

   frame[10] = 0x000000ff&data_size;
   frame[9] = (0x0000ff00&data_size)>>8;
   frame[8] = (0x00ff0000&data_size)>>16;

   for (ct_for_datareload=0;ct_for_datareload<data_size;ct_for_datareload++)
    {
      frame[11+ct_for_datareload]=*(data+ct_for_datareload);
    }
   uint16_t checksum16;
   InitCrc16_Kermit(base, 0xFFFFU);
   CRC_WriteData(base, (const uint8_t *)frame, data_size+11);//sizeof(mass) - 3);
   checksum16 = CRC_Get16bitResult(base);
   frame[data_size+12] = 0x000000ff&checksum16;
   frame[data_size+11] = (0x0000ff00&checksum16)>>8;
 }
  int out_len_tx_spi=0;
int decoding_frame_spi (char *mass, unsigned int len)
{
//int len_data_of_frame=0;
uint8_t status_command=0;


      frame.id=mass[1]|(mass[0]<<8);
      frame.len=mass[4]|(mass[3]<<8)|(mass[2]<<16);
      frame.begin_b=mass[7]|(mass[6]<<8)|(mass[5]<<16);
      frame.data_size=mass[10]|(mass[9]<<8)|(mass[8]<<16);
      frame.data = (uint32_t*)&mass[11];
      frame.crc = mass[frame.data_size+12]|(mass[frame.data_size+11]<<8);
        if (frame.begin_b==0)
            {
              flag_frame_run_udp=1;
              flag_frame_run = 1;
            }
        if (crc_compar(mass,frame.crc,frame.data_size+11))
            {
              status_command = 1;
             

            }
        else
            {
             status_command= mass[11];

            }
       return status_command;     //
}

     uint32_t ct_can_remap =0;
 uint8_t frame_can_buf[256]={0};
 int out_len_tx=0;
int decoding_frame (char *mass, unsigned int len,unsigned char type_interface,uint8_t *data_tx,uint32_t * out_len)
{
//int len_data_of_frame=0;
uint8_t status_command=0;


if (type_interface==0)
    {
      frame.id=mass[1]|(mass[0]<<8);
      frame.len=mass[4]|(mass[3]<<8)|(mass[2]<<16);
      frame.begin_b=mass[7]|(mass[6]<<8)|(mass[5]<<16);
      frame.data_size=mass[10]|(mass[9]<<8)|(mass[8]<<16);
      frame.data = (uint32_t*)&mass[11];
      frame.crc = mass[len-1]|(mass[len-2]<<8);
        if (frame.begin_b==0)
            {
              flag_frame_run_udp=1;
              flag_frame_run = 1;
            }
        if (crc_compar(mass,frame.crc,len-2))
            {
              status_command = 1;
              frame.data[0]=status_command;
              *out_len=13+1;
              data_coding_frame (data_tx,frame.id,1,(uint8_t*)frame.data,frame.begin_b,1);
           //   netconn_send(conn, &data_tx);
            }
        else
            {
             status_command= frame_run(frame.id,frame.data_size,frame.data,frame.begin_b,frame.len,&out_len_tx,data_tx);
             uint8_t ct_for;
              for(ct_for=0;ct_for<out_len_tx;ct_for++)
              {
                frame.data[ct_for]=data_tx[ct_for];
              }
              
              *out_len=out_len_tx+13;
              data_coding_frame (data_tx,frame.id,out_len_tx,(uint8_t*)frame.data,frame.begin_b,out_len_tx);
            }
       return status_command;     //

    }


  if (type_interface==1)
     {
       
       if ((flag_frame_run_can==0)||(ct_timeout_frame>100))
          {
            ct_timeout_frame=0;
            frame_ct_can=0;
            flag_frame_run_can=1;        
            vTaskResume(xHandle_timeout_frame);
            ct_timeout_frame=0;
             // portEND_SWITCHING_ISR(xHandle_timeout_frame);
           
          }
       else
          {
            frame_ct_can=frame_ct_can+8;
            if (frame_ct_can==8)
              {
                len_can_frame =13+ mass[2]|(mass[1]<<8)|(mass[0]<<16);
              
               
              }
            if ((frame_ct_can+8)>=len_can_frame)
            {
              frame_ct_can=len_can_frame;
            }
            
          }
       
  
                
       if (frame_ct_can<len_can_frame)
        {
          
            frame_can_buf[0+frame_ct_can]=mass[0];
            frame_can_buf[1+frame_ct_can]=mass[1];
            frame_can_buf[2+frame_ct_can]=mass[2];
            frame_can_buf[3+frame_ct_can]=mass[3];
            frame_can_buf[4+frame_ct_can]=mass[4];
            frame_can_buf[5+frame_ct_can]=mass[5];
            frame_can_buf[6+frame_ct_can]=mass[6];
            frame_can_buf[7+frame_ct_can]=mass[7];
        }
       if (frame_ct_can==len_can_frame)
       {
         if ((len_can_frame%8)==0)
         {
            frame_can_buf[0+frame_ct_can]=mass[0];
            frame_can_buf[1+frame_ct_can]=mass[1];
            frame_can_buf[2+frame_ct_can]=mass[2];
            frame_can_buf[3+frame_ct_can]=mass[3];
            frame_can_buf[4+frame_ct_can]=mass[4];
            frame_can_buf[5+frame_ct_can]=mass[5];
            frame_can_buf[6+frame_ct_can]=mass[6];
            frame_can_buf[7+frame_ct_can]=mass[7];
         }
         else
         {
      
           for (ct_can_remap=(8*(len_can_frame/8));ct_can_remap<=len_can_frame;ct_can_remap++)
            {
              frame_can_buf[ct_can_remap]=mass[(len_can_frame%8)-(len_can_frame-ct_can_remap)];
            }
         }
       }
        if (frame_ct_can==len_can_frame)
       {
          vTaskSuspend( xHandle_timeout_frame );
           ct_timeout_frame=0;
      //    len_can_frame=1037;
          frame_ct_can=0;
          frame.id=frame_can_buf[1]|(frame_can_buf[0]<<8);
          frame.len=frame_can_buf[4]|(frame_can_buf[3]<<8)|(frame_can_buf[2]<<16);
          frame.begin_b=frame_can_buf[7]|(frame_can_buf[6]<<8)|(frame_can_buf[5]<<16);
          frame.data_size=frame_can_buf[10]|(frame_can_buf[9]<<8)|(frame_can_buf[8]<<16);
          frame.data = (uint32_t*)&frame_can_buf[11];
          frame.crc = frame_can_buf[len_can_frame-1]|(frame_can_buf[len_can_frame-2]<<8);
           if (frame.begin_b==0)
            {
           ///   flag_frame_run_udp=1;
              flag_frame_run = 1;
            }
       if (crc_compar(frame_can_buf,frame.crc,len_can_frame-2))
            {
              flag_frame_run_can=0;
              frame_ct_can=0;
              status_command = 1;
              frame.data[0]=status_command;
              *out_len=13+1;
              data_coding_frame (data_tx,frame.id,1,(uint8_t*)frame.data,frame.begin_b,1);
           
            }
        else
            {
              status_command= frame_run(frame.id,frame.data_size,frame.data,frame.begin_b,frame.len,&out_len_tx,data_tx);
             uint8_t ct_for;
              for(ct_for=0;ct_for<out_len_tx;ct_for++)
              {
                frame.data[ct_for]=data_tx[ct_for];
              }
              
              *out_len=out_len_tx+13;
              data_coding_frame (data_tx,frame.id,out_len_tx,(uint8_t*)frame.data,frame.begin_b,out_len_tx);
            }
       
             len_can_frame=1037;
             flag_frame_run_can=0;
       return status_command; 
       }
       else
        {
          *out_len=0;
        }
    
     }






return 0;
}
uint32_t index_mass=0;
  uint32_t sector_buffer[2048];
  uint32_t id_store=0xffffffff;
  uint32_t begin_b_store=0xffffffff;
  
int frame_run(int id, int data_size, uint32_t* data,int begin_b,int len,int * out_len_data,uint8_t *data_tx_out)
{
   uint8_t ct_for=0;
//////// uint32_t crc_symm[8]={0};
 uint8_t status_run=0;
//////// uint32_t len_fw1[8]={0};
////////  uint32_t len_fw2[8]={0};
//////// uint32_t crc_flash[8]={0};


  if ((id_store==id)&&(begin_b_store==begin_b))
  {
      data_tx_out[0]=0;
      *out_len_data=1;
  }
else
  {
     switch ( id )
      {
         case 12288:
            {  // DC_bus_volt 
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=Hi_DCDC.dc_bus_volt;
                *out_len_data=1;
           }
            break;
            case 12289:
            {  // Power_bus_Volt
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=Hi_DCDC.power_bus_volt;
                *out_len_data=1;
           }
            break;
            case 12290:
            {   //Power_pwm_out
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=Hi_DCDC.power;
                *out_len_data=1;
           }
            break;
            case 12300:
            {    // Start_DCDC
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=Hi_DCDC.start_flag;
                *out_len_data=1;
           }
            break;
             case 12301:
            {   //Fault_DCDC
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=Hi_DCDC.fault_bus;
                *out_len_data=1;
           }
            break;
            case 12307:
            {  //ON_led_all
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0; 
                Hi_DCDC.start_flag=1;
                Hi_DCDC.stop_flag=0;
                data_tx_out[0]=0;
                *out_len_data=1;
           }
            break;
            case 12308:
            {   //OFF_led_all
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;       
                Hi_DCDC.stop_flag=1;
                Hi_DCDC.start_flag=0;
                data_tx_out[0]=0;
                *out_len_data=1;
           }
            break;
            
            case 12309:
            {  //load_dot_correction
              
                memcpy(lamp_state.led_data_r,data,all_modul*4);
                memcpy(lamp_state.led_data_g,((uint8_t *)(data+all_modul*4)),all_modul*4);
                memcpy(lamp_state.led_data_b,((uint8_t *)(data+all_modul*8)),all_modul*4);
                memcpy(lamp_state.led_data_ir,((uint8_t *)(data+all_modul*12)),all_modul*4);
                lamp_state.reload_dot=1;
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=0;
                *out_len_data=1;
           }
            break;
            
            case 12310:
            {   //load_power_led
              
                lamp_state.lamp_power_req=0x00007fff&(((0x00ff&data[0])<<8)|((0xff00&data[0])>>8));
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=0;
                *out_len_data=1;
           }
            break;
            
            case 12311:
            {  //OFF_power_all
              
                lamp_state.lamp_power_req=0;
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=0;
                *out_len_data=1;
           }
            break;
            
            case 12312:
            {   // Index_pixel_set
               // lamp_state.pixel_edit_index = all_modul*16-(0x000000ff&data[0]);
               lamp_state.pixel_edit_index = 0x000000ff&data[0];
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=0;
                *out_len_data=1;
           }
            break;
            
             case 12313:
            {   //up_dot_correct 
            switch(lamp_state.color_index)
              {
              case 0:
              {
                if (lamp_state.pixel_edit_index<=all_modul*4*4)
                  {
                    if (lamp_state.led_data_r[lamp_state.pixel_edit_index]<0xff)
                      {
                        lamp_state.led_data_r[lamp_state.pixel_edit_index]++;
                      }
                  }
                
                if ((lamp_state.pixel_edit_index<=all_modul*8*4)&&(lamp_state.pixel_edit_index>all_modul*4*4))
                  {
                    if (lamp_state.led_data_g[lamp_state.pixel_edit_index-all_modul*4*4]<0xff)
                      {
                        lamp_state.led_data_g[lamp_state.pixel_edit_index-all_modul*4*4]++;
                      }
                  }
                
                if ((lamp_state.pixel_edit_index<=all_modul*12*4)&&(lamp_state.pixel_edit_index>all_modul*8*4))
                  {
                    if (lamp_state.led_data_b[lamp_state.pixel_edit_index-all_modul*8*4]<0xff)
                      {
                        lamp_state.led_data_b[lamp_state.pixel_edit_index-all_modul*8*4]++;
                      }
                  }
                if ((lamp_state.pixel_edit_index<=all_modul*14*4)&&(lamp_state.pixel_edit_index>all_modul*12*4))
                  {
                    if (lamp_state.led_data_ir[lamp_state.pixel_edit_index-all_modul*12*4]<0xff)
                      {
                        lamp_state.led_data_ir[lamp_state.pixel_edit_index-all_modul*12*4]++;
                      }
                  }
              }
               break;
              case 1:
              {
                if (lamp_state.pixel_edit_index<=all_modul*4*4)
                  {
                    if (lamp_state.led_data_r[lamp_state.pixel_edit_index]<0xff)
                      {
                        lamp_state.led_data_r[lamp_state.pixel_edit_index]++;
                      }
                  }
                
              }
               break;
              case 2:
              {
                 if ((lamp_state.pixel_edit_index<=all_modul*8*4)&&(lamp_state.pixel_edit_index>all_modul*4*4))
                  {
                    if (lamp_state.led_data_g[lamp_state.pixel_edit_index-all_modul*4*4]<0xff)
                      {
                        lamp_state.led_data_g[lamp_state.pixel_edit_index-all_modul*4*4]++;
                      }
                  }
              }
               break;
              case 3:
              {
                                
                if ((lamp_state.pixel_edit_index<=all_modul*12*4)&&(lamp_state.pixel_edit_index>all_modul*8*4))
                  {
                    if (lamp_state.led_data_b[lamp_state.pixel_edit_index-all_modul*8*4]<0xff)
                      {
                        lamp_state.led_data_b[lamp_state.pixel_edit_index-all_modul*8*4]++;
                      }
                  }
              }
               break;
              case 4:
              {
                
                if ((lamp_state.pixel_edit_index<=all_modul*14*4)&&(lamp_state.pixel_edit_index>all_modul*12*4))
                  {
                    if (lamp_state.led_data_ir[lamp_state.pixel_edit_index-all_modul*12*4]<0xff)
                      {
                        lamp_state.led_data_ir[lamp_state.pixel_edit_index-all_modul*12*4]++;
                      }
                  }
              }
               break;
              }
                 if (lamp_state.led_data_r[lamp_state.pixel_edit_index]>0x7f)
                 {
                   lamp_state.led_data_r[lamp_state.pixel_edit_index]=0x7f;
                 }
              if (lamp_state.led_data_g[lamp_state.pixel_edit_index]>0x7f)
                 {
                   lamp_state.led_data_g[lamp_state.pixel_edit_index]=0x7f;
                 }
              if (lamp_state.led_data_b[lamp_state.pixel_edit_index]>0x7f)
                 {
                   lamp_state.led_data_b[lamp_state.pixel_edit_index]=0x7f;
                 }
              if (lamp_state.led_data_ir[lamp_state.pixel_edit_index]>0x7f)
                 {
                   lamp_state.led_data_ir[lamp_state.pixel_edit_index]=0x7f;
                 }
                lamp_state.reload_dot=1;
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=0;
                *out_len_data=1;
           }
            break;
            
             case 12314:
            {  //down_dot_correct    
                     
              switch (lamp_state.color_index)
              {
              case 0:
                {
                  if (lamp_state.led_data_r[lamp_state.pixel_edit_index]>0)
                    {
                    if (lamp_state.pixel_edit_index<=all_modul*4*4)
                      {
                        if (lamp_state.led_data_r[lamp_state.pixel_edit_index]>0)
                          {
                          lamp_state.led_data_r[lamp_state.pixel_edit_index]--;
                      }
                    }
                   }
                 if (lamp_state.led_data_g[lamp_state.pixel_edit_index]>0)
                    {
                    if (lamp_state.pixel_edit_index<=all_modul*4*4)
                      {
                        if (lamp_state.led_data_g[lamp_state.pixel_edit_index]>0)
                          {
                          lamp_state.led_data_g[lamp_state.pixel_edit_index]--;
                      }
                    }
                   }
                 if (lamp_state.led_data_b[lamp_state.pixel_edit_index]>0)
                    {
                    if (lamp_state.pixel_edit_index<=all_modul*4*4)
                      {
                        if (lamp_state.led_data_b[lamp_state.pixel_edit_index]>0)
                          {
                          lamp_state.led_data_b[lamp_state.pixel_edit_index]--;
                      }
                    }
                   }
                 if (lamp_state.led_data_ir[lamp_state.pixel_edit_index]>0)
                    {
                    if (lamp_state.pixel_edit_index<=all_modul*4*4)
                      {
                        if (lamp_state.led_data_ir[lamp_state.pixel_edit_index]>0)
                          {
                          lamp_state.led_data_ir[lamp_state.pixel_edit_index]--;
                      }
                    }
                   }
               }
                break;
               case 1:
                {
                  if (lamp_state.led_data_r[lamp_state.pixel_edit_index]>0)
                    {
                    if (lamp_state.pixel_edit_index<=all_modul*4*4)
                      {
                        if (lamp_state.led_data_r[lamp_state.pixel_edit_index]>0)
                          {
                          lamp_state.led_data_r[lamp_state.pixel_edit_index]--;
                      }
                    }
                   }
                
               }
                break;
                case 2:
                {

                 if (lamp_state.led_data_g[lamp_state.pixel_edit_index]>0)
                    {
                    if (lamp_state.pixel_edit_index<=all_modul*4*4)
                      {
                        if (lamp_state.led_data_g[lamp_state.pixel_edit_index]>0)
                          {
                          lamp_state.led_data_g[lamp_state.pixel_edit_index]--;
                      }
                    }
                   }

               }
                break;
                case 3:
                {
          
                 if (lamp_state.led_data_b[lamp_state.pixel_edit_index]>0)
                    {
                    if (lamp_state.pixel_edit_index<=all_modul*4*4)
                      {
                        if (lamp_state.led_data_b[lamp_state.pixel_edit_index]>0)
                          {
                          lamp_state.led_data_b[lamp_state.pixel_edit_index]--;
                      }
                    }
                   }
                
               }
                break;
                case 4:
                {
                  
                 if (lamp_state.led_data_ir[lamp_state.pixel_edit_index]>0)
                    {
                    if (lamp_state.pixel_edit_index<=all_modul*4*4)
                      {
                        if (lamp_state.led_data_ir[lamp_state.pixel_edit_index]>0)
                          {
                          lamp_state.led_data_ir[lamp_state.pixel_edit_index]--;
                          }
                     }
                   }
               }
                break;
             }
                      
                 lamp_state.reload_dot=1;
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=0;
                *out_len_data=1;
           }
            break;
             case 12315:
            {   // set_color
               // lamp_state.pixel_edit_index = all_modul*16-(0x000000ff&data[0]);
               lamp_state.color_index = 0x000000ff&data[0];
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=0;
                *out_len_data=1;
           }
            break;
            case 12316:
            {   // up_color
               switch (lamp_state.color_index)
              {
              case 0:
                {
                  for (ct_for=0;ct_for<all_modul*4;ct_for++)
                    {
                      if (lamp_state.led_data_r[ct_for]<0x7f)
                        {
                          lamp_state.led_data_r[ct_for]++;
                        }
                      if (lamp_state.led_data_g[ct_for]<0x7f)
                        {
                          lamp_state.led_data_g[ct_for]++;
                        }
                      if (lamp_state.led_data_b[ct_for]<0x7f)
                        {
                          lamp_state.led_data_b[ct_for]++;
                        }
                      if (lamp_state.led_data_ir[ct_for]<0x7f)
                        {
                          lamp_state.led_data_ir[ct_for]++;
                        }
                    }
                }
                break;
                case 1:
                {
                  for (ct_for=0;ct_for<all_modul*4;ct_for++)
                    {
                      if (lamp_state.led_data_r[ct_for]<0x7f)
                        {
                          lamp_state.led_data_r[ct_for]++;
                        }

                    }
                }
                break;
                case 2:
                {
                  for (ct_for=0;ct_for<all_modul*4;ct_for++)
                    {

                      if (lamp_state.led_data_g[ct_for]<0x7f)
                        {
                          lamp_state.led_data_g[ct_for]++;
                        }

                    }
                }
                break;
                case 3:
                {
                  for (ct_for=0;ct_for<all_modul*4;ct_for++)
                    {

                      if (lamp_state.led_data_b[ct_for]<0x7f)
                        {
                          lamp_state.led_data_b[ct_for]++;
                        }
                
                    }
                }
                break;
                case 4:
                {
                  for (ct_for=0;ct_for<all_modul*4;ct_for++)
                    {
               
                      if (lamp_state.led_data_ir[ct_for]<0x7f)
                        {
                          lamp_state.led_data_ir[ct_for]++;
                        }
                    }
                }
                break;
              }
              lamp_state.reload_dot=1;
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=0;
                *out_len_data=1;
           }
            break;
           case 12317:
            {   // down_color
                switch (lamp_state.color_index)
              {
              case 0:
                {
                   for (ct_for=0;ct_for<all_modul*4;ct_for++)
                    {
                      if (lamp_state.led_data_r[ct_for]>0)
                        {
                          lamp_state.led_data_r[ct_for]--;
                        }
                      if (lamp_state.led_data_g[ct_for]>0)
                        {
                          lamp_state.led_data_g[ct_for]--;
                        }
                      if (lamp_state.led_data_b[ct_for]>0)
                        {
                          lamp_state.led_data_b[ct_for]--;
                        }
                      if (lamp_state.led_data_ir[ct_for]>0)
                        {
                          lamp_state.led_data_ir[ct_for]--;
                        }
                    }
                }
                break;
                
          
                case 1:
                {
                   for (ct_for=0;ct_for<all_modul*4;ct_for++)
                    {
                      if (lamp_state.led_data_r[ct_for]>0)
                        {
                          lamp_state.led_data_r[ct_for]--;
                        }
                    }
                }  
                break;
                case 2:
                {   
                   for (ct_for=0;ct_for<all_modul*4;ct_for++)
                    {
                      if (lamp_state.led_data_g[ct_for]>0)
                        {
                          lamp_state.led_data_g[ct_for]--;
                        }
                    }
                }
                break;
                case 3:
                {
                   for (ct_for=0;ct_for<all_modul*4;ct_for++)
                    {

                      if (lamp_state.led_data_b[ct_for]>0)
                        {
                          lamp_state.led_data_b[ct_for]--;
                        }
                    
                    }
                }
                break;
                case 4:
                {
                   for (ct_for=0;ct_for<all_modul*4;ct_for++)
                    {
                      if (lamp_state.led_data_ir[ct_for]>0)
                        {
                          lamp_state.led_data_ir[ct_for]--;
                        }
                    }
                }               
                break;
              }
              lamp_state.reload_dot=1;
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;               
                data_tx_out[0]=0;
                *out_len_data=1;
           }
            break;
             case 12318:
           { //Get temperatur
             
             for (ct_for=0;ct_for<all_modul;ct_for++)
             {
               data_tx_out[ct_for]=lamp_state.modul_state[ct_for].temp;
             }
               *out_len_data=all_modul;
              flag_frame_run=0;
              flag_frame_run_udp=0;
              flag_frame_run_can=0;
           }
            break;
             case 12319:
           { //Set all out 1
             
             for (ct_for=0;ct_for<all_modul*2*4;ct_for++)
             {
                lamp_state.led_onoff[ct_for]=0xff;
             }
             lamp_state.onoff_pix=1;
              data_tx_out[0]=0;
               *out_len_data=1;
              flag_frame_run=0;
              flag_frame_run_udp=0;
              flag_frame_run_can=0;
           }
            break; 
             case 12320:
           { //Set all out 0
             
             for (ct_for=0;ct_for<all_modul*2*4;ct_for++)
             {
                lamp_state.led_onoff[ct_for]=0x0;
             }
             lamp_state.onoff_pix=1;
              data_tx_out[0]=0;
               *out_len_data=1;
              flag_frame_run=0;
              flag_frame_run_udp=0;
              flag_frame_run_can=0;
           }
            break; 
           
         case 0x00:
           { // GET ID
             
             for (ct_for=0;ct_for<8;ct_for++)
             {
               data_tx_out[ct_for]=Serial[ct_for];
             }
               *out_len_data=8;
              flag_frame_run=0;
              flag_frame_run_udp=0;
              flag_frame_run_can=0;
           }
            break;
         case 0x02:
           { // SET PROTECT
//////////             uint32_t ct_for_p=0;
//////////             for (ct_for_p=0;ct_for_p<2048;ct_for_p++)
//////////              {
//////////                sector_buffer[ct_for_p] = *((uint32_t*)0x10000000+ct_for_p);
//////////              }
//////////             
//////////              sector_buffer[258]=0x00000001;
//////////             sector_buffer[259]=0xfffffffc;
//////////          
//////////             taskENTER_CRITICAL();
//////////              if (progSector( 0x10000000,sector_buffer, 8192))
//////////                      //read len storege flash programm
//////////
//////////                        {
//////////                          PRINTF("Protect set error  \r\n"); //Ошибка
//////////                          flag_frame_run=0;
//////////                          flag_frame_run_udp=0;
//////////                          flag_frame_run_can=0;
//////////                          data_tx_out[0]=2;
//////////                          *out_len_data=1;
//////////                            FTFE->FPROT0=1;
//////////                            FTFE->FPROT1=0;
//////////                            FTFE->FPROT2=0;
//////////                            FTFE->FPROT3=0;
//////////                     //       FTFE->FSEC = 0;
//////////                         // data[0]=2;
//////////                          break;
//////////                        }
//////////   
//////////               flag_frame_run=0;
//////////                flag_frame_run_udp=0;
//////////                flag_frame_run_can=0;
//////////               
//////////                data_tx_out[0]=0;
//////////                *out_len_data=1;
//////////                taskEXIT_CRITICAL();
               
           }
            break;
         case 0x03:
           { // ERASE
////////              data_tx_out[0]=0;
////////              taskENTER_CRITICAL();
////////               uint32_t write_init[8]={0};
////////              if (progEraseSector( FW1,393216))
////////                            {
////////                              PRINTF("Error Erase FW1 \r\n"); //Ошибка
////////                              flag_frame_run=0;
////////                              flag_frame_run_udp=0;
////////                              flag_frame_run_can=0;
////////                              data_tx_out[0]=2;
////////                              *out_len_data=1;
////////                             
////////                              break;
////////                            }
////////              write_init[0]=0x1fff;
////////              if (progSector( FW1_LEN_A,write_init, 32))
////////                      //read len storege flash programm
////////
////////                        {
////////                          PRINTF("Error prog LEN FW1 \r\n"); //Ошибка
////////                          flag_frame_run=0;
////////                          flag_frame_run_udp=0;
////////                          flag_frame_run_can=0;
////////                          data_tx_out[0]=2;
////////                          *out_len_data=1;
////////                         // data[0]=2;
////////                          break;
////////                        }
////////              write_init[0]=0;
////////                if (progSector( FW1_CRC_A,write_init, 32))
////////                      //read len storege flash programm
////////
////////                        {
////////                          PRINTF("Error prog CRC FW1 \r\n"); //Ошибка
////////                          flag_frame_run=0;
////////                          flag_frame_run_udp=0;
////////                          flag_frame_run_can=0;
////////                          data_tx_out[0]=2;
////////                          *out_len_data=1;
////////                         // data[0]=2;
////////                          break;
////////                        }
////////              
////////              if (progEraseSector( FW2,393216))
////////                            {
////////                              PRINTF("Error Erase FW1 \r\n"); //Ошибка
////////                              flag_frame_run=0;
////////                              flag_frame_run_udp=0;
////////                              flag_frame_run_can=0;
////////                              data_tx_out[0]=2;
////////                              *out_len_data=1;
////////                             
////////                              break;
////////                            }
////////                write_init[0]=0x0000;
////////               if (progSector( FW2_LEN_A,write_init, 32))
////////                      //read len storege flash programm
////////
////////                        {
////////                          PRINTF("Error prog LEN FW2 \r\n"); //Ошибка
////////                          flag_frame_run=0;
////////                          flag_frame_run_udp=0;
////////                          flag_frame_run_can=0;
////////                          data_tx_out[0]=2;
////////                          *out_len_data=1;
////////                         // data[0]=2;
////////                          break;
////////                        }
////////               write_init[0]=0;
////////                if (progSector( FW2_CRC_A,write_init, 32))
////////                      //read len storege flash programm
////////
////////                        {
////////                          PRINTF("Error prog CRC FW2 \r\n"); //Ошибка
////////                          flag_frame_run=0;
////////                          flag_frame_run_udp=0;
////////                          flag_frame_run_can=0;
////////                          data_tx_out[0]=2;
////////                          *out_len_data=1;
////////                         // data[0]=2;
////////                          break;
////////                        }
////////              
////////              
////////           taskEXIT_CRITICAL();
////////                  *out_len_data=1;
////////                   data_tx_out[0]=0;
////////             flag_frame_run=0;
////////             flag_frame_run_udp=0;
////////             flag_frame_run_can=0;
////////              
              
           }
            break;
         case 0x04:
           { //GET SAVE VERSION
                 flag_frame_run=0;
                 flag_frame_run_udp=0;
                 flag_frame_run_can=0;
                 
                 data_tx_out[0]=*((uint8_t*)FW2_VER_A);
                 data_tx_out[1]=*((uint8_t*)FW2_VER_A+1);
                 data_tx_out[2]=*((uint8_t*)FW2_VER_A+2);
                 data_tx_out[3]=*((uint8_t*)FW2_VER_A+3);
                 data_tx_out[4]=*((uint8_t*)FW2_VER_A+4);
                 data_tx_out[5]=*((uint8_t*)FW2_VER_A+5);
                 
                 *out_len_data=6;
           }
            break;
          case 0x05:
           { //GET CURR VERSION
                  flag_frame_run=0;
                 flag_frame_run_udp=0;
                 flag_frame_run_can=0;
                 
                 data_tx_out[0]=*((uint8_t*)FW1_VER_A);
                 data_tx_out[1]=*((uint8_t*)FW1_VER_A+1);
                 data_tx_out[2]=*((uint8_t*)FW1_VER_A+2);
                 data_tx_out[3]=*((uint8_t*)FW1_VER_A+3);
                 data_tx_out[4]=*((uint8_t*)FW1_VER_A+4);
                 data_tx_out[5]=*((uint8_t*)FW1_VER_A+5);
                 
                 *out_len_data=6;
           }
            break;
          case 0x06:
           { // restore app
////               taskENTER_CRITICAL();
////              
////              if (progSector( FW1,(uint32_t*)FW2,393216))
////                            {
////                              PRINTF("Error Erase FW1 \r\n"); //Ошибка
////                              flag_frame_run=0;
////                              flag_frame_run_udp=0;
////                              flag_frame_run_can=0;
////                              data_tx_out[0]=2;
////                              *out_len_data=1;
////                             
////                              break;
////                            }
////              
////              if (progSector( FW1_LEN_A,(uint32_t*)FW2_LEN_A, 32))
////                      //read len storege flash programm
////                        {
////                          PRINTF("Error prog LEN FW1 \r\n"); //Ошибка
////                          flag_frame_run=0;
////                          flag_frame_run_udp=0;
////                          flag_frame_run_can=0;
////                          data_tx_out[0]=2;
////                          *out_len_data=1;
////                         // data[0]=2;
////                          break;
////                        }
////              
////                if (progSector( FW1_CRC_A,(uint32_t*)FW2_CRC_A, 32))
////                      //read len storege flash programm
////                        {
////                          PRINTF("Error prog CRC FW1 \r\n"); //Ошибка
////                          flag_frame_run=0;
////                          flag_frame_run_udp=0;
////                          flag_frame_run_can=0;
////                          data_tx_out[0]=2;
////                          *out_len_data=1;
////                         // data[0]=2;
////                          break;
////                        }
////                taskEXIT_CRITICAL();
////             *out_len_data=1;
////              data_tx_out[0]=0;
////             flag_frame_run=0;
////             flag_frame_run_udp=0;
////             flag_frame_run_can=0;
////             
////           }
////            break;
////          case 0x07:
////           {
////              jamp_app();
////              *out_len_data=1;
////              data_tx_out[0]=0;
////              flag_frame_run=0;
////              flag_frame_run_udp=0;
////              flag_frame_run_can=0;
           }
            break;
          case 0x08:
           { //Erase FW1
//////               taskENTER_CRITICAL();
//////               uint32_t write_init[8]={0};
//////              if (progEraseSector( FW1,393216))
//////                            {
//////                              PRINTF("Error Erase FW1 \r\n"); //Ошибка
//////                              flag_frame_run=0;
//////                              flag_frame_run_udp=0;
//////                              flag_frame_run_can=0;
//////                              data_tx_out[0]=2;
//////                              *out_len_data=1;
//////                             
//////                              break;
//////                            }
//////              write_init[0]=0x1fff;
//////              if (progSector( FW1_LEN_A,write_init, 32))
//////                      //read len storege flash programm
//////
//////                        {
//////                          PRINTF("Error prog LEN FW1 \r\n"); //Ошибка
//////                          flag_frame_run=0;
//////                          flag_frame_run_udp=0;
//////                          flag_frame_run_can=0;
//////                          data_tx_out[0]=2;
//////                          *out_len_data=1;
//////                         // data[0]=2;
//////                          break;
//////                        }
//////              write_init[0]=0;
//////                if (progSector( FW1_CRC_A,write_init, 32))
//////                      //read len storege flash programm
//////
//////                        {
//////                          PRINTF("Error prog CRC FW1 \r\n"); //Ошибка
//////                          flag_frame_run=0;
//////                          flag_frame_run_udp=0;
//////                          flag_frame_run_can=0;
//////                          data_tx_out[0]=2;
//////                          *out_len_data=1;
//////                         // data[0]=2;
//////                          break;
//////                        }
//////                taskEXIT_CRITICAL();
//////              *out_len_data=1;
//////              data_tx_out[0]=0;
//////             flag_frame_run=0;
//////             flag_frame_run_udp=0;
//////             flag_frame_run_can=0;
//////                
                
           }
            break;
          case 0x09:
           { //Erase FW2
////             uint32_t write_init[8]={0};
////               taskENTER_CRITICAL();
////              if (progEraseSector( FW2,393216))
////                            {
////                              PRINTF("Error Erase FW1 \r\n"); //Ошибка
////                              flag_frame_run=0;
////                              flag_frame_run_udp=0;
////                              flag_frame_run_can=0;
////                              data_tx_out[0]=2;
////                              *out_len_data=1;
////                             
////                              break;
////                            }
////                write_init[0]=0x0000;
////               if (progSector( FW2_LEN_A,write_init, 32))
////                      //read len storege flash programm
////
////                        {
////                          PRINTF("Error prog LEN FW2 \r\n"); //Ошибка
////                          flag_frame_run=0;
////                          flag_frame_run_udp=0;
////                          flag_frame_run_can=0;
////                          data_tx_out[0]=2;
////                          *out_len_data=1;
////                         // data[0]=2;
////                          break;
////                        }
////               write_init[0]=0;
////                if (progSector( FW2_CRC_A,write_init, 32))
////                      //read len storege flash programm
////
////                        {
////                          PRINTF("Error prog CRC FW2 \r\n"); //Ошибка
////                          flag_frame_run=0;
////                          flag_frame_run_udp=0;
////                          flag_frame_run_can=0;
////                          data_tx_out[0]=2;
////                          *out_len_data=1;
////                         // data[0]=2;
////                          break;
////                        }
////              
////              
////           taskEXIT_CRITICAL();
////            *out_len_data=1;
////             data_tx_out[0]=0;
////             flag_frame_run=0;
////             flag_frame_run_udp=0;
////             flag_frame_run_can=0;
           }
            break;
          case 10:
           { //set IP
             uint32_t IP_data[8]={0};
             taskENTER_CRITICAL();
             
              IP_data[6] =  *((uint32_t*)(IP_CONFIG_A+24));
              IP_data[5] =  *((uint32_t*)(IP_CONFIG_A+20));
              IP_data[4] =  *((uint32_t*)(IP_CONFIG_A+16));
              IP_data[3] =  *((uint32_t*)(IP_CONFIG_A+12));
              IP_data[2] =  *((uint32_t*)(IP_CONFIG_A+8));
              IP_data[1] =  *((uint32_t*)(IP_CONFIG_A+4));
              IP_data[0] = data[0];
              
              
              InitCrc16(CRC0, 0xFFFFFFFFU);
              CRC_WriteData(CRC0, (const uint8_t *)IP_data, 28);
              IP_data[7] = CRC_Get16bitResult(CRC0);
              
              //Копируем длинну прошивки  из области 1 в облость 2 прошивку
                    if (progSector( IP_CONFIG_A, (uint32_t*)IP_data, 32))
                      //read len storege flash programm
                        {
                          PRINTF("Error prog LEN FW2 \r\n"); //Ошибка
                          flag_frame_run=0;
                          flag_frame_run_udp=0;
                          flag_frame_run_can=0;
                          data_tx_out[0]=2;
                          *out_len_data=1;
                         // data[0]=2;
                          break;
                        }

                taskEXIT_CRITICAL();
                flag_frame_run=0;
                flag_frame_run_udp=0;
                flag_frame_run_can=0;
                data_tx_out[0]=0;
                *out_len_data=1;
             
           }
            break;
          case 11:
           { //get_loader_version
               flag_frame_run=0;
                 flag_frame_run_udp=0;
                 flag_frame_run_can=0;
                 
                 data_tx_out[0]=*((uint8_t*)FW_LOADER_VER_A);
                 data_tx_out[1]=*((uint8_t*)FW_LOADER_VER_A+1);
                 data_tx_out[2]=*((uint8_t*)FW_LOADER_VER_A+2);
                 data_tx_out[3]=*((uint8_t*)FW_LOADER_VER_A+3);
                 data_tx_out[4]=*((uint8_t*)FW_LOADER_VER_A+4);
                 data_tx_out[5]=*((uint8_t*)FW_LOADER_VER_A+5);
                 
                 *out_len_data=6;
           }
           break;
          case 12:
           { //RESET
              NVIC_SystemReset();
           }
            break;
          case 13:
           { //unset_protect
//////             uint32_t ct_for_p=0;
//////             for (ct_for_p=0;ct_for_p<2048;ct_for_p++)
//////              {
//////                sector_buffer[ct_for_p] = *((uint32_t*)0x10000000+ct_for_p);
//////              }
//////               // Flash_set_protect ();
//////            /* uint32_t protect_sect[8]={0};
//////             protect_sect[0]=0xffffffff;
//////             protect_sect[1]=0xffffffff;
//////             protect_sect[2]=0xffffffff;
//////             protect_sect[3]=0xfffffffe;
//////             
//////             protect_sect[4]=0xffffffff;
//////             protect_sect[5]=0xffffffff;
//////             protect_sect[6]=0xffffffff;
//////             protect_sect[7]=0xffffffff;*/
//////             sector_buffer[258]=0xffffffff;
//////             sector_buffer[259]=0xfffffffe;
//////             FTFE->FPROT0=0xff;
//////            FTFE->FPROT1=0xff;
//////            FTFE->FPROT2=0xff;
//////            FTFE->FPROT3=0xff;
//////            //FTFE->FSEC = 0xFE;
//////             taskENTER_CRITICAL();
//////              if (progSector( 0x10000000,sector_buffer, 8192))
//////                      //read len storege flash programm
//////                        {
//////                          PRINTF("Protect set error  \r\n"); //Ошибка
//////                          flag_frame_run=0;
//////                          flag_frame_run_udp=0;
//////                          flag_frame_run_can=0;
//////                          data_tx_out[0]=2;
//////                          *out_len_data=1;
//////                         // data[0]=2;
//////                          break;
//////                        }
//////               flag_frame_run=0;
//////                flag_frame_run_udp=0;
//////                flag_frame_run_can=0;
//////               
//////                data_tx_out[0]=0;
//////                *out_len_data=1;
//////                taskEXIT_CRITICAL();
//////             //    Flash_unset_protect ();
           }
            break;
          case  14 :
           { //CAN ADDR TX
////////               uint32_t CAN_data[8]={0};
////////              taskENTER_CRITICAL();
////////              
////////              CAN_data[6] = 0;
////////              CAN_data[5] = 0;
////////              CAN_data[4] = 0;
////////              CAN_data[3] = 0;
////////              CAN_data[2] = *((uint32_t*)(IP_CONFIG_A+8));
////////              CAN_data[1] = data[0];
////////              CAN_data[1] = CAN_data[1]&0xff;
////////              CAN_data[0] =  *((uint32_t*)(IP_CONFIG_A));
////////              
////////              InitCrc16(CRC0, 0xFFFFFFFFU);
////////              CRC_WriteData(CRC0, (const uint8_t *)CAN_data, 28);
////////              CAN_data[7] = CRC_Get16bitResult(CRC0);
////////             
////////              
////////              //Копируем длинну прошивки  из области 1 в облость 2 прошивку
////////                    if (progSector( IP_CONFIG_A, (uint32_t*)CAN_data, 32))
////////                      //read len storege flash programm
////////
////////                        {
////////                          PRINTF("Error prog LEN FW2 \r\n"); //Ошибка
////////                          flag_frame_run=0;
////////                          flag_frame_run_udp=0;
////////                          flag_frame_run_can=0;
////////                          data_tx_out[0]=2;
////////                          *out_len_data=1;
////////                         // data[0]=2;
////////                          break;
////////                        }
////////
////////                taskEXIT_CRITICAL();
////////                flag_frame_run=0;
////////                flag_frame_run_udp=0;
////////                flag_frame_run_can=0;
////////                data_tx_out[0]=0;
////////                *out_len_data=1;
           }
            break;
          case 15:
           { //CAN ADDR RX
//////               uint32_t CAN_data[8]={0};
//////              taskENTER_CRITICAL();
//////              
//////              CAN_data[6] = 0;
//////              CAN_data[5] = 0;
//////              CAN_data[4] = 0;
//////              CAN_data[3] = 0;
//////              CAN_data[2] = data[0];
//////              CAN_data[2] = CAN_data[2]&0xff;
//////              CAN_data[1] = *((uint32_t*)(IP_CONFIG_A+4));
//////             
//////              CAN_data[0] =  *((uint32_t*)(IP_CONFIG_A));
//////              
//////              InitCrc16(CRC0, 0xFFFFFFFFU);
//////              CRC_WriteData(CRC0, (const uint8_t *)CAN_data, 28);
//////              CAN_data[7] = CRC_Get16bitResult(CRC0);
//////             
//////              
//////              //Копируем длинну прошивки  из области 1 в облость 2 прошивку
//////                    if (progSector( IP_CONFIG_A, (uint32_t*)CAN_data, 32))
//////                      //read len storege flash programm
//////
//////                        {
//////                          PRINTF("Error prog LEN FW2 \r\n"); //Ошибка
//////                          flag_frame_run=0;
//////                          flag_frame_run_udp=0;
//////                          flag_frame_run_can=0;
//////                          data_tx_out[0]=2;
//////                          *out_len_data=1;
//////                         // data[0]=2;
//////                          break;
//////                        }
//////
//////                taskEXIT_CRITICAL();
//////                flag_frame_run=0;
//////                flag_frame_run_udp=0;
//////                flag_frame_run_can=0;
//////                data_tx_out[0]=0;
//////                *out_len_data=1;
           }
            break;

         default:
           PRINTF("Error command =: 0x%x\r\n", id);
           data_tx_out[0]=3;
           status_run= 3;
           break;

      
     
     begin_b_store=begin_b;
     id_store=id;
  }
  }
  return 0;
}