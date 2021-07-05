#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "html_page.h"
#include "hard_init.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"

#define TO_HEX(i) (i <= 9 ? '0' + i : 'A' - 10 + i)
//#include "flash_if.h"
uint8_t ct_logoff_time;
uint8_t flag_logon=0;


unsigned char key_http[30]="asfasdvas";
int key_http_len = 10;
char HTTP401[]={0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x31,0x20,0x34,0x30,0x31,0x20,0x55,0x6e,0x61,   //HTTP/1.1 401 Una
0x75,0x74,0x68,0x6f,0x72,0x69,0x7a,0x65,0x64,0x0d,0x0a,0x57,0x57,0x57,0x2d,0x41,   //uthorized..WWW-A
0x75,0x74,0x68,0x65,0x6e,0x74,0x69,0x63,0x61,0x74,0x65,0x3a,0x20,0x42,0x61,0x73,   //uthenticate: Bas
0x69,0x63,0x20,0x72,0x65,0x61,0x6c,0x6d,0x3d,0x22}; 

char HTTP401end[]={0x22,0x0d,0x0a,0x0d,0x0a};

static const uint8_t bmp_base[]={0x42,0x4d,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
0x00,0x00,0x08,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,
0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
0x00,0x40,0x20,0xe0,0x40,0xc0,0x20,0xc0,0x40,0x40,0xc0,0x40,0xa0,0x00,0xff,0xff,
0xf0,0xca,0xa6,0x80,0x80,0x80};
const char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
const uint8_t http_201_hdr[] = {0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x31,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
0x0a};
  //char * data[]="<a href=\"settings.html\" target=\"_self\" rel=\"nofollow\">????????? &emsp; </a>";/*<body onload=\"onload()\"\>*/
//static const char http_html_start_constr[] = "\<!DOCTYPE html>\r\n <body onload=\"onload()\">\n\r<a href=\"http:\/\/www.csort.ru/\"></a>\n\r<img src=\"/img/logo.gif\" height=\"63\" width=\"270\" border=\"0\" alt=\"csort logo\" title=\"Csort\">\n\r <html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n\r";
//          "<script\>"
//
//	  "var xhr\;"
//           "var idTimer1\";"
//	   "function onload(){"
//	   "xhr = new (XMLHttpRequest);"
//	   "}\n\r"
//           
//	   "function swich1(){"
//           "xhr.open(\"GET\", \"swich?c=1\", true);"
//	   "xhr.responseType = \"text\";"
//	   "xhr.send(null);"
//	   "}\n\r"	
//	  "function swich2(){"
//	  "xhr.open(\"GET\", \"swich?c=2\", true);"
//	  "xhr.responseType = \"text\";"
//	  "xhr.send(null);"
//	  "}\n\r"
//	  "function swich3(){"
//	   "xhr.open(\"GET\", \"swich?c=3\", true);"
//	    "xhr.responseType = \"text\";"
//	    "xhr.send(null);"
//	  "}\n\r"
//"</script>";

static const char http_logs_hdr[]="<style> textarea { width: 90%;height:200px;resize:none;}</style><center><form action>";
//
//<p>Скопируйте приведённый текст и вставьте его в 
//  поле запроса пароля.</p>

static const char http_logs_area[]="<p><textarea name=\"comment\" align=\"center\" readonly rows=\"10\" cols=\"2\" align=\"center\" disabled >";
static const char http_logs_end[] ="</textarea></p>  </center>"; 
    
    
    
    

static const char http_html_start_constr1[] = "<!DOCTYPE html>"
"<body onload=\"onload()\">"
"<img src=\"img/logo.gif\" alt=\"undefined\" height=\"63\" width=\"270\">"
"<html>"
"<head>"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
"<script>"
"	  var xhr;"
"           var idTimer1;"
"	   function onload(){"
"	   xhr = new (XMLHttpRequest);"
"	   }"           
"	   function swich1(){"
"          xhr.open(\"GET\", \"swich?c=1\", true);"
"	   xhr.responseType = \"text\";"
"	   xhr.send(null);"
"	   }"	
"	  function swich2(){"
"	  xhr.open(\"GET\", \"swich?c=2\", true);"
"	  xhr.responseType = \"text\";"
"	  xhr.send(null);"
"	  }"
"	  function swich3(){"
"	    xhr.open(\"GET\", \"swich?c=3\", true);"
"	    xhr.responseType = \"text\";"
"	    xhr.send(null);"
"	  }"
"</script>";

