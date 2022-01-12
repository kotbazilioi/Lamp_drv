
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "stdio.h"
#include "stdlib.h"
#include "hard_init.h"
#include "httpd_structs.h"
#include "fs.h"
#include "DLib_Config_Full.h"
#include "httpserver-netconn.h"
#include "html_page.h"
#include "heap_3.h"
#include "base64.h"
#include "app.h"
#if LWIP_NETCONN

#ifndef HTTPD_DEBUG
#define HTTPD_DEBUG         LWIP_DBG_OFF
#endif
int fr;

uint8_t page_index;
static const unsigned char PAGE_HEADER_200_OK[] = {
  //"HTTP/1.1 200 OK"
  0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
  0x0a,
  //zero
  0x00
};
static const unsigned char PAGE_HEADER_SERVER[] = {
  //"Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)"
  0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x6c,0x77,0x49,0x50,0x2f,0x31,0x2e,0x33,
  0x2e,0x31,0x20,0x28,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x73,0x61,0x76,0x61,0x6e,
  0x6e,0x61,0x68,0x2e,0x6e,0x6f,0x6e,0x67,0x6e,0x75,0x2e,0x6f,0x72,0x67,0x2f,0x70,
  0x72,0x6f,0x6a,0x65,0x63,0x74,0x73,0x2f,0x6c,0x77,0x69,0x70,0x29,0x0d,0x0a,
  //zero
  0x00
};
static const unsigned char PAGE_HEADER_CONTENT_TEXT[] = {
  //"Content-type: text/html"
  0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,0x74,0x65,
  0x78,0x74,0x2f,0x68,0x74,0x6d,0x6c,0x0d,0x0a,0x0d,0x0a,
  //zero
  0x00
};
//*
static const unsigned char PAGE_HEADER_CONTENT_STREAM[] = {
  //"Content-Type: application/octet-stream"
  0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x54,0x79,0x70,0x65,0x3a,0x20,0x61,0x70,
  0x70,0x6c,0x69,0x63,0x61,0x74,0x69,0x6f,0x6e,0x2f,0x6f,0x63,0x74,0x65,0x74,0x2d,
  0x73,0x74,0x72,0x65,0x61,0x6d,0x0d,0x0a,
  //zero
  0x00
};
static const unsigned char PAGE_HEADER_LEN[] = {
  //"Content-Length: "
  0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x4c,0x65,0x6e,0x67,0x74,0x68,0x3a,0x20,
  //zero
  0x00
};
static const unsigned char PAGE_HEADER_BYTES[] = {
  //"Accept-Ranges: bytes"
  0x41,0x63,0x63,0x65,0x70,0x74,0x2d,0x52,0x61,0x6e,0x67,0x65,0x73,0x3a,0x20,0x62,
  0x79,0x74,0x65,0x73,0x0d,0x0a,0x0d,0x0a,
  //zero
  0x00
};



