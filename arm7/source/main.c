/*---------------------------------------------------------------------------------

default ARM7 core

Copyright (C) 2005 - 2010
	Michael Noland (joat)
	Jason Rogers (dovoto)
	Dave Murphy (WinterMute)

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1.	The origin of this software must not be misrepresented; you
	must not claim that you wrote the original software. If you use
	this software in a product, an acknowledgment in the product
	documentation would be appreciated but is not required.

2.	Altered source versions must be plainly marked as such, and
	must not be misrepresented as being the original software.

3.	This notice may not be removed or altered from any source
	distribution.

---------------------------------------------------------------------------------*/
#include <nds.h>

#include <nds/ndstypes.h>

//---------------------------------------------------------------------------------
void VcountHandler() {
//---------------------------------------------------------------------------------
	inputGetAndSend();
}

void myFIFOValue32Handler(u32 value,void* data)
{
  nocashMessage("myFIFOValue32Handler");

  nocashMessage("default");
  nocashMessage("fifoSendValue32");
  fifoSendValue32(FIFO_USER_02,*((unsigned int*)value));	

}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	// Switch to NTR Mode
	REG_SCFG_ROM = 0x703;
	REG_SCFG_EXT = 0x93A40000;
		
	// read User Settings from firmware
	readUserSettings();		
	irqInit();
	
	// Start the RTC tracking IRQ
	initClockIRQ();		
	fifoInit();	

	SetYtrigger(80);	
	
	installSystemFIFO();		
	
	irqSet(IRQ_VCOUNT, VcountHandler);

	irqEnable( IRQ_VBLANK | IRQ_VCOUNT);

	fifoWaitValue32(FIFO_USER_03);	
	fifoSendValue32(FIFO_USER_05, 1);	
	
	fifoSetValue32Handler(FIFO_USER_01,myFIFOValue32Handler,0);	

	// Keep the ARM7 mostly idle
	while (1) { swiWaitForVBlank();}
}

