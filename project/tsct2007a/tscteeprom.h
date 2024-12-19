#include <unistd.h>

#define EEPROM_I2C_ADDR 0x50    //AT24CM02-SSHD

void EEPROM_i2c_write_page(uint8_t dev_addr, uint8_t addr, uint8_t* data, size_t length);


//이전 명령의 다음 주소에서 1Byte 읽기(Page 끝 도달 시 Page 처음으로 이동 후 계속)
void EEPROM_i2c_read_next(uint8_t dev_addr, uint8_t* dataBuffer);


//이전 명령과 같은 Page(256B 단위)내 addr부터 length 만큼 연속 읽기(page 끝 도달 시 Page 처음으로 이동 후 계속)
void EEPROM_i2c_read_seq(uint8_t dev_addr, uint8_t addr, uint8_t* dataBuffer, size_t length);

void EEPROM_i2c_write_byte(uint8_t dev_addr, uint8_t addr, uint8_t data);

void EEPROM_i2c_read_byte(uint8_t dev_addr, uint8_t addr, uint8_t* dataBuffer);

void EEPROMTest();