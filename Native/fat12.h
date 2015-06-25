#pragma once

#include "../Runtime/global.h"

struct Fat12BootSector {
	uint8 JumpInstruction[3];
	uint8 OemName[8];
	uint16 BytesPerSector;
};


void BlReadDrive(uint8 driveId, uint16 firstBlock, uint16 blockCount, void * buffer);
