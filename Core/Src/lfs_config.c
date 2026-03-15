/*
 * lfs_config.c
 *
 *  Created on: 27.01.2026
 *      Author: Anton
 */

#include "lfs_config.h"

__attribute__((aligned(4))) uint8_t read_buffer[LFS_CACHE_SIZE];
__attribute__((aligned(4))) uint8_t prog_buffer[LFS_CACHE_SIZE];
__attribute__((aligned(4))) uint8_t lookahead_buffer[LFS_LOOKAHEAD_SIZE];

// Functions for LittleFS Mounting
int lfs_flash_read (const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
	printf("the flash is being read... block: %lu off: %lu size:%lu\n", (uint32_t) block, (uint32_t) off, (uint32_t) size);
	uint32_t address = block * c->block_size + off;
	w25Q_Read(address, buffer, size);
//	w25Q_Fast_Read(address, buffer, size);

	uint8_t *buf = (uint8_t*)buffer;
	printf("Data read: %02X %02X %02X %02X %02X %02X %02X %02X...\n",
	           buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
	return 0;
}
int lfs_flash_prog (const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
	printf("the flash is being written...block: %lu off: %lu size:%lu\n", (uint32_t) block, (uint32_t) off, (uint32_t) size);
	uint32_t address = block * c->block_size + off;
	const uint8_t *data = buffer;
	w25Q_Erase_Sector_4KB(address);
	printf("Write data: %02X %02X %02X %02X %02X %02X %02X %02X...\n",
			data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
	while (size > 0) {
		uint32_t spaceOnCurrentPage = W25Q_PAGE_SIZE - address % W25Q_PAGE_SIZE;
		if (size < spaceOnCurrentPage) {
			spaceOnCurrentPage = size;
		}
		w25Q_Write_Page(address, data, spaceOnCurrentPage);
		address += spaceOnCurrentPage;
		data += spaceOnCurrentPage;
		size -= spaceOnCurrentPage;
	}
	return 0;
}
//int lfs_flash_prog (const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
//	printf("the flash is being written...block: %lu off: %lu size:%lu\n", (uint32_t) block, (uint32_t) off, (uint32_t) size);
//	uint32_t address = block * c->block_size + off;
//	const uint8_t *data = buffer;
//
//	w25Q_Erase_Sector_4KB(address);
//	printf("Write data: %02X %02X %02X %02X %02X %02X %02X %02X...\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
//
//	uint32_t spaceOnCurrentPage = W25Q_PAGE_SIZE - address % W25Q_PAGE_SIZE;
//	if (size < spaceOnCurrentPage) {
//		spaceOnCurrentPage = size;
//	}
//
//	write_buf.address = address + spaceOnCurrentPage;
//	write_buf.data = data + spaceOnCurrentPage;
//	write_buf.size = size - spaceOnCurrentPage;
//
//	w25Q_Write_Page_DMA(address, data, spaceOnCurrentPage);
//	return 0;
//}

//void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef *hqspi)
//{
//	if (write_buf.size > 0) {
//		write_buf.spaceOnCurrentPage = W25Q_PAGE_SIZE - write_buf.address % W25Q_PAGE_SIZE;
//		if (size < spaceOnCurrentPage) {
//			spaceOnCurrentPage = size;
//		}
//		w25Q_Write_Page_DMA(address, data, spaceOnCurrentPage);
//		address += spaceOnCurrentPage;
//		data += spaceOnCurrentPage;
//		size -= spaceOnCurrentPage;
//	}
//}
int lfs_flash_erase (const struct lfs_config *c, lfs_block_t block)
{
	printf("the flash is being erased... block: %lu\n", (uint32_t) block);
	uint32_t address = block * c->block_size;
	w25Q_Erase_Sector_4KB(address);

	uint8_t verify_buf[16];
	    w25Q_Fast_Read(address, verify_buf, 16);
	    printf("  After erase: %02X %02X %02X %02X...\n",
	           verify_buf[0], verify_buf[1], verify_buf[2], verify_buf[3]);

	    // Check if erased (should be all 0xFF)
	    int erased = 1;
	    for(int i = 0; i < 16; i++) {
	        if(verify_buf[i] != 0xFF) {
	            erased = 0;
	            break;
	        }
	    }
	    printf("  Erase verification: %s\n", erased ? "PASS" : "FAIL");
	return 0;
}
int lfs_flash_sync (const struct lfs_config *c)
{
	printf("the flash is being synced...\n");
	w25Q_Busy_Wait();
	return 0;
}

const struct lfs_config cfg =
{
	.read  = lfs_flash_read,
	.prog  = lfs_flash_prog,
	.erase = lfs_flash_erase,
	.sync  = lfs_flash_sync,

	.read_size = LFS_READ_SIZE,
	.prog_size = LFS_PROG_SIZE,
	.block_size = LFS_BLOCK_SIZE,
	.block_count = LFS_BLOCK_COUNT,
	.cache_size = LFS_CACHE_SIZE,
	.lookahead_size = LFS_LOOKAHEAD_SIZE,
    .block_cycles = LFS_BLOCK_CYCLES,

    .read_buffer = read_buffer,
    .prog_buffer = prog_buffer,
    .lookahead_buffer = lookahead_buffer,
};

lfs_t lfs;

int Flash_Init(void) {
	if (!w25Q_CheckID()) printf("Initialization failed: Chip ID mismatch\n");
    int err = lfs_mount(&lfs, &cfg);
    if (err < 0) {
    	printf("Initial mounting failed... Error: %d\n", err);
    	fflush(stdout);
        err = lfs_format(&lfs, &cfg);
        if (err < 0) {
        	printf("Reformatting also failed...\n");
        	fflush(stdout);
        	return err;
        }
        err = lfs_mount(&lfs, &cfg);
        if (err < 0) {
        	printf("Second mounting attempt failed...\n");
        	fflush(stdout);
        	return err;
        }
    }
    return 0;
}

int Flash_Write(const char* filename, const void* data, uint32_t size) {
	 lfs_file_t file;

	 int err = lfs_file_open(&lfs, &file, filename, LFS_O_WRONLY | LFS_O_CREAT);
	 if (err < 0) {
		 printf("File can't be opened / created... Error: %d\n", err);
		 return -1;
	 }

	 lfs_ssize_t written = lfs_file_write(&lfs, &file, data, size);
	 if (written < 0) {
		 lfs_file_close(&lfs, &file);
		 return -2;
	 }

	 if (lfs_file_close(&lfs, &file) < 0) {
		 return -3;
	 }

	 return 0;
}

int Flash_Read(const char* filename, void *buf, uint32_t size) {
	lfs_file_t file;

	if (lfs_file_open(&lfs, &file, filename, LFS_O_RDONLY) < 0) {
		return -1;
	}

	int sizeOfReadBytes = lfs_file_read(&lfs, &file, buf, size);
	if (sizeOfReadBytes < 0) {
		lfs_file_close(&lfs, &file);
		return -2;
	}

	if (lfs_file_close(&lfs, &file) < 0) {
		return -3;
	}

	return sizeOfReadBytes;
}

int Flash_Delete(const char* filename) {
    if (lfs_remove(&lfs, filename) < 0) {
        return -1;
    }
    return 0;
}

int Flash_CheckFileSize(const char* filename, uint32_t* size) {
	lfs_file_t file;

	if (lfs_file_open(&lfs, &file, filename, LFS_O_RDONLY) < 0) {
		return -1;
	}
	lfs_soff_t fsize = lfs_file_size(&lfs, &file);
    if (fsize < 0) {
        lfs_file_close(&lfs, &file);
        return -2;
    }
    *size = (uint32_t)fsize;

    if (lfs_file_close(&lfs, &file) < 0) {
    	return -3;
    }
    return 0;
}





