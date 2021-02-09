/**
  ******************************************************************************
  * @file    httpd_cg_ssi.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011
  * @brief   Webserver SSI and CGI handlers
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/


#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "main.h"

#include <string.h>
#include <stdlib.h>

//tSSIHandler ADC_Page_SSI_Handler;

RTC_TimeTypeDef httpdRTC_TimeStructure;
RTC_DateTypeDef httpdRTC_DateStructure;
extern uint8_t LastPitanie;
extern uint16_t LogWrAddr;
extern struct LogStruct Log[];
extern struct LogID LID;
extern uint8_t LastStatusPrivod;
extern char const* EventText[];
extern char const TextYes[];
extern char const TextNo[];
extern uint32_t LIDMaxAddr;
struct LogID tmpLID;
volatile uint32_t Index=0;

/* we will use character "t" as tag for CGI */
char const* TAGS[]={"t","a","b","c","d","e","f","g","h","i","k","l","m"};


/* CGI handler for LED control */ 
const char * SetDateTime_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char * ClearArchive_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char * LoadArchive_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

int DYNReadHandler(char * pBuffer,const char *uri,int MaxSize);
int DYNSizeHandler(const char *uri);

/* Html request for "/leds.cgi" will start LEDS_CGI_Handler */
const tCGI DATE_CGI={"/setdatetime.cgi", SetDateTime_CGI_Handler};
const tCGI CLRARC_CGI={"/cleararchive.cgi", ClearArchive_CGI_Handler};
/* Cgi call table, only one CGI used */
tCGI CGI_TAB[2];

const char DYNName1[]={"/Archive.bin"};
const char *DYNNames[]={DYNName1};
const tDYN DYNHandler={DYNReadHandler,DYNSizeHandler,DYNNames,1};


//Возвращает общий размер динамического фйала
int DYNSizeHandler(const char *uri)
{
	if (strcmp(uri, DYNName1) == 0)
	{
		memcpy(&tmpLID,&LID, sizeof(struct LogID));
		Index=0;
		return GetArchiveSize(&LID)*sizeof(struct LogStruct);		
	}
	return 0;
}
//Функция чтения/генерации динамического файла в буфер
int DYNReadHandler(char * pBuffer,const char *uri,int MaxSize)
{
	int tmp;
	if (strcmp(uri, DYNName1) == 0)
	{
		tmp=GetArchiveElements(Index,(struct LogStruct*)pBuffer,&tmpLID,MaxSize/sizeof(struct LogStruct));
		Index+=tmp;
		return tmp*sizeof(struct LogStruct); 	
	}
	return 0;
}
u16_t CurrentStatus_Handler(int iIndex, char *pcInsert, int iInsertLen)
{
  /* We have only one SSI handler iIndex = 0 */
  uint8_t Stat=(LogWrAddr-iIndex)&LogSizeMask;
  if (iIndex ==0)
  {
    RTC_GetTime(RTC_Format_BIN, &httpdRTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &httpdRTC_DateStructure);

	if(!LastStatusPrivod)
	{
		if(LastPitanie)
		{
			Stat=0;
		}
		else
		{
			Stat=1;
		}
	}
	else 
	{
		Stat=4;
	}
    return sprintf(pcInsert,"20%02d.%02d.%02d  %02d:%02d.%02d %s <br/> LastAddr=%d, Overflow=%d, MaxAddr=%d",httpdRTC_DateStructure.RTC_Year,httpdRTC_DateStructure.RTC_Month,httpdRTC_DateStructure.RTC_Date,
	httpdRTC_TimeStructure.RTC_Hours,httpdRTC_TimeStructure.RTC_Minutes,httpdRTC_TimeStructure.RTC_Seconds,EventText[Stat],(uint32_t)LID.LastAddr,LID.Overflow,LIDMaxAddr);
  }else if((iIndex>0)&&(iIndex<10))
  {
  	if(Log[Stat].Event==0xFF)
		return 0;
  	return sprintf(pcInsert,"20%02d.%02d.%02d  %02d:%02d.%02d %s<br />",Log[Stat].Date.RTC_Year,Log[Stat].Date.RTC_Month,Log[Stat].Date.RTC_Date,
	Log[Stat].Time.RTC_Hours,Log[Stat].Time.RTC_Minutes,Log[Stat].Time.RTC_Seconds,EventText[Log[Stat].Event]);
  }
  else if (iIndex==11)
  {
  	return sprintf(pcInsert,"%d",GetArchiveSize(&LID));
  }
  else if (iIndex==12)
  {
  	if(LID.Overflow)
	{
		memcpy(pcInsert,TextYes,4);
		return 4;
	}
	else
	{
		memcpy(pcInsert,TextNo,6);
		return 6;
	}
  }
  return 0;
}

