/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_flash.h"
#if defined(FSL_FEATURE_HAS_L1CACHE) && FSL_FEATURE_HAS_L1CACHE
#include "fsl_cache.h"
#endif /* FSL_FEATURE_HAS_L1CACHE */
#include "fsl_ftfx_flash.h"
#include "pin_mux.h"
#include "fsl_ftfx_controller.h"
#include "fsl_ftfx_cache.h"
#include "fsl_ftfx_flexnvm.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define BUFFER_LEN 4

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
/*! @brief Buffer for program */
//////static uint32_t s_buffer[BUFFER_LEN];
///////*! @brief Buffer for readback */
//////static uint32_t s_buffer_rbc[BUFFER_LEN];

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
    while (1)
    {
    }
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
    while (1)
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

int progEraseSector (uint32_t adress,uint32_t pflashSectorSize)
{
   
        status_t result; 
        result = FLASH_Erase(&s_flashDriver, adress, pflashSectorSize, kFTFx_ApiEraseKey);
        if (kStatus_FTFx_Success != result)
        {
            error_trap();
            return result;
        }

        /* Verify sector if it's been erased. */
        result = FLASH_VerifyErase(&s_flashDriver, adress, pflashSectorSize, kFTFx_MarginValueUser);
        if (kStatus_FTFx_Success != result)
        {
            error_trap();
            return result;
        }
        

        return kStatus_FLASH_Success;
}



/////*!
//// * @brief Use Standard Software Drivers (SSD) to modify pflash.
//// *
//// * @details This function uses SSD to demonstrate flash mode:
//// *            + Check pflash information.
//// *            + Erase a sector and verify.
//// *            + Program a sector and verify.
//// */
int progFLASH_init(void)
  {
    ftfx_security_state_t securityStatus = kFTFx_SecurityStateNotSecure; /* Return protection status */
    status_t result;    /* Return code from each flash driver function */
   // uint32_t destAdrss; /* Address of the target location */
//////    uint32_t i, failAddr, failDat;

    uint32_t pflashBlockBase = 0;
    uint32_t pflashTotalSize = 0;
    uint32_t pflashSectorSize = 0;



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

//////    /* print welcome message */
//////    PRINTF("\r\n PFlash Example Start \r\n");
//////    /* Print flash information - PFlash. */
//////    PRINTF("\r\n PFlash Information: ");
//////    PRINTF("\r\n Total Program Flash Size:\t%d KB, Hex: (0x%x)", (pflashTotalSize / 1024), pflashTotalSize);
//////    PRINTF("\r\n Program Flash Sector Size:\t%d KB, Hex: (0x%x) ", (pflashSectorSize / 1024), pflashSectorSize);

    /* Check security status. */
    result = FLASH_GetSecurityState(&s_flashDriver, &securityStatus);
    if (kStatus_FTFx_Success != result)
    {
        error_trap();
    }
    /* Print security status. */
    switch (securityStatus)
    {
        case kFTFx_SecurityStateNotSecure:
           //// PRINTF("\r\n Flash is UNSECURE!");
            break;
        case kFTFx_SecurityStateBackdoorEnabled:
            PRINTF("\r\n Flash is SECURE, BACKDOOR is ENABLED!");
            break;
        case kFTFx_SecurityStateBackdoorDisabled:
            PRINTF("\r\n Flash is SECURE, BACKDOOR is DISABLED!");
            break;
        default:
            break;
    }
 //   PRINTF("\r\n");

    /* Test pflash basic opeation only if flash is unsecure. */
    if (kFTFx_SecurityStateNotSecure == securityStatus)
    {
        /* Pre-preparation work about flash Cache/Prefetch/Speculation. */
        FTFx_CACHE_ClearCachePrefetchSpeculation(&s_cacheDriver, true);

        /* Debug message for user. */
        /* Erase several sectors on upper pflash block where there is no code */
////        PRINTF("\r\n Erase a sector of flash");
    }
    return 0;
  }
 
