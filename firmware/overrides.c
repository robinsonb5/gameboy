#include "keyboard.h"
#include "spi.h"
#include "user_io.h"
#include "osd.h"

/* Key -> gamepad mapping.  We override this to swap buttons A and B for NES. */

unsigned char joy_keymap[]=
{
	KEY_CAPSLOCK,
	KEY_LSHIFT,
	KEY_LCTRL,
	KEY_ALT,
	KEY_W,
	KEY_S,
	KEY_A,
	KEY_D,
	KEY_ENTER,
	KEY_RSHIFT,
	KEY_RCTRL,
	KEY_ALTGR,
	KEY_UPARROW,
	KEY_DOWNARROW,
	KEY_LEFTARROW,
	KEY_RIGHTARROW,
};

void fillram(int index,int size,int pattern)
{
	int i;
	SPI_ENABLE(HW_SPI_FPGA);
	SPI(SPI_FPGA_FILE_INDEX);
	SPI(index); /* Memory clear */
	SPI_DISABLE(HW_SPI_FPGA);

	SPI_ENABLE(HW_SPI_FPGA);
	SPI(SPI_FPGA_FILE_TX);
	SPI(0x01); /* Upload */
	SPI_DISABLE(HW_SPI_FPGA);

	SPI_ENABLE_FAST_INT(HW_SPI_FPGA);
	SPI(SPI_FPGA_FILE_TX_DAT);
	for(i=0;i<(size>>2);++i)
	{
		SPI(pattern>>24);
		SPI(pattern>>16);
		SPI(pattern>>8);
		SPI(pattern);
	}
	SPI_DISABLE(HW_SPI_FPGA);

	SPI_ENABLE(HW_SPI_FPGA);
	SPI(SPI_FPGA_FILE_TX);
	SPI(0x00); /* End Upload */
	SPI_DISABLE(HW_SPI_FPGA);
}


const char *bootrom_name="GBC_BIOSBIN not found";
const char *bootrom2_name="AUTOBOOTGB ";
extern unsigned char romtype;
char *autoboot()
{
	char *result=0;
	romtype=0;
	if(!LoadROM(bootrom_name))
		result=bootrom_name;
	romtype=1;
	LoadROM(bootrom2_name);
	return(result);
}