static const char http_html_start_constr_dy[] = 
          "\<!DOCTYPE HTML>\n\r"
          "<body onload=\"onload()\"\>\n\r"
          "<html>\n\r"
          "<head>\n\r"
          "<title>Csort</title>\n\r"
          "<a href=\"http:\/\/www.csort.ru/\"><img src=\"img/logo.gif\" height=\"63\" width=\"270\" border=\"0\" alt=\"csort logo\" title=\"200 OK\"></a>\n\r"
          "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
          "\n\r\<script\>\n\r"
	  "var xhr,xhr1\;\n\r "
           "var idTimer1,idTimer2\;\n\r "
	   "function onload(){\n\r"
	   "xhr = new (XMLHttpRequest); \n\r" 
           "xhr1 = new (XMLHttpRequest); \n\r"   
	   "}\n\r"
           
             
           "setInterval(function Timer1(){\n\r"
           "xhr.open(\"GET\", \"content.html?r=\" + Math.random(), true); \n\r"
	   "xhr.responseType = \"text\"; \n\r"
	   "xhr.onload = function (oEvent){\n\r"
	   " document.getElementById('information').innerHTML = xhr.response; \n\r"
	   "}\n\r"
           "xhr.send(null); \n\r"
    	   "idTimer1 = setTimeout(\"Timer1()\", 3000); \n\r"
	   "},3000); \n\r"
             
             
           "setInterval(function Timer2(){\n\r"
           "xhr1.open(\"GET\", \"content1.html?r=\" + Math.random(), true); \n\r"
	   "xhr1.responseType = \"text\"; \n\r"
	   "xhr1.onload = function (oEvent){\n\r"
	   " document.getElementById('information1').innerHTML = xhr1.response; \n\r"
	   "}\n\r"            
	   "xhr1.send(null); \n\r"
    	   "idTimer2 = setTimeout(\"Timer2()\", 900); \n\r"
	   "},900); \n\r"
"</script>"
;

static const char http_html_start_constr[] = "\<!DOCTYPE html> <body onload=\"onload()\"\>  <a href=\"http:\/\/www.netping.ru/\"><img src=\"img/netping.gif\" height=\"59\" width=\"169\" border=\"0\" alt=\"netping logo\" title=\"200 OK\"></a> <html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
         "\<script\>"
	  "var xhr\;"
           "var idTimer1\";"
	   "function onload(){"
	   "xhr = new (XMLHttpRequest);"
	   "}"

"</script>"
;

static const char http_html_style[] = 
"<title>Fon DRV</title>"
"  <style>\n\r"
"    lf {"
	"font-family: Courier;" /* Гарнитура шрифта */
	"src: url(fonts/Courier.ttf);" /* Путь к файлу со шрифтом */
///////////////	"font-family: Courier, 'Comic Sans MS', cursive;"
        "text-align: center;"
          "font-size: 16pt;"
    "}\n\r"
    "h2 {"
	"font-size: 14pt;"
        "text-align: center;"
    "}\n\r"
      
     "h3 {"
       // "text-decoration:  underline 1px solid green;"
      //  "border-bottom: 2px  solid Seagreen;"/* Параметры линии под текстом */
        "font-weight: normal;" /* Убираем жирное начертание */
      //  "padding-bottom: 1px;" /* Расстояние от текста до линии */          
	"font-size: 12pt;"
        "text-align: center;"
      "}\n\r"      
   "display: inline-block;"
  "</style>"
    "</head>";
//uint8_t pass_compar(char* in_buf)
//{
//uint8_t ct_pass;
//uint8_t len_login,len_pass;
//char login_in[32] = {0};
//char pass_in[32]= {0};
//for(ct_pass=23;ct_pass<120;ct_pass++)
//  {
//    if (((char)(*(in_buf+ct_pass)))=='&')
//      {
//       len_login = ct_pass-23;
//      }
//    if (in_buf[ct_pass]==' ')
//      {
//        len_pass=ct_pass-23-len_login-10;
//        ct_pass=120;
//      }
//   
//  }
//    memcpy(login_in,(char*)(in_buf+23),len_login);
//    memcpy(pass_in,(char*)(in_buf+23+len_login+10),len_pass);
//    if (((strncmp((char*)FW_data.V_LOGIN,(char*)login_in,len_login))==0)&&((strncmp((char*)FW_data.V_PASSWORD,(char*)pass_in,len_pass))==0))
//    {
//      return 1;
//    }
//return 0;
//}


void ltoa(long int n,char *str,uint8_t arg)
{
unsigned long i;
unsigned char j,p;
i=1000000000L;
p=0;


if(n<0)
{
n=-n;
*str++='-';
}
do
{
j=(unsigned char) (n/i);
if (j || p || (i==1))
{
*str++=j+'0';
p=1;
}
n%=i;
i/=arg;
}
while (i!=0);
*str=0;
}

void  ltoa_hex (long int x,char *res )
{
    res[0] = TO_HEX(((x & 0xF00000000) >> 32));
    res[1] = TO_HEX(((x & 0xF0000000) >> 28));
    res[2] = TO_HEX(((x & 0xF000000) >> 24));   
    res[3] = TO_HEX(((x & 0xF00000) >> 20));
    res[4] = TO_HEX(((x & 0xF0000) >> 16));
    res[5] = TO_HEX(((x & 0xF000) >> 12));   
    res[6] = TO_HEX(((x & 0x0F00) >> 8));
    res[7] = TO_HEX(((x & 0x00F0) >> 4));
    res[8] = TO_HEX((x & 0x000F));
    res[9] = '\0';
    
  do  
    {
      res[0]=res[1];    
      res[1]=res[2];
      res[2]=res[3];
      res[3]=res[4];
      res[4]=res[5];
      res[5]=res[6];
      res[6]=res[7];
      res[7]=res[8];
      res[8]=res[9];
      res[9]=res[10];
    }while( res[0]==0x30);
}



