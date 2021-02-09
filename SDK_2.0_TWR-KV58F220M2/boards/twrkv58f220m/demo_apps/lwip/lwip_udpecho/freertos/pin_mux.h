/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_
      
#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */
#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
#define PIN20_IDX                       20u   /*!< Pin number for pin 20 in a port */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port */
#define PIN23_IDX                       23u   /*!< Pin number for pin 23 in a port */
#define PIN24_IDX                       24u   /*!< Pin number for pin 24 in a port */
#define PIN25_IDX                       25u   /*!< Pin number for pin 25 in a port */
#define PIN26_IDX                       26u   /*!< Pin number for pin 26 in a port */
#define PIN29_IDX                       29u   /*!< Pin number for pin 29 in a port */
#define PIN30_IDX                       30u   /*!< Pin number for pin 30 in a port */
#define SOPT5_UART0RXSRC_UART_RX      0x00u   /*!< UART 0 receive data source select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART 0 transmit data source select: UART0_TX pin */

#define PCR_ODE_ENABLED                                                                                                \
    0x01u /*!< Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a \
             digital output. */
#define PCR_PE_ENABLED                                                                                              \
    0x01u /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is \
             configured as a digital input. */
#define PCR_PS_UP                                                                                                 \
    0x01u /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE \
             field is set. */


/*!
 * @addtogroup pin_mux
 * @{
 */
void Ethernet_pin_init (void);
/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);
/*! @name PORTE0 (number 1), BUS_kontaktor
  @{ */
#define BOARD_INITPINS_BUS_kontaktor_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_INITPINS_BUS_kontaktor_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_INITPINS_BUS_kontaktor_PIN 0U     /*!<@brief PORTE pin index: 0 */
                                                /* @} */ 
   /*! @name PORTB23 (number 69), OUT_CMP3
  @{ */
#define BOARD_INITPINS_OUT_CMP3_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_INITPINS_OUT_CMP3_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITPINS_OUT_CMP3_PIN 23U    /*!<@brief PORTB pin index: 23 */
                                           /* @} */
/*!
 * @brief RMII clock source select: EXTAL clock */
#define SOPT2_RMIISRC_EXTAL 0x00u
/*!
 * @brief IEEE 1588 timestamp clock source select: External bypass clock (ENET_1588_CLKIN) */
#define SOPT2_TIMESRC_ENET 0x03u
/*!
 * @brief FTM0 channel 1 output source: FTM0_CH1 pin is output of FTM0 channel 1 output */
#define SOPT8_FTM0OCH1SRC_FTM 0x00u
/*!
 * @brief FTM0 channel 2 output source: FTM0_CH2 pin is output of FTM0 channel 2 output */
#define SOPT8_FTM0OCH2SRC_FTM 0x00u

/*! @name PORTE1 (number 2), DVR_SO
  @{ */
#define BOARD_INITPINS_DRV_SO_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_INITPINS_DRV_SO_PIN 1U     /*!<@brief PORTE pin index: 1 */
                                         /* @} */

/*! @name PORTE29 (number 26), STEP_OUT
  @{ */
#define BOARD_INITPINS_M_STEP_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_INITPINS_M_STEP_PIN 29U    /*!<@brief PORTE pin index: 29 */
                                         /* @} */

/*! @name PORTE30 (number 27), M_DIR
  @{ */
#define BOARD_INITPINS_M_DIR_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_INITPINS_M_DIR_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_INITPINS_M_DIR_PIN 30U    /*!<@brief PORTE pin index: 30 */
                                        /* @} */

/*! @name PORTE24 (number 31), M_RES
  @{ */
#define BOARD_INITPINS_M_RES_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_INITPINS_M_RES_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_INITPINS_M_RES_PIN 24U    /*!<@brief PORTE pin index: 24 */
                                        /* @} */

/*! @name PORTE25 (number 32), M_SLEEP
  @{ */
