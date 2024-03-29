

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
/*! @brief Buffer for program */
static uint32_t s_buffer[BUFFER_LEN];

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

/*!
 * @brief Use Standard Software Drivers (SSD) to modify pflash.
 *
 * @details This function uses SSD to demonstrate flash mode:
 *            + Check pflash information.
 *            + Erase a sector and verify.
 *            + Program a sector and verify.
 */
int progF (void)
{
    flash_security_state_t securityStatus = kFLASH_SecurityStateNotSecure; /* Return protection status */
    status_t result;    /* Return code from each flash driver function */
    uint32_t destAdrss; /* Address of the target location */
    uint32_t i, failAddr, failDat;

    uint32_t pflashBlockBase = 0;
    uint32_t pflashTotalSize = 0;
    uint32_t pflashSectorSize = 0;

   
    /* Clean up Flash driver Structure*/
    memset(&s_flashDriver, 0, sizeof(flash_config_t));

    /* Setup flash driver structure for device and initialize variables. */
    result = FLASH_Init(&s_flashDriver);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }
    /* Get flash properties*/
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflashBlockBaseAddr, &pflashBlockBase);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflashTotalSize, &pflashTotalSize);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflashSectorSize, &pflashSectorSize);

    /* print welcome message */
    PRINTF("\r\n PFlash Example Start \r\n");
    /* Print flash information - PFlash. */
    PRINTF("\r\n PFlash Information: ");
    PRINTF("\r\n Total Program Flash Size:\t%d KB, Hex: (0x%x)", (pflashTotalSize / 1024), pflashTotalSize);
    PRINTF("\r\n Program Flash Sector Size:\t%d KB, Hex: (0x%x) ", (pflashSectorSize / 1024), pflashSectorSize);

    /* Check security status. */
    result = FLASH_GetSecurityState(&s_flashDriver, &securityStatus);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }
    /* Print security status. */
    switch (securityStatus)
    {
        case kFLASH_SecurityStateNotSecure:
            PRINTF("\r\n Flash is UNSECURE!");
            break;
        case kFLASH_SecurityStateBackdoorEnabled:
            PRINTF("\r\n Flash is SECURE, BACKDOOR is ENABLED!");
            break;
        case kFLASH_SecurityStateBackdoorDisabled:
            PRINTF("\r\n Flash is SECURE, BACKDOOR is DISABLED!");
            break;
        default:
            break;
    }
    PRINTF("\r\n");

    /* Test pflash basic opeation only if flash is unsecure. */
    if (kFLASH_SecurityStateNotSecure == securityStatus)
    {
        /* Debug message for user. */
        /* Erase several sectors on upper pflash block where there is no code */
        PRINTF("\r\n Erase a sector of flash");

/* Erase a sector from destAdrss. */
#if defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
        /* Note: we should make sure that the sector shouldn't be swap indicator sector*/
        destAdrss = pflashBlockBase + (pflashTotalSize - (pflashSectorSize * 2));
#else
        destAdrss = pflashBlockBase + (pflashTotalSize - pflashSectorSize);
#endif
        result = FLASH_Erase(&s_flashDriver, destAdrss, pflashSectorSize, kFLASH_ApiEraseKey);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }

        /* Verify sector if it's been erased. */
        result = FLASH_VerifyErase(&s_flashDriver, destAdrss, pflashSectorSize, kFLASH_MarginValueUser);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }

        /* Print message for user. */
        PRINTF("\r\n Successfully Erased Sector 0x%x -> 0x%x\r\n", destAdrss, (destAdrss + pflashSectorSize));

        /* Print message for user. */
        PRINTF("\r\n Program a buffer to a sector of flash ");
        /* Prepare buffer. */
        for (i = 0; i < BUFFER_LEN; i++)
        {
            s_buffer[i] = i;
        }
        result = FLASH_Program(&s_flashDriver, destAdrss, s_buffer, sizeof(s_buffer));
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }

        /* Program Check user margin levels */
        result = FLASH_VerifyProgram(&s_flashDriver, destAdrss, sizeof(s_buffer), s_buffer, kFLASH_MarginValueUser,
                                     &failAddr, &failDat);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }
        PRINTF("\r\n Successfully Programmed and Verified Location 0x%x -> 0x%x \r\n", destAdrss,
               (destAdrss + sizeof(s_buffer)));

        /* Erase the context we have progeammed before*/
        /* Note: we should make sure that the sector which will be set as swap indicator should be blank*/
        FLASH_Erase(&s_flashDriver, destAdrss, pflashSectorSize, kFLASH_ApiEraseKey);
    }
    else
    {
        PRINTF("\r\n Erase/Program opeation will not be executed, as Flash is SECURE!");
    }

    app_finalize();

    return 0;
}
