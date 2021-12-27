/*
	Noobish Noobsicle wrote this IPS patching code
	Adapted to C by RocketRobz
*/

#include <nds/ndstypes.h>
#include <nds/memory.h>
#include "nds_header.h"
#include "locations.h"
#include "tonccpy.h"

extern u8 consoleModel;
extern bool dsiModeConfirmed;
extern bool extendedMemoryConfirmed;
extern bool overlaysInRam;

bool applyIpsPatch(const tNDSHeader* ndsHeader, u8* ipsbyte, bool arm9Only, bool isSdk5, bool ROMinRAM) {
	if (ipsbyte[0] != 'P' && ipsbyte[1] != 'A' && ipsbyte[2] != 'T' && ipsbyte[3] != 'C' && ipsbyte[4] != 'H' && ipsbyte[5] != 0) {
		return false;
	}

	const char* romTid = getRomTid(ndsHeader);
	bool doLow = (strncmp(romTid, "BKW", 3) == 0);
	bool armPatched = false;

	int ipson = 5;
	int totalrepeats = 0;
	u32 offset = 0;
	void* rombyte = 0;
	while (1) {
		offset = ipsbyte[ipson] * 0x10000 + ipsbyte[ipson + 1] * 0x100 + ipsbyte[ipson + 2];
		if (offset >= ndsHeader->arm9romOffset && ((offset < ndsHeader->arm9romOffset+ndsHeader->arm9binarySize) || arm9Only)) {
			// ARM9 binary
			rombyte = ndsHeader->arm9destination - ndsHeader->arm9romOffset;
			armPatched = true;
		} else if (offset >= ndsHeader->arm7romOffset && offset < ndsHeader->arm7romOffset+ndsHeader->arm7binarySize) {
			// ARM7 binary
			rombyte = ndsHeader->arm7destination - ndsHeader->arm7romOffset;
			armPatched = true;
		} else if (offset >= ndsHeader->arm9romOffset+ndsHeader->arm9binarySize && offset < ndsHeader->arm7romOffset) {
			// Overlays
			if (!overlaysInRam) {
				return armPatched;
			}
			rombyte = (void*)(isSdk5 ? ROM_SDK5_LOCATION : ROM_LOCATION);
			if (extendedMemoryConfirmed) {
				rombyte = (void*)ROM_LOCATION_EXT;
			} else if (consoleModel == 0 && ndsHeader->unitCode == 0x02 && dsiModeConfirmed) {
				rombyte = (void*)retail_OVARLAYS_ADRESS_START_TWLSDK;
			} else if (consoleModel == 0 && isSdk5) {
				rombyte = (void*)CACHE_ADRESS_START;

				if (doLow) {
					rombyte = (void*)CACHE_ADRESS_START_low;
				}
			}
			if (ROMinRAM && (dsiModeConfirmed || extendedMemoryConfirmed)) {
				rombyte -= 0x8000;
			} else {
				rombyte -= ndsHeader->arm9romOffset;
				rombyte -= ndsHeader->arm9binarySize;
			}
		}
		ipson += 3;
		if (ipsbyte[ipson] * 256 + ipsbyte[ipson + 1] == 0) {
			ipson += 2;
			totalrepeats = ipsbyte[ipson] * 256 + ipsbyte[ipson + 1];
			ipson += 2;
			u8 repeatbyte[totalrepeats];
			for (int ontime = 0; ontime < totalrepeats; ontime++) {
				repeatbyte[ontime] = ipsbyte[ipson];
			}
			tonccpy(rombyte+offset, repeatbyte, totalrepeats);
			ipson++;
		} else {
			totalrepeats = ipsbyte[ipson] * 256 + ipsbyte[ipson + 1];
			ipson += 2;
			tonccpy(rombyte+offset, ipsbyte+ipson, totalrepeats);
			ipson += totalrepeats;
		}
		if (ipsbyte[ipson] == 69 && ipsbyte[ipson + 1] == 79 && ipsbyte[ipson + 2] == 70) {
			break;
		}
	}
	return true;
}