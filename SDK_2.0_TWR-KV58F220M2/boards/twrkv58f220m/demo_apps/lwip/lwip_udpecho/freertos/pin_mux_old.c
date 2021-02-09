/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
 *
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
PinsProfile:
- !!product 'Pins v2.0'
- !!processor 'MKV58F1M0xxx24'
- !!package 'MKV58F1M0VLL24'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.2.0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_xbara.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "board.h"


#define PCR_ODE_ENABLED                                                                                                \
    0x01u /*!< Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a \
             digital output. */
#define PCR_PE_ENABLED                                                                                              \
    0x01u /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is \
             configured as a digital input. */
#define PCR_PS_UP                                                                                                 \
    0x01u /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE \
             field is set. */



/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '34', peripheral: GPIOA, signal: 'GPIO, 0', pin_signal: PTA0/UART0_CTS_b/UART0_COL_b/FTM0_CH5/XB_IN4/EWM_IN/JTAG_TCLK/SWD_CLK}
  - {pin_num: '35', peripheral: UART0, signal: RX, pin_signal: PTA1/UART0_RX/FTM0_CH6/CMP0_OUT/FTM2_QD_PHA/FTM1_CH1/JTAG_TDI}
  - {pin_num: '36', peripheral: GPIOA, signal: 'GPIO, 2', pin_signal: PTA2/UART0_TX/FTM0_CH7/CMP1_OUT/FTM2_QD_PHB/FTM1_CH0/JTAG_TDO/TRACE_SWO}
  - {pin_num: '37', peripheral: GPIOA, signal: 'GPIO, 3', pin_signal: PTA3/UART0_RTS_b/FTM0_CH0/XB_IN9/EWM_OUT_b/FLEXPWM0_A0/JTAG_TMS/SWD_DIO}
  - {pin_num: '38', peripheral: PWM0, signal: 'B, 0', pin_signal: PTA4/LLWU_P3/FTM0_CH1/XB_IN10/FTM0_FLT3/FLEXPWM0_B0/NMI_b}
  - {pin_num: '39', peripheral: ENET, signal: RMII_RXER, pin_signal: PTA5/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/JTAG_TRST_b}
  - {pin_num: '42', peripheral: ENET, signal: RMII_RXD1, pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/FTM1_QD_PHA/I2C0_SCL}
  - {pin_num: '43', peripheral: ENET, signal: RMII_RXD0, pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/FTM1_QD_PHB/I2C1_SDA}
  - {pin_num: '44', peripheral: ENET, signal: RMII_CRS_DV, pin_signal: CMP3_IN0/PTA14/SPI0_PCS0/UART0_TX/CAN2_TX/RMII0_CRS_DV/MII0_RXDV/I2C1_SCL}
  - {pin_num: '45', peripheral: ENET, signal: RMII_TXEN, pin_signal: CMP3_IN1/PTA15/SPI0_SCK/UART0_RX/CAN2_RX/RMII0_TXEN/MII0_TXEN}
  - {pin_num: '46', peripheral: ENET, signal: RMII_TXD0, pin_signal: CMP3_IN2/PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0}
  - {pin_num: '47', peripheral: ENET, signal: RMII_TXD1, pin_signal: HSADC0A_CH15/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1}
  - {pin_num: '50', peripheral: OSC0, signal: EXTAL0, pin_signal: EXTAL0/PTA18/XB_IN7/FTM0_FLT2/FTM_CLKIN0/XB_OUT8/FTM3_CH2}
  - {pin_num: '51', peripheral: OSC0, signal: XTAL0, pin_signal: XTAL0/PTA19/XB_IN8/FTM1_FLT0/FTM_CLKIN1/XB_OUT9/LPTMR0_ALT1}
  - {pin_num: '53', peripheral: ENET, signal: RMII_MDIO, pin_signal: HSADC0B_CH2/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/FTM1_QD_PHA/UART0_RX/RMII0_MDIO/MII0_MDIO}
  - {pin_num: '54', peripheral: ENET, signal: RMII_MDC, pin_signal: HSADC0B_CH3/PTB1/I2C0_SDA/FTM1_CH1/FTM0_FLT2/EWM_IN/FTM1_QD_PHB/UART0_TX/RMII0_MDC/MII0_MDC}
  - {pin_num: '55', peripheral: UART0, signal: RTS, pin_signal: HSADC0A_CH14/CMP2_IN2/PTB2/I2C0_SCL/UART0_RTS_b/FTM0_FLT1/ENET0_1588_TMR0/FTM0_FLT3}
  - {pin_num: '56', peripheral: UART0, signal: CTS, pin_signal: HSADC0B_CH15/CMP3_IN5/PTB3/I2C0_SDA/UART0_CTS_b/UART0_COL_b/ENET0_1588_TMR1/FTM0_FLT0}
  - {pin_num: '57', peripheral: GPIOB, signal: 'GPIO, 9', pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/ENET0_1588_TMR2/FB_AD20}
  - {pin_num: '58', peripheral: GPIOB, signal: 'GPIO, 10', pin_signal: HSADC0B_CH6/PTB10/SPI1_PCS0/UART3_RX/ENET0_1588_TMR3/FTM0_FLT1/FB_AD19}
  - {pin_num: '59', peripheral: GPIOB, signal: 'GPIO, 11', pin_signal: HSADC0B_CH7/PTB11/SPI1_SCK/UART3_TX/FTM0_FLT2/FB_AD18}
  - {pin_num: '62', peripheral: PWM0, signal: 'FAULT, 2', pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN2/CAN0_TX/EWM_IN/XB_IN5/FB_AD17}
  - {pin_num: '63', peripheral: UART0, signal: TX, pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/CAN0_RX/EWM_OUT_b/FB_AD16}
  - {pin_num: '64', peripheral: CAN0, signal: TX, pin_signal: PTB18/CAN0_TX/FTM2_CH0/FTM3_CH2/FLEXPWM1_A1/FTM2_QD_PHA/FB_AD15}
  - {pin_num: '65', peripheral: CAN0, signal: RX, pin_signal: PTB19/CAN0_RX/FTM2_CH1/FTM3_CH3/FLEXPWM1_B1/FTM2_QD_PHB/FB_OE_b}
  - {pin_num: '66', peripheral: PWM0, signal: 'X, 0', pin_signal: PTB20/SPI2_PCS0/FLEXPWM0_X0/CMP0_OUT/FB_AD31}
  - {pin_num: '67', peripheral: GPIOB, signal: 'GPIO, 21', pin_signal: PTB21/SPI2_SCK/FLEXPWM0_X1/CMP1_OUT/FB_AD30}
  - {pin_num: '68', peripheral: GPIOB, signal: 'GPIO, 22', pin_signal: PTB22/SPI2_SOUT/FLEXPWM0_X2/CMP2_OUT/FB_AD29}
  - {pin_num: '69', peripheral: GPIOB, signal: 'GPIO, 23', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FLEXPWM0_X3/CMP3_OUT/FB_AD28}
  - {pin_num: '70', peripheral: HSADC0, signal: 'ADCB, CH8', pin_signal: HSADC0B_CH8/PTC0/SPI0_PCS4/PDB0_EXTRG/FTM0_FLT1/SPI0_PCS0/FB_AD14}
  - {pin_num: '71', peripheral: PWM0, signal: 'FAULT, 0', pin_signal: HSADC0B_CH9/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FLEXPWM0_A3/XB_IN11/FB_AD13}
  - {pin_num: '72', peripheral: PWM0, signal: 'FAULT, 1', pin_signal: HSADC1B_CH10/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FLEXPWM0_B3/XB_IN6/FB_AD12}
  - {pin_num: '73', peripheral: SPI0, signal: PCS1, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/FTM3_FLT0}
  - {pin_num: '76', peripheral: GPIOC, signal: 'GPIO, 4', pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/CMP1_OUT/FB_AD11}
  - {pin_num: '77', peripheral: GPIOC, signal: 'GPIO, 5', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/XB_IN2/CMP0_OUT/FTM0_CH2/FB_AD10}
  - {pin_num: '78', peripheral: I2C0, signal: SCL, pin_signal: CMP2_IN4/CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/XB_IN3/UART0_RX/XB_OUT6/I2C0_SCL/FB_AD9}
  - {pin_num: '79', peripheral: I2C0, signal: SDA, pin_signal: CMP3_IN4/CMP0_IN1/PTC7/SPI0_SIN/XB_IN4/UART0_TX/XB_OUT7/I2C0_SDA/FB_AD8}
  - {pin_num: '80', peripheral: GPIOC, signal: 'GPIO, 8', pin_signal: HSADC1B_CH11/CMP0_IN2/PTC8/FTM3_CH4/FLEXPWM1_A2/FB_AD7}
  - {pin_num: '81', peripheral: GPIOC, signal: 'GPIO, 9', pin_signal: HSADC1B_CH12/CMP0_IN3/PTC9/FTM3_CH5/FLEXPWM1_B2/FB_AD6}
  - {pin_num: '83', peripheral: PWM1, signal: 'B, 3', pin_signal: HSADC1B_CH14/PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/FLEXPWM1_B3/FB_RW_b}
  - {pin_num: '82', peripheral: GPIOC, signal: 'GPIO, 10', pin_signal: HSADC1B_CH13/PTC10/I2C1_SCL/FTM3_CH6/FLEXPWM1_A3/FB_AD5}
  - {pin_num: '84', peripheral: PWM1, signal: 'A, 1', pin_signal: PTC12/CAN2_TX/FTM_CLKIN0/FLEXPWM1_A1/FTM3_FLT0/SPI2_PCS1/FB_AD27/UART4_RTS_b}
  - {pin_num: '85', peripheral: PWM1, signal: 'B, 1', pin_signal: PTC13/CAN2_RX/FTM_CLKIN1/FLEXPWM1_B1/FB_AD26/UART4_CTS_b}
  - {pin_num: '86', peripheral: PWM1, signal: 'A, 0', pin_signal: PTC14/I2C1_SCL/I2C0_SCL/FLEXPWM1_A0/FB_AD25/UART4_RX}
  - {pin_num: '87', peripheral: PWM1, signal: 'B, 0', pin_signal: PTC15/I2C1_SDA/I2C0_SDA/FLEXPWM1_B0/FB_AD24/UART4_TX}
  - {pin_num: '90', peripheral: PWM1, signal: 'A, 2', pin_signal: PTC16/CAN1_RX/UART3_RX/ENET0_1588_TMR0/FLEXPWM1_A2/FB_CS5_b/FB_TSIZ1/FB_BE23_16_b}
  - {pin_num: '91', peripheral: PWM1, signal: 'B, 2', pin_signal: PTC17/CAN1_TX/UART3_TX/ENET0_1588_TMR1/FLEXPWM1_B2/FB_CS4_b/FB_TSIZ0/FB_BE31_24_b}
  - {pin_num: '92', peripheral: PWM1, signal: 'A, 3', pin_signal: PTC18/UART3_RTS_b/ENET0_1588_TMR2/FLEXPWM1_A3/FB_TBST_b/FB_CS2_b/FB_BE15_8_b}
  - {pin_num: '100', peripheral: GPIOD, signal: 'GPIO, 7', pin_signal: PTD7/UART0_TX/FTM0_CH7/FTM1_CH1/FTM0_FLT1/SPI1_SIN}
  - {pin_num: '99', peripheral: GPIOD, signal: 'GPIO, 6', pin_signal: HSADC1A_CH9/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH6/FTM1_CH0/FTM0_FLT0/SPI1_SOUT/FB_AD0}
  - {pin_num: '98', peripheral: PWM0, signal: 'B, 2', pin_signal: HSADC1A_CH8/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FLEXPWM0_B2/EWM_OUT_b/SPI1_SCK/FB_AD1}
  - {pin_num: '97', peripheral: PWM0, signal: 'A, 2', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FLEXPWM0_A2/EWM_IN/SPI1_PCS0/FB_AD2}
  - {pin_num: '96', peripheral: PWM0, signal: 'B, 1', pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FTM0_CH3/FLEXPWM0_B1/I2C0_SDA/FB_AD3/FLEXPWM1_B1}
  - {pin_num: '95', peripheral: PWM0, signal: 'A, 1', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FTM0_CH2/FLEXPWM0_A1/I2C0_SCL/FB_AD4/FLEXPWM1_A1}
  - {pin_num: '93', peripheral: PWM0, signal: 'A, 0', pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FTM0_CH0/FLEXPWM0_A0/FB_ALE/FB_CS1_b/FB_TS_b/FLEXPWM1_A0}
  - {pin_num: '94', peripheral: GPIOD, signal: 'GPIO, 1', pin_signal: HSADC1A_CH11/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FTM0_CH1/FLEXPWM0_B0/FB_CS0_b/FLEXPWM1_B0}
  - {pin_num: '1', peripheral: I2C1, signal: SDA, pin_signal: HSADC0B_CH16/HSADC1A_CH0/PTE0/SPI1_PCS1/UART1_TX/XB_OUT10/XB_IN11/I2C1_SDA/TRACE_CLKOUT}
  - {pin_num: '2', peripheral: I2C1, signal: SCL, pin_signal: HSADC0B_CH17/HSADC1A_CH1/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/XB_OUT11/XB_IN7/I2C1_SCL/TRACE_D3}
  - {pin_num: '3', peripheral: HSADC0, signal: 'ADCB, CH10', pin_signal: HSADC0B_CH10/HSADC1B_CH0/PTE2/LLWU_P1/SPI1_SCK/UART1_CTS_b/TRACE_D2}
  - {pin_num: '4', peripheral: HSADC0, signal: 'ADCB, CH11', pin_signal: HSADC0B_CH11/HSADC1B_CH1/PTE3/SPI1_SIN/UART1_RTS_b/TRACE_D1}
  - {pin_num: '5', peripheral: GPIOE, signal: 'GPIO, 4', pin_signal: HSADC1A_CH4/ADC0_SE2/ADC0_DP2/PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/TRACE_D0}
  - {pin_num: '6', peripheral: GPIOE, signal: 'GPIO, 5', pin_signal: HSADC1A_CH5/ADC0_SE10/ADC0_DM2/PTE5/SPI1_PCS2/UART3_RX/FLEXPWM1_A0/FTM3_CH0}
  - {pin_num: '7', peripheral: GPIOE, signal: 'GPIO, 6', pin_signal: HSADC1B_CH7/ADC0_SE4a/PTE6/LLWU_P16/SPI1_PCS3/UART3_CTS_b/FLEXPWM1_B0/FTM3_CH1}
  - {pin_num: '10', peripheral: SPI0, signal: PCS0_SS, pin_signal: HSADC0A_CH0/ADC0_SE1/ADC0_DP1/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3}
  - {pin_num: '11', peripheral: SPI0, signal: SCK, pin_signal: HSADC0A_CH1/ADC0_SE9/ADC0_DM1/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3}
  - {pin_num: '12', peripheral: SPI0, signal: SOUT, pin_signal: HSADC0B_CH0/ADC0_SE5a/PTE18/LLWU_P20/SPI0_SOUT/UART2_CTS_b/I2C0_SDA}
  - {pin_num: '13', peripheral: SPI0, signal: SIN, pin_signal: HSADC0B_CH1/ADC0_SE6a/PTE19/SPI0_SIN/UART2_RTS_b/I2C0_SCL/CMP3_OUT}
  - {pin_num: '16', peripheral: FTM1, signal: 'QD_PH, A', pin_signal: HSADC0A_CH8/ADC0_SE5b/PTE20/FTM1_CH0/UART0_TX/FTM1_QD_PHA}
  - {pin_num: '17', peripheral: PWM1, signal: 'FAULT, 3', pin_signal: HSADC0A_CH9/HSADC1A_CH7/PTE21/XB_IN9/FTM1_CH1/UART0_RX/FTM1_QD_PHB}
  - {pin_num: '31', peripheral: GPIOE, signal: 'GPIO, 24', pin_signal: HSADC0B_CH4/HSADC1B_CH4/PTE24/CAN1_TX/FTM0_CH0/XB_IN2/I2C0_SCL/EWM_OUT_b/XB_OUT4/UART4_TX}
  - {pin_num: '32', peripheral: GPIOE, signal: 'GPIO, 25', pin_signal: HSADC0B_CH5/HSADC1B_CH5/PTE25/LLWU_P21/CAN1_RX/FTM0_CH1/XB_IN3/I2C0_SDA/EWM_IN/XB_OUT5/UART4_RX}
  - {pin_num: '33', peripheral: GPIOE, signal: 'GPIO, 26', pin_signal: PTE26/ENET_1588_CLKIN/FTM0_CH4/UART4_CTS_b}
  - {pin_num: '26', peripheral: GPIOE, signal: 'GPIO, 29', pin_signal: HSADC0A_CH4/CMP1_IN5/CMP0_IN5/PTE29/FTM0_CH2/FTM_CLKIN0}
  - {pin_num: '27', peripheral: GPIOE, signal: 'GPIO, 30', pin_signal: DAC0_OUT/CMP1_IN3/HSADC0A_CH5/PTE30/FTM0_CH3/FTM_CLKIN1}
  - {pin_num: '18', peripheral: HSADC0, signal: 'ADCA, CH2', pin_signal: HSADC0A_CH2/HSADC1A_CH2}
  - {pin_num: '19', peripheral: HSADC0, signal: 'ADCA, CH3', pin_signal: HSADC0A_CH3/HSADC1A_CH3}
  - {pin_num: '14', peripheral: HSADC0, signal: 'ADCA, CH6', pin_signal: HSADC0A_CH6/ADC0_SE7a}
  - {pin_num: '15', peripheral: HSADC0, signal: 'ADCA, CH7', pin_signal: HSADC0A_CH7/ADC0_SE4b}
  - {pin_num: '20', peripheral: HSADC0, signal: 'ADCA, CH10', pin_signal: HSADC0A_CH10/HSADC1B_CH2}
  - {pin_num: '21', peripheral: HSADC0, signal: 'ADCA, CH11', pin_signal: HSADC0A_CH11/HSADC1B_CH3}
  - {pin_num: '28', peripheral: HSADC0, signal: 'ADCA, CH12', pin_signal: HSADC0A_CH12/CMP0_IN4/CMP2_IN3}
  - {pin_num: '24', peripheral: ADC0, signal: VREFL, pin_signal: VREFL}
  - {pin_num: '23', peripheral: ADC0, signal: VREFH, pin_signal: VREFH}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
 // Define a digital input pin configuration,

