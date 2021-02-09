
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "stdio.h"
#include "stdlib.h"
//#include "html.h"
#include "httpd_structs.h"
#include "fs.h"
#include "DLib_Config_Full.h"
#include "httpserver-netconn.h"
#include "html_page.h"
#include "heap_3.h"
#if LWIP_NETCONN

#ifndef HTTPD_DEBUG
#define HTTPD_DEBUG         LWIP_DBG_OFF
#endif
int fr;

uint8_t page_index;



//////static void
//////http_server_netconn_serve(struct netconn *conn)
//////{
//////  struct netbuf *inbuf;
//////  char *buf;
//////  u16_t buflen;
//////  err_t err;
//////  
//////  /* Read the data from the port, blocking if nothing yet there. 
//////   We assume the request (the part we care about) is in one netbuf */
//////  err = netconn_recv(conn, &inbuf);
//////  
//////  if (err == ERR_OK) {
//////    netbuf_data(inbuf, (void**)&buf, &buflen);
//////    
//////    /* Is this an HTTP GET command? (only check the first 5 chars, since
//////    there are other formats for GET, and we're keeping it very simple )*/
//////    if (buflen>=5 &&
//////        buf[0]=='G' &&
//////        buf[1]=='E' &&
//////        buf[2]=='T' &&
//////        buf[3]==' ' &&
//////        buf[4]=='/' ) {
//////       /* Send the HTML header 
//////             * subtract 1 from the size, since we dont send the \0 in the string
//////             * NETCONN_NOCOPY: our data is const static, so no need to copy it
//////       */
//////      netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);
//////      
//////      /* Send our HTML page */
//////      netconn_write(conn, html_2, sizeof(html_2)-1, NETCONN_NOCOPY);
//////      
//////      /* Send our HTML page */
//////   //   netconn_write(conn, http_index_html, sizeof(http_index_html)-1, NETCONN_NOCOPY);
//////    }
//////    
//////  }
//////  /* Close the connection (server closes in HTTP) */
//////  netconn_close(conn);
//////  
//////  /* Delete the buffer (netconn_recv gives us ownership,
//////   so we have to make sure to deallocate the buffer) */
//////  netbuf_delete(inbuf);
//////}

typedef struct post_data_t
  {
    char name[32];    
    char data[32];    
    //uint8_t len_par;
  }post_data_t;


void parser_post(char* buf_in,uint16_t buf_in_len,uint8_t index)
{
uint16_t ct_index,start_pars;
uint16_t start_par;
post_data_t elem_post_data;

memset(elem_post_data.data,0,32);
memset(elem_post_data.name,0,32);

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
      //elem_post_data=&post_mess[index_sec];
      if (buf_in[ct_index]=='=')
        {
          
        //  memset(elem_post_data->name,0,32);
          memcpy(elem_post_data.name,(uint8_t*)(buf_in+start_par),(ct_index-start_par));
          start_par=ct_index+1;
        }
      if ((buf_in[ct_index]=='&')||(ct_index==buf_in_len))
        {
         // memset(elem_post_data->data,0,32);
          memcpy(elem_post_data.data,(uint8_t*)(buf_in+start_par),(ct_index-start_par));
          start_par=ct_index+1;
          
//          elem_post_data->len_par=index_sec+1;
//          index_sec++;
          
        }
    }
  
}



static void http_server_netconn_serve(struct netconn *conn) 
{
  //post_data_t post_data[32];
  
  struct netbuf *inbuf;
  err_t recv_err;
  char* buf;
  u16_t buflen;
  struct fs_file file;
  char buf_list[5000]={0};
  uint16_t len_buf_list;

  
  /* Read the data from the port, blocking if nothing yet there. 
   We assume the request (the part we care about) is in one netbuf */
  recv_err = netconn_recv(conn, &inbuf);
  
  if (recv_err == ERR_OK)
  {
    if (netconn_err(conn) == ERR_OK) 
    {
      netbuf_data(inbuf, (void**)&buf, &buflen);
    
      /* Is this an HTTP GET command? (only check the first 5 chars, since
      there are other formats for GET, and we're keeping it very simple )*/
      
      if ((buflen >=5) && (strncmp(buf, "GET /", 5) == 0))
      {
        /* Check if request to get ST.gif */ 
        if (strncmp((char const *)buf,"GET /img/logo.gif",17)==0)
        {
          recv_err=fs_open(&file, "/img/logo.gif");           
          netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
          fs_close(&file);
        }          
      else if (strncmp((char const *)buf,"GET /settings.html",17)==0)
            {
              page_index=1;
              len_buf_list=costr_settings((char*)buf_list);
              netconn_write(conn, (const unsigned char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
              fs_close(&file);
            }
      else if (strncmp((char const *)buf,"GET /data_strim.html",17)==0)
            {
              page_index=2;
              fs_open(&file, "/data_strim.html");
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
              fs_close(&file);
            }
      else if((strncmp(buf, "GET /index.html", 15) == 0)||(strncmp(buf, "GET / HTTP/1.1", 14) == 0))
        {
         page_index=0;
          len_buf_list=costr_page1((char*)buf_list);
          netconn_write(conn, (const unsigned char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
          fs_close(&file);
        }
        else 
        {
          /* Load Error page */
          fs_open(&file, "/img/404.gif"); 
          netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
          fs_close(&file);
        }
      }
    else
      {
        if ((buflen >=5) && (strncmp(buf, "POST /", 5) == 0))
        {
      
          
          parser_post(buf,buflen,page_index);
        }
      }      
    }
    
    
      
  }
  /* Close the connection (server closes in HTTP) */
  netconn_close(conn);
  
  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
  netbuf_delete(inbuf);
}

/** The main function, never returns! */
static void
http_server_netconn_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err;
  LWIP_UNUSED_ARG(arg);
  
  /* Create a new TCP connection handle */
  /* Bind to port 80 (HTTP) with default IP address */
#if LWIP_IPV6
  conn = netconn_new(NETCONN_TCP_IPV6);
  netconn_bind(conn, IP6_ADDR_ANY, 80);
#else /* LWIP_IPV6 */
  conn = netconn_new(NETCONN_TCP);
  netconn_bind(conn, IP_ADDR_ANY, 80);
#endif /* LWIP_IPV6 */
  LWIP_ERROR("http_server: invalid conn", (conn != NULL), return;);
  
  /* Put the connection into LISTEN state */
  netconn_listen(conn);
  
  do {
    err = netconn_accept(conn, &newconn);
    if (err == ERR_OK) {
      http_server_netconn_serve(newconn);
      netconn_delete(newconn);
    }
  } while(err == ERR_OK);
  LWIP_DEBUGF(HTTPD_DEBUG,
    ("http_server_netconn_thread: netconn_accept received error %d, shutting down",
    err));
  netconn_close(conn);
  netconn_delete(conn);
}

/** Initialize the HTTP server (start its thread) */
void http_server_netconn_init(void)
{
  sys_thread_new("http_server_netconn", http_server_netconn_thread, NULL,8*1024, tskIDLE_PRIORITY+1);
}

#endif /* LWIP_NETCONN*/
