#include <string.h>
#include "html_page.h"
static const char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
  //char * data[]="<a href=\"settings.html\" target=\"_self\" rel=\"nofollow\">????????? &emsp; </a>";
static const char http_html_start_constr[] = "\<!DOCTYPE html> <body onload=\"onload()\"\> <img src=\"img/logo.gif\" alt=\"undefined\" height=\"63\" width=\"270\"> <html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\<script\>"
	  "var xhr\;"
           "var idTimer1\";"
	   "function onload(){"
	   "xhr = new (XMLHttpRequest);"
	   "}"
           
	   "function swich1(){"
           "xhr.open(\"GET\", \"swich?c=1\", true);"
	   "xhr.responseType = \"text\";"
	   "xhr.send(null);"
	   "}"	
	  "function swich2(){"
	  "xhr.open(\"GET\", \"swich?c=2\", true);"
	  "xhr.responseType = \"text\";"
	  "xhr.send(null);"
	  "}"
	  "function swich3(){"
	   "xhr.open(\"GET\", \"swich?c=3\", true);"
	    "xhr.responseType = \"text\";"
	    "xhr.send(null);"
	  "}"
"</script>";


static const char http_html_style[] = 
"<title>Fon DRV</title>"
"  <style>"
"    lf {"
	"font-family: Pompadur; /* Гарнитура шрифта */"
	"src: url(fonts/pompadur.ttf); /* Путь к файлу со шрифтом */"
	"font-family: Pompadur, 'Comic Sans MS', cursive;"
        "text-align: center;"
          "font-size: 16pt;"
    "}"
    "h2 {"
	"font-size: 14pt;"
        "text-align: center;"
    "}"
   "display: inline-block;"
  "</style>"
    "</head>";

uint16_t set_open_block (char* str1,char* name)
{ 

uint16_t len=strlen((char*)name)+2;
sprintf(str1,"<%s>",name);
return len;
}
uint16_t reset_open_block (char* str1,char* name)
{ 

uint16_t len=strlen(name)+3;
sprintf(str1,"<%s/>",name);
return len;
}
uint16_t set_space (char* str1,uint8_t n)
{ 
uint16_t len=6*n;
 memset (str1,0, 64);
for(;n>0;n--)
{
  strcat(str1,"&ensp;");
}

return len;
}

uint16_t set_link (char* str1,char* name,char* link)
{
  //<a href="settings.html" target="_self" rel="nofollow">Настройки &emsp; </a>
    
//char str2[64];  
uint16_t len=strlen(name)+strlen(link)+52;  //<a href="" target="_self" rel="nofollow">&emsp; </a>
sprintf(str1,"<a href=\"%s\" target=\"_self\" rel=\"nofollow\">%s &emsp; </a>",link,name);

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
///<input type="text" name=""  size='' value="">&ensp;
uint16_t len=51;
len +=strlen(name);
len+= strlen(value);
len+=strlen(size);
sprintf(str1,"<input type=\"text\" name=\"%s\"  size='%s' value=\"%s\">&ensp;",name,size,value);

////sprintf(str2,"%s",name);
////strcat(str1,str2);
////
////sprintf(str1,"\"  size='");
////
////sprintf(str2,"%s",size);
////strcat(str1,str2);
////
////sprintf(str1,"<' value=\"");
////
////sprintf(str2,"%s",value);
////strcat(str1,str2);
////
////sprintf(str1,"\">&ensp;");

return len;
}


uint32_t costr_page1(char* str1)
{
  uint32_t len;
  char str2[256];
  memset (str1,0, 2000);
  len = sizeof(http_html_start_constr);
  strcat(str1,http_html_start_constr);
  
  len = len+sizeof(http_html_style);
  strcat(str1,http_html_style);
  
  len=len+set_open_block(str2,"body");
  strcat(str1,str2);
  
  len=len+set_open_block(str2,"ul");
  strcat(str1,str2);
  
  len=len+set_open_block(str2,"lf");
  strcat(str1,str2);
  
  len=len+set_open_block(str2,"h4");
  strcat(str1,str2);
  
  len=len+sizeof("Драйвер лампы и фона HW2.0 ");
  strcat(str1,"Драйвер лампы и фона HW2.0 ");
  
  len=len+reset_open_block(str2,"h4");
  strcat(str1,str2);
  
  len=len+reset_open_block(str2,"lf");
  strcat(str1,str2);
  
  len=len+set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  len=len+set_link(str2,"Настройки ","settings.html");
  strcat(str1,str2);
  
  len=len+set_link(str2,"Текущие данные ","data_strim.html");
  strcat(str1,str2);  
    
  len=len+reset_open_block(str2,"h2");
  strcat(str1,str2);

  len=len+reset_open_block(str2,"ul");
  strcat(str1,str2);
  
  len=len+set_open_block(str2,"form action='/' method='GET'");
  strcat(str1,str2);
  
    len=len+set_open_block(str2,"ul");
  strcat(str1,str2);
  
  
  len=len+set_open_block(str2,"h2");
  strcat(str1,str2);
  
  len=len+set_open_block(str2,"br");
  strcat(str1,str2);


  len=len+set_space(str2,6);
  strcat(str1,str2);  

  len=len+set_open_block(str2,"b>IP addres</b");
  strcat(str1,str2);

////uint16_t set_intext (char* str1,char* name,char* value,char* size)
  len=len+set_intext(str2,"IP_addres","15","192.168.000.122");
  strcat(str1,str2);

  len=len+set_intext(str2,"MASK_addres","15","255.255.255.000");
  strcat(str1,str2);

  len=len+set_intext(str2,"IP_getway","15","192.168.000.001");
  strcat(str1,str2);  
  
  return len;
};

