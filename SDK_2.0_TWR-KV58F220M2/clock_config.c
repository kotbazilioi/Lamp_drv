/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
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

/*
 * How to setup clock using clock driver functions:
 *
 * 1. CLOCK_SetSimSafeDivs, to make sure core clock, bus clock, flexbus clock
 *    and flash clock are in allowed range during clock mode switch.
 *
 * 2. Call CLOCK_Osc0Init to setup OSC clock, if it is used in target mode.
 *
 * 3. Set MCG configuration, MCG includes three parts: FLL clock, PLL clock and
 *    internal reference clock(MCGIRCLK). Follow the steps to setup:
 *
 *    1). Call CLOCK_BootToXxxMode to set MCG to target mode.
 *
 *    2). If target mode is FBI/BLPI/PBI mode, the MCGIRCLK has been configured
 *        correctly. For other modes, need to call CLOCK_SetInternalRefClkConfig
 *        explicitly to setup MCGIRCLK.
 *
 *    3). Don't need to configure FLL explicitly, because if target mode is FLL
 *        mode, then FLL has been configured by the function CLOCK_BootToXxxMode,
 *        if the target mode is not FLL mode, the FLL is disabled.
 *
 *    4). If target mode is PEE/PBE/PEI/PBI mode, then the related PLL has been
 *        setup by CLOCK_BootToXxxMode. In FBE/FBI/FEE/FBE mode, the PLL could
 *        be enabled independently, call CLOCK_EnablePll0 explicitly in this case.
 *
 * 4. Call CLOCK_SetSimConfig to set the clock configuration in SIM.
 */

/* TEXT BELOW IS USED AS SETTING FOR THE CLOCKS TOOL *****************************
!!ClocksProfile
product: Clocks v1.0
processor: MKV58F1M0xxx24
package_id: MKV58F1M0VLQ24
mcu_data: ksdk2_0
processor_version: 1.2.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE CLOCKS TOOL **/

#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MCG_IRCLK_DISABLE                                 0U  /*!< MCGIRCLK disabled */
#define SIM_ENET_RMII_CLK_SEL_CLKIN_CLK                   1U  /*!< SDHC clock select: CLKIN (External bypass clock) */
#define SIM_OSC32KSEL_LPO_CLK                             3U  /*!< OSC32KSEL select: LPO clock */
#define SIM_PLLFLLSEL_MCGFLLCLK_CLK                       0U  /*!< PLLFLL select: MCGFLLCLK clock */

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_CONFIG_SetSimSafeDivs
 * Description   : This function sets the system clock dividers in SIM to safe 
 * value.
 *
 *END**************************************************************************/
static void CLOCK_CONFIG_SetSimSafeDivs(void)
{
    SIM->CLKDIV1 = 0x01170000U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_CONFIG_SetFllExtRefDiv
 * Description   : Configure FLL external reference divider (FRDIV).
 * Param frdiv   : The value to set FRDIV.
 *
 *END**************************************************************************/
static void CLOCK_CONFIG_SetFllExtRefDiv(uint8_t frdiv)
{
    MCG->C1 = ((MCG->C1 & ~MCG_C1_FRDIV_MASK) | MCG_C1_FRDIV(frdiv));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_CONFIG_SetRmii0Clock
 * Description   : Set RMII clock source.
 * Param src     : The value to set RMII clock source.
 *
 *END**************************************************************************/
static void CLOCK_CONFIG_SetRmii0Clock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_RMIISRC_MASK) | SIM_SOPT2_RMIISRC(src));
}

/*******************************************************************************
 ********************* Configuration BOARD_BootClockHSRUN **********************
 ******************************************************************************/
/* TEXT BELOW IS USED AS SETTING FOR THE CLOCKS TOOL *****************************
!!Configuration
name: BOARD_BootClockHSRUN
outputs:
- {id: Bus_clock.outFreq, value: 100 MHz}
- {id: Core_clock.outFreq, value: 100 MHz}
- {id: ERCLK32K.outFreq, value: 1 kHz}
- {id: Flash_clock.outFreq, value: 25 MHz}
- {id: FlexBus_clock.outFreq, value: 50 MHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: MCGFFCLK.outFreq, value: 195.3125/2 kHz}
- {id: NANOEDGE2XCLK.outFreq, value: 200 MHz}
- {id: OSCERCLK.outFreq, value: 25 MHz}
- {id: OSCERCLK_UNDIV.outFreq, value: 25 MHz}
- {id: RMIICLK.outFreq, value: 50 MHz}
- {id: System_clock.outFreq, value: 100 MHz}
settings:
- {id: MCGMode, value: PEE}
- {id: MCG.FLL_mul.scale, value: '2929', locked: true}
- {id: MCG.FRDIV.scale, value: '256', locked: true}
- {id: MCG.IRCS.sel, value: MCG.FCRDIV}
- {id: MCG.IREFS.sel, value: MCG.FRDIV}
- {id: MCG.PLLS.sel, value: MCG.PLL_DIV2}
- {id: MCG.PRDIV.scale, value: '2', locked: true}
- {id: MCG.VDIV.scale, value: '32', locked: true}
- {id: MCG_C2_OSC_MODE_CFG, value: ModeOscLowPower}
- {id: MCG_C2_RANGE0_CFG, value: Very_high}
- {id: MCG_C2_RANGE0_FRDIV_CFG, value: Very_high}
- {id: MCG_C5_PLLCLKEN0_CFG, value: Enabled}
- {id: MCG_C5_PLLSTEN0_CFG, value: Enabled}
- {id: NANOEDGE2XClkConfig, value: 'yes'}
- {id: OSC_CR_ERCLKEN_CFG, value: Enabled}
- {id: OSC_CR_ERCLKEN_UNDIV_CFG, value: Enabled}
- {id: OSC_CR_EREFSTEN_CFG, value: Enabled}
- {id: OSC_CR_EREFSTEN_UNDIV_CFG, value: Enabled}
- {id: OSC_CR_SYS_OSC_CAP_LOAD_CFG, value: SC10PF}
- {id: RMIISrcConfig, value: 'yes'}
- {id: SIM.OSC32KSEL.sel, value: PMC.LPOCLK}
- {id: SIM.OUTDIV1.scale, value: '2', locked: true}
- {id: SIM.OUTDIV2.scale, value: '2', locked: true}
- {id: SIM.OUTDIV3.scale, value: '4', locked: true}
- {id: SIM.OUTDIV4.scale, value: '8', locked: true}
- {id: SIM.RMIICLKSEL.sel, value: SIM.ENET_1588_CLK_EXT}
sources:
- {id: OSC.OSC.outFreq, value: 25 MHz, enabled: true}
- {id: SIM.ENET_1588_CLK_EXT.outFreq, value: 50 MHz, enabled: true}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE CLOCKS TOOL **/

