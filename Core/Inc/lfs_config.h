/*
 * lfs_config.h
 *
 *  Created on: 27.01.2026
 *      Author: Anton
 */

#ifndef INC_LFS_CONFIG_H_
#define INC_LFS_CONFIG_H_

#include "lfs.h"
#include "lfs_util.h"
#include "w25qxx.h"
#include "stdio.h"
#include "stdlib.h"

#define LFS_PROG_SIZE      256
#define LFS_READ_SIZE      256
#define LFS_BLOCK_SIZE     4096
#define LFS_BLOCK_COUNT    4096
#define LFS_CACHE_SIZE     256
#define LFS_LOOKAHEAD_SIZE 256
#define LFS_BLOCK_CYCLES   500

int lfs_flash_read (const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int lfs_flash_prog (const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
int lfs_flash_erase (const struct lfs_config *c, lfs_block_t block);
int lfs_flash_sync (const struct lfs_config *c);

extern const struct lfs_config cfg;

int Flash_Init(void);
int Flash_Write(const char* filename, const void* data, uint32_t size);
int Flash_Read(const char* filename, void *buf, uint32_t size);
int Flash_Delete(const char* filename);
int Flash_CheckFileSize(const char* filename, uint32_t* size);

#endif /* INC_LFS_CONFIG_H_ */