typedef struct post_data_t
  {
    char name[32];    
    char data[32];    
    //uint8_t len_par;
  }post_data_t;

 uint8_t scanf_ip (char* data,uint8_t* ip_mass,uint8_t len_ip)
  {
     signed int  ct_char=0;
     uint16_t mul_k=1;
     memset(ip_mass,0,4);
    for(ct_char=len_ip-1;ct_char>=0;ct_char--)
      {
        if (data[ct_char]!='.')
          {
            ip_mass[3]=(data[ct_char]-0x30)*mul_k+ip_mass[3];
            mul_k=mul_k*10;  
            if (mul_k>1000)
            {// break;
              return 1;
            }
          }
        else
          {
          break;
          }
      }
    ct_char--;
    mul_k=1;
    for(;ct_char>=0;ct_char--)
      {
        if (data[ct_char]!='.')
          {
            ip_mass[2]=(data[ct_char]-0x30)*mul_k+ip_mass[2];
            mul_k=mul_k*10;  
            if (mul_k>1000)
            { 
              //break;
            return 1;
            }
          }
        else
          {
          break;
          }
      }
    ct_char--;
     mul_k=1;
     for(;ct_char>=0;ct_char--)
      {
        if (data[ct_char]!='.')
          {
            ip_mass[1]=(data[ct_char]-0x30)*mul_k+ip_mass[1];
            mul_k=mul_k*10;  
            if (mul_k>1000)
            { //break;
            return 1;
            }
          }
        else
          {
          break;
          }
      }
     ct_char--;
      mul_k=1;
     for(;ct_char>=0;ct_char--)
      {
        if ((data[ct_char]!='.')||(ct_char!=0))
          {
            ip_mass[0]=(data[ct_char]-0x30)*mul_k+ip_mass[0];
            mul_k=mul_k*10;  
            if (mul_k>1000)
            {// break;
            return 1;
            }
          }
        else
          {
          break;
          }
      }
    return 0;
  }
  
  
   uint8_t scanf_port (char* data,uint16_t* ip_mass,uint8_t len_ip)
  {
     signed int  ct_char=0;
     uint16_t mul_k=1;

      mul_k=1;
      *ip_mass=0;
     for(ct_char=len_ip;ct_char>=0;ct_char--)
      {
        if (data[ct_char]>0x39)
          {
            return 1;
          }
        if (ct_char!=0)
          {
            *ip_mass=(data[ct_char-1]-0x30)*mul_k+(*ip_mass);
            mul_k=mul_k*10;  
            if (mul_k>100000)
            {// break;
            return 1;
            }
          }
        else
          {
          break;
          }
      }
    return 0;
  }
  void param_run(post_data_t* post_data,uint8_t index)
{
  uint8_t IP_buf[4];
  uint16_t port_n;
        uint8_t ct_temp0;
  uint8_t len_mess=0;
//  if (index==3)
//  {
    if (strncmp((char*)post_data->name,"name_dev", sizeof("name_dev")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            memset((char*)FW_data.V_Name_dev,0,strlen((char*)FW_data.V_Name_dev));
            for (ct_temp0=0;ct_temp0<len_mess;ct_temp0++)
              {
                if (post_data->data[ct_temp0]==0x2b)
                  {
                    post_data->data[ct_temp0]=0x20;
                  }
             
              }
            memcpy((char*)FW_data.V_Name_dev, (char*)post_data->data,len_mess );
          }
    else if (strncmp((char*)post_data->name,"IP_addr", sizeof("IP_addr")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_CONFIG, (char*)IP_buf,4 );
            }
          
          }
    else if (strncmp((char*)post_data->name,"Mask_net", sizeof("Mask_net")) == 0)
          {
             len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_MASK, (char*)IP_buf,4 );
            }
          }
    else if (strncmp((char*)post_data->name,"IP_Getway", sizeof("IP_Getway")) == 0)
          {
             len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_GET, (char*)IP_buf,4 );
            }
          }
    else if (strncmp((char*)post_data->name,"dhcp_flag", sizeof("dhcp_flag")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (post_data->data[0]==0x31)
            {
              FW_data.V_DHCP=1;
            }
          }
    else if (strncmp((char*)post_data->name,"Color_fon", sizeof("Color_fon")) == 0)
          {
           // FW_data.V_Name_dev
            uint8_t r_in,gr_in,bl_in;
            char hex_c[2];
            len_mess=strlen(post_data->data);
            hex_c[1]=post_data->data[4];
            hex_c[0]=post_data->data[3];
            lamp_state.led_data_r_req=strtol(hex_c,NULL,16);
            
            hex_c[1]=post_data->data[6];
            hex_c[0]=post_data->data[5];
            lamp_state.led_data_g_req=strtol(hex_c,NULL,16);
            
            hex_c[1]=post_data->data[8];
            hex_c[0]=post_data->data[7];
            lamp_state.led_data_b_req=strtol(hex_c,NULL,16);
            
            set_color_power(lamp_state.led_data_r_req,lamp_state.led_data_g_req,lamp_state.led_data_b_req,0);                     
            
//             memset((char*)FW_data.V_LOGIN,0,strlen((char*)FW_data.V_LOGIN));
//            memcpy((char*)FW_data.V_LOGIN, (char*)post_data->data,len_mess );
          }
//    else if (strncmp((char*)post_data->name,"password", sizeof("password")) == 0)
//          {
//           // FW_data.V_Name_dev
//            len_mess=strlen(post_data->data);
//            memset((char*)FW_data.V_PASSWORD,0,strlen((char*)FW_data.V_PASSWORD));
//            memcpy((char*)FW_data.V_PASSWORD, (char*)post_data->data,len_mess );
//          }
    else if (strncmp((char*)post_data->name,"Power_lamp", sizeof("Power_lamp")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              lamp_state.lamp_power_req=port_n;
              //memcpy((uint8_t*)lamp_state.lamp_power_req, (char*)IP_buf,4 );
            }
          }
    else if (strncmp((char*)post_data->name,"Color_n", sizeof("Color_n")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              lamp_state.color_index=port_n;
            }
          }
    else if (strncmp((char*)post_data->name,"Pixel_n", sizeof("Pixel_n")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              lamp_state.pixel_edit_index=port_n;
            }
          }
    else if (strncmp((char*)post_data->name,"Save_settings", sizeof("save_settings")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (post_data->data[0]=='1')
            {
              flag_save_param = 1;
             // save_settings();
              
              //memcpy((uint8_t*)FW_data.V_IP_NTP1, (char*)IP_buf,4 );
            }
          }
    else if (strncmp((char*)post_data->name,"color_fon", sizeof("color_fon")) == 0)
          {
          // len_mess=strlen(post_data->data);
         //   if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
         //   {
         //     memcpy((uint8_t*)FW_data.V_IP_NTP2, (char*)IP_buf,4 );
         //   }
          }
    else if (strncmp((char*)post_data->name,"Power", sizeof("Power")) == 0)
          {
           len_mess=strlen(post_data->data);
        
           //
           
//           if (post_data->data[0]=='-')
//           {
//            post_data->data[0]=post_data->data[1];
//            post_data->data[1]=post_data->data[2];
//            len_mess--;
//            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
//            {
//              
//              FW_data.V_NTP_CIRCL=(signed char)port_n-2*port_n;
//            }
//           }
//           else
//           {
//            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
//            {
//              
//              FW_data.V_NTP_CIRCL=(signed char)port_n;
//            }
//           
//           }
           
          }
    else if (strncmp((char*)post_data->name,"time_set", sizeof("time_set")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
          //  memcpy((char*)FW_data.V_PASSWORD, (char*)post_data->data,len_mess );
          }
    else if (strncmp((char*)post_data->name,"Power_on", sizeof("mess_on")) == 0)
          {
              if (post_data->data[0]==0x31)
                {
                  lamp_state.onoff_pix=1;
                       vTaskDelay(100);
                  Hi_DCDC.start_flag=1;
                  Hi_DCDC.stop_flag=0;
                }
          }
    else if (strncmp((char*)post_data->name,"Power_off", sizeof("mess_off")) == 0)
          {
            if (post_data->data[0]==0x31)
                {
                  Hi_DCDC.stop_flag=1;
                  Hi_DCDC.start_flag=0;
                }
     
          }
//    else if (strncmp((char*)post_data->name,"output_type", sizeof("output_type")) == 0)
//          {
//           len_mess=strlen(post_data->data);
//            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
//            {
//              FW_data.V_TYPE_OUT=port_n;
//            }
//          }
//    else if (strncmp((char*)post_data->name,"save_all", sizeof("save_all")) == 0)
//          {
//            if (post_data->data[0]==0x31)
//              {
//                flag_global_save_data=1;
//              }
//          }
    else if (strncmp((char*)post_data->name,"load_def", sizeof("load_def")) == 0)
          {
           if (post_data->data[0]==0x31)
              {
                flag_global_load_def=1;
              }
          }
    else if (strncmp((char*)post_data->name,"load_boot", sizeof("load_boot")) == 0)
          {
           ///jamp_to_boot();
          }
    else
     
    {
    
    }
    
//  }
  
}
void parser_post(char* buf_in,uint16_t buf_in_len,uint8_t index)
{
uint16_t ct_index,start_pars;
uint16_t start_par;
post_data_t elem_post_data;

 for(ct_index=buf_in_len;ct_index>0;ct_index--)
   {
     if (buf_in[ct_index]==0x0a)
       {
         if ((buf_in[ct_index-2]==0x0a)&&(buf_in[ct_index-1]==0x0d)&&(buf_in[ct_index-3]==0x0d))
           {
             start_pars=ct_index+1;
             ct_index=1;
           }
       }
   }
 start_par=start_pars;
 for(ct_index=start_pars;ct_index<=buf_in_len;ct_index++)
   {
    // elem_post_data=&post_mess[index_sec];
     if (buf_in[ct_index]=='=')
       {
         
         memset(elem_post_data.name,0,32);
         memcpy(elem_post_data.name,(uint8_t*)(buf_in+start_par),(ct_index-start_par));
         start_par=ct_index+1;
       }
     if ((buf_in[ct_index]=='&')||(ct_index==buf_in_len))
       {
         memset(elem_post_data.data,0,32);
         memcpy(elem_post_data.data,(uint8_t*)(buf_in+start_par),(ct_index-start_par));
         start_par=ct_index+1;
         param_run(&elem_post_data,index);
         //case_data_in
      //   elem_post_data.len_par=index_sec+1;
       // index_sec++;
         
       }
   }
 
}




static void http_server_netconn_serve(struct netconn *conn1) 
{
  //post_data_t post_data[32];
    char* buf_list=(char*)pvPortMalloc(size_page_bufer);
//char buf_list[size_page_bufer]={0};
  struct netbuf *inbuf;
  err_t recv_err;
  char* buf;
  u16_t buflen;
  struct fs_file file;  
  uint16_t len_buf_list;
  uint16_t ct_fpass=0;
  uint16_t start_key=0;
  uint8_t status=0;
  /* Read the data from the port, blocking if nothing yet there. 
   We assume the request (the part we care about) is in one netbuf */
 recv_err = netconn_recv(conn1, &inbuf);
  if (recv_err == ERR_OK)
   {
    if (netconn_err(conn1) == ERR_OK) 
     {
      netbuf_data(inbuf, (void**)&buf, &buflen);
         
      memset((void*)key_http,0,30);
      
      for (ct_fpass=0;ct_fpass<1024;ct_fpass++)
        {
          if (strncmp((char*)&buf[ct_fpass],"Basic",5) == 0)
            {
              start_key=ct_fpass+6;
              break;
            }
        }
      memcpy((char*)buf_list,(char*)&buf[start_key],16);  
      unbase64((char*)buf_list,16,&key_http_len,key_http);
      if ((flag_logon==0)&&(strncmp(key_http,"admin:csort",key_http_len) != 0))
       {          
        len_buf_list=costr_pass((char*)buf_list);
        netconn_write(conn1, (const unsigned char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
        flag_logon=0;
       }
      if ((flag_logon==0)&&(strncmp(key_http,"admin:csort",key_http_len) == 0))
       {
        flag_logon=1;
       }
      if(strncmp(key_http,"admin:csort",key_http_len) == 0)  
       {
      if ((buflen >=5) && (strncmp(buf, "GET /", 5) == 0))
       {
        if (strncmp((char const *)buf,"GET /img/rgb24.bmp",17)==0)
         {
//////////          costr_graf_pl_start(conn1,buf_list);
//////////          
//////////          vTaskDelay(100);
////////////          len_buf_list=sizeof((uint8_t*)http_html_hdr);
////////////          memcpy(buf_list,http_html_hdr,len_buf_list);
////////////          netconn_write(conn1, (const unsigned char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
         }         
        
        else if (strncmp((char const *)buf,"GET /img/logo.gif",17)==0)
         {
          recv_err=fs_open(&file, "/img/logo.gif");           
          status=netconn_write(conn1, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
         // vTaskDelay(100);
          fs_close(&file);
          
         }          
        else if (strncmp((char const *)buf,"GET /settings.html",17)==0)
         {
          page_index=1;
          len_buf_list=costr_settings((char*)buf_list);
          status=netconn_write(conn1, (const unsigned char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
             // vTaskDelay(100);
//              len_buf_list=sizeof((uint8_t*)http_html_hdr);
//              memcpy(buf_list,http_html_hdr,len_buf_list);
//              netconn_write(conn1, (const unsigned char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
     
         }
        else if (strncmp((char const *)buf,"GET /content.html",17)==0)
         {
          sprintf(buf_list,"%s%s%s Текущая температура: M1=%dС M2=%dC M3=%dC M4=%dC M5=%dC M6=%dC M7=%dC ",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,lamp_state.modul_state[0].temp,lamp_state.modul_state[1].temp,lamp_state.modul_state[2].temp,lamp_state.modul_state[3].temp,
          lamp_state.modul_state[4].temp,lamp_state.modul_state[5].temp,lamp_state.modul_state[6].temp);
          len_buf_list = strlen(buf_list);
          netconn_write(conn1, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);      
          vTaskDelay(100);   
         }
        else if (strncmp((char const *)buf,"GET /content1.html",18)==0)
         {
           sprintf(buf_list,"%s%s%s  Текущая мощность=%d: Текущая состояние ON/OFF %d : Текущий цвет %d",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,lamp_state.lamp_power,Hi_DCDC.stop_off,lamp_state.color_index);
          len_buf_list = strlen(buf_list);
          netconn_write(conn1, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);      
          vTaskDelay(100);  
         }
         
   //   else if (strncmp((char const *)buf,"GET /data_strim.html",17)==0)
    //        {
//              page_index=2;
//              len_buf_list=costr_logs((char*)buf_list);
//              status=netconn_write(conn1, (const unsigned char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
//            //  vTaskDelay(100);
////              len_buf_list=sizeof((uint8_t*)http_html_hdr);
////              memcpy(buf_list,http_html_hdr,len_buf_list);
////              netconn_write(conn1, (const unsigned char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
//              
//             // fs_close(&file);
    //        }
       else if((strncmp(buf, "GET /index.html", 15) == 0)||(strncmp(buf, "GET / HTTP/1.1", 14) == 0))
        {
         page_index=0;
         
          len_buf_list=costr_page1((char*)buf_list);
          status=netconn_write(conn1, (const unsigned char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
        }
       }
       else 
       {
         if ((buflen >=5) && (strncmp(buf, "POST /", 5) == 0))
        {  
          
              parser_post(buf,buflen,page_index);
              if (page_index==0)
              {
               len_buf_list=costr_page1((char*)buf_list);
               netconn_write(conn1, (const unsigned char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
              }
              if (page_index==1)
              {
              len_buf_list=costr_settings((char*)buf_list);
              netconn_write(conn1, (const unsigned char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
              }
              
              if (page_index==2)
              {
              len_buf_list=costr_logs((char*)buf_list);
              netconn_write(conn1, (const unsigned char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
              //fs_close(&file);
              }
              
              
              if (page_index==3)
              {
              fs_open(&file, "/img/404.gif"); 
              netconn_write(conn1, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
              fs_close(&file);
              }
          
        }
      }  
    
    
    }
    else
      {
         flag_logon=0;     
      }
      
  }
  
  }
  
  vPortFree(buf_list);  
  netconn_close(conn1);
  
  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
  
    netbuf_delete(inbuf);
  /* delete connection */
  netconn_delete(conn1);
}
static void http_server_netconn_thread(void *arg)
{ 
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  //for(;;)
 // {
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!= NULL)
  {
    //  conn = netconn_new(NETCONN_TCP);
    err=netconn_bind(conn, IP_ADDR_ANY, 80);
//    /* Bind to port 80 (HTTP) with default IP address */
//    err = netconn_bind(conn, NULL, 80);
    
    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);
  
      while(1) 
      {
        /* accept any icoming connection */
        accept_err = netconn_accept(conn, &newconn);
        if(accept_err == ERR_OK)
        {
          /* serve connection */
          http_server_netconn_serve(newconn);

        
        }
      }
    }
  }
}
//
///** The main function, never returns! */
//static void
//http_server_netconn_thread(void *arg)
//{
//   LWIP_UNUSED_ARG(arg);
//  struct netconn *conn, *newconn;
//  err_t err;
// 
//  
//  /* Create a new TCP connection handle */
//  /* Bind to port 80 (HTTP) with default IP address */
//#if LWIP_IPV6
//  conn = netconn_new(NETCONN_TCP_IPV6);
//  netconn_bind(conn, IP6_ADDR_ANY, 80);
//#else /* LWIP_IPV6 */
//  conn = netconn_new(NETCONN_TCP);
//  netconn_bind(conn, IP_ADDR_ANY, 80);
//#endif /* LWIP_IPV6 */
// // LWIP_ERROR("http_server: invalid conn", (conn != NULL), return;);
//    netconn_listen(conn);
//     for(;;)
//  { 
// 
//  /* Put the connection into LISTEN state */
//
//  
//  do {
//    err = netconn_accept(conn, &newconn);
//    if (err == ERR_OK) 
//    {
//      http_server_netconn_serve(newconn);
//      
//      netconn_delete(newconn);
//    }
//  } while(err == ERR_OK);
//  
////  LWIP_DEBUGF(HTTPD_DEBUG,
////    ("http_server_netconn_thread: netconn_accept received error %d, shutting down",
////    err));
////  netconn_close(conn);
////  netconn_delete(conn);
//  
//    vTaskDelay(100);
//  }
//}

/** Initialize the HTTP server (start its thread) */
void http_server_netconn_init(void)
{
  sys_thread_new("http_server_netconn", http_server_netconn_thread, NULL,4*1024, tskIDLE_PRIORITY+1);
}

#endif /* LWIP_NETCONN*/
