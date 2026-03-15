/*
 * w25qxx.c
 *
 *  Created on: Jan 17, 2026
 *      Author: Anton
 */

#include "w25qxx.h"

extern QSPI_HandleTypeDef hqspi;
extern QSPI_CommandTypeDef qcmd;

int w25Q_Read_Status_Bit(uint8_t instruction, int idx) {
	uint8_t values = 0;
	qcmd.Instruction = instruction;
	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	qcmd.AddressMode = QSPI_ADDRESS_NONE;
	qcmd.DataMode = QSPI_DATA_1_LINE;
	qcmd.NbData = 1;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	qcmd.DummyCycles = 0;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_QSPI_Receive(&hqspi, &values, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
	return (values >> idx) & 0x01;
}

void w25Q_Busy_Wait(void) {
	while (w25Q_Read_Status_Bit(0x05, 0)) {

	}
}

void w25Q_Reset (void) {
	while (w25Q_Read_Status_Bit(0x35, 0) || w25Q_Read_Status_Bit(0x35, 7)) {
		HAL_Delay(1000);
	}
	qcmd.Instruction = 0x66;
	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	qcmd.AddressMode = QSPI_ADDRESS_NONE;
	qcmd.DataMode = QSPI_DATA_NONE;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	qcmd.DummyCycles = 0;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
	HAL_Delay(1);

	qcmd.Instruction = 0x99;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
}

int w25Q_CheckID (void) {
	uint8_t id[3] = {0};
	qcmd.Instruction = 0x9F;
	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	qcmd.AddressMode = QSPI_ADDRESS_NONE;
	qcmd.DataMode = QSPI_DATA_1_LINE;
	qcmd.NbData = 3;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	qcmd.DummyCycles = 0;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_QSPI_Receive(&hqspi, id, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
	return ((id[0]<<16)|(id[1]<<8)|(id[2])) == W25Q_ID;
}

void w25Q_Fast_Read_Dual_IO_DMA(uint32_t addr, uint8_t *buf, uint32_t size) {
	qcmd.Instruction = 0xBB;
	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	qcmd.AddressMode = QSPI_ADDRESS_2_LINES;
	qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
	qcmd.Address = addr;
	qcmd.DataMode = QSPI_DATA_2_LINES;
	qcmd.NbData = size;
    qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_2_LINES;
    qcmd.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
    qcmd.AlternateBytes = 0xFF;

	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_QSPI_Receive_DMA(&hqspi, buf) != HAL_OK) {
		Error_Handler();
	}
}

void w25Q_Write_Enable(void) {
	qcmd.Instruction = 0x06;
	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	qcmd.AddressMode = QSPI_ADDRESS_NONE;
	qcmd.DataMode = QSPI_DATA_NONE;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	qcmd.DummyCycles = 0;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
	printf("Write enabled!\n");
}

void w25Q_Write_Disable(void) {
	qcmd.Instruction = 0x04;
	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	qcmd.AddressMode = QSPI_ADDRESS_NONE;
	qcmd.DataMode = QSPI_DATA_NONE;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	qcmd.DummyCycles = 0;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
}

void w25Q_Write_Page(uint32_t addr, const uint8_t *buf, uint16_t size) {
	w25Q_Write_Enable();
	qcmd.Instruction = 0x02;
	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	qcmd.AddressMode = QSPI_ADDRESS_1_LINE;
	qcmd.Address = addr;
	qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
	qcmd.DataMode = QSPI_DATA_1_LINE;
	qcmd.NbData = size;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	qcmd.DummyCycles = 0;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_QSPI_Transmit(&hqspi, (uint8_t *) buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
}

void w25Q_Write_Page_DMA(uint32_t addr, const uint8_t *buf, uint16_t size) {
	w25Q_Write_Enable();
	qcmd.Instruction = 0x02;
	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	qcmd.AddressMode = QSPI_ADDRESS_1_LINE;
	qcmd.Address = addr;
	qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
	qcmd.DataMode = QSPI_DATA_1_LINE;
	qcmd.NbData = size;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	qcmd.DummyCycles = 0;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    	printf("Write instruction failed...\n");
    	fflush(stdout);
		Error_Handler();
	}
	if (HAL_QSPI_Transmit(&hqspi, (uint8_t *) buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    	printf("Write failed...\n");
    	fflush(stdout);
		Error_Handler();
	}
	printf("Write done!\n");
	fflush(stdout);
}

void w25Q_Erase_Sector_4KB(uint32_t addr) {
	w25Q_Write_Enable();
	qcmd.Instruction = 0x20;
	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	qcmd.AddressMode = QSPI_ADDRESS_1_LINE;
	qcmd.Address = addr;
	qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
	qcmd.DataMode = QSPI_DATA_NONE;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	qcmd.DummyCycles = 0;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
	w25Q_Busy_Wait();
}

void w25Q_Power_Down(void) {
	qcmd.Instruction = 0xB9;
	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	qcmd.AddressMode = QSPI_ADDRESS_NONE;
	qcmd.DataMode = QSPI_DATA_NONE;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	qcmd.DummyCycles = 0;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
}

// Unused Functions
//
void w25Q_Read(uint32_t addr, uint8_t *buf, uint32_t size)
{
    qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    qcmd.Instruction = 0x03;
    qcmd.AddressMode = QSPI_ADDRESS_1_LINE;
    qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
    qcmd.Address = addr;
    qcmd.DataMode = QSPI_DATA_1_LINE;
    qcmd.NbData = size;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	qcmd.DummyCycles = 0;

    if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    	Error_Handler();
    }
    if (HAL_QSPI_Receive(&hqspi, buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    	Error_Handler();
    }
}

void w25Q_Fast_Read(uint32_t addr, uint8_t *buf, uint32_t size)
{
    qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    qcmd.Instruction = 0x0B;
    qcmd.AddressMode = QSPI_ADDRESS_1_LINE;
    qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
    qcmd.Address = addr;
    qcmd.DataMode = QSPI_DATA_1_LINE;
    qcmd.NbData = size;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    qcmd.DummyCycles = 8;

    if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    	printf("Read instruction failed...\n");
    	fflush(stdout);
		Error_Handler();
    }
    if (HAL_QSPI_Receive(&hqspi, buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
    	printf("Read failed...\n");
    	fflush(stdout);
		Error_Handler();
    }
}
//
void w25Q_Fast_Read_Dual_Output(uint32_t addr, uint8_t *buf, uint32_t size) {
	qcmd.Instruction = 0x3B;
	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	qcmd.AddressMode = QSPI_ADDRESS_1_LINE;
	qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
	qcmd.Address = addr;
	qcmd.DataMode = QSPI_DATA_2_LINES;
	qcmd.NbData = size;
    qcmd.DummyCycles = 8;
    qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;

	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_QSPI_Receive(&hqspi, buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
}
//void w25Q_Erase_Block_32KB(uint32_t addr) {
//	w25Q_Write_Enable();
//	qcmd.Instruction = 0x52;
//	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
//	qcmd.AddressMode = QSPI_ADDRESS_1_LINE;
//	qcmd.Address = addr;
//	qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
//	qcmd.DataMode = QSPI_DATA_NONE;
//	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
//	qcmd.DummyCycles = 0;
//	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
//		Error_Handler();
//	}
//	w25Q_Busy_Wait();
//}
//
//void w25Q_Erase_Block_64KB(uint32_t addr) {
//	w25Q_Write_Enable();
//	qcmd.Instruction = 0xD8;
//	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
//	qcmd.AddressMode = QSPI_ADDRESS_1_LINE;
//	qcmd.Address = addr;
//	qcmd.AddressSize = QSPI_ADDRESS_24_BITS;
//	qcmd.DataMode = QSPI_DATA_NONE;
//	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
//	qcmd.DummyCycles = 0;
//	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
//		Error_Handler();
//	}
//	w25Q_Busy_Wait();
//}
//
void w25Q_Erase_Chip(void) {
	w25Q_Write_Enable();
	qcmd.Instruction = 0x60;
	qcmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	qcmd.AddressMode = QSPI_ADDRESS_NONE;
	qcmd.DataMode = QSPI_DATA_NONE;
	qcmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	qcmd.DummyCycles = 0;
	if (HAL_QSPI_Command(&hqspi, &qcmd, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		Error_Handler();
	}
	w25Q_Busy_Wait();
}



