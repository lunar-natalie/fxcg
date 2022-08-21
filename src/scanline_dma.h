/// @file display.h
/// @author Natalie Wiggins (islifepeachy@outlook.com)
/// @brief Display manipulation using DMA.
/// @version 0.1
/// @date 2022-08-20
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

#pragma once

#include <fxcg/display.h>

/// @brief Changes the color of the display border (re-implementation of the
/// unreliable DrawFrame syscall).
/// @param color Color with which to fill the border.
void draw_frame_reimpl(color_t color);