void Ethernet_pin_init (void)
{
  PORT_SetPinMux(PORTA, PIN12_IDX, kPORT_MuxAlt5);           /* PORTA12 (pin 42) is configured as RMII0_RXD1 */
  PORT_SetPinMux(PORTA, PIN13_IDX, kPORT_MuxAlt5);           /* PORTA13 (pin 43) is configured as RMII0_RXD0 */
  PORT_SetPinMux(PORTA, PIN14_IDX, kPORT_MuxAlt5);           /* PORTA14 (pin 44) is configured as RMII0_CRS_DV */
  PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_MuxAlt5);           /* PORTA15 (pin 45) is configured as RMII0_TXEN */
  PORT_SetPinMux(PORTA, PIN16_IDX, kPORT_MuxAlt5);           /* PORTA16 (pin 46) is configured as RMII0_TXD0 */
  PORT_SetPinMux(PORTA, PIN17_IDX, kPORT_MuxAlt5);           /* PORTA17 (pin 47) is configured as RMII0_TXD1 */
  PORT_SetPinMux(PORTA, PIN5_IDX, kPORT_MuxAsGpio);          /* PORTA5 (pin 39) is configured as kPORT_MuxAsGpio */
  
////// /* Port A Clock Gate Control: Clock enabled */
//////    CLOCK_EnableClock(kCLOCK_PortA);
//////    /* Port B Clock Gate Control: Clock enabled */
//////    CLOCK_EnableClock(kCLOCK_PortB);
//////
//////    /* PORTA12 (pin 42) is configured as RMII0_RXD1 */
//////    PORT_SetPinMux(PORTA, 12U, kPORT_MuxAlt5);
//////
//////    PORTA->PCR[12] = ((PORTA->PCR[12] &
//////                       /* Mask bits to zero which are setting */
//////                       (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))
//////
//////                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
//////                       * corresponding PE field is set. */
//////                      | PORT_PCR_PS(kPORT_PullUp));
//////
//////    /* PORTA13 (pin 43) is configured as RMII0_RXD0 */
//////    PORT_SetPinMux(PORTA, 13U, kPORT_MuxAlt5);
//////
//////    PORTA->PCR[13] = ((PORTA->PCR[13] &
//////                       /* Mask bits to zero which are setting */
//////                       (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))
//////
//////                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
//////                       * corresponding PE field is set. */
//////                      | PORT_PCR_PS(kPORT_PullUp));
//////
//////    /* PORTA14 (pin 44) is configured as RMII0_CRS_DV */
//////    PORT_SetPinMux(PORTA, 14U, kPORT_MuxAlt5);
//////
//////    PORTA->PCR[14] = ((PORTA->PCR[14] &
//////                       /* Mask bits to zero which are setting */
//////                       (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))
//////
//////                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
//////                       * corresponding PE field is set. */
//////                      | PORT_PCR_PS(kPORT_PullUp));
//////
//////    /* PORTA15 (pin 45) is configured as RMII0_TXEN */
//////    PORT_SetPinMux(PORTA, 15U, kPORT_MuxAlt5);
//////
//////    /* PORTA16 (pin 46) is configured as RMII0_TXD0 */
//////    PORT_SetPinMux(PORTA, 16U, kPORT_MuxAlt5);
//////
//////    /* PORTA17 (pin 47) is configured as RMII0_TXD1 */
//////    PORT_SetPinMux(PORTA, 17U, kPORT_MuxAlt5);
//////
//////    /* PORTA5 (pin 39) is configured as RMII0_RXER */
//////    PORT_SetPinMux(PORTA, 5U, kPORT_MuxAlt4);
//////
//////    PORTA->PCR[5] = ((PORTA->PCR[5] &
//////                      /* Mask bits to zero which are setting */
//////                      (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))
//////
//////                     /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
//////                      * corresponding PE field is set. */
//////                     | PORT_PCR_PS(kPORT_PullUp));
//////
//////    /* PORTB0 (pin 53) is configured as RMII0_MDIO */
//////    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt8);
//////
//////    PORTB->PCR[0] = ((PORTB->PCR[0] &
//////                      /* Mask bits to zero which are setting */
//////                      (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))
//////
//////                     /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
//////                      * corresponding PE field is set. */
//////                     | PORT_PCR_PS(kPORT_PullUp));
//////
//////    /* PORTB1 (pin 54) is configured as RMII0_MDC */
//////    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt8);
//////
//////    PORTB->PCR[1] = ((PORTB->PCR[1] &
//////                      /* Mask bits to zero which are setting */
//////                      (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))
//////
//////                     /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
//////                      * corresponding PE field is set. */
//////                     | PORT_PCR_PS(kPORT_PullUp));
}

