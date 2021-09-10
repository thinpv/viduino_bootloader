#include "slip.h"
#include "printf.h"
#include "slip_handle.h"
#include <uart.h>
#include <gpio.h>
#include <timer.h>

#define SLIP_UART UART1

#define SLIP_START 0xC0
#define SLIP_SYMBOR_START 0xDB

#define SLIP_MESSAGE_HEADER_LEN 1 + 1 + 2 + 4

typedef enum
{
	SLIP_WAIT_SYNC = 0,
	SLIP_SYNCING,
	SLIP_SYNCED,
	SLIP_FINISH,
} slip_state_e;

typedef enum
{
	SLIP_DIRECTION = 0,
	SLIP_COMMAND,
	SLIP_SIZE,
	SLIP_CHECKSUM,
	SLIP_DATA
} slip_step_e;

static slip_state_e slip_state;
static slip_message_st slip_req;
static slip_step_e slip_step;
static uint8_t have_symbor;
static uint8_t slip_size_index;
static uint8_t slip_checksum_index;
static uint16_t slip_data_index;

static slip_message_st slip_resp;
__attribute__((aligned(1024))) static uint8_t buff[MAX_DATA_LEN];
static uint32_t data_size;

void slip_printf_binary(uint8_t *buff, uint32_t len)
{
	uint32_t i;
	for (i = 0; i < len; i++)
	{
		if (i % 16 == 0)
			SLIP_LOG("%04x:", i);
		if (i % 4 == 0)
			SLIP_LOG(" ");
		SLIP_LOG("%02x", buff[i]);
		if (i % 16 == 15)
			SLIP_LOG("\r\n");
	}
	if (len % 16 != 0)
		SLIP_LOG("\r\n");
}

static void slip_printf(slip_message_st *slip_message_ptr)
{
	SLIP_LOG("direction: 0x%02X, command: 0x%02X, size: %d, checksum: 0x%08lX",
					 slip_message_ptr->direction, slip_message_ptr->command,
					 slip_message_ptr->size, slip_message_ptr->value_crc);
	if (slip_message_ptr->data && slip_message_ptr->size > 0)
	{
		if (slip_message_ptr->size <= 16)
		{
			SLIP_LOG(", data:");
			for (int i = 0; i < slip_message_ptr->size; i++)
			{
				if (i % 4 == 0)
					SLIP_LOG(" ");
				SLIP_LOG("%02X", slip_message_ptr->data[i]);
			}
			SLIP_LOG("\r\n");
		}
		else
		{ // if(slip_message_ptr->size < 2560) {
			SLIP_LOG("\r\n");
			// slip_printf_binary(slip_message_ptr->data, slip_message_ptr->size);
		}
	}
	else
		SLIP_LOG("\r\n");
}

static uint32_t slip_send_data(uint8_t *buff, uint32_t len)
{
	if (uart_write(SLIP_UART, buff, len, 1000) != len)
	{
		SLIP_LOG("slip send data error\r\n");
	}
	return len;
}

static void slip_add_symbol(uint8_t *buff, uint32_t *index, uint8_t data)
{
	if (SLIP_START == data)
	{
		buff[(*index)++] = SLIP_SYMBOR_START;
		buff[(*index)++] = 0xDC;
	}
	else if (SLIP_SYMBOR_START == data)
	{
		buff[(*index)++] = SLIP_SYMBOR_START;
		buff[(*index)++] = 0xDB;
	}
	else
		buff[(*index)++] = data;
}

static uint8_t slip_create_data_buff(slip_message_st *slip_message_ptr, uint8_t *buff, uint32_t max_len)
{
	if (max_len < SLIP_MESSAGE_HEADER_LEN)
		return 1;
	uint32_t index = 0;
	uint32_t i;
	buff[index++] = SLIP_START;
	slip_add_symbol(buff, &index, slip_message_ptr->direction);
	slip_add_symbol(buff, &index, slip_message_ptr->command);
	slip_add_symbol(buff, &index, slip_message_ptr->size & 0xFF);
	slip_add_symbol(buff, &index, (slip_message_ptr->size >> 8) & 0xFF);
	slip_add_symbol(buff, &index, slip_message_ptr->value_crc & 0xFF);
	slip_add_symbol(buff, &index, (slip_message_ptr->value_crc >> 8) & 0xFF);
	slip_add_symbol(buff, &index, (slip_message_ptr->value_crc >> 16) & 0xFF);
	slip_add_symbol(buff, &index, (slip_message_ptr->value_crc >> 24) & 0xFF);
	for (i = 0; i < slip_message_ptr->size; i++)
	{
		slip_add_symbol(buff, &index, slip_message_ptr->data[i]);
	}
	buff[index++] = SLIP_START;
	if (index > max_len)
		SLIP_LOG("buff khong du lon\r\n");
	return index;
	// chua check max_len
}

static uint8_t handle_req_message(slip_message_st *slip_message_ptr)
{
	SLIP_LOG("recv: ");
	slip_printf(slip_message_ptr);
	switch (slip_message_ptr->command)
	{
	case SLIP_FLASH_BEGIN:
		slip_state = SLIP_SYNCED;
		slip_handle_flash_begin_message(slip_message_ptr, &slip_resp);
		data_size = slip_create_data_buff(&slip_resp, buff, MAX_DATA_LEN);
		SLIP_LOG("send: ");
		slip_printf_binary(buff, data_size);
		slip_send_data(buff, data_size);
		break;

	case SLIP_FLASH_DATA:
		slip_state = SLIP_SYNCED;
		slip_handle_flash_data_message(slip_message_ptr, &slip_resp);
		data_size = slip_create_data_buff(&slip_resp, buff, MAX_DATA_LEN);
		SLIP_LOG("send: ");
		slip_printf_binary(buff, data_size);
		slip_send_data(buff, data_size);
		break;

	case SLIP_SYNC:
		slip_state = SLIP_SYNCING;
		break;

	case SLIP_FLASH_END:
		SLIP_LOG("SLIP_FLASH_END ==================>\r\n");
		slip_state = SLIP_FINISH;
		break;

	case SLIP_READ_REG:
		slip_state = SLIP_SYNCED;
		slip_handle_read_reg_message(slip_message_ptr, &slip_resp);
		data_size = slip_create_data_buff(&slip_resp, buff, MAX_DATA_LEN);
		SLIP_LOG("send: ");
		slip_printf_binary(buff, data_size);
		slip_send_data(buff, data_size);
		break;

	case SLIP_SPI_ATTACH:
		slip_state = SLIP_SYNCED;
		slip_handle_spi_attach_message(slip_message_ptr, &slip_resp);
		data_size = slip_create_data_buff(&slip_resp, buff, MAX_DATA_LEN);
		SLIP_LOG("send: ");
		slip_printf_binary(buff, data_size);
		slip_send_data(buff, data_size);
		break;

	case SLIP_CHANGE_BAUDRATE:
		slip_state = SLIP_SYNCED;
		slip_handle_change_baudrate_message(slip_message_ptr, &slip_resp);
		data_size = slip_create_data_buff(&slip_resp, buff, MAX_DATA_LEN);
		SLIP_LOG("send: ");
		slip_printf_binary(buff, data_size);
		slip_send_data(buff, data_size);
		delay(20);

		slip_change_baudrate_message_st *slip_change_baudrate_message = (slip_change_baudrate_message_st *)slip_message_ptr->data;
		uart_set(SLIP_UART, slip_change_baudrate_message->baudrate1, 8, 0, 1);
		SLIP_LOG("new baudrate: %d\r\n", slip_change_baudrate_message->baudrate1);
		break;

	case SLIP_SPI_FLASH_MD5:
		SLIP_LOG("SLIP_SPI_FLASH_MD5\r\n");
		slip_state = SLIP_SYNCED;
		slip_handle_md5_message(slip_message_ptr, &slip_resp);
		data_size = slip_create_data_buff(&slip_resp, buff, MAX_DATA_LEN);
		SLIP_LOG("send: ");
		slip_printf_binary(buff, data_size);
		slip_send_data(buff, data_size);
		slip_state = SLIP_FINISH;
		break;

	default:
		break;
	}
	return 0;
}

