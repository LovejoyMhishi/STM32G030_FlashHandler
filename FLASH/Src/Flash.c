
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                            			                					    *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: Flash.c                              ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: July 25, 2025                        ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║       STM32 Flash Memory Driver - Low-Level Read/Write API           ║                   *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This driver provides low-level functions to write to and read from the internal Flash memory             *
 * of STM32 microcontrollers.                                                                               *
 *                                                                                                          *
 * Key Features:                                  		                                            *
 *   - Writes a sequence of bytes to a specified Flash address.					            *
 *   - Reads a sequence of bytes from Flash into a buffer.	                                            *
 *   - Suitable for storing calibration data, device settings, or logs.		             		    *
 * 								                                	    *
 * Intended Use:                                                                   		            *
 *   - To store data when the MCU looses power,  counters, or configuration values).       	            *
 *										       		            *
 * Dependencies: 									 		    *
 *   - STM32 HAL library									            *
 *   - Device-specific memory map (defined in main.h)							    *
 *					                                                                    *
 * Notes:										                    *
 *   - Flash write operations require memory to be erased beforehand.	                                    *
 *   - Ensure address alignment and Flash sector access rules are followed.	                            *
 *     Reduce the total Flash size by 2KB to prevent accidental overwriting of application code.	    *
 *     For example, reserve 2 pages (2KB) at the end of Flash as shown below:                               *
 *   - Update the Flash size in the linker script to reserve space for user data storage.          	    *
 *													    *
 *     MEMORY												    *
 *     {												    *
 *       RAM    (xrw) : ORIGIN = 0x20000000, LENGTH = 8K						    *
 *       FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 32K - 2K   ***── Reserve 2KB for Flash storage ──***  *
 *     }												    *
 *         												    *
 *   - Always verify that the Flash operations do not conflict with the main program region.		    *
 *   - Use absolute memory addresses carefully when reading/writing to internal Flash.			    *
 *							   					            */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */



/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*                                            INCLUDES                                                      */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#include "Flash.h"
#include "stdio.h"
#include "main.h"


/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*                                            FLASH FLAGS                                                   */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
bool isFlashBusy = false;

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*                                           DATA-PROCESSING FUNCTIONS                                      */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
void write_flash_memory(uint32_t memory_address, uint64_t *data) {

	/*
	 * Disable all interrupts
	 */
	__disable_irq();

	/*
	 * Unlocking the flash memory
	 */
	HAL_FLASH_Unlock();

	/*
	 *  Flash memory page erase
	 */
	flash_memory_page_erase();

	/*
	 * Check if flash_memory_page_erase has failed
	 */
	if(isFlashBusy) {

		HAL_FLASH_Lock();

		__enable_irq();

		isFlashBusy = false;

		return;
	}

	/*
	 * Check that no flash memory operation is ongoing on the targeted bank, by checking the BSY1 bit of the FLASH status register (FLASH_SR)
	 */
	if(FLASH->SR & FLASH_SR_BSY1)  {

		/*
		 * If flash is still busy due to previous erase, wait for 1s
		 */
		TimeOut(1000);

		if(FLASH->SR & FLASH_SR_BSY1) {

			HAL_FLASH_Lock();

			__enable_irq();

			return;
		}

	}

	/*
	 * Check and clear all error programming flags due to a previous programming
	 */
	FLASH->SR = FLASH_SR_CLEAR;

	/*
	 *  Write to Flash
	 */

	uint64_t Data = data;

	/*
	 * Set the PG bit of the FLASH control register (FLASH_CR).
	 */
	FLASH->CR |= FLASH_CR_PG;

	/*
	 * Perform the data write operation at the desired memory address
	 */
	*(uint32_t *)memory_address = (uint32_t)Data;

	__ISB();


	*(uint32_t *)(memory_address + 4) = (uint32_t)(Data >> 32U);

	/*
	 * Wait until the BSY1 bit of the FLASH status register (FLASH_SR) is cleared.
	 */
	while (FLASH->SR & FLASH_SR_BSY1) {
		/*
		 * ⏳...
		 */

	}

	/*
	 *  Check that EOP flag of the FLASH status register (FLASH_SR) is set (programming operation succeeded), and clear it by software.
	 */
	if(FLASH->SR & FLASH_SR_EOP) {

		FLASH->SR = FLASH_SR_EOP;
	}

	/*
	 *  Clear the PG bit of the FLASH control register (FLASH_CR) if there no more programming request anymore.
	 */
	FLASH->CR &= ~FLASH_CR_PG;

	/*
	 * Lock flash memory after a write
	 */
	HAL_FLASH_Lock();

	/*
	 * Re-Enable all Interrupts once again
	 */
	__enable_irq();
}


/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*                                           LOW-LEVEL FUNCTIONS                                            */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
void flash_memory_page_erase(void) {
	/*
	 * Check that no flash memory operation is ongoing on the targeted bank, by checking the BSY1 bit of the FLASH status register (FLASH_SR)
	 */
	if(FLASH->SR & FLASH_SR_BSY1)  {

		/*
		 * Use a blocking wait if flash is Busy
		 */
		TimeOut(1000);

		/*
		 * Check If flash is still busy after wait
		 */
		if(FLASH->SR & FLASH_SR_BSY1) {

			isFlashBusy = true;

			return;
		}
	}

	/*
	 *  Check and clear all error programming flags due to a previous programming
	 */
	FLASH->SR = FLASH_SR_CLEAR;

	/*
	 * Set the PER bit and select the page to erase (PNB) in the FLASH control register (FLASH_CR).
	 */
	FLASH->CR |= FLASH_CR_PER;
	FLASH->CR &= ~FLASH_CR_PNB_Msk;
	FLASH->CR |= (PAGE << FLASH_CR_PNB_Pos);
	FLASH->CR |= FLASH_CR_STRT;
	FLASH->CR &= ~FLASH_CR_PER;

	/*
	 * Wait until the BSY1 bit of the FLASH status register (FLASH_SR) is cleared
	 */
	if(FLASH->SR & FLASH_SR_BSY1)  {

			/*
			 * Use a blocking wait if flash is Busy
			 */
			TimeOut(1000);

			/*
			 * Check If flash is still busy after wait
			 */
			if(FLASH->SR & FLASH_SR_BSY1) {

				isFlashBusy = true;

				return;
			}
		}
}

uint64_t read_flash_memory(uint32_t memory_address) {

	uint32_t * ptr = 	(uint32_t *) memory_address;

	uint32_t LSB = *ptr;
	uint32_t MSB = *(ptr + 1);

	return((uint64_t)MSB<<32|LSB);
}