void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */
  XBARA_Init(XBARA);
 // PORT_SetPinMux(PORTA, PIN0_IDX, kPORT_MuxAsGpio);          /* PORTA0 (pin 34) is configured as PTA0 */
  PORT_SetPinMux(PORTA, PIN1_IDX, kPORT_MuxAlt2);            /* PORTA1 (pin 35) is configured as UART0_RX */

  PORT_SetPinMux(PORTA, PIN18_IDX, kPORT_MuxAsGpio); /* PORTA18 (pin 50) is configured as EXTAL0 */
  PORT_SetPinMux(PORTA, PIN19_IDX, kPORT_MuxAsGpio); /* PORTA19 (pin 51) is configured as XTAL0 */
  PORT_SetPinMux(PORTA, PIN2_IDX, kPORT_MuxAsGpio);          /* PORTA2 (pin 36) is configured as PTA2 */
 // PORT_SetPinMux(PORTA, PIN3_IDX, kPORT_MuxAsGpio);          /* PORTA3 (pin 37) is configured as PTA3 */
  PORT_SetPinMux(PORTA, PIN4_IDX, kPORT_MuxAlt6);            /* PORTA4 (pin 38) is configured as FLEXPWM0_B0 */
 PORT_SetPinMux(PORTA, PIN8_IDX, kPORT_MuxAlt5);           /* PORTA17 (pin 47) is configured as RMII0_TXD1 */
  PORT_SetPinMux(PORTB, PIN10_IDX, kPORT_MuxAsGpio);         /* PORTB10 (pin 58) is configured as PTB10 */
  PORT_SetPinMux(PORTB, PIN11_IDX, kPORT_MuxAsGpio);         /* PORTB11 (pin 59) is configured as PTB11 */
  PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAlt7);           /* PORTB16 (pin 62) is configured as XB_IN5 */
  PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAlt3);           /* PORTB17 (pin 63) is configured as UART0_TX */
  PORT_SetPinMux(PORTB, PIN18_IDX, kPORT_MuxAlt2);           /* PORTB18 (pin 64) is configured as CAN0_TX */
  PORT_SetPinMux(PORTB, PIN19_IDX, kPORT_MuxAlt2);           /* PORTB19 (pin 65) is configured as CAN0_RX */
  PORT_SetPinMux(PORTB, PIN2_IDX, kPORT_MuxAlt3);            /* PORTB2 (pin 55) is configured as UART0_RTS_b */
  PORT_SetPinMux(PORTB, PIN20_IDX, kPORT_MuxAlt5);           /* PORTB20 (pin 66) is configured as FLEXPWM0_X0 */
  PORT_SetPinMux(PORTB, PIN21_IDX, kPORT_MuxAsGpio);         /* PORTB21 (pin 67) is configured as PTB21 */
  PORT_SetPinMux(PORTB, PIN22_IDX, kPORT_MuxAsGpio);         /* PORTB22 (pin 68) is configured as PTB22 */
  PORT_SetPinMux(PORTB, PIN23_IDX, kPORT_MuxAsGpio);         /* PORTB23 (pin 69) is configured as PTB23 */
  PORT_SetPinMux(PORTB, PIN3_IDX, kPORT_MuxAlt3);            /* PORTB3 (pin 56) is configured as UART0_CTS_b */
  PORT_SetPinMux(PORTB, PIN9_IDX, kPORT_MuxAsGpio);          /* PORTB9 (pin 57) is configured as PTB9 */
  PORT_SetPinMux(PORTC, PIN0_IDX, kPORT_PinDisabledOrAnalog); /* PORTC0 (pin 70) is configured as HSADC0B_CH8 */
  PORT_SetPinMux(PORTC, PIN1_IDX, kPORT_MuxAlt6);            /* PORTC1 (pin 71) is configured as XB_IN11 */
  PORT_SetPinMux(PORTC, PIN10_IDX, kPORT_MuxAsGpio);         /* PORTC10 (pin 82) is configured as PTC10 */
  PORT_SetPinMux(PORTC, PIN11_IDX, kPORT_MuxAlt4);           /* PORTC11 (pin 83) is configured as FLEXPWM1_B3 */
  PORT_SetPinMux(PORTC, PIN12_IDX, kPORT_MuxAlt5);           /* PORTC12 (pin 84) is configured as FLEXPWM1_A1 */
  PORT_SetPinMux(PORTC, PIN13_IDX, kPORT_MuxAsGpio);           /* PORTC13 (pin 85) is configured as FLEXPWM1_B1 */
  PORT_SetPinMux(PORTC, PIN14_IDX, kPORT_MuxAlt5);           /* PORTC14 (pin 86) is configured as FLEXPWM1_A0 */
  PORT_SetPinMux(PORTC, PIN15_IDX, kPORT_MuxAsGpio);           /* PORTC15 (pin 87) is configured as FLEXPWM1_B0 */
  PORT_SetPinMux(PORTC, PIN16_IDX, kPORT_MuxAlt5);           /* PORTC16 (pin 90) is configured as FLEXPWM1_A2 */
  PORT_SetPinMux(PORTC, PIN17_IDX, kPORT_MuxAlt5);           /* PORTC17 (pin 91) is configured as FLEXPWM1_B2 */
  PORT_SetPinMux(PORTC, PIN18_IDX, kPORT_MuxAlt5);           /* PORTC18 (pin 92) is configured as FLEXPWM1_A3 */
  PORT_SetPinMux(PORTC, PIN2_IDX, kPORT_MuxAlt6);            /* PORTC2 (pin 72) is configured as XB_IN6 */
  PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt2);            /* PORTC3 (pin 73) is configured as SPI0_PCS1 */
  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAsGpio);          /* PORTC4 (pin 76) is configured as PTC4 */
  PORT_SetPinMux(PORTC, PIN5_IDX, kPORT_MuxAsGpio);          /* PORTC5 (pin 77) is configured as PTC5 */
     /* PORTC6 (pin 111) is configured as SPI0_SOUT */
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt2);

    /* PORTC7 (pin 112) is configured as SPI0_SIN */
    PORT_SetPinMux(PORTC, 7U, kPORT_MuxAlt2);
  PORT_SetPinMux(PORTC, PIN8_IDX, kPORT_MuxAsGpio);          /* PORTC8 (pin 80) is configured as PTC8 */
  PORT_SetPinMux(PORTC, PIN9_IDX, kPORT_MuxAsGpio);          /* PORTC9 (pin 81) is configured as PTC9 */
  PORT_SetPinMux(PORTC, PIN11_IDX, kPORT_MuxAsGpio);
  PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_MuxAlt6);            /* PORTD0 (pin 93) is configured as FLEXPWM0_A0 */
  PORT_SetPinMux(PORTD, PIN1_IDX, kPORT_MuxAsGpio);          /* PORTD1 (pin 94) is configured as PTD1 */
  PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAlt6);            /* PORTD2 (pin 95) is configured as FLEXPWM0_A1 */
  PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAlt6);            /* PORTD3 (pin 96) is configured as FLEXPWM0_B1 */
  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_MuxAsGpio);            /* PORTD4 (pin 97) is configured as FLEXPWM0_A2 */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAlt5);            /* PORTD5 (pin 98) is configured as FLEXPWM0_B2 */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_MuxAsGpio);          /* PORTD6 (pin 99) is configured as PTD6 */
 // PORT_SetPinMux(PORTD, PIN7_IDX, kPORT_MuxAsGpio);          /* PORTD7 (pin 100) is configured as PTD7 */
  PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_MuxAlt6);            /* PORTE0 (pin 1) is configured as I2C1_SDA */
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_MuxAlt6);            /* PORTE1 (pin 2) is configured as I2C1_SCL */
  PORT_SetPinMux(PORTE, PIN16_IDX, kPORT_MuxAlt2);           /* PORTE16 (pin 10) is configured as SPI0_PCS0 */
  PORT_SetPinMux(PORTE, PIN17_IDX, kPORT_MuxAlt2);           /* PORTE17 (pin 11) is configured as SPI0_SCK */
 // PORT_SetPinMux(PORTE, PIN18_IDX, kPORT_MuxAlt2);           /* PORTE18 (pin 12) is configured as SPI0_SOUT */
