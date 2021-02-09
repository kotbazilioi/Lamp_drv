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



#include "progFlash.h"

//#define BL_TARGET_ROM
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void error_trap(void);
void app_finalize(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Flash driver Structure */
static flash_config_t s_flashDriver;
/*! @brief Flash cache driver Structure */
static ftfx_cache_config_t s_cacheDriver;
/////*! @brief Buffer for program */
////static uint32_t s_buffer[BUFFER_LEN];
/////*! @brief Buffer for readback */
////static uint32_t s_buffer_rbc[BUFFER_LEN];




/*******************************************************************************
 * Code
 ******************************************************************************/

/*
* @brief Gets called when an error occurs.
*
* @details Print error message and trap forever.
*/
void error_trap(void)
{
    PRINTF("\r\n\r\n\r\n\t---- HALTED DUE TO FLASH ERROR! ----");
   // while (1)
  //  {
   // }
}

/*
* @brief Gets called when the app is complete.
*
* @details Print finshed message and trap forever.
*/
void app_finalize(void)
{
    /* Print finished message. */
    PRINTF("\r\n End of PFlash Example \r\n");
   // while (1)
    {
    }
}
void CRC_WriteData_RDContrl(CRC_Type *base, const uint8_t *data, size_t dataSize)
{
    const uint32_t *data32;
     uint32_t buf_read=0;
    /* 8-bit reads and writes till source address is aligned 4 bytes */
    while ((dataSize) && ((uint32_t)data & 3U))
    {
      buf_read= *data;
      if ((FTFE->FSTAT&0x40)==0)
      {
        base->ACCESS8BIT.DATALL = buf_read;
        data++;
        dataSize--;
      }
    }
    uint32_t data_bufer;
    /* use 32-bit reads and writes as long as possible */
    data32 = (const uint32_t *)data;
    FMC->PFB0CR |= 1<<19;
    while (dataSize >= sizeof(uint32_t))
    {
       buf_read= *data32;
      if ((FTFE->FSTAT&0x40)==0)
      {
       data_bufer = buf_read;
        base->DATA = data_bufer;
        data32++;
        dataSize -= sizeof(uint32_t);
      }
      else
      {
      buf_read= *data32;
      }
    }

    data = (const uint8_t *)data32;

    /* 8-bit reads and writes till end of data buffer */
    while (dataSize)
    {
      buf_read= *data;
      if ((FTFE->FSTAT&0x40)==0)
      {
        base->ACCESS8BIT.DATALL = buf_read;
        data++;
        dataSize--;
      }
    }
}
/*!
 * @brief Use Standard Software Drivers (SSD) to modify pflash.
 *
 * @details This function uses SSD to demonstrate flash mode:
 *            + Check pflash information.
 *            + Erase a sector and verify.
 *            + Program a sector and verify.
 */
int progFLASH_init(void)
  {
    ftfx_security_state_t securityStatus = kFTFx_SecurityStateNotSecure; /* Return protection status */
    status_t result;    /* Return code from each flash driver function */


    uint32_t pflashBlockBase = 0;
    uint32_t pflashTotalSize = 0;
    uint32_t pflashSectorSize = 0;

    /* Init hardware */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Clean up Flash, Cache driver Structure*/
    memset(&s_flashDriver, 0, sizeof(flash_config_t));
    memset(&s_cacheDriver, 0, sizeof(ftfx_cache_config_t));

    /* Setup flash driver structure for device and initialize variables. */
    result = FLASH_Init(&s_flashDriver);
    if (kStatus_FTFx_Success != result)
    {
        error_trap();
    }
    /* Setup flash cache driver structure for device and initialize variables. */
    result = FTFx_CACHE_Init(&s_cacheDriver);
    if (kStatus_FTFx_Success != result)
    {
        error_trap();
    }
    /* Get flash properties*/
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0BlockBaseAddr, &pflashBlockBase);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0TotalSize, &pflashTotalSize);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0SectorSize, &pflashSectorSize);

//    /* print welcome message */
//    PRINTF("\r\n PFlash Example Start \r\n");
//    /* Print flash information - PFlash. */
//    PRINTF("\r\n PFlash Information: ");
//    PRINTF("\r\n Total Program Flash Size:\t%d KB, Hex: (0x%x)", (pflashTotalSize / 1024), pflashTotalSize);
//    PRINTF("\r\n Program Flash Sector Size:\t%d KB, Hex: (0x%x) ", (pflashSectorSize / 1024), pflashSectorSize);

    /* Check security status. */
    result = FLASH_GetSecurityState(&s_flashDriver, &securityStatus);
    if (kStatus_FTFx_Success != result)
    {
        error_trap();
    }
return kStatus_FLASH_Success;
  }
