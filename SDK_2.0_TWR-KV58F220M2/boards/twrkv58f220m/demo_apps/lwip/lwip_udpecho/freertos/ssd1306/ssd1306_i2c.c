/**
 * original author:  Tilen Majerle<tilen@majerle.eu>
 * modification for STM32f10x: Alexander Lutsai<s.lyra@ya.ru>

   ----------------------------------------------------------------------
   	Copyright (C) Alexander Lutsai, 2016
    Copyright (C) Tilen Majerle, 2015

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
 */
#include "ssd1306_i2c.h"
#include <stdint.h>
#include "fsl_i2c.h"
#include "fsl_i2c_edma.h"
#include "fsl_dmamux.h"
#include "MKV58F24.h"
/*  Standard C Included Files */
#include <string.h>
/*  SDK Included Files */
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
/* Private variables */
//static uint32_t ssd1306_I2C_Timeout;

/* Private defines */
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0





/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_SSD1306_txBuff[SSD1306_I2C_DATA_LENGTH];
uint8_t g_SSD1306_rxBuff[SSD1306_I2C_DATA_LENGTH];
/* I2C source clock */

void ssd1306_I2C_Init() {
  
  i2c_master_config_t masterConfig;
  uint32_t sourceClock;

    
    

    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = SSD1306_I2C_BAUDRATE;

    sourceClock = SSD1306_I2C_MASTER_CLK_FREQ;

    I2C_MasterInit(SSD1306_I2C_MASTER_BASEADDR, &masterConfig, sourceClock);

}





void ssd1306_I2C_WriteMulti(I2C_Type* I2Cx, uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {

        i2c_master_transfer_t masterXfer;
   memset(&masterXfer, 0, sizeof(masterXfer));

    /* subAddress = 0x01, data = g_master_txBuff - write to slave.
      start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
    uint8_t deviceAddress     = reg;
    masterXfer.slaveAddress   = address;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)deviceAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = data;
    masterXfer.dataSize       = 1+count;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2Cx, &masterXfer);
        
        
}




/* Private functions */
int16_t ssd1306_I2C_Start(I2C_Type* I2Cx, uint8_t address, uint8_t direction, uint8_t ack) {

	return 0;
}

void ssd1306_I2C_WriteData(I2C_Type* I2Cx, uint8_t data) {
  
   
    i2c_master_transfer_t masterXfer;
    memset(&masterXfer, 0, sizeof(masterXfer));

    /* subAddress = 0x01, data = g_master_txBuff - write to slave.
      start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
    uint8_t deviceAddress     = 0x81U;
    masterXfer.slaveAddress   = SSD1306_I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)deviceAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = &data;
    masterXfer.dataSize       = 1;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(SSD1306_I2C_MASTER_BASEADDR, &masterXfer);
    

}

void ssd1306_I2C_Write(I2C_Type* I2Cx, uint8_t address, uint8_t reg, uint8_t data) {

  
  
  
   i2c_master_transfer_t masterXfer;
   memset(&masterXfer, 0, sizeof(masterXfer));

    /* subAddress = 0x01, data = g_master_txBuff - write to slave.
      start + slaveaddress(w) + subAddress + length of data buffer + data buffer + stop*/
   // uint8_t deviceAddress     = 0x81U;
    masterXfer.slaveAddress   = address;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)reg;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = &data;
    masterXfer.dataSize       = 1;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2Cx, &masterXfer);
    
}




uint8_t ssd1306_I2C_IsDeviceConnected(I2C_Type* I2Cx, uint8_t address) {
	uint8_t connected = 0;
	/* Try to start, function will return 0 in case device will send ACK */
        I2C_MasterStart(I2Cx, address,kI2C_Write);
        uint32_t p = 25000;
	while(p>0)
		p--;
        I2C_MasterStop(I2Cx);
	
        connected = I2Cx->S;
        if ((connected&0x01)==1)
          {
            connected=0;
          }
        else
          {
            connected=1;
          }
	/* Return status */
	return connected;
}
