/*
 * Author: Florian Simon
 * Institution: Technische Universitaet Berlin
 */

#include "main.h"
#include <string.h>
#include "control.h"

void control_init(control_t* ctrl)
{
	ctrl->command_available = 0;
	ctrl->bluetooth_connected = 0;
	ctrl->initiate_dcf77 = 0;
	ctrl->initiate_alarm = 0;

	memset(ctrl->cmd_buf, 0, 9);

	ctrl->dcf77_sync_seconds = 0;
	ctrl->dcf77_sync_minutes = 0;
	ctrl->dcf77_sync_hours = 0;

	ctrl->alarm_seconds = 0;
	ctrl->alarm_minutes = 0;
	ctrl->alarm_hours = 0;
}

void control_transmit_command(control_t* ctrl, rnbd451_t* rnbd451, uint8_t channel, uint8_t type, uint32_t value)
{
	ctrl->cmd_buf[0] = CMD_AUTH1;
	ctrl->cmd_buf[1] = CMD_AUTH2;
	ctrl->cmd_buf[2] = CMD_SOURCE_DEVICE;
	ctrl->cmd_buf[3] = channel;
	ctrl->cmd_buf[4] = type;
	*((uint32_t*) &ctrl->cmd_buf[5]) = value;

	if (channel == CMD_CHANNEL_USB)
	{
//		CDC_Transmit_FS(ctrl->cmd_buf, 9);
	}
	else if (channel == CMD_CHANNEL_BT)
	{
//		HAL_UART_Transmit_DMA(rnbd451->huart, ctrl->cmd_buf, 9);
	}
}

void control_process_command(control_t* ctrl, rtc_t* rtc, dcf77_t* dcf77, TMP126_t* tmp126, rnbd451_t* rnbd451)
{
	if (!ctrl->command_available)
	{
		return;
	}
	switch (ctrl->cmd_buf[4])
	{
	    case CMD_RESET:
	        NVIC_SystemReset();
	        break;
	    case CMD_INCREASE_SECOND:
	        break;
	    case CMD_DECREASE_SECOND:
	        break;
	    case CMD_SET_SECOND:
	        break;
	    case CMD_INCREASE_MINUTE:
	        break;
	    case CMD_DECREASE_MINUTE:
	        break;
	    case CMD_SET_MINUTE:
	        break;
	    case CMD_INCREASE_HOUR:
	        break;
	    case CMD_DECREASE_HOUR:
	        break;
	    case CMD_SET_HOUR:
	        break;
	    case CMD_SET_TIME:
	        break;
	    case CMD_ENABLE_ALARM:
	        break;
	    case CMD_DISABLE_ALARM:
	        break;
	    case CMD_SET_ALARM:
	        break;
	    case CMD_SYNC_RADIO_NOW:
	        break;
	    case CMD_SET_RADIO_SYNC_TIME:
	        break;
	    case CMD_SYNC_NETWORK_NOW:
	        break;
	    case CMD_READ_RTC:
	        break;
	    case CMD_WRITE_RTC:
	        break;
	    default:
	        break;
	}
	ctrl->command_available = 0;
}
