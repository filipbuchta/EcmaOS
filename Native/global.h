#pragma once

#include "../Runtime/global.h"

#define outportb(port, value) _asm { mov al, value } _asm { out port, al }