uint16_t set_open_block (char* str1,char* name)
{ 

uint16_t len;
sprintf(str1,"<%s>\n\r",name);
return len;
}








uint16_t set_2row_str (char* str1,char* name,char* name2,uint16_t height, uint16_t width,uint16_t width2)
{ 
////  set_open_block(str2,"tr style=\"height: 20px;\"");
////  strcat(str1,str2);
////  
////  set_open_block(str2,"td style=\"width: 60%; text-align: center; height: 20px;\">Параметр&nbsp;</td");
////  strcat(str1,str2);
////  
////  set_open_block(str2,"td style=\"width: 20%; text-align: center; height: 20px;\">Значение&nbsp;</td");
////  strcat(str1,str2);
////  
////  reset_open_block(str2,"tr");
////  strcat(str1,str2);
  
uint16_t len;
sprintf(str1,"<tr><td style=\"width: %d\%; text-align: center; height: %dpx;\">%s&nbsp;</td>\n\r<td style=\"width: %d\%; text-align: center; height: %dpx;\">%s&nbsp;</td>\n\r</tr>",width,height,name,width2,height,name2);
return len;
}

uint16_t reset_open_block (char* str1,char* name)
{ 

uint16_t len;
sprintf(str1,"</%s>\n\r",name);
return len;
}
uint16_t set_space (char* str1,uint8_t n)
{ 
uint16_t len=6*n;
 memset (str1,0, len);
for(;n>0;n--)
{
  strcat(str1,"&ensp;");
}

return len;
}
uint16_t set_tab (char* str1,uint8_t n)
{ 
uint16_t len=6*n;
 memset (str1,0, len);
for(;n>0;n--)
{
  strcat(str1,"&ensp;");
}

return len;
}

uint16_t set_br (char* str1,uint8_t n)
{ 
uint16_t len=4*n;
 memset (str1,0, len);
for(;n>0;n--)
{
  strcat(str1,"<br>");
}

return len;
}

//<p style="text-align:center"><input name="Start_boot" required="required" type="checkbox" value="boot" />&ensp; <input type="submit" value="Start frimware update? Start bootloader " /></p>
//<label><input type="checkbox" name="name"> messeg</label>
 uint16_t set_checkbox (char* str1,char* name,char* value)
{ 

//<button type="button" name="Save_power" formtarget="_self" onclick="Save_Power()" >Save Power </button>	

sprintf(str1,"<label><input type=\"checkbox\" name=\"%s\"> %s</label>",name,value);

return 0;
}

 uint16_t set_checkbox_bot (char* str1,char* name,char* value,char* mess)
{ 

//<p style="text-align:center"><input name="Start_boot" required="required" type="checkbox" value="boot" />&ensp; <input type="submit" value="Start frimware update? Start bootloader " /></p>

sprintf(str1,"<p style=\"text-align:center\"><input name=\"%s\" required=\"required\" type=\"checkbox\" value=\"%s\" />&ensp; <input type=\"submit\" value=\"%s\" /></p>",name,value,mess);

return 0;
}


uint16_t set_link (char* str1,char* name,char* link)
{
  //<a href="" target="_self" rel="nofollow">&emsp; </a>
    
//char str2[64];  
uint16_t len;
sprintf(str1,"<a href=\"%s\" target=\"_self\" rel=\"nofollow\">%s&emsp; </a>\n\r",link,name);

////strcat(str1,"<a href=\"");
////
////sprintf(str2,"%s",link);
////strcat(str1,str2);
////
////strcat(str1,"\" target=\"_self\" rel=\"nofollow\">");
////
////sprintf(str2,"%s",name);
////strcat(str1,str2);
////
////strcat(str1,"&emsp; </a>");

return len;
}

uint16_t set_intext (char* str1,char* name,char* size,char* value)
{ 

///<input type="text" name="IP_addres"  size='15' value="192.168.000.122">&ensp;
///<input type="text" name=""  size='' value="">&ensp; style="text-align: center
uint16_t len;
sprintf(str1,"<input type=\"text\" name=\"%s\"  size=\"%s\" maxlength=\"%s\" value=\"%s\"style=\"text-align: center\">&ensp;\n\r",name,size,size,value);

return len;
}