/**
  * @brief  CGI handler for LEDs control 
  */
const char * SetDateTime_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  uint32_t i=0;
  /* We have only one SSI handler iIndex = 0 */
  if (iIndex==0)
  {
    
    /* Check cgi parameter : example GET /setdatetime.cgi?led=2&led=4 */
    for (i=0; i<iNumParams; i++)
    {
      if (strcmp(pcParam[i] , "Year")==0)   
      {
        httpdRTC_DateStructure.RTC_Year=atoi(pcValue[i])%100;
      }
	  if (strcmp(pcParam[i] , "Day")==0)   
      {
        httpdRTC_DateStructure.RTC_Date=atoi(pcValue[i]);
      }
	  if (strcmp(pcParam[i] , "Month")==0)   
      {
        httpdRTC_DateStructure.RTC_Month=atoi(pcValue[i]);
      }
	  RTC_SetDate(RTC_Format_BIN, &httpdRTC_DateStructure);
	  //
	  if (strcmp(pcParam[i] , "Hour")==0)   
      {
        httpdRTC_TimeStructure.RTC_Hours=atoi(pcValue[i]);
      }
	  if (strcmp(pcParam[i] , "Minutes")==0)   
      {
        httpdRTC_TimeStructure.RTC_Minutes=atoi(pcValue[i]);
      }
	  if (strcmp(pcParam[i] , "Seconds")==0)   
      {
        httpdRTC_TimeStructure.RTC_Seconds=atoi(pcValue[i]);
      }
	  RTC_SetTime(RTC_Format_BIN, &httpdRTC_TimeStructure);
    }
  }
  /* uri to send after cgi call*/
    
  return "/config.html";  
}
/**
  * @brief  CGI handler for LEDs control 
  */
const char * ClearArchive_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  /* We have only one SSI handler iIndex = 1 */
  if (iIndex==1)
  {
    LogWrAddr=0;
	memset(Log,0xFF,sizeof(struct LogStruct)*LogSize);
	LID.Overflow=0;
	LID.LastAddr=(struct LogStruct*)sizeof(struct LogID);
	sFLASH_WriteBuffer((uint8_t*)&LID,LogIDAddr,sizeof(LID));

  }
  /* uri to send after cgi call*/
  return "/config_arch.shtml";  
}

/**
 * Initialize SSI handlers
 */
void httpd_ssi_init(void)
{  
  /* configure SSI handlers */
  http_set_ssi_handler(CurrentStatus_Handler, (char const **)TAGS, 13);
}

/**
 * Initialize CGI handlers
 */
void httpd_cgi_init(void)
{ 
  /* configure CGI handlers (LEDs control CGI) */
  CGI_TAB[0] = DATE_CGI;
  CGI_TAB[1] = CLRARC_CGI;
  http_set_cgi_handlers(CGI_TAB, 2);
}
/**
 * Initialize CGI handlers
 */
void httpd_dyn_init(void)
{ 
  http_set_dyn_handlers(&DYNHandler);
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
