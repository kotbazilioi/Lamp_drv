#include <stdint.h>
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#define size_page_bufer 5000
#define size_page_bufer_slave 256
void costr_graf_pl_start(struct netconn *conn,char* str1);
uint32_t costr_logs(char* str1);
uint32_t costr_pass(char* str1);
uint32_t costr_page1(char* str1);
extern uint8_t flag_logon;
extern unsigned char key_http[30];
extern char HTTP401[];
extern char HTTP401end[];
extern int key_http_len;
extern const char http_html_hdr[];
extern const uint8_t http_201_hdr[];