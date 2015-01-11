

BytesPerSector		DW	512
SectorsPerCluster	DB	1
ReservedSectors		DW	1
NumberOfFATs		DB	2
RootEntries		DW	224
TotalSectors		DW	2880
Media			DB	0xF0
SectorsPerFAT		DW	9
SectorsPerTrack		DW	18
HeadsPerCylinder	DW	2
HiddenSectors		DD	0
TotalSectorsBig		DD	0
DriveNumber		DB	0
			DB	0
ExtBootSignature	DB	0x29
SerialNumber		DD	0xa0a1a2a3