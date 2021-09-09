#include "slip_handle.h"
#include <string.h>
#include <printf.h>
#include <sys_spiflash.h>
#include <timer.h>
#include "md5sum.h"

static slip_flash_begin_message_st slip_flash_begin_message;
static slip_flash_data_message_st* slip_flash_data_message;
static uint32_t current_package;

static MD5state md5state;
static byte digest[16];

uint32_t slip_checksum_data(uint8_t* buff, uint32_t len)
{
  uint8_t result = 0xEF;
  for(uint32_t i=0; i<len; i++) {
    result ^= buff[i];
  }
  return (uint32_t)result;
}

uint32_t slip_handle_sync_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des)
{
  if(!slip_message_des)
    return 1;
  slip_message_des->direction = SLIP_RESPONSE;
  slip_message_des->command = SLIP_SYNC;
  slip_message_des->size = 4;
  slip_message_des->value_crc = 0x55201207;
  *((uint32_t*)slip_message_des->data) = 0;
  return 0;
}

static uint32_t slip_handle_read_reg(uint32_t addr)
{
  switch (addr)
  {
  case 0x40001000:// 00100040
    return 0x00f01d83;// 831df000

  case 0x3ff5a00c:// 0ca0f53f
    return 0x00008000;// 00800000

  case 0x3ff5a014:// 14a0f53f
    return 0x00000000;// 00000000

  case 0x3ff6607c:// 7c60f63f
    return 0x16042000;// 00200416

  case 0x3ff5a010:// 10a0f53f
    return 0x00000039;// 39000000

  case 0x3ff5a018:// 18a0f53f
    return 0x00000004;// 04000000

  case 0x3ff40014:// 1400f43f
    return 0x00000162;// 62010000

  case 0x3ff5a008:// 08a0f53f
    return 0x00eb30ae;// ae30eb00

  case 0x3ff5a004:// 04a0f53f
    return 0xa4499358;// 589349a4
  
  default:
    return 0;
  }
}

uint32_t slip_handle_read_reg_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des)
{
  if(!slip_message_des)
    return 1;
  slip_message_des->direction = SLIP_RESPONSE;
  slip_message_des->command = SLIP_READ_REG;
  slip_message_des->size = 4;
  slip_message_des->value_crc = slip_handle_read_reg(*((uint32_t*)slip_message_src->data));//0xfff0c101;
  *((uint32_t*)slip_message_des->data) = 0;
  return 0;
}

uint32_t slip_handle_spi_attach_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des)
{
  if(!slip_message_src || !slip_message_des)
    return 1;
  slip_message_des->direction = SLIP_RESPONSE;
  slip_message_des->command = SLIP_SPI_ATTACH;
  slip_message_des->size = 4;
  slip_message_des->value_crc = 0;
  *((uint32_t*)slip_message_des->data) = 0;
  return 0;
}

uint32_t slip_handle_change_baudrate_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des)
{
  if(!slip_message_src || !slip_message_des)
    return 1;

  slip_message_des->direction = SLIP_RESPONSE;
  slip_message_des->command = SLIP_CHANGE_BAUDRATE;
  slip_message_des->size = 4;
  slip_message_des->value_crc = 0;
  *((uint32_t*)slip_message_des->data) = 0;
  return 0;
}

uint32_t slip_handle_md5_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des)
{
	char pr64[65];
  if(!slip_message_src || !slip_message_des)
    return 1;

  if(md5state.done == 0) {
    md5((byte*)pr64, 0, digest, &md5state);
  }

  slip_message_des->direction = SLIP_RESPONSE;
  slip_message_des->command = SLIP_SPI_FLASH_MD5;
  slip_message_des->size = 4 + 32;
  slip_message_des->value_crc = 0;
  slip_message_des->data[0] = '\0';
  for(uint8_t i=0; i<16; i++) {
    sprintf((char*)slip_message_des->data, "%s%02x", (char*)slip_message_des->data, digest[i]);
  }
  *((uint32_t*)(slip_message_des->data + 32)) = 0;
  return 0;
}

