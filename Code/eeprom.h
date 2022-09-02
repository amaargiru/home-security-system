#ifndef _eeprom_defs_
#define _eeprom_defs_

__eeprom __no_init unsigned char Loop1Set   @ 0x01;
__eeprom __no_init unsigned char Loop2Set   @ 0x02;
__eeprom __no_init unsigned char Loop3Set   @ 0x03;

#define MAX_IB_KEYS 20
#define IB_KEY_LENGTH 8

__eeprom __no_init unsigned char CurrentKeys @ 004;
__eeprom __no_init unsigned char ibKeys[MAX_IB_KEYS][IB_KEY_LENGTH] @ 0x05;	// Ключи iButton. 20 ключей по 8 байт (7 байт данных + CRC8)

#endif
