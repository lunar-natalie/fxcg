/// @file renderer.c
/// @author Natalie Wiggins (islifepeachy@outlook.com)
/// @brief Renders display contents.
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

#include "renderer.h"
#include "scanline_dma.h"

#include <fxcg/display.h>

static color_t *vram;

void render(void)
{
	// Disable status area.
	EnableStatusArea(3);

	// Fill outer frame (inaccessible via VRAM) with the background.
	draw_frame_reimpl(COLOR_BLACK);

	// Get VRAM address to start drawing to.
	vram = GetVRAMAddress();

	// Draw top border.
	for (color_t x = 0; x < LCD_WIDTH_PX; ++x)
		*vram++ = COLOR_MAGENTA;

	// Draw edge borders and background.
	for (color_t y = 1; y < LCD_HEIGHT_PX - 1; ++y) {
		*vram++ = COLOR_MAGENTA;
		for (color_t x = 1; x < LCD_WIDTH_PX - 1; ++x)
			*vram++ = COLOR_BLACK;
		*vram++ = COLOR_MAGENTA;
	}

	// Draw bottom border.
	for (color_t x = 0; x < LCD_WIDTH_PX; ++x)
		*vram++ = COLOR_MAGENTA;
}
