#include "video.h"

unsigned short* BlVideoBuffer = (unsigned short*)BL_VIDEO_BASE_ADDRESS;
unsigned short BlVideoCursorPosition = 0;

void ConsoleWrite(const char* text) {
	unsigned short x = BlVideoCursorPosition % BL_VIDEO_COL_COUNT;
	unsigned short y = BlVideoCursorPosition / BL_VIDEO_COL_COUNT;

	const char* current = text;

	while (*current != 0) {
		if (*current == '\n') {
			y++;
			x = 0;
		} else {
			BL_VIDEO_BASE_ADDRESS[x + y * BL_VIDEO_COL_COUNT] = 0x0700 | *current;

			x++;
			if (x == BL_VIDEO_ROW_COUNT) {
				x = 0;
				y++;
			}
		}
		current++;
	}

	BlVideoCursorPosition = x + y * BL_VIDEO_COL_COUNT;
}

void ClearScreen() {
	for (int i = 0; i < BL_VIDEO_COL_COUNT * BL_VIDEO_ROW_COUNT; i++) {
		BlVideoBuffer[i] = 0x0700 | ' ';
	}
	BlVideoCursorPosition = 0;
}