//int Flash_set_protect (void)
//{
//  /*! @brief Flash driver Structure */
// status_t result;    /* Return code from each flash driver function */
//
//
//// __disable_irq();
//// __disable_fiq();
//    result = FLASH_Init(&s_flashDriver);
//    if (kStatus_FLASH_Success != result)
//    {
//        error_trap();
//        return result;
//    }
//  result=FLASH_PflashSetProtection(&s_flashDriver,0x01000000);
//     if (kStatus_FLASH_Success != result)
//    {
//        error_trap();
//        return result;
//    }
//    return 0;
//}
//int Flash_unset_protect (void)
//{
//  /*! @brief Flash driver Structure */
// status_t result;    /* Return code from each flash driver function */
//
//
//// __disable_irq();
//// __disable_fiq();
//    result = FLASH_Init(&s_flashDriver);
//    if (kStatus_FLASH_Success != result)
//    {
//        error_trap();
//        return result;
//    }
//  result=FLASH_PflashSetProtection(&s_flashDriver,0xffffffff);
//     if (kStatus_FLASH_Success != result)
//    {
//        error_trap();
//        return result;
//    }
//    return 0;
//}
int progSector (uint32_t adress,uint32_t *data_prog, uint32_t len_s)
{
 /*! @brief Flash driver Structure */
 status_t result;    /* Return code from each flash driver function */

taskENTER_CRITICAL();
// __disable_irq();
// __disable_fiq();
    result = FLASH_Init(&s_flashDriver);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
        return result;
    }
    FMC->PFB0CR |= 1<<19;
//PFB0CR.  S_INV=1;
 result = FLASH_Erase(&s_flashDriver, adress, len_s, kFLASH_ApiEraseKey);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
            return result;
        }


        result = FLASH_VerifyErase(&s_flashDriver, adress, len_s, kFLASH_MarginValueUser);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
                return result;
        }
    FLASH_Init(&s_flashDriver);
    result = FLASH_Program(&s_flashDriver, adress, data_prog,len_s);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
            return result;

        }
       while (((FTFE->FSTAT)&FTFE_FSTAT_CCIF_MASK)==0){

        }

       result = FLASH_VerifyProgram(&s_flashDriver, adress, len_s,
                                     data_prog, kFLASH_MarginValueUser,0, 0);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
            return result;

        }
        taskEXIT_CRITICAL();
//         __enable_irq();
//         __enable_fiq();


   //   FMC->PFB0CR |= 1<<19;
return kStatus_FLASH_Success;
}

