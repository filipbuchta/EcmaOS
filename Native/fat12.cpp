#include "fat12.h"

#include "debug.h"


#pragma pack(push, 1)
struct SegmentedAddress
{
	uint16 Segment;
	uint16 Offset;
};

struct Int13Packet {
	uint8 PacketSize;
	uint8 Reserved;
	uint16 BlockCount;
	SegmentedAddress Buffer;
	uint64 FirstBlock;
};

#pragma pack(pop)

void BlReadDrive(uint8 driveId, uint16 firstBlock, uint16 blockCount, void * buffer) {
	Log("BlReadDrive");
	Log(driveId);

	Int13Packet packet = Int13Packet();
	packet.PacketSize = sizeof(Int13Packet);
	packet.FirstBlock = firstBlock;
	packet.BlockCount = blockCount;


	SegmentedAddress bufferAddress;
	bufferAddress.Segment = ((uint32)buffer) >> 4;
	bufferAddress.Offset = ((uint32)buffer) & 0x0F;
	packet.Buffer = bufferAddress;
	
	SegmentedAddress packetAddress;
	Log((uint32)&packet);
	packetAddress.Segment = ((uint32)&packet) >> 4;
	packetAddress.Offset = ((uint32)&packet) & 0x0F;
	Log((uint32)packetAddress.Segment);

	_asm {
		xchg bx, bx}
	uint32 _offset32 = packetAddress.Offset;

	_asm 
	{
			mov dl, driveId
			mov eax, 0x42
			mov ds, packetAddress.Segment
			mov esi, _offset32
			int 0x13
	}

}