uint16_t set_innum (char* str1,char* name,char* size,char* max,char* value)
{ 

///<input type="text" name="IP_addres"  size='15' value="192.168.000.122">&ensp;
///<input type="text" name=""  size='' value="">&ensp; style="text-align: center
uint16_t len;
sprintf(str1,"<input type=\"number\" name=\"%s\"  size=\"%s\" maxlength=\"%s\" min=\"0\" max=\"%s\" step=\"1\" value=\"%s\"style=\"text-align: center\">&ensp;\n\r",name,size,size,max,value);

return len;
}
uint16_t set_incolor (char* str1,char* name,char* value)
{ 
uint16_t len;
//input type=\"color\" name=\"Color_fon\" value=\"#%s\" placeholder=\"Color_fon1\"
sprintf(str1,"<input type=\"color\" name=\"%s\" value=\"#%s\" placeholder=\"%s\">&ensp;\n\r",name,value,name);

return len;
}
uint16_t set_button (char* str1,char* name,char* value)
{ 

//<button type="button" name="Save_power" formtarget="_self" onclick="Save_Power()" >Save Power </button>	

sprintf(str1,"<button type=\"button\" name=\"%s\" formtarget=\"_self\" onclick=\"%s\">%s</button>\n\r",name,value,name);

return 0;
}


 uint16_t set_submit (char* str1,char* name,char* value)
{ 

//<button type="button" name="Save_power" formtarget="_self" onclick="Save_Power()" >Save Power </button>	

sprintf(str1,"<button type=\"submit\" name=\"%s\" formtarget=\"_self\" value=\"%s\">%s</button>\n\r",name,value,name);

return 0;
}



uint32_t costr_pass(char* str1)
{
uint32_t len_out;
memset (str1,0, 5000);
memcpy(str1,(char*)(HTTP401),sizeof(HTTP401));
strcat(str1,"Name_dev");
strcat(str1,(char*)HTTP401end);
len_out=strlen(str1);        
return len_out;
}



