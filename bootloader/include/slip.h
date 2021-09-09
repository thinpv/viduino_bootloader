#ifndef SLIP_H_
#define SLIP_H_

#include "stdbool.h"
#include <sys_spiflash.h>

#define MAX_DATA_LEN      1100

typedef enum {
  SLIP_REQUEST = 0,
  SLIP_RESPONSE,
} slip_direction_e;

typedef enum {
  SLIP_FLASH_BEGIN = 0x02,
  SLIP_FLASH_DATA,
  SLIP_FLASH_END,
  SLIP_MEM_BEGIN,
  SLIP_MEM_END,
  SLIP_MEM_DATA,
  SLIP_SYNC,
  SLIP_WRITE_REG,
  SLIP_READ_REG,
  SLIP_SPI_SET_PARAMS,
  SLIP_SPI_ATTACH = 0x0d,
  SLIP_CHANGE_BAUDRATE = 0x0f,
  SLIP_FLASH_DEFL_BEGIN,
  SLIP_FLASH_DEFL_DATA,
  SLIP_FLASH_DEFL_END,
  SLIP_SPI_FLASH_MD5
} slip_command_e;

typedef struct {
  slip_direction_e direction;
  slip_command_e command;
  uint16_t size;
  uint32_t value_crc;
  uint8_t data[MAX_DATA_LEN];
} slip_message_st;

typedef struct {
    uint32_t baudrate1;
    uint32_t baudrate2;;
} slip_change_baudrate_message_st;

typedef struct {
    uint32_t erase_size;
    uint32_t number_of_packet;
    uint32_t package_size;
    uint32_t offset;
} slip_flash_begin_message_st;

typedef struct {
    uint32_t data_size;
    uint32_t sequence_number;
    uint32_t rev1;
    uint32_t rev2;
    uint8_t data[];
} slip_flash_data_message_st;

void slip_printf_binary(uint8_t* buff, uint32_t len);
void slip_start(void);

#endif
