

/* ****************** ──────────────────────────────────────────────────────────────── ****************** */
/*                         																	              *
 *              ╔═════════════════════════════════════════════════════════════════════════╗               *
 *               ║                            FILE: Flash.h                              ║                *
 *               ║                             AUTHOR: Lovejoy Mhishi                    ║                *
 *               ║                            DATE: July 25, 2025                        ║                *
 *              ╠═════════════════════════════════════════════════════════════════════════╣               *
 *               ║      STM32 Flash Memory Driver - Low-Level Read/Write API             ║                *
 *              ╚═════════════════════════════════════════════════════════════════════════╝               *
 *						                                                                                  *
 *This driver provides low-level functions to write to and read from the              			          *
 *         			                                                                         			  *
 * Key Features:                                                                						  *
 *   - Contains Function prototypes.                                             						  */
/* ****************** ──────────────────────────────────────────────────────────────── ****************** */


#ifndef CUSTOMDRIVERS_INC_FLASH_H_
#define CUSTOMDRIVERS_INC_FLASH_H_


#include "stdio.h"
#include <stdbool.h>
#include "main.h"
#include "stm32g0xx_hal.h"

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*                                            FLASH ADDRESS DEFINES                                         */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

#define PAGE 15

#define FLASH_PAGE_START_ADDRESS 0x08007800

/*
 * Writing 64-bit words requires 8 bytes
 */
#define FLASH_ADDR_1  0x08007808
#define FLASH_ADDR_2  0x08007810
#define FLASH_ADDR_3  0x08007818
#define FLASH_ADDR_4  0x08007820
#define FLASH_ADDR_5  0x08007828
#define FLASH_ADDR_6  0x08007830
#define FLASH_ADDR_7  0x08007838
#define FLASH_ADDR_8  0x08007840
#define FLASH_ADDR_9  0x08007848
#define FLASH_ADDR_10 0x08007850
#define FLASH_ADDR_11 0x08007858
#define FLASH_ADDR_12 0x08007860
#define FLASH_ADDR_13 0x08007868
#define FLASH_ADDR_14 0x08007870
#define FLASH_ADDR_15 0x08007878
#define FLASH_ADDR_16 0x08007880
#define FLASH_ADDR_17 0x08007888
#define FLASH_ADDR_18 0x08007890
#define FLASH_ADDR_19 0x08007898
#define FLASH_ADDR_20 0x080078A0

/*
               ┆
               ┆
               ┆
               ┆
       #define ┆ FLASH_ADDR_xx 0x0800Fxxx
               ┆
               ┆
               ┆
               ┆
               ┆
               ↓
 */

#define FLASH_PAGE_END_ADDRESS 0x08007FFF

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*                                            FLASH FLAGS                                                   */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
extern bool isFlashBusy;

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*                                           DATA-PROCESSING FUNCTIONS                                      */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
void write_flash_memory(uint32_t memory_address, uint64_t *data);
uint64_t read_flash_memory(uint32_t memory_address);


/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*                                           LOW-LEVEL FUNCTIONS                                            */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
void flash_memory_page_erase(void);

#endif /* CUSTOMDRIVERS_INC_FLASH_H_ */