uint32_t slip_handle_flash_begin_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des)
{
  if(!slip_message_src || !slip_message_des)
    return 1;
  memcpy(((uint8_t*)&slip_flash_begin_message), slip_message_src->data, sizeof(slip_flash_begin_message_st));
  printf("erase_size: %ld, number_of_packet: %ld, package_size: %ld, offset: 0x%08lX\r\n", 
          slip_flash_begin_message.erase_size, slip_flash_begin_message.number_of_packet,
          slip_flash_begin_message.package_size, slip_flash_begin_message.offset);

  current_package = 0;
  md5sum_init(&md5state);

  uint32_t data_size = slip_flash_begin_message.number_of_packet * slip_flash_begin_message.package_size;
  uint32_t number_of_64k_block = data_size / SIZE_64K;
  if(data_size & 0xFFFF)
    ++number_of_64k_block;

  for(uint32_t i=0; i<number_of_64k_block; i++) {
    sys_spi_flash_erase_64k(slip_flash_begin_message.offset + i*SIZE_64K);
  }

  sys_spi_flash_erase_4k(FLASH_APP_LEN_START_ADDRESS);
  printf("write 0x%08lX to 0x%08X\r\n", slip_flash_begin_message.erase_size, FLASH_APP_LEN_START_ADDRESS);
  sys_spi_flash_write(FLASH_APP_LEN_START_ADDRESS, (uint8_t*)&slip_flash_begin_message.erase_size, 4);
  
  slip_message_des->direction = SLIP_RESPONSE;
  slip_message_des->command = SLIP_FLASH_BEGIN;
  slip_message_des->size = 4;
  slip_message_des->value_crc = 0;
  *((uint32_t*)slip_message_des->data) = 0;
  return 0;
}

uint32_t slip_handle_flash_data_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des)
{
  if(!slip_message_src || !slip_message_des)
    return 1;
  slip_flash_data_message = (slip_flash_data_message_st*)slip_message_src->data;
  uint32_t crc_cal = slip_checksum_data(slip_flash_data_message->data, slip_flash_data_message->data_size);
  printf("data_size: %ld, sequence_number: %ld, crc_cal: 0x%08lX\r\n", 
          slip_flash_data_message->data_size, slip_flash_data_message->sequence_number, crc_cal);

  slip_message_des->direction = SLIP_RESPONSE;
  slip_message_des->command = SLIP_FLASH_DATA;
  slip_message_des->size = 4;
  slip_message_des->value_crc = 0;
  *((uint32_t*)slip_message_des->data) = 0;

  if(current_package > slip_flash_data_message->sequence_number) {
    // lap lai ban tin do master khong nhan duoc phan hoi
    return 1;
  } else if(current_package == slip_flash_data_message->sequence_number) {
    if(slip_flash_data_message->data_size == slip_flash_begin_message.package_size) {
      if(slip_message_src->value_crc == crc_cal) {
        uint32_t write_addr = slip_flash_begin_message.offset + slip_flash_data_message->sequence_number * slip_flash_begin_message.package_size;
        if(sys_spi_flash_write(write_addr, slip_flash_data_message->data, slip_flash_data_message->data_size) == 0) {
          memset(slip_flash_data_message->data, 0x00, slip_flash_data_message->data_size);
          if(sys_spi_flash_read(write_addr, slip_flash_data_message->data, slip_flash_data_message->data_size) == 0) {
            crc_cal = slip_checksum_data(slip_flash_data_message->data, slip_flash_data_message->data_size);
            if(slip_message_src->value_crc == crc_cal) {
              ++current_package;
              if(md5state.len + slip_flash_data_message->data_size > slip_flash_begin_message.erase_size) {
                md5(slip_flash_data_message->data, slip_flash_begin_message.erase_size - md5state.len, digest, &md5state);
              } else {
                md5(slip_flash_data_message->data, slip_flash_data_message->data_size, digest, &md5state);
              }
              return 0;
            }
          }
        }
        printf("flash write error ==============================================>\r\n");
        slip_message_des->data[0] = 1;
        slip_message_des->data[1] = 0x08; // flash write error
        return 1;
      } else {
        printf("CRC not match ==============================================>\r\n");
        slip_message_des->data[0] = 1;
        slip_message_des->data[1] = 0x07; // Invalid CRC in message
        return 1;
      }
    }
  }
  printf("Received message is invalid ==============================================>\r\n");
  slip_message_des->data[0] = 1;
  slip_message_des->data[1] = 0x05; // Received message is invalid
  return 1;
}
