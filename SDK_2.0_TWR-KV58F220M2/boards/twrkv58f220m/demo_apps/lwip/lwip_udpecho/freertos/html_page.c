#include <string.h>
#include "html_page.h"
#include "hard_init.h"

//#include "flash_if.h"
uint8_t ct_logoff_time;
uint8_t flag_logon=0;
static const char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
  //char * data[]="<a href=\"settings.html\" target=\"_self\" rel=\"nofollow\">????????? &emsp; </a>";/*<body onload=\"onload()\"\>*/
static const char http_html_start_constr[] = "\<!DOCTYPE html>\r\n <body onload=\"onload()\">\n\r<a href=\"http:\/\/www.csort.ru/\"></a>\n\r<img src=\"img/logo.gif\" height=\"63\" width=\"270\" border=\"0\" alt=\"csort logo\" title=\"404 error\">\n\r <html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n\r<script\>"

	  "var xhr\;"
           "var idTimer1\";"
	   "function onload(){"
	   "xhr = new (XMLHttpRequest);"
	   "}\n\r"
           
	   "function swich1(){"
           "xhr.open(\"GET\", \"swich?c=1\", true);"
	   "xhr.responseType = \"text\";"
	   "xhr.send(null);"
	   "}\n\r"	
	  "function swich2(){"
	  "xhr.open(\"GET\", \"swich?c=2\", true);"
	  "xhr.responseType = \"text\";"
	  "xhr.send(null);"
	  "}\n\r"
	  "function swich3(){"
	   "xhr.open(\"GET\", \"swich?c=3\", true);"
	    "xhr.responseType = \"text\";"
	    "xhr.send(null);"
	  "}\n\r"
"</script>";


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
sprintf(str1,"<input type=\"text\" name=\"%s\"  size='%s' value=\"%s\"style=\"text-align: center\">&ensp;\n\r",name,size,value);

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