///////* In case of the protected sectors at the end of the pFlash just select
//////the block from the end of pFlash to be used for operations
//////SECTOR_INDEX_FROM_END = 1 means the last sector,
//////SECTOR_INDEX_FROM_END = 2 means (the last sector - 1) ...
//////in case of FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP it is
//////SECTOR_INDEX_FROM_END = 1 means the last 2 sectors with width of 2 sectors,
//////SECTOR_INDEX_FROM_END = 2 means the last 4 sectors back
//////with width of 2 sectors ...
//////*/
//////#ifndef SECTOR_INDEX_FROM_END
//////  #define SECTOR_INDEX_FROM_END 1U
//////#endif
//////
///////* Erase a sector from destAdrss. */
//////#if defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
//////        /* Note: we should make sure that the sector shouldn't be swap indicator sector*/
//////        destAdrss = pflashBlockBase + (pflashTotalSize - (SECTOR_INDEX_FROM_END * pflashSectorSize * 2));
//////#else
//////        destAdrss = pflashBlockBase + (pflashTotalSize - (SECTOR_INDEX_FROM_END * pflashSectorSize));
//////#endif
//////
//////        
//////        
//////        
//////        result = FLASH_Erase(&s_flashDriver, destAdrss, pflashSectorSize, kFTFx_ApiEraseKey);
//////        if (kStatus_FTFx_Success != result)
//////        {
//////            error_trap();
//////        }
//////
//////        /* Verify sector if it's been erased. */
//////        result = FLASH_VerifyErase(&s_flashDriver, destAdrss, pflashSectorSize, kFTFx_MarginValueUser);
//////        if (kStatus_FTFx_Success != result)
//////        {
//////            error_trap();
//////        }
//////
//////        /* Print message for user. */
//////        PRINTF("\r\n Successfully Erased Sector 0x%x -> 0x%x\r\n", destAdrss, (destAdrss + pflashSectorSize));
//////
//////        /* Print message for user. */
//////        PRINTF("\r\n Program a buffer to a sector of flash ");
//////        /* Prepare user buffer. */
//////        for (i = 0; i < BUFFER_LEN; i++)
//////        {
//////            s_buffer[i] = i;
//////        }
//////
//////        /* Program user buffer into flash*/
//////        result = FLASH_Program(&s_flashDriver, destAdrss, (uint8_t *)s_buffer, sizeof(s_buffer));
//////        if (kStatus_FTFx_Success != result)
//////        {
//////            error_trap();
//////        }
//////
//////        /* Verify programming by Program Check command with user margin levels */
//////        result = FLASH_VerifyProgram(&s_flashDriver, destAdrss, sizeof(s_buffer), (const uint8_t *)s_buffer, kFTFx_MarginValueUser,
//////                                     &failAddr, &failDat);
//////        if (kStatus_FTFx_Success != result)
//////        {
//////            error_trap();
//////        }
//////
//////        /* Post-preparation work about flash Cache/Prefetch/Speculation. */
//////        FTFx_CACHE_ClearCachePrefetchSpeculation(&s_cacheDriver, false);
//////
//////#if defined(FSL_FEATURE_HAS_L1CACHE) && FSL_FEATURE_HAS_L1CACHE
//////        L1CACHE_InvalidateCodeCache();
//////#endif /* FSL_FEATURE_HAS_L1CACHE */
//////
//////#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
//////        /* Clean the D-Cache before reading the flash data*/
//////        SCB_CleanInvalidateDCache();
//////#endif
////////        /* Verify programming by reading back from flash directly*/
////////        for (uint32_t i = 0; i < BUFFER_LEN; i++)
////////        {
////////            s_buffer_rbc[i] = *(volatile uint32_t *)(destAdrss + i * 4);
////////            if (s_buffer_rbc[i] != s_buffer[i])
////////            {
////////                error_trap();
////////            }
////////        }
//////
//////
//////    return 0;
//////    }
//////  }
    int progSector (uint32_t adress,uint32_t *data_prog, uint32_t len_s)
{
  
  status_t result;    /* Return code from each flash driver function */
  
    taskENTER_CRITICAL();
    
    progFLASH_init();
    
    result = FLASH_Erase(&s_flashDriver, adress, len_s, kFTFx_ApiEraseKey);
        if (kStatus_FTFx_Success != result)
        {
            error_trap();
        }

        /* Verify sector if it's been erased. */
        result = FLASH_VerifyErase(&s_flashDriver, adress, len_s, kFTFx_MarginValueUser);
        if (kStatus_FTFx_Success != result)
        {
            error_trap();
        }

//        /* Print message for user. */
//        PRINTF("\r\n Successfully Erased Sector 0x%x -> 0x%x\r\n", destAdrss, (destAdrss + pflashSectorSize));
//
//        /* Print message for user. */
//        PRINTF("\r\n Program a buffer to a sector of flash ");
//        /* Prepare user buffer. */
//        for (i = 0; i < BUFFER_LEN; i++)
//        {
//            s_buffer[i] = i;
//        }

        /* Program user buffer into flash*/
        result = FLASH_Program(&s_flashDriver, adress, (uint8_t *)data_prog, len_s);
        if (kStatus_FTFx_Success != result)
        {
            error_trap();
        }

        /* Verify programming by Program Check command with user margin levels */
        result = FLASH_VerifyProgram(&s_flashDriver, adress, len_s, (uint8_t *)data_prog, kFTFx_MarginValueUser,0, 0);
        
      ////  adress, len_s,
////////                                     data_prog, kFLASH_MarginValueUser,0, 0);
        if (kStatus_FTFx_Success != result)
        {
            error_trap();
        }

        /* Post-preparation work about flash Cache/Prefetch/Speculation. */
        FTFx_CACHE_ClearCachePrefetchSpeculation(&s_cacheDriver, false);

#if defined(FSL_FEATURE_HAS_L1CACHE) && FSL_FEATURE_HAS_L1CACHE
        L1CACHE_InvalidateCodeCache();
#endif /* FSL_FEATURE_HAS_L1CACHE */

#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
        /* Clean the D-Cache before reading the flash data*/
        SCB_CleanInvalidateDCache();
#endif
        
        
        
//////// /*! @brief Flash driver Structure */
//////// status_t result;    /* Return code from each flash driver function */
////////
////////taskENTER_CRITICAL();
////////
////////
////////
////////    result = FLASH_Init(&s_flashDriver);
////////    if (kStatus_FLASH_Success != result)
////////    {
////////        error_trap();
////////        return result;
////////    }
////////    FMC->PFB0CR |= 1<<19;
////////
//////// result = FLASH_Erase(&s_flashDriver, adress, len_s, kFLASH_ApiEraseKey);
////////        if (kStatus_FLASH_Success != result)
////////        {
////////            error_trap();
////////            return result;
////////        }
////////
////////
////////        result = FLASH_VerifyErase(&s_flashDriver, adress, len_s, kFLASH_MarginValueUser);
////////        if (kStatus_FLASH_Success != result)
////////        {
////////            error_trap();
////////                return result;
////////        }
////////    FLASH_Init(&s_flashDriver);
////////    result = FLASH_Program(&s_flashDriver, adress, data_prog,len_s);
////////        if (kStatus_FLASH_Success != result)
////////        {
////////            error_trap();
////////            return result;
////////
////////        }
////////       while (((FTFE->FSTAT)&FTFE_FSTAT_CCIF_MASK)==0){
////////
////////        }
////////
////////       result = FLASH_VerifyProgram(&s_flashDriver, adress, len_s,
////////                                     data_prog, kFLASH_MarginValueUser,0, 0);
////////        if (kStatus_FLASH_Success != result)
////////        {
////////            error_trap();
////////            return result;
////////
////////        }
        taskEXIT_CRITICAL();

return kStatus_FLASH_Success;
}
    
