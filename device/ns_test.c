#include "ns_eeprom.h"

const u8 TEXT_Buffer[]={"iic&eeprom test:Hello World!"};
#define SIZE sizeof(TEXT_Buffer)	
u8 datatemp[SIZE];	
void eeprom_test(){
	AT24CXX_Init();
	if(AT24CXX_Check()){
		xprintf_s("check eeprom failed\r\n");
	}
	AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
	AT24CXX_Read(0,datatemp,SIZE);
	xprintf_s("%s\r\n",datatemp);
}