uint32_t costr_page1(char* str1)
{
  uint32_t len;
  char str2[size_page_bufer_slave*4];
  char str3[size_page_bufer_slave*2];
  char str4[size_page_bufer_slave];
  memset (str1,0, size_page_bufer);
  //len = sizeof(http_html_start_constr);
  strcat(str1,http_html_start_constr_dy);
  
  //len = len+sizeof(http_html_style);
  strcat(str1,http_html_style);
  
  set_open_block(str2,"body");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
  set_open_block(str2,"lf");
  strcat(str1,str2);
  
  set_open_block(str2,"h4");
  strcat(str1,str2);
  
  sizeof("Драйвер лампы и фона HW2.0 ");
  strcat(str1,"Драйвер лампы и фона HW2.0 ");
  
  reset_open_block(str2,"h4");
  strcat(str1,str2);
  
  reset_open_block(str2,"lf");
  strcat(str1,str2);
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_link(str2,"Настройки ","settings.html");
  strcat(str1,str2);
  
  set_link(str2,"Текущие данные","data_strim.html");
  strcat(str1,str2);  
    
  reset_open_block(str2,"h2");
  strcat(str1,str2);

  reset_open_block(str2,"ul");
  strcat(str1,str2);
  
  set_open_block(str2,"form action='/' method='GET'");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);  
  
  set_open_block(str2,"h3");
  strcat(str1,str2);
  
  set_open_block(str2,"br");
  strcat(str1,str2);

  set_space(str2,6);
  strcat(str1,str2);  

  set_open_block(str2,"b>IP addres</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2);
  
  memset(str2,0,256);
  sprintf(str2,"<b>%d.%d.%d.%d</b>",FW_data.V_IP_CONFIG[0],FW_data.V_IP_CONFIG[1],FW_data.V_IP_CONFIG[2],FW_data.V_IP_CONFIG[3]);
  strcat(str1,str2);
  
  
  set_space(str2,6);
  strcat(str1,str2);  
  
  set_open_block(str2,"b>Mask addres</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2);
  
  memset(str2,0,256);
  sprintf(str2,"<b>%d.%d.%d.%d</b>",FW_data.V_IP_MASK[0],FW_data.V_IP_MASK[1],FW_data.V_IP_MASK[2],FW_data.V_IP_MASK[3]);
  strcat(str1,str2);

  
    
  set_space(str2,6);
  strcat(str1,str2);  
  
  set_open_block(str2,"b>IP getway</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2); 
  
  memset(str2,0,256);
  sprintf(str2,"<b>%d.%d.%d.%d</b>",FW_data.V_IP_GET[0],FW_data.V_IP_GET[1],FW_data.V_IP_GET[2],FW_data.V_IP_GET[3]);
  strcat(str1,str2);
  
  set_space(str2,6);
  strcat(str1,str2);  
  
  set_open_block(str2,"b>MAC adress</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2); 
  
  memset(str2,0,256);
  sprintf(str2,"<b>%x.%x.%x.%x.%x.%x</b>",FW_data.V_ID_MAC[0],FW_data.V_ID_MAC[1],FW_data.V_ID_MAC[2],FW_data.V_ID_MAC[3],FW_data.V_ID_MAC[4],FW_data.V_ID_MAC[5]);
  strcat(str1,str2);
  
   set_br(str2,2);
  strcat(str1,str2);
  
  memset(str2,0,256);
  sprintf(str2,"<pre id=\"information\"></pre>\n\r");
  strcat(str1,str2);
  memset(str2,0,256);
 sprintf(str2,"<pre id=\"information1\"></pre>\n\r");
  strcat(str1,str2);
   
   set_br(str2,2);
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h3");
  strcat(str1,str2);
  
  reset_open_block(str2,"ul");
  strcat(str1,str2);
  
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
    
  set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
 
  set_open_block(str2,"h2");
  strcat(str1,str2);

   
     set_br(str2,2);
  strcat(str1,str2);
  
  
  set_open_block(str2,"b>Ручная установка цвета фона</b");
  strcat(str1,str2);
  
   set_space(str2,2);
  strcat(str1,str2); 
  
  ltoa_hex(((lamp_state.led_data_r_req<<16)|(lamp_state.led_data_g_req<<8)|(lamp_state.led_data_b_req)), str3);
  len=strlen(str3);
  while(len<6)
  {
    str3[6]=str3[5];
    str3[5]=str3[4];
    str3[4]=str3[3];
    str3[3]=str3[2];
    str3[2]=str3[1];
    str3[1]=str3[0];
    str3[0]=0x30;
    len++;
  }
  
  set_incolor(str2,"Color_fon",str3);
  strcat(str1,str2);
    
  set_space(str2,2);
  strcat(str1,str2); 
  
  
  memset(str2,0,size_page_bufer_slave*4);
  sprintf(str4,"%d",lamp_state.lamp_power_req);
  set_innum(str2,"Power_lamp","5","32768",str4);
  //set_innum(str2,"Power","5","32768","0000");
  strcat(str1,str2);  
  

  
  set_checkbox_bot(str2,"Save_power","1","Сохранить");
  strcat(str1,str2);  
  
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  

    
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  
  
  
  
  
  
  
   set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
 
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
    set_br(str2,3);
  strcat(str1,str2); 
  
  set_open_block(str2,"b>Управление состоянием</b");
  strcat(str1,str2);
  
   set_space(str2,3);
  strcat(str1,str2); 
  
  // uint16_t set_checkbox_bot (char* str1,char* name,char* value,char* mess)
 // set_submit(str2,"Power_on","1");
  

  
     set_space(str2,3);
  strcat(str1,str2); 
  
  
   set_submit(str2,"Power_off","1");
  strcat(str1,str2);  
  
     set_space(str2,3);
  strcat(str1,str2); 
  
//   
//  set_checkbox_bot(str2,"Power_on","1","Power_on");
//  strcat(str1,str2);  
//  
  
  set_submit(str2,"Power_on","1");
  strcat(str1,str2);  
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  

    
  reset_open_block(str2,"form");
  strcat(str1,str2); 
  
  
  
  
  
  
  
  
  
  
  len=strlen(str1);
  
  
  
  return len;
};

////   DCDC_data_t Hi_DCDC;
////    lamp_state_t lamp_state;




uint32_t costr_settings(char* str1)
{
  uint32_t len;
  char str2[size_page_bufer_slave*4];
  char str3[size_page_bufer_slave*2];
  char str4[size_page_bufer_slave];
  memset (str1,0, size_page_bufer);

  //len = sizeof(http_html_start_constr);
  strcat(str1,http_html_start_constr1);
  
  //len = len+sizeof(http_html_style);
  strcat(str1,http_html_style);
  
//  set_open_block(str2,"body");
//  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
  set_open_block(str2,"lf");
  strcat(str1,str2);
  
  set_open_block(str2,"h4");
  strcat(str1,str2);
  
  sizeof("Драйвер лампы и фона HW2.0 ");
  strcat(str1,"Драйвер лампы и фона HW2.0 ");
  
  reset_open_block(str2,"h4");
  strcat(str1,str2);
  
  reset_open_block(str2,"lf");
  strcat(str1,str2);
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_link(str2,"Главная ","index.html");
  strcat(str1,str2);
  
  set_link(str2,"Текущие данные","data_strim.html");
  strcat(str1,str2);  
    
  reset_open_block(str2,"h2");
  strcat(str1,str2);

  reset_open_block(str2,"ul");
  strcat(str1,str2);
  
  set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
  
  set_open_block(str2,"h3");
  strcat(str1,str2);
  
  set_open_block(str2,"br");
  strcat(str1,str2);
  
  
 set_space(str2,6);
  strcat(str1,str2);  

  
////  
////  set_open_block(str2,"b> Имя устройства </b");
////  strcat(str1,str2);
////  
////  set_space(str2,2);
////  strcat(str1,str2);
////  
////  memset(str2,0,256);
////  sprintf(str2,"<b>%s</b>",FW_data.V_Name_dev);
////  strcat(str1,str2);
////
////  set_space(str2,6);
////  strcat(str1,str2);
  

  set_open_block(str2,"b>IP addres</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2);
  
  memset(str2,0,size_page_bufer_slave*4);
  sprintf(str2,"<b>%d.%d.%d.%d</b>",FW_data.V_IP_CONFIG[0],FW_data.V_IP_CONFIG[1],FW_data.V_IP_CONFIG[2],FW_data.V_IP_CONFIG[3]);
  strcat(str1,str2);

  set_space(str2,6);
  strcat(str1,str2);  

  
  
  
   set_open_block(str2,"b>Mask addres</b");
  strcat(str1,str2);
  
  
  
  set_space(str2,2);
  strcat(str1,str2);
  
  memset(str2,0,size_page_bufer_slave*4);
  sprintf(str2,"<b>%d.%d.%d.%d</b>",FW_data.V_IP_MASK[0],FW_data.V_IP_MASK[1],FW_data.V_IP_MASK[2],FW_data.V_IP_MASK[3]);
  strcat(str1,str2);

  
    
  set_space(str2,6);
  strcat(str1,str2);  
  
  set_open_block(str2,"b>IP getway</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2); 
  
  memset(str2,0,size_page_bufer_slave*4);
  sprintf(str2,"<b>%d.%d.%d.%d</b>",FW_data.V_IP_GET[0],FW_data.V_IP_GET[1],FW_data.V_IP_GET[2],FW_data.V_IP_GET[3]);
  strcat(str1,str2);
  
  set_space(str2,6);
  strcat(str1,str2);  
  
  set_open_block(str2,"b>MAC adress</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2); 
  
  memset(str2,0,size_page_bufer_slave*4);
  sprintf(str2,"<b>%x.%x.%x.%x.%x.%x</b>",FW_data.V_ID_MAC[0],FW_data.V_ID_MAC[1],FW_data.V_ID_MAC[2],FW_data.V_ID_MAC[3],FW_data.V_ID_MAC[4],FW_data.V_ID_MAC[5]);
  strcat(str1,str2);
  
  
//  len=len+set_intext(str2,"IP_getway","15","192.168.000.001");
//  strcat(str1,str2);  
  
  reset_open_block(str2,"h3");
  strcat(str1,str2);
  
  reset_open_block(str2,"ul");
  strcat(str1,str2);
  
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  

  
  set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
 
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_br(str2,2);  
  strcat(str1,str2);
  
  set_open_block(str2,"b>Параметры драйвера</b");
  strcat(str1,str2);

  
  set_br(str2,2);  
  strcat(str1,str2);
  
  
  set_open_block(str2,"table border=\"1\" style=\"width: 80%; margin: auto;\"><tbody");
  strcat(str1,str2);
  
  
  memset(str2,0,size_page_bufer_slave*4);
  sprintf(str4,"%s",FW_data.V_Name_dev);
  set_intext(str3,"Name_dev","15",str4);
  set_2row_str (str2,"Имя устройства  ",str3,10,60,20);
  strcat(str1,str2);

  
  memset(str2,0,size_page_bufer_slave*4);
  sprintf(str4,"%d.%d.%d.%d",FW_data.V_IP_CONFIG[0],FW_data.V_IP_CONFIG[1],FW_data.V_IP_CONFIG[2],FW_data.V_IP_CONFIG[3]);
  set_intext(str3,"IP_addr","15",str4);
  set_2row_str (str2,"IP адрес ",str3,10,60,20);
  strcat(str1,str2);
  
  
    memset(str2,0,size_page_bufer_slave*4);
  sprintf(str4,"%d.%d.%d.%d",FW_data.V_IP_MASK[0],FW_data.V_IP_MASK[1],FW_data.V_IP_MASK[2],FW_data.V_IP_MASK[3]);
  set_intext(str3,"Mask_net","15",str4);
  set_2row_str (str2,"Маска сети",str3,10,60,20);
  strcat(str1,str2);
  
  
    memset(str2,0,size_page_bufer_slave*4);
  sprintf(str4,"%d.%d.%d.%d",FW_data.V_IP_GET[0],FW_data.V_IP_GET[1],FW_data.V_IP_GET[2],FW_data.V_IP_GET[3]);
  set_intext(str3,"IP_Getway","15",str4);
  set_2row_str (str2,"Адрес шлюза",str3,10,60,20);
  strcat(str1,str2);
  
  memset(str2,0,size_page_bufer_slave*4);

//  set_open_block(str3,"input type=\"checkbox\" name=\"option2\" value=\"1\"");
//<input type="checkbox" name="option1" value="a1" checked>
  if (FW_data.V_DHCP==0)
  {
  set_open_block(str3,"input type=\"checkbox\" name=\"dhcp_flag\" value=\"1\" ");
  }
  else
  {
    set_open_block(str3,"input type=\"checkbox\" name=\"dhcp_flag\" value=\"1\" checked");
  }
  
  //strcat(str1,str2);
  
  set_2row_str (str2,"использовать DHCP для получения адреса",str3,10,60,20);
  strcat(str1,str2);
  
  
  
    memset(str2,0,size_page_bufer_slave*4);
  sprintf(str4,"%d",lamp_state.lamp_power);
  set_innum(str3,"Power_lamp","5","32768",str4);
  //set_intext(str3,"Power_lamp","5",str4);
  set_2row_str (str2,"Мощность лампы",str3,10,60,20);
  strcat(str1,str2);
  
  
  
  memset(str2,0,size_page_bufer_slave*4);
  sprintf(str4,"%d",lamp_state.color_index);
//  set_intext(str3,"Color_n","2",str4);
  set_innum(str3,"Color_n","1","4","0");
  set_2row_str (str2,"Выбран цвет ",str3,10,60,20);
  strcat(str1,str2);
  
  
  
  
  memset(str2,0,size_page_bufer_slave*4);
  sprintf(str4,"%d",lamp_state.pixel_edit_index);
  set_innum(str3,"Pixel_n","","1024","0");
  //set_intext(str3,"Pixel_n","6",str4);
  set_2row_str (str2,"Выбрать пиксель",str3,10,60,20);
  strcat(str1,str2);
  
  
  
  memset(str2,0,size_page_bufer_slave*4);  
  set_submit(str3,"Save_settings","1");   
  set_2row_str (str2," ",str3,10,60,20);
  strcat(str1,str2);  
 
  
  
  
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  

    
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
   reset_open_block(str2,"body");
  strcat(str1,str2);
  
  
  
  
  
//  
//  
//  
//   set_open_block(str2,"form action='/' method='POST'");
//  strcat(str1,str2);
//  
// 
//  set_open_block(str2,"h2");
//  strcat(str1,str2);
//  
//    set_br(str2,3);
//  strcat(str1,str2); 
//  
//  set_open_block(str2,"b>Управление состоянием</b");
//  strcat(str1,str2);
//  
//   set_space(str2,3);
//  strcat(str1,str2); 
//  
//  set_submit(str2,"Power_on","1");
//  strcat(str1,str2);  
//  
//     set_space(str2,3);
//  strcat(str1,str2); 
//  
//  
//   set_submit(str2,"Power_off","0");
//  strcat(str1,str2);  
//  
//     set_space(str2,3);
//  strcat(str1,str2); 
//  
//  
//  
//  reset_open_block(str2,"h2");
//  strcat(str1,str2);
//  
//  
//
//    
//  reset_open_block(str2,"form");
//  strcat(str1,str2); 
//  
  
  
  
  len=strlen(str1);
  
  
  
  return len;
};


uint32_t costr_logs(char* str1)
{
  uint32_t len;
  char str2[size_page_bufer_slave*4];
  char str3[size_page_bufer_slave*2];
  char str4[size_page_bufer_slave];
  memset (str1,0, size_page_bufer);

  //len = sizeof(http_html_start_constr);
  strcat(str1,http_html_start_constr1);
  
  //len = len+sizeof(http_html_style);
  strcat(str1,http_html_style);
  
  set_open_block(str2,"body");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
  set_open_block(str2,"lf");
  strcat(str1,str2);
  
  set_open_block(str2,"h4");
  strcat(str1,str2);
  
  sizeof("Драйвер лампы и фона HW2.0 ");
  strcat(str1,"Драйвер лампы и фона HW2.0 ");
  
  reset_open_block(str2,"h4");
  strcat(str1,str2);
  
  reset_open_block(str2,"lf");
  strcat(str1,str2);
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_link(str2,"Главная ","index.html");
  strcat(str1,str2);
  
  set_link(str2,"Настройки","settings.html");
  strcat(str1,str2);  
    
  reset_open_block(str2,"h2");
  strcat(str1,str2);

  reset_open_block(str2,"ul");
  strcat(str1,str2);
  
  set_open_block(str2,"form action='/' method='GET'");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
  
  set_open_block(str2,"h3");
  strcat(str1,str2);
  
  set_open_block(str2,"br");
  strcat(str1,str2);
  
  
 set_space(str2,6);
  strcat(str1,str2);  

  
////  
////  set_open_block(str2,"b> Имя устройства </b");
////  strcat(str1,str2);
////  
////  set_space(str2,2);
////  strcat(str1,str2);
////  
////  memset(str2,0,256);
////  sprintf(str2,"<b>%s</b>",FW_data.V_Name_dev);
////  strcat(str1,str2);
////
////  set_space(str2,6);
////  strcat(str1,str2);
  

  set_open_block(str2,"b>IP addres</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2);
  
  memset(str2,0,size_page_bufer_slave*4);
  sprintf(str2,"<b>%d.%d.%d.%d</b>",FW_data.V_IP_CONFIG[0],FW_data.V_IP_CONFIG[1],FW_data.V_IP_CONFIG[2],FW_data.V_IP_CONFIG[3]);
  strcat(str1,str2);

  set_space(str2,6);
  strcat(str1,str2);  

  
  
  
   set_open_block(str2,"b>Mask addres</b");
  strcat(str1,str2);
  
  
  
  set_space(str2,2);
  strcat(str1,str2);
  
  memset(str2,0,size_page_bufer_slave*4);
  sprintf(str2,"<b>%d.%d.%d.%d</b>",FW_data.V_IP_MASK[0],FW_data.V_IP_MASK[1],FW_data.V_IP_MASK[2],FW_data.V_IP_MASK[3]);
  strcat(str1,str2);

  
    
  set_space(str2,6);
  strcat(str1,str2);  
  
  set_open_block(str2,"b>IP getway</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2); 
  
  memset(str2,0,size_page_bufer_slave*4);
  sprintf(str2,"<b>%d.%d.%d.%d</b>",FW_data.V_IP_GET[0],FW_data.V_IP_GET[1],FW_data.V_IP_GET[2],FW_data.V_IP_GET[3]);
  strcat(str1,str2);
  
  set_space(str2,6);
  strcat(str1,str2);  
  
  set_open_block(str2,"b>MAC adress</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2); 
  
  memset(str2,0,size_page_bufer_slave*4);
  sprintf(str2,"<b>%x.%x.%x.%x.%x.%x</b>",FW_data.V_ID_MAC[0],FW_data.V_ID_MAC[1],FW_data.V_ID_MAC[2],FW_data.V_ID_MAC[3],FW_data.V_ID_MAC[4],FW_data.V_ID_MAC[5]);
  strcat(str1,str2);
  
  
//  len=len+set_intext(str2,"IP_getway","15","192.168.000.001");
//  strcat(str1,str2);  
  
  reset_open_block(str2,"h3");
  strcat(str1,str2);
  
  reset_open_block(str2,"ul");
  strcat(str1,str2);
  
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  

  
  set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
 
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_br(str2,2);  
  strcat(str1,str2);
  

//  strcat(str1,http_logs_hdr);
//  
//  
//  set_open_block(str2,"p>Скопируйте приведённый текст и вставьте его в поле запроса пароля.</p");
//  strcat(str1,str2);
//   
//
//  strcat(str1,http_logs_area);
//    
//  sprintf(str2,"Скопируйте приведённый текст и вставьте его в поле запроса пароля");
//  strcat(str1,str2);
//  
//  
//  strcat(str1,http_logs_end);
  
  
//<img src=\"img/logo.gif\" alt=\"undefined\" height=\"63\" width=\"270\">"

set_open_block(str2,"img src=\"img/rgb24.bmp\" alt=\"undefined\" height=\"276\" width=\"1052\"");
  strcat(str1,str2);
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  


    
  reset_open_block(str2,"form");
  strcat(str1,str2); 
  
  
  
  
  len=strlen(str1);
  
  
  
  return len;
};
#define Height_sec 8
#define speed 300
#define Width_img 1052
#define Height_img 20+32*Height_sec

void costr_graf_pl_start(struct netconn *conn,char* str1)
{
 
  uint32_t len;
  uint8_t ct_str;
  char str2[size_page_bufer_slave*4];
//  char str3[size_page_bufer_slave*2];
//  char str4[size_page_bufer_slave];
  memset (str1,0, size_page_bufer);
  memcpy(str1,(const char *) bmp_base,102);  
  //strcat(str1,(const char *)bmp_base);
//  uint32_t offset = *((uint32_t*)(str1+10));
  *((uint32_t*)(str1+18))=(uint32_t)Width_img;
  *((uint32_t*)(str1+22))=(uint32_t) Height_img;
  *((uint32_t*)(str1+34))=(uint32_t) Height_img*Width_img*3;
 // memset (&str1[54],0xe0, Width_img*3);  
  len=54;
  netconn_write(conn, (const unsigned char*)(str1), (size_t)len, NETCONN_NOCOPY);
  

    
  for (ct_str=0;ct_str<10;ct_str++)
  {
    memset (str2,0xff, Width_img*3);  
    len=Width_img*3;
  //  vTaskDelay(speed);
    netconn_write(conn, (const unsigned char*)(str2), (size_t)len, NETCONN_NOCOPY);    
    
    
    memset (str1,0xff, Width_img*3);  
    len=Width_img*3;
  //  vTaskDelay(speed);
    netconn_write(conn, (const unsigned char*)(str1), (size_t)len, NETCONN_NOCOPY);    

  }  
  
    memset (str2,0xff, Width_img*3);  
    len=Width_img*3;
  //  vTaskDelay(speed);
    netconn_write(conn, (const unsigned char*)(str2), (size_t)len, NETCONN_NOCOPY);
    
    memset (str1,0x00, Width_img*3);  
    len=Width_img*3;
  //  vTaskDelay(speed);
    netconn_write(conn, (const unsigned char*)(str1), (size_t)len, NETCONN_NOCOPY);

  
    
  uint8_t ct_blok=0;
for(ct_blok=0;ct_blok<Height_sec;ct_blok++)
{
    
 
     for (ct_str=0;ct_str<15;ct_str++)
  {
    memset (str2,0xff, Width_img*3);  
    len=Width_img*3;
  //  vTaskDelay(speed);
    netconn_write(conn, (const unsigned char*)(str2), (size_t)len, NETCONN_NOCOPY);    
    
    
    memset (str1,0xff, Width_img*3);  
    len=Width_img*3;
  //  vTaskDelay(speed);
    netconn_write(conn, (const unsigned char*)(str1), (size_t)len, NETCONN_NOCOPY);  
  }
  
  
    memset (str2,0xff, Width_img*3);  
    len=Width_img*3;
  //  vTaskDelay(speed);
    netconn_write(conn, (const unsigned char*)(str2), (size_t)len, NETCONN_NOCOPY);
    
    memset (str1,0x00, Width_img*3);  
    len=Width_img*3;
  //  vTaskDelay(speed);
    netconn_write(conn, (const unsigned char*)(str1), (size_t)len, NETCONN_NOCOPY);
    
    
}
  
  
  
  
}
  