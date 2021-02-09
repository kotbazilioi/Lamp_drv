#include "fsl_flash.h"
#define BUFFER_LEN 2048

int Flash_set_protect (void);
int Flash_unset_protect (void);
void CRC_WriteData_RDContrl(CRC_Type *base, const uint8_t *data, size_t dataSize);
int progFLASH_init(void);
int progSector (uint32_t adress,uint32_t *data_prog, uint32_t len_s);
int progEraseSector (uint32_t adress,uint32_t pflashSectorSize);
int progF (void);
extern  uint32_t s_buffer[BUFFER_LEN];
extern  flash_config_t s_flashDriver;