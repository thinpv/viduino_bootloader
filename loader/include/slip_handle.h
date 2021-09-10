#ifndef SLIP_MESSAGE_H_
#define SLIP_MESSAGE_H_

#include "slip.h"

uint32_t slip_handle_sync_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des);
uint32_t slip_handle_read_reg_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des);
uint32_t slip_handle_spi_attach_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des);
uint32_t slip_handle_change_baudrate_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des);
uint32_t slip_handle_md5_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des);
uint32_t slip_handle_flash_begin_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des);
uint32_t slip_handle_flash_data_message(slip_message_st* slip_message_src, slip_message_st* slip_message_des);

#endif