/*******************************************************************************
 * Variables for BOARD_BootClockHSRUN configuration
 ******************************************************************************/
const mcg_config_t mcgConfig_BOARD_BootClockHSRUN =
    {
        .mcgMode = kMCG_ModePEE,                  /* PEE - PLL Engaged External */
        .irclkEnableMode = MCG_IRCLK_DISABLE,     /* MCGIRCLK disabled */
        .ircs = kMCG_IrcFast,                     /* Fast internal reference clock selected */
        .fcrdiv = 0x1U,                           /* Fast IRC divider: divided by 2 */
        .frdiv = 0x3U,                            /* FLL reference clock divider: divided by 256 */
        .drs = kMCG_DrsHigh,                      /* High frequency range */
        .dmx32 = kMCG_Dmx32Fine,                  /* DCO is fine-tuned for maximum frequency with 32.768 kHz reference */
        .pll0Config =
            {
                .enableMode = kMCG_PllEnableIndependent | kMCG_PllEnableInStop,/* MCGPLLCLK enabled independently of MCG clock mode as well as in STOP mode */
                .prdiv = 0x1U,                    /* PLL Reference divider: divided by 2 */
                .vdiv = 0x10U,                    /* VCO divider: multiplied by 32 */
            },
    };
const sim_clock_config_t simConfig_BOARD_BootClockHSRUN =
    {
        .pllFllSel = SIM_PLLFLLSEL_MCGFLLCLK_CLK, /* PLLFLL select: MCGFLLCLK clock */
        .er32kSrc = SIM_OSC32KSEL_LPO_CLK,        /* OSC32KSEL select: LPO clock */
        .clkdiv1 = 0x11370000U,                   /* SIM_CLKDIV1 - OUTDIV1: /2, OUTDIV2: /2, OUTDIV3: /4, OUTDIV4: /8 */
    };
const osc_config_t oscConfig_BOARD_BootClockHSRUN =
    {
        .freq = 25000000U,                        /* Oscillator frequency: 25000000Hz */
        .capLoad = (kOSC_Cap2P | kOSC_Cap8P),     /* Oscillator capacity load: 10pF */
        .workMode = kOSC_ModeOscLowPower,         /* Oscillator low power */
        .oscerConfig =
            {
                .enableMode = kOSC_ErClkEnable | kOSC_ErClkEnableInStop,/* Enable external reference clock, enable external reference clock in STOP mode */
                .erclkDiv = 0,                    /* Divider for OSCERCLK: divided by 1 */
            }
    };

/*******************************************************************************
 * Code for BOARD_BootClockHSRUN configuration
 ******************************************************************************/
void BOARD_BootClockHSRUN(void)
{
    /* Set the system clock dividers in SIM to safe value. */
    CLOCK_CONFIG_SetSimSafeDivs();
    /* Initializes OSC0 according to board configuration. */
    CLOCK_InitOsc0(&oscConfig_BOARD_BootClockHSRUN);
    CLOCK_SetXtal0Freq(oscConfig_BOARD_BootClockHSRUN.freq);
    /* Configure FLL external reference divider (FRDIV). */
    CLOCK_CONFIG_SetFllExtRefDiv(mcgConfig_BOARD_BootClockHSRUN.frdiv);
    /* Set MCG to PEE mode. */
    CLOCK_BootToPeeMode(kMCG_OscselOsc,
                        kMCG_PllClkSelPll0,
                        &mcgConfig_BOARD_BootClockHSRUN.pll0Config);
    /* Set the clock configuration in SIM module. */
    CLOCK_SetSimConfig(&simConfig_BOARD_BootClockHSRUN);
    /* Set SystemCoreClock variable. */
    SystemCoreClock = BOARD_BOOTCLOCKHSRUN_CORE_CLOCK;
    /* Set RMII clock source. */
    CLOCK_CONFIG_SetRmii0Clock(SIM_ENET_RMII_CLK_SEL_CLKIN_CLK);
}