uint32_t costr_page1(char* str1)
{
  uint32_t len;
  char str2[256];
  memset (str1,0, 4000);
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
  sprintf(str2,"<b>%d.%d.%d.%d</b>",Hi_DCDC.V_IP_config[0],Hi_DCDC.V_IP_config[1],Hi_DCDC.V_IP_config[2],Hi_DCDC.V_IP_config[3]);
  strcat(str1,str2);
  
  
  set_space(str2,6);
  strcat(str1,str2);  
  
  set_open_block(str2,"b>Mask addres</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2);
  
  memset(str2,0,256);
  sprintf(str2,"<b>%d.%d.%d.%d</b>",Hi_DCDC.V_MASK_config[0],Hi_DCDC.V_MASK_config[1],Hi_DCDC.V_MASK_config[2],Hi_DCDC.V_MASK_config[3]);
  strcat(str1,str2);

  
    
  set_space(str2,6);
  strcat(str1,str2);  
  
  set_open_block(str2,"b>IP getway</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2); 
  
  memset(str2,0,256);
  sprintf(str2,"<b>%d.%d.%d.%d</b>",Hi_DCDC.V_GET_config[0],Hi_DCDC.V_GET_config[1],Hi_DCDC.V_GET_config[2],Hi_DCDC.V_GET_config[3]);
  strcat(str1,str2);
  
  set_space(str2,6);
  strcat(str1,str2);  
  
  set_open_block(str2,"b>MAC adress</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2); 
  
  memset(str2,0,256);
  sprintf(str2,"<b>%x.%x.%x.%x.%x.%x</b>",Hi_DCDC.V_MAC_config[0],Hi_DCDC.V_MAC_config[1],Hi_DCDC.V_MAC_config[2],Hi_DCDC.V_MAC_config[3],Hi_DCDC.V_MAC_config[4],Hi_DCDC.V_MAC_config[5]);
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
  
  set_open_block(str2,"input type=\"color\" name=\"Color_fon\" placeholder=\"Color_fon1\"");
  strcat(str1,str2);
  
   set_space(str2,2);
  strcat(str1,str2); 
  
  
  set_intext(str2,"Power","5","00");
  strcat(str1,str2);  
  

  
  set_submit(str2,"Save","1");
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
  
  set_submit(str2,"Power_on","1");
  strcat(str1,str2);  
  
     set_space(str2,3);
  strcat(str1,str2); 
  
  
   set_submit(str2,"Power_off","0");
  strcat(str1,str2);  
  
     set_space(str2,3);
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
  char str2[256];
  char str3[256];
  char str4[256];
  memset (str1,0, 4000);

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
  sprintf(str2,"<b>%d.%d.%d.%d</b>",Hi_DCDC.V_IP_config[0],Hi_DCDC.V_IP_config[1],Hi_DCDC.V_IP_config[2],Hi_DCDC.V_IP_config[3]);
  strcat(str1,str2);

  set_space(str2,6);
  strcat(str1,str2);  

   set_open_block(str2,"b>Mask addres</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2);
  
  memset(str2,0,256);
  sprintf(str2,"<b>%d.%d.%d.%d</b>",Hi_DCDC.V_MASK_config[0],Hi_DCDC.V_MASK_config[1],Hi_DCDC.V_MASK_config[2],Hi_DCDC.V_MASK_config[3]);
  strcat(str1,str2);

  
    
  set_space(str2,6);
  strcat(str1,str2);  
  
  set_open_block(str2,"b>IP getway</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2); 
  
  memset(str2,0,256);
  sprintf(str2,"<b>%d.%d.%d.%d</b>",Hi_DCDC.V_GET_config[0],Hi_DCDC.V_GET_config[1],Hi_DCDC.V_GET_config[2],Hi_DCDC.V_GET_config[3]);
  strcat(str1,str2);
  
  set_space(str2,6);
  strcat(str1,str2);  
  
  set_open_block(str2,"b>MAC adress</b");
  strcat(str1,str2);
  
  set_space(str2,2);
  strcat(str1,str2); 
  
  memset(str2,0,256);
  sprintf(str2,"<b>%x.%x.%x.%x.%x.%x</b>",Hi_DCDC.V_MAC_config[0],Hi_DCDC.V_MAC_config[1],Hi_DCDC.V_MAC_config[2],Hi_DCDC.V_MAC_config[3],Hi_DCDC.V_MAC_config[4],Hi_DCDC.V_MAC_config[5]);
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
  

  
  memset(str2,0,256);
  sprintf(str4,"%d.%d.%d.%d",Hi_DCDC.V_IP_config[0],Hi_DCDC.V_IP_config[1],Hi_DCDC.V_IP_config[2],Hi_DCDC.V_IP_config[3]);
  set_intext(str3,"IP_addr","15",str4);
  set_2row_str (str2,"IP адрес ",str3,10,60,20);
  strcat(str1,str2);
  
  
    memset(str2,0,256);
  sprintf(str4,"%d.%d.%d.%d",Hi_DCDC.V_MASK_config[0],Hi_DCDC.V_MASK_config[1],Hi_DCDC.V_MASK_config[2],Hi_DCDC.V_MASK_config[3]);
  set_intext(str3,"Mask_net","15",str4);
  set_2row_str (str2,"Маска сети",str3,10,60,20);
  strcat(str1,str2);
  
  
    memset(str2,0,256);
  sprintf(str4,"%d.%d.%d.%d",Hi_DCDC.V_GET_config[0],Hi_DCDC.V_GET_config[1],Hi_DCDC.V_GET_config[2],Hi_DCDC.V_GET_config[3]);
  set_intext(str3,"IP_Getway","15",str4);
  set_2row_str (str2,"Адрес шлюза",str3,10,60,20);
  strcat(str1,str2);
  
  memset(str2,0,256);

//  set_open_block(str3,"input type=\"checkbox\" name=\"option2\" value=\"1\"");
//<input type="checkbox" name="option1" value="a1" checked>
  set_open_block(str3,"input type=\"checkbox\" name=\"dhcp_flag\" value=\"SET\" ");
  //strcat(str1,str2);
  
  set_2row_str (str2,"использовать DHCP для получения адреса",str3,10,60,20);
  strcat(str1,str2);
  
  
  
    memset(str2,0,256);
  sprintf(str4,"%X",lamp_state.lamp_power);
  set_intext(str3,"Power lamp","5",str4);
  set_2row_str (str2,"Мощность лампы",str3,10,60,20);
  strcat(str1,str2);
  
    memset(str2,0,256);
  sprintf(str4,"%d",lamp_state.color_index);
  set_intext(str3,"Color_n","2",str4);
  set_2row_str (str2,"Выбран цвет ",str3,10,60,20);
  strcat(str1,str2);
  
  
    memset(str2,0,256);
  sprintf(str4,"%d",lamp_state.pixel_edit_index);
  set_intext(str3,"Pixel_n","6",str4);
  set_2row_str (str2,"Выбрать пиксель",str3,10,60,20);
  strcat(str1,str2);
  
  
  
  memset(str2,0,256);  
  set_submit(str3,"Save","1");   
  set_2row_str (str2," ",str3,10,60,20);
  strcat(str1,str2);  
 
  
  
  
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  

    
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  
  
  
  
  
  
  
   set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
 
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
////    set_br(str2,3);
////  strcat(str1,str2); 
////  
////  set_open_block(str2,"b>Управление состоянием</b");
////  strcat(str1,str2);
////  
////   set_space(str2,3);
////  strcat(str1,str2); 
////  
////  set_submit(str2,"Power_on","1");
////  strcat(str1,str2);  
////  
////     set_space(str2,3);
////  strcat(str1,str2); 
////  
////  
////   set_submit(str2,"Power_off","0");
////  strcat(str1,str2);  
////  
////     set_space(str2,3);
////  strcat(str1,str2); 
  
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  

    
  reset_open_block(str2,"form");
  strcat(str1,str2); 
  
  
  
  
  len=strlen(str1);
  
  
  
  return len;
};