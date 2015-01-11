#pragma once

#define BL_VIDEO_ROW_COUNT              50
#define BL_VIDEO_COL_COUNT              80
#define BL_VIDEO_DEFAULT_ATTRIBUTE      0x1F00

#define BL_VIDEO_REGISTER_INDEX_PORT    0x3D4
#define BL_VIDEO_REGISTER_DATA_PORT     0x3D5

#define BL_VIDEO_REGISTER_CURSOR_LOW    0x0F
#define BL_VIDEO_REGISTER_CURSOR_HIGH   0x0E

#define BL_VIDEO_BASE_ADDRESS           ((unsigned short*) 0x000B8000)


void ConsoleWrite(const char* text);
void ClearScreen();