/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#include "frame_decod.h"
#include "udpecho.h"

#include "lwip/opt.h"

#if LWIP_NETCONN

#include "lwip/api.h"
#include "lwip/sys.h"


/*-----------------------------------------------------------------------------------*/
static void udpecho_thread(void *arg)
{
  struct netconn *conn;
    struct netconn *conn1;
  struct netbuf *buf;
 // uint8_t data_tx_udp[1024+13]={0};
  char buffer[4096];
  err_t err;
  LWIP_UNUSED_ARG(arg);
  uint32_t out_len=0;

#if LWIP_IPV6
  conn = netconn_new(NETCONN_UDP_IPV6);
  netconn_bind(conn, IP6_ADDR_ANY, 2048);
#else /* LWIP_IPV6 */
  conn = netconn_new(NETCONN_UDP);
  netconn_bind(conn, IP_ADDR_ANY, 2048);
  
  
  conn1 = netconn_new(NETCONN_UDP);
  netconn_bind(conn1, IP_ADDR_ANY,161); 
#endif /* LWIP_IPV6 */
  LWIP_ERROR("udpecho: invalid conn", (conn != NULL), return;);

#if LWIP_IGMP /* Only for testing of multicast join*/
{
  #include "lwip\netif.h"

  ip4_addr_t multiaddr;
  IP4_ADDR(&multiaddr, 224, 5, 6, 7);

  err = netconn_join_leave_group(conn, &multiaddr, &netif_default->ip_addr, NETCONN_JOIN);
  LWIP_ERROR("udpecho: join group is failed", (err == ERR_OK), return;);
}
#endif

  while (1) {
    err = netconn_recv(conn, &buf);
    if (err == ERR_OK) {
      /*  no need netconn_connect here, since the netbuf contains the address */
      if(netbuf_copy(buf, buffer, sizeof(buffer)) != buf->p->tot_len)
        {
          LWIP_DEBUGF(LWIP_DBG_ON, ("netbuf_copy failed\n"));
        }
      else
        {
          decoding_frame(buffer,buf->p->tot_len,0,buf->p->payload,&out_len);
          buf->p->tot_len=out_len;
          buf->p->len =out_len;
          netconn_send(conn, buf);
//  buffer[buf->p->tot_len] = '\0';
        //  err = netconn_send(conn, buf);
          if(err != ERR_OK)
            {
              LWIP_DEBUGF(LWIP_DBG_ON, ("netconn_send failed: %d\n", (int)err));
            }
          else
            {
            //  LWIP_DEBUGF(LWIP_DBG_ON, ("got %s\n", buffer));

            }
        }
      netbuf_delete(buf);
    }
    
    
    
    
   err = netconn_recv(conn1, &buf);
    if (err == ERR_OK) {
      /*  no need netconn_connect here, since the netbuf contains the address */
      if(netbuf_copy(buf, buffer, sizeof(buffer)) != buf->p->tot_len)
        {
          LWIP_DEBUGF(LWIP_DBG_ON, ("netbuf_copy failed\n"));
        }
      else
        {
          decoding_frame(buffer,buf->p->tot_len,0,buf->p->payload,&out_len);
          buf->p->tot_len=out_len;
          buf->p->len =out_len;
          netconn_send(conn, buf);
//  buffer[buf->p->tot_len] = '\0';
        //  err = netconn_send(conn, buf);
          if(err != ERR_OK)
            {
              LWIP_DEBUGF(LWIP_DBG_ON, ("netconn_send failed: %d\n", (int)err));
            }
          else
            {
            //  LWIP_DEBUGF(LWIP_DBG_ON, ("got %s\n", buffer));

            }
        }
      netbuf_delete(buf);
    }
    
    
    
    
    
//       vTaskDelay(5);
  }
}
/*-----------------------------------------------------------------------------------*/
void udpecho_init(void)
{
  sys_thread_new("udpecho_thread", udpecho_thread, NULL, 4000, TCPIP_THREAD_PRIO);
//  sys_thread_new("udp1", udp1, NULL, 400, TCPIP_THREAD_PRIO);
}

#endif /* LWIP_NETCONN */