//int progEraseSector (uint32_t adress,uint32_t pflashSectorSize)
//{
//  /*! @brief Flash driver Structure */
//
//      status_t result;    /* Return code from each flash driver function */
//        result = FLASH_Erase(&s_flashDriver, adress, pflashSectorSize, kFLASH_ApiEraseKey);
//        if (kStatus_FLASH_Success != result)
//        {
//            error_trap();
//                return result;
//        }
//
//        /* Verify sector if it's been erased. */
//        result = FLASH_VerifyErase(&s_flashDriver, adress, pflashSectorSize, kFLASH_MarginValueUser);
//        if (kStatus_FLASH_Success != result)
//        {
//            error_trap();
//                return result;
//        }
//        return kStatus_FLASH_Success;
//}
//int progF (void)
//{
//  /*! @brief Flash driver Structure */
//
//    flash_security_state_t securityStatus = kFLASH_SecurityStateNotSecure; /* Return protection status */
//    status_t result;    /* Return code from each flash driver function */
//    uint32_t destAdrss; /* Address of the target location */
//    uint32_t i, failAddr, failDat;
//
//    uint32_t pflashBlockBase = 0;
//    uint32_t pflashTotalSize = 0;
//    uint32_t pflashSectorSize = 0;
//
//
//    /* Clean up Flash driver Structure*/
//    memset(&s_flashDriver, 0, sizeof(flash_config_t));
//
//    /* Setup flash driver structure for device and initialize variables. */
//    result = FLASH_Init(&s_flashDriver);
//    if (kStatus_FLASH_Success != result)
//    {
//        error_trap();
//    }
//    /* Get flash properties*/
//    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflashBlockBaseAddr, &pflashBlockBase);
//    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflashTotalSize, &pflashTotalSize);
//    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflashSectorSize, &pflashSectorSize);
//
//    /* print welcome message */
//    PRINTF("\r\n PFlash Example Start \r\n");
//    /* Print flash information - PFlash. */
//    PRINTF("\r\n PFlash Information: ");
//    PRINTF("\r\n Total Program Flash Size:\t%d KB, Hex: (0x%x)", (pflashTotalSize / 1024), pflashTotalSize);
//    PRINTF("\r\n Program Flash Sector Size:\t%d KB, Hex: (0x%x) ", (pflashSectorSize / 1024), pflashSectorSize);
//
//    /* Check security status. */
//    result = FLASH_GetSecurityState(&s_flashDriver, &securityStatus);
//    if (kStatus_FLASH_Success != result)
//    {
//        error_trap();
//    }
//    /* Print security status. */
//    switch (securityStatus)
//    {
//        case kFLASH_SecurityStateNotSecure:
//            PRINTF("\r\n Flash is UNSECURE!");
//            break;
//        case kFLASH_SecurityStateBackdoorEnabled:
//            PRINTF("\r\n Flash is SECURE, BACKDOOR is ENABLED!");
//            break;
//        case kFLASH_SecurityStateBackdoorDisabled:
//            PRINTF("\r\n Flash is SECURE, BACKDOOR is DISABLED!");
//            break;
//        default:
//            break;
//    }
//    PRINTF("\r\n");
//
//    /* Test pflash basic opeation only if flash is unsecure. */
//    if (kFLASH_SecurityStateNotSecure == securityStatus)
//    {
//        /* Debug message for user. */
//        /* Erase several sectors on upper pflash block where there is no code */
//        PRINTF("\r\n Erase a sector of flash");
//
///* Erase a sector from destAdrss. */
//#if defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
//        /* Note: we should make sure that the sector shouldn't be swap indicator sector*/
//        destAdrss = pflashBlockBase + (pflashTotalSize - (pflashSectorSize * 2));
//#else
//        destAdrss = pflashBlockBase +(pflashTotalSize - 0x2000- pflashSectorSize);
//#endif
//        result = FLASH_Erase(&s_flashDriver, destAdrss, pflashSectorSize, kFLASH_ApiEraseKey);
//        if (kStatus_FLASH_Success != result)
//        {
//            error_trap();
//        }
//
//        /* Verify sector if it's been erased. */
//        result = FLASH_VerifyErase(&s_flashDriver, destAdrss, pflashSectorSize, kFLASH_MarginValueUser);
//        if (kStatus_FLASH_Success != result)
//        {
//            error_trap();
//        }
//
//        /* Print message for user. */
//        PRINTF("\r\n Successfully Erased Sector 0x%x -> 0x%x\r\n", destAdrss, (destAdrss + pflashSectorSize));
//
//        /* Print message for user. */
//        PRINTF("\r\n Program a buffer to a sector of flash ");
//        /* Prepare buffer. */
//        for (i = 0; i < BUFFER_LEN; i++)
//        {
//            s_buffer[i] = i;
//        }
//        GPIO_TogglePinsOutput(GPIOE,1<<PIN30_IDX);
//
//
//        result = FLASH_Program(&s_flashDriver, destAdrss, s_buffer, sizeof(s_buffer));
//        if (kStatus_FLASH_Success != result)
//        {
//            error_trap();
//        }
//
//        /* Program Check user margin levels */
//        result = FLASH_VerifyProgram(&s_flashDriver, destAdrss, sizeof(s_buffer), s_buffer, kFLASH_MarginValueUser,
//                                     &failAddr, &failDat);
//        if (kStatus_FLASH_Success != result)
//        {
//            error_trap();
//        }
//        destAdrss+=0x2000;
//         result = FLASH_Erase(&s_flashDriver, destAdrss, pflashSectorSize, kFLASH_ApiEraseKey);
//        if (kStatus_FLASH_Success != result)
//        {
//            error_trap();
//        }
//
//        /* Verify sector if it's been erased. */
//        result = FLASH_VerifyErase(&s_flashDriver, destAdrss, pflashSectorSize, kFLASH_MarginValueUser);
//        if (kStatus_FLASH_Success != result)
//        {
//            error_trap();
//        }
//
//         result = FLASH_Program(&s_flashDriver, destAdrss, s_buffer, sizeof(s_buffer));
//        if (kStatus_FLASH_Success != result)
//        {
//            error_trap();
//        }
//
//        /* Program Check user margin levels */
//        result = FLASH_VerifyProgram(&s_flashDriver, destAdrss, sizeof(s_buffer), s_buffer, kFLASH_MarginValueUser,
//                                     &failAddr, &failDat);
//        if (kStatus_FLASH_Success != result)
//        {
//            error_trap();
//        }
//
//           uint32_t u[8]={0x01,0x02,0x03,0x04};
//                    if (progSector( 0x100a0420, u, sizeof(u)))//read len storege flash programm
//                        {
//                          PRINTF("Error prog LEN FW2 \r\n"); //Îøèáêà
//                          return 1;
//                        }
//        GPIO_TogglePinsOutput(GPIOE,1<<PIN30_IDX);
//        PRINTF("\r\n Successfully Programmed and Verified Location 0x%x -> 0x%x \r\n", destAdrss,
//               (destAdrss + sizeof(s_buffer)));
//
//        /* Erase the context we have progeammed before*/
//        /* Note: we should make sure that the sector which will be set as swap indicator should be blank*/
//        FLASH_Erase(&s_flashDriver, destAdrss, pflashSectorSize, kFLASH_ApiEraseKey);
//    }
//    else
//    {
//        PRINTF("\r\n Erase/Program opeation will not be executed, as Flash is SECURE!");
//    }
//
//    app_finalize();
//
//    return 0;
//}