static uint8_t handle_resq_message(slip_message_st *slip_message_ptr)
{
	SLIP_LOG("not supported\r\n");
	return 1;
}

static uint8_t handle_message(slip_message_st *slip_message_ptr)
{
	if (slip_message_ptr->direction == SLIP_REQUEST)
		return handle_req_message(slip_message_ptr);
	else if (slip_message_ptr->direction == SLIP_RESPONSE)
		return handle_resq_message(slip_message_ptr);
	return 1;
}

static void slip_reset_params()
{
	have_symbor = 0;
	slip_step = SLIP_DIRECTION;
	slip_size_index = 0;
	slip_checksum_index = 0;
	slip_data_index = 0;
}

static uint8_t have_checksum(uint8_t command)
{
	switch (command)
	{
	case SLIP_MEM_DATA:
	case SLIP_FLASH_DATA:
	case SLIP_FLASH_DEFL_DATA:
		return 1;

	default:
		return 0;
	}
	return 0;
}

static void slip_handle_data(uint8_t d)
{
	switch (d)
	{
	case SLIP_START:
		slip_reset_params();
		break;

	case SLIP_SYMBOR_START:
		have_symbor = 1;
		break;

	default:
		if (have_symbor)
		{
			if (d == 0xDC)
				d = 0xC0;
			else if (d == 0xDD)
				d = 0xDB;
			have_symbor = 0;
		}
		switch (slip_step)
		{
		case SLIP_DIRECTION:
			slip_req.direction = d;
			++slip_step;
			break;

		case SLIP_COMMAND:
			slip_req.command = d;
			++slip_step;
			break;

		case SLIP_SIZE:
			if (slip_size_index == 0)
			{
				slip_req.size = d;
			}
			else
			{
				slip_req.size |= (d << (8 * slip_size_index));
			}
			++slip_size_index;
			if (slip_size_index == sizeof(slip_req.size))
			{
				++slip_step;
			}
			break;

		case SLIP_CHECKSUM:
			if (slip_checksum_index == 0)
			{
				slip_req.value_crc = d;
			}
			else
			{
				slip_req.value_crc |= (d << (8 * slip_checksum_index));
			}
			++slip_checksum_index;
			if (slip_checksum_index == sizeof(slip_req.value_crc))
			{
				++slip_step;
			}
			break;

		case SLIP_DATA:
			if (slip_data_index < MAX_DATA_LEN)
				slip_req.data[slip_data_index++] = d;
			if (slip_data_index == slip_req.size)
			{
				handle_message(&slip_req);
			}
			break;

		default:
			break;
		}
		break;
	}
}

void slip_start()
{
	uint8_t d;
	unsigned long current_time = 0;
	slip_state = SLIP_WAIT_SYNC;

	if (SLIP_UART == UART0)
	{
		gpio_set_cfg(GPIOE, 0, 5);
		gpio_set_cfg(GPIOE, 1, 5);
	}
	else if (SLIP_UART == UART1)
	{
		gpio_set_cfg(GPIOA, 2, 5);
		gpio_set_cfg(GPIOA, 3, 5);
	}
	uart_set(SLIP_UART, 115200, 8, 0, 1);

	while (1)
	{
		if (uart_read(SLIP_UART, &d, 1, 0) == 1)
		{
			slip_handle_data(d);
		}

		switch (slip_state)
		{
		case SLIP_WAIT_SYNC:
			break;

		case SLIP_SYNCING:
			if (millis() - current_time > 10)
			{
				current_time = millis();
				slip_handle_sync_message(NULL, &slip_resp);
				data_size = slip_create_data_buff(&slip_resp, buff, MAX_DATA_LEN);
				SLIP_LOG("send: ");
				slip_printf_binary(buff, data_size);
				slip_send_data(buff, data_size);
			}
			break;

		case SLIP_SYNCED:
			break;

		case SLIP_FINISH:
			SLIP_LOG("SLIP_FINISH ==================>\r\n");
			return;

		default:
			break;
		}
	}
}
