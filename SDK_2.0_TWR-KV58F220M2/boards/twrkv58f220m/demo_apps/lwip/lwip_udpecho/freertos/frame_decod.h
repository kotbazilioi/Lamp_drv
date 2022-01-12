#include "board.h"
#include "fsl_flexcan.h"

#include "fsl_flexcan_edma.h"
#include "fsl_uart_edma.h"
#include "fsl_dma_manager.h"

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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_crc.h"
#define FW1_CRC_A 0x10036000 //4byte CRC32

#define FW1_CRC_A 0x10036000 //4byte CRC32
#define FW1_LEN_A 0x10038000 //4byte LEN
#define FW1       0x10040000 //Start ADRR FW1
#define FW1_VER_A 0x10040430 //6byte VER
//#define a_call_data 0x10040450 // calibrovka lamp_fon

////#define FW2       0x100a0000 //Start ADRR FW2
#define FW2_VER_A 0x100a0430 //6byte VER
#define FW2_CRC_A 0x1003c000 //4byte CRC32
#define FW2_LEN_A 0x1003e000 //4byte LEN

#define FW_LOADER_VER_A 0x10000430
#define IP_CONFIG_A 0x10032000
#define K_CORR_R_A 0x10040450+0x200
#define K_CORR_G_A 0x10040450+0x400
#define K_CORR_B_A 0x10040450+0x600
#define K_CORR_IR_A 0x10040450+0x800
#define SPI0_SIZE all_modul



#define type_fw lamp_sys+1
#define release 0
#define build 10



#ifdef __cplusplus
extern "C" {
  #endif
typedef struct SCommands
{
    int id;
    int len;
    int begin_b;
    int data_size;
    uint32_t* data;
    unsigned int crc;
} SCommands_t;



#ifdef __cplusplus
}
#endif
extern xTaskHandle xHandle_tx_udp;
extern SCommands_t frame;
extern int frame_ct_udp;
extern bool flag_frame_run_can;
extern uint32_t ct_timeout_frame;
extern const uint8_t K_CORR_R[512];
extern const uint8_t K_CORR_G[512];
extern const uint8_t K_CORR_B[512];
extern const uint8_t K_CORR_IR[512];

void InitCrc32(CRC_Type *base, uint32_t seed);
void InitCrc16(CRC_Type *base, uint32_t seed);
void InitCrc16_Kermit(CRC_Type *base, uint32_t seed);
int frame_run(int id, int data_size, uint32_t* data,int begin_b,int len,int *out_len_data,uint8_t* data_tx_out);
status_t crc_compar (char *mass,int in_crc,int len_mass);

int decoding_frame (char *mass, unsigned int len,unsigned char type_interface,uint8_t *data_tx,uint32_t * out_len);
