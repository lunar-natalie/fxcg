/// @file main.c
/// @author Natalie Wiggins (islifepeachy@outlook.com)
/// @brief Entry point to Prizmatron: 2084, a multidirectional shooter for the
/// Casio Prizm.
/// @version 0.1
/// @date 2022-08-19
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

#include <fxcg/keyboard.h>

int main(void)
{
	// TODO(Natalie): Fix: Requires input before outer frame is drawn.

	int key;
	while (1) {
		render();

		// Wait for input and write VRAM contents to the display.
		// Returns to the Main Menu if MENU is pressed.
		GetKey(&key);
	}

	return 0;
}