//  PORT_SetPinMux(PORTE, PIN19_IDX, kPORT_MuxAlt2);           /* PORTE19 (pin 13) is configured as SPI0_SIN */
  PORT_SetPinMux(PORTE, PIN2_IDX, kPORT_PinDisabledOrAnalog); /* PORTE2 (pin 3) is configured as HSADC0B_CH10 */
  PORT_SetPinMux(PORTE, PIN20_IDX, kPORT_MuxAlt3);           /* PORTE20 (pin 16) is configured as FTM1_CH0 */
  PORT_SetPinMux(PORTE, PIN21_IDX, kPORT_MuxAlt2);           /* PORTE21 (pin 17) is configured as XB_IN9 */
  PORT_SetPinMux(PORTE, PIN24_IDX, kPORT_MuxAsGpio);         /* PORTE24 (pin 31) is configured as PTE24 */
  PORT_SetPinMux(PORTE, PIN25_IDX, kPORT_MuxAsGpio);         /* PORTE25 (pin 32) is configured as PTE25 */
  PORT_SetPinMux(PORTE, PIN26_IDX, kPORT_MuxAlt2);           /* PORTE26 (pin 33) is configured as ENET_1588_CLKIN */
  PORT_SetPinMux(PORTE, PIN29_IDX, kPORT_MuxAsGpio);         /* PORTE29 (pin 26) is configured as PTE29 */
  PORT_SetPinMux(PORTE, PIN3_IDX, kPORT_PinDisabledOrAnalog); /* PORTE3 (pin 4) is configured as HSADC0B_CH11 */
  PORT_SetPinMux(PORTE, PIN30_IDX, kPORT_MuxAsGpio);         /* PORTE30 (pin 27) is configured as PTE30 */
  PORT_SetPinMux(PORTE, PIN4_IDX, kPORT_MuxAsGpio);          /* PORTE4 (pin 5) is configured as PTE4 */
  PORT_SetPinMux(PORTE, PIN5_IDX, kPORT_MuxAsGpio);          /* PORTE5 (pin 6) is configured as PTE5 */
  PORT_SetPinMux(PORTE, PIN6_IDX, kPORT_MuxAsGpio);          /* PORTE6 (pin 7) is configured as PTE6 */

  PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_MuxAlt8);            /* PORTB0 (pin 53) is configured as RMII0_MDIO */
  PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_MuxAlt8);            /* PORTB1 (pin 54) is configured as RMII0_MDC */


  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART 0 transmit data source select: UART0_TX pin */
      | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX)       /* UART 0 receive data source select: UART0_RX pin */
    );
  XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn11, kXBARA_OutputPwm0Fault0); /* XB_IN11 input pin output assigned to XBARA_IN11 input is connected to XBARA_OUT29 output assigned to PWM0 and PWM1 fault 0 */
  XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn6, kXBARA_OutputPwm0Fault1); /* XB_IN6 input pin output assigned to XBARA_IN6 input is connected to XBARA_OUT30 output assigned to PWM0 and PWM1 fault 1 */
  XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn5, kXBARA_OutputPwm0Fault2); /* XB_IN5 input pin output assigned to XBARA_IN5 input is connected to XBARA_OUT31 output assigned to PWM0 and PWM1 fault 2 */
  XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn9, kXBARA_OutputPwm0Fault3); /* XB_IN9 input pin output assigned to XBARA_IN9 input is connected to XBARA_OUT32 output assigned to PWM0 and PWM1 fault 3 */

