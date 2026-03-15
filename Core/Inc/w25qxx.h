/*
 * w25qxx.h
 *
 *  Created on: Jan 17, 2026
 *      Author: Anton
 */

#ifndef INC_W25QXX_H_
#define INC_W25QXX_H_

#include <stdint.h>
#include <stdio.h>
#include "main.h"

#define W25Q_PAGE_SIZE 256
#define W25Q_ID 15679512

int w25Q_Read_Status_Bit(uint8_t instruction, int idx);
void w25Q_Busy_Wait(void);
void w25Q_Reset (void);
int w25Q_CheckID (void);

// Dual SPI Read
void w25Q_Fast_Read_Dual_Output(uint32_t addr, uint8_t *buf, uint32_t size);
void w25Q_Fast_Read_Dual_IO_DMA(uint32_t addr, uint8_t *buf, uint32_t size);

// Write
void w25Q_Write_Enable();
void w25Q_Write_Disable();
void w25Q_Write_Page(uint32_t addr, const uint8_t *buf, uint16_t size);

// Erase
void w25Q_Erase_Sector_4KB(uint32_t addr);
//void w25Q_Erase_Block_32KB(uint32_t addr);
//void w25Q_Erase_Block_64KB(uint32_t addr);
void w25Q_Erase_Chip(void);

// Just Power Down :)
void w25Q_Power_Down(void);

// Standard SPI
void w25Q_Read(uint32_t addr, uint8_t *buf, uint32_t size);
void w25Q_Fast_Read(uint32_t addr, uint8_t *buf, uint32_t size);


#endif /* INC_W25QXX_H_ */