#define BOARD_INITPINS_M_SLEEP_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_INITPINS_M_SLEEP_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_INITPINS_M_SLEEP_PIN 25U    /*!<@brief PORTE pin index: 25 */
                                          /* @} */

/*! @name PORTA1 (number 35), M_e
  @{ */
#define BOARD_INITPINS_M_EN_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_INITPINS_M_EN_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_INITPINS_M_EN_PIN 1U     /*!<@brief PORTA pin index: 1 */
                                       /* @} */
/*! @name PORTC5 (number 77), drv_clk
  @{ */
#define BOARD_INITPINS_CONF_CLK_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITPINS_CONF_CLK_PIN 5U     /*!<@brief PORTC pin index: 5 */
                                           /* @} */

/*! @name PORTC6 (number 78), CONF_SO
  @{ */
#define BOARD_INITPINS_CONF_SO_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITPINS_CONF_SO_PIN 6U     /*!<@brief PORTC pin index: 6 */
                                          /* @} */

/*! @name PORTC14 (number 86), HI_SWICH
  @{ */
#define BOARD_INITPINS_HI_SWICH_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITPINS_HI_SWICH_GPIO GPIOC
#define BOARD_INITPINS_HI_SWICH_PIN 14U    /*!<@brief PORTC pin index: 14 */
                                           /* @} */

/*! @name PORTC15 (number 87), LO_SWICH
  @{ */
#define BOARD_INITPINS_LO_SWICH_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITPINS_LO_SWICH_GPIO GPIOC
#define BOARD_INITPINS_LO_SWICH_PIN 15U    /*!<@brief PORTC pin index: 15 */
                                           /* @} */

/*! @name PORTD1 (number 94), OVER_T
  @{ */
#define BOARD_INITPINS_OVER_T_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_INITPINS_OVER_T_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_INITPINS_OVER_T_PIN 1U     /*!<@brief PORTD pin index: 1 */
                                         /* @} */

/*! @name PORTD6 (number 99), DRV_OFF
  @{ */
#define BOARD_INITPINS_DRV_OFF_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_INITPINS_DRV_OFF_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_INITPINS_DRV_OFF_PIN 6U     /*!<@brief PORTD pin index: 6 */
                                          /* @} */

/*! @name PORTD4 (number 97), LDAC
  @{ */
#define BOARD_INITPINS_LDAC_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_INITPINS_LDAC_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_INITPINS_LDAC_PIN 4U     /*!<@brief PORTD pin index: 4 */

/*! @name PORTC2 (number 72), CLK_OUT
  @{ */
#define BOARD_INITPINS_CLK_OUT_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITPINS_CLK_OUT_PIN 2U     /*!<@brief PORTC pin index: 2 */
                                          /* @} */
                                       /* @} */
    /*! @name PORTB10 (number 58), DRV_TCLK
  @{ */
#define BOARD_INITPINS_DRV_XLAT_GPIO GPIOC /*!<@brief GPIO device name: GPIOB */
#define BOARD_INITPINS_DRV_XLAT_PORT PORTC /*!<@brief PORT device name: PORTB */
#define BOARD_INITPINS_DRV_XLAT_PIN 12U    /*!<@brief PORTB pin index: 10 */
                                           /* @} */


/*! @name PORTC13 (number 85), LED
  @{ */
#define BOARD_INITPINS_LED_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_INITPINS_LED_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITPINS_LED_PIN 13U    /*!<@brief PORTC pin index: 13 */
/*! @name PORTB9 (number 57), RES_PHY
  @{ */
#define BOARD_INITPINS_RES_PHY_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_INITPINS_RES_PHY_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITPINS_RES_PHY_PIN 9U     /*!<@brief PORTB pin index: 9 */
                                          /* @} */                                      /* @} */
/*! @name PORTE2 (number 3), DRV_CLK
  @{ */
#define BOARD_INITPINS_DRV_CLK_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_INITPINS_DRV_CLK_PIN 2U     /*!<@brief PORTE pin index: 2 */
                                          /* @} */
/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/