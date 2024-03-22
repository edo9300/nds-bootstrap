#ifndef LOCATIONS_H
#define LOCATIONS_H

#define LOAD_CRT0_LOCATION 0x06840000 // LCDC_BANK_C

#define SDENGINE_LOCATION             0x03000000
#define SDENGINE_LOCATION_ALT         0x037C0000
#define BOOT_INJECT_LOCATION          0x037D0000
#define TEMP_MEM 0x02FFD000
#define TEMP_ARM9_START_ADDRESS (*(vu32*)0x02FFFFF4)

#define CARDENGINE_SHARED_ADDRESS     0x02FFFA00

#define NDS_HEADER      0x023FFE00
#define NDS_HEADER_8MB  0x027FFE00
#define NDS_HEADER_16MB 0x02FFFE00

#define ARM9_START_ADDRESS_LOCATION      (NDS_HEADER + 0x1F4) //0x02FFFFF4

#define RAM_DISK_LOCATION              0x0C400000
#define RAM_DISK_MDROMSIZE             0x929C
#define RAM_DISK_MDROM                 0xDE00
#define RAM_DISK_SNESROMSIZE           0xDE7C
#define RAM_DISK_SNESROM               0xEA00
#define RAM_DISK_SNESCFGSIZE           0x92FC
#define RAM_DISK_SNESCFG               0x40EE00
#define RAM_DISK_LOCATION_LZ77ROM      0x0C900000
#define RAM_DISK_LOCATION_DSIMODE      0x0D000000

#define CACHE_ADDRESS_START            0x03700000
#define CACHE_ADDRESS_START_ALT        0x02FE4000

#endif // LOCATIONS_H
