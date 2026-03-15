/*
 * Author: Florian Simon
 * Institution: Technische Universitaet Berlin
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include "main.h"
#include "rtc_ds1307.h"
#include "tmp126.h"
//#include "led_driver.h"
#include "dcf77.h"
#include "rnbd451.h"

#define CMD_AUTH1 					0xC7
#define CMD_AUTH2 					0x41
#define CMD_SOURCE_HOST				0x00
#define CMD_SOURCE_DEVICE			0x01
#define CMD_CHANNEL_USB             0x00
#define CMD_CHANNEL_BT              0x01
#define CMD_RESET                   0x00
#define CMD_INCREASE_SECOND         0x01
#define CMD_DECREASE_SECOND         0x02
#define CMD_SET_SECOND              0x03
#define CMD_INCREASE_MINUTE         0x04
#define CMD_DECREASE_MINUTE         0x05
#define CMD_SET_MINUTE              0x06
#define CMD_INCREASE_HOUR           0x07
#define CMD_DECREASE_HOUR           0x08
#define CMD_SET_HOUR                0x09
#define CMD_SET_TIME                0x0A
#define CMD_WRITE_RTC               0x0B
#define CMD_READ_RTC                0x0C
#define CMD_ENABLE_ALARM            0x0D
#define CMD_DISABLE_ALARM           0x0E
#define CMD_SET_ALARM               0x0F
#define CMD_SYNC_RADIO_NOW          0x10
#define CMD_SET_RADIO_SYNC_TIME     0x11
#define CMD_SYNC_NETWORK_NOW        0x12

typedef struct
{
	uint8_t state;
	uint8_t command_available;
	uint8_t bluetooth_connected;
	uint8_t initiate_dcf77;
	uint8_t initiate_alarm;

	uint8_t cmd_buf[9];

	uint8_t dcf77_sync_seconds;
	uint8_t dcf77_sync_minutes;
	uint8_t dcf77_sync_hours;

	uint8_t alarm_seconds;
	uint8_t alarm_minutes;
	uint8_t alarm_hours;
} control_t;

void control_init(control_t* ctrl);
void control_transmit_command(control_t* ctrl, rnbd451_t* rnbd451, uint8_t channel, uint8_t type, uint32_t value);
void control_process_command(control_t* ctrl, rtc_t* rtc, dcf77_t* dcf77, TMP126_t* tmp126, rnbd451_t* rnbd451);

#endif /* INC_CONTROL_H_ */