////  PORTC->PCR[7] =
////        ((PORTC->PCR[7] & (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK |
////                             PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
////         | PORT_PCR_PS(PCR_PS_UP) /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
////                                     corresponding PE field is set. */
////         | PORT_PCR_PE(PCR_PE_ENABLED)   /* Pull Enable: Internal pullup or pulldown resistor is enabled on the
////                                            corresponding pin, if the pin is configured as a digital input. */
////         | PORT_PCR_ODE(PCR_ODE_ENABLED) /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if
////                                            the pin is configured as a digital output. */
////         );
////   PORTC->PCR[6] =
////        ((PORTC->PCR[6] & (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK |
////                             PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
////         | PORT_PCR_PS(PCR_PS_UP) /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
////                                     corresponding PE field is set. */
////         | PORT_PCR_PE(PCR_PE_ENABLED)   /* Pull Enable: Internal pullup or pulldown resistor is enabled on the
////                                            corresponding pin, if the pin is configured as a digital input. */
////         | PORT_PCR_ODE(PCR_ODE_ENABLED) /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if
////                                            the pin is configured as a digital output. */
////         );

   PORTE->PCR[0] =
        ((PORTE->PCR[0] & (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK |
                             PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
         | PORT_PCR_PS(PCR_PS_UP) /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                                     corresponding PE field is set. */
         | PORT_PCR_PE(PCR_PE_ENABLED)   /* Pull Enable: Internal pullup or pulldown resistor is enabled on the
                                            corresponding pin, if the pin is configured as a digital input. */
         | PORT_PCR_ODE(PCR_ODE_ENABLED) /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if
                                            the pin is configured as a digital output. */
         );
   PORTE->PCR[1] =
        ((PORTE->PCR[1] & (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK |
                             PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
         | PORT_PCR_PS(PCR_PS_UP) /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                                     corresponding PE field is set. */
         | PORT_PCR_PE(PCR_PE_ENABLED)   /* Pull Enable: Internal pullup or pulldown resistor is enabled on the
                                            corresponding pin, if the pin is configured as a digital input. */
         | PORT_PCR_ODE(PCR_ODE_ENABLED) /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if
                                            the pin is configured as a digital output. */
         );


   PORTA->PCR[12] =
        ((PORTA->PCR[12] & (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK |
                             PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
         | PORT_PCR_PS(PCR_PS_UP) /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                                     corresponding PE field is set. */
         | PORT_PCR_PE(PCR_PE_ENABLED)   /* Pull Enable: Internal pullup or pulldown resistor is enabled on the
                                            corresponding pin, if the pin is configured as a digital input. */
          /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if
                                            the pin is configured as a digital output. */
         );
   PORTA->PCR[13] =
        ((PORTA->PCR[13] & (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK |
                             PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
         | PORT_PCR_PS(PCR_PS_UP) /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                                     corresponding PE field is set. */
         | PORT_PCR_PE(PCR_PE_ENABLED)   /* Pull Enable: Internal pullup or pulldown resistor is enabled on the
                                            corresponding pin, if the pin is configured as a digital input. */
          /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if
                                            the pin is configured as a digital output. */
         );
   PORTA->PCR[14] =
        ((PORTA->PCR[14] & (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK |
                             PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
         | PORT_PCR_PS(PCR_PS_UP) /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                                     corresponding PE field is set. */
         | PORT_PCR_PE(PCR_PE_ENABLED)   /* Pull Enable: Internal pullup or pulldown resistor is enabled on the
                                            corresponding pin, if the pin is configured as a digital input. */
         /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if
                                            the pin is configured as a digital output. */
         );
    PORTA->PCR[5] =
        ((PORTA->PCR[5] & (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK |
                             PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
         | PORT_PCR_PS(PCR_PS_UP) /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                                     corresponding PE field is set. */
         | PORT_PCR_PE(PCR_PE_ENABLED)   /* Pull Enable: Internal pullup or pulldown resistor is enabled on the
                                            corresponding pin, if the pin is configured as a digital input. */
         /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if
                                            the pin is configured as a digital output. */
         );
    
        /* PORTC3 (pin 106) is configured as SPI0_PCS1 */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAsGpio);
        /* PORTC6 (pin 111) is configured as SPI0_SOUT */
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt2);

    /* PORTC7 (pin 112) is configured as SPI0_SIN */
    PORT_SetPinMux(PORTC, 7U, kPORT_MuxAlt2);
    
    /* PORTE16 (pin 18) is configured as SPI0_PCS0 */
    PORT_SetPinMux(PORTE, 16U, kPORT_MuxAsGpio);
    PORTC->PCR[3] = ((PORTC->PCR[3] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                      * corresponding PE field is set. */
                     | PORT_PCR_PS(kPORT_PullUp));
//
//    /* PORTE17 (pin 19) is configured as SPI0_SCK */
//    PORT_SetPinMux(PORTE, 17U, kPORT_MuxAlt2);
     PORTE->PCR[16] = ((PORTE->PCR[16] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | PORT_PCR_PS(kPORT_PullUp));
    
    GPIO_PinInit (GPIOE,PIN16_IDX,&config_out); 
    GPIO_PinInit (GPIOC,PIN3_IDX,&config_out);
    GPIO_SetPinsOutput(GPIOE,1<<PIN16_IDX); 
    GPIO_SetPinsOutput(GPIOC,1<<PIN3_IDX);  
    
   GPIO_PinInit (GPIOB,PIN18_IDX,&config_out);
   GPIO_PinInit (GPIOE,PIN30_IDX,&config_out);
//   GPIO_PinInit (GPIOB,PIN23_IDX,&config_out);
   GPIO_PinInit (GPIOC,PIN13_IDX,&config_out);
   
   
   PORT_SetPinMux(PORTD, PIN12_IDX, kPORT_MuxAsGpio); 
   PORT_SetPinMux(PORTD, PIN13_IDX, kPORT_MuxAsGpio); 
   GPIO_PinInit (GPIOD,PIN12_IDX,&config_out);
   GPIO_PinInit (GPIOD,PIN13_IDX,&config_out);
   GPIO_SetPinsOutput(GPIOD,1<<PIN12_IDX);
   GPIO_SetPinsOutput(GPIOD,1<<PIN13_IDX);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
