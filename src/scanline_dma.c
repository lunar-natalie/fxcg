/// @file scanline_dma.c
/// @author Natalie Wiggins (islifepeachy@outlook.com)
/// @brief Display manipulation using DMA.
/// @version 0.1
/// @date 2022-08-21
///
/// @copyright Copyright (c) 2022 Natalie Wiggins.
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "scanline_dma.h"

#include <string.h>

#include <fxcg/display.h>
#include <fxcg/registers.h>

// LCD width including both the VRAM and frame area.
#define LCD_FULL_WIDTH_PX		396
// LCD height including both the VRAM and frame area.
#define LCD_FULL_HEIGHT_PX		224
// LCD top frame height.
#define LCD_TOP_FRAME_HEIGHT_PX		4
// LCD bottom frame height.
#define LCD_BOTTOM_FRAME_HEIGHT_PX	8
// LCD left/right frame width.
#define LCD_SIDE_FRAME_WIDTH_PX		6

// LCD select register.
#define LCD_GRAM	0x202
// LCD data register.
#define LCD_BASE	0xB4000000
// Unknown DMA instruction.
#define SYNCO(void)	__asm__ volatile ("SYNCO\n\t":::"memory");

// DMA channel 0 operation register.
#define DMA0_DMAOR	(volatile color_t *) 0xFE008060
// DMA0 channel 0 channel controll register (?)
#define DMA0_CHCR_0	(volatile unsigned int *) 0xFE00802C
// DMA0 channel 0 source address register.
#define DMA0_SAR_0	(volatile unsigned int *) 0xFE008020
// DMA0 channel 0 data address register.
#define DMA0_DAR_0	(volatile unsigned int *) 0xFE008024
// DMA0 channel 0 transfer count register.
#define DMA0_TCR_0	(volatile unsigned int *) 0xFE008028

// Line buffer in on-CPU 'Y' Memory, divided into two 4KiB pages. Appears not to
// be used by the OS.
static color_t *scanlines[2] = {
	(color_t *) 0xE5007000,
	(color_t *) 0xE5017000};

static unsigned int current_dma_buffer = 0;

/// @brief Waits for the last DMA transfer to complete.
static inline void dma_wait(void)
{
	while (1) {
		if ((*DMA0_DMAOR) & 4) // Address error
			break;
		if ((*DMA0_CHCR_0) & 2) // Transfer complete
			break;
	}

	SYNCO();

	// Disable DMA to issue new commands
	*DMA0_CHCR_0 &= ~1;
	*DMA0_DMAOR = 0;
}

/// @brief Draws to the display (non-blocking).
/// @param src Video data.
/// @param size Size of video data.
static inline void dma_draw(void *src, unsigned int size)
{
	// Disable DMA to issue command
	*DMA0_CHCR_0 &= ~1;
	*DMA0_DMAOR = 0;

	*((volatile unsigned int *) MSTPCR0) &= ~(1 << 21); // Clear bit 21 in unknown peripheral register
	*DMA0_SAR_0 = ((unsigned int) src); // On-chip 'IL' memory (avoids bus conflicts)
	*DMA0_DAR_0 = LCD_BASE & 0x1FFFFFFF; // Set destination to LCD
	*DMA0_TCR_0 = size / 32; // Transfer count
	*DMA0_CHCR_0 = 0x00101400; // Here be dragons
	*DMA0_DMAOR |= 1; // Enable DMA on all channels
	*DMA0_DMAOR &= ~6; // Clear flags
	*DMA0_CHCR_0 |= 1; // Enable channel 0
}

/// @brief Flushes and writes the conents of the scanline buffer to the display.
/// Alternates between each of the two scanlines between calls.
/// @param start_x Start x-coordinate.
/// @param end_x End x-coordinate.
/// @param start_y Start y-coordinate.
/// @param end_y End y-coordinate.
/// @param size Size of video data.
void flush_scan_buffer(unsigned int start_x, unsigned int end_x,
	unsigned int start_y, unsigned int end_y, unsigned int size)
{
	dma_wait();

	Bdisp_WriteDDRegister3_bit7(1);
	Bdisp_DefineDMARange(start_x, end_x, start_y, end_y);
	Bdisp_DDRegisterSelect(LCD_GRAM);

	dma_draw(scanlines[current_dma_buffer], size);
	current_dma_buffer = 1 - current_dma_buffer;
}

void draw_frame_reimpl(color_t color)
{
	memset(scanlines[0], color, 8192);

	// Top.
	flush_scan_buffer(0,
		LCD_FULL_WIDTH_PX - 1,
		0,
		LCD_TOP_FRAME_HEIGHT_PX,
		LCD_FULL_WIDTH_PX * 2 * LCD_TOP_FRAME_HEIGHT_PX);
	// Bottom.
	flush_scan_buffer(0,
		LCD_FULL_WIDTH_PX - 1,
		LCD_HEIGHT_PX,
		LCD_FULL_HEIGHT_PX,
		LCD_FULL_WIDTH_PX * 2 * LCD_BOTTOM_FRAME_HEIGHT_PX);
	// Left.
	flush_scan_buffer(0,
		LCD_SIDE_FRAME_WIDTH_PX - 1,
		0,
		LCD_FULL_HEIGHT_PX,
		LCD_FULL_HEIGHT_PX * 2 * LCD_SIDE_FRAME_WIDTH_PX);
	// Right.
	flush_scan_buffer(LCD_FULL_WIDTH_PX - LCD_SIDE_FRAME_WIDTH_PX,
		LCD_FULL_WIDTH_PX - 1,
		0,
		LCD_FULL_HEIGHT_PX,
		LCD_FULL_HEIGHT_PX * 2 * LCD_SIDE_FRAME_WIDTH_PX);

	dma_wait();
}
