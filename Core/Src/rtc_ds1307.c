/*
 * Author: Feng-Shou Hsieh
 * Institution: Technische Universitaet Berlin
 */

#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "rtc_ds1307.h"
#include "tmp126.h"
#include "rnbd451.h"

extern UART_HandleTypeDef huart3;

void rtc_init(rtc_t* rtc, TIM_HandleTypeDef* htim, I2C_HandleTypeDef* hi2c, DMA_HandleTypeDef* hdma_i2c_rx, DMA_HandleTypeDef* hdma_i2c_tx)
{
	uint8_t dummy = 0b1000000;

	rtc->htim = htim;
	rtc->tim = htim->Instance;
	rtc->hi2c = hi2c;
	rtc->hdma_i2c_rx = hdma_i2c_rx;
	rtc->hdma_i2c_tx = hdma_i2c_tx;
	rtc_reset(rtc);
//	rtc_setTime(rtc, 30, 50, 12, 0, 0, 0, 0);
	HAL_I2C_Mem_Write(rtc->hi2c, DS1307_ADDRESS, 0x07, I2C_MEMADD_SIZE_8BIT, &dummy, 1, 100);
	HAL_TIM_Base_Start_IT(htim);
	htim->Instance->CNT = 0;
}
void rtc_reset(rtc_t *rtc)
{
    rtc->seconds = 0;
    rtc->minutes = 0;
    rtc->hours = 0;
    rtc->day = 0;
    rtc->date = 0;
    rtc->month = 0;
    rtc->year = 0;

    memset(rtc->get_time_buf, 0, sizeof(rtc->get_time_buf));
    memset(rtc->set_time_buf, 0, sizeof(rtc->set_time_buf));

    rtc->tim->CNT = 0;
}

void rtc_setTime(rtc_t *rtc, uint8_t sec, uint8_t min, uint8_t hr, uint8_t day, uint8_t date, uint8_t month, uint8_t year)
{
	rtc->set_time_buf[0] = dec2bcd(sec);
	rtc->set_time_buf[1] = dec2bcd(min);
	rtc->set_time_buf[2] = dec2bcd(hr);
	rtc->set_time_buf[3] = dec2bcd(day);
	rtc->set_time_buf[4] = dec2bcd(date);
	rtc->set_time_buf[5] = dec2bcd(month);
	rtc->set_time_buf[6] = dec2bcd(year);
	HAL_I2C_Mem_Write(rtc->hi2c, DS1307_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, rtc->set_time_buf, 7, 100);
}

void rtc_getTime(rtc_t *rtc)
{
	HAL_I2C_Mem_Read_DMA(rtc->hi2c, DS1307_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, rtc->get_time_buf, 7);
}

void rtc_I2C_Mem_Read_callback(rtc_t *rtc, TMP126_t *tmp126, rnbd451_t *rnbd451)
{
	if (bcd2dec(rtc->get_time_buf[0]) % 80 != rtc->seconds)
	{
		if (((bcd2dec(rtc->get_time_buf[0]) % 80) == rtc->seconds + 1) || ((rtc->seconds == 59) && ((bcd2dec(rtc->get_time_buf[0]) % 80) == 0)))
		{
			HAL_GPIO_TogglePin(STM_LD1_GPIO_Port, STM_LD1_Pin);
		}
		rtc->seconds = bcd2dec(rtc->get_time_buf[0]) % 80;
		rtc->minutes = bcd2dec(rtc->get_time_buf[1]);
		rtc->hours = bcd2dec(rtc->get_time_buf[2]);
		rtc->day = bcd2dec(rtc->get_time_buf[3]);
		rtc->date = bcd2dec(rtc->get_time_buf[4]);
		rtc->month = bcd2dec(rtc->get_time_buf[5]);
		rtc->year = bcd2dec(rtc->get_time_buf[6]);
		snprintf(rnbd451->tx_buf, 128, "%02u:%02u:%02u, %02u C\r", rtc->hours, rtc->minutes, rtc->seconds, (uint8_t) tmp126->Temperature);
		HAL_UART_Transmit(rnbd451->huart, (uint8_t*) rnbd451->tx_buf, 15, HAL_MAX_DELAY);
		/*
		clearDigits();
		writeToDigit(0, rtc.seconds % 10);
		writeToDigit(1, rtc.seconds / 10);
		writeToDigit(2, rtc.minutes % 10);
		writeToDigit(3, rtc.minutes / 10);
		writeToDigit(4, rtc.hours % 10);
		writeToDigit(5, rtc.hours / 10);
		LED_Refresh();
		if ((rtc.seconds == ctrl.dcf77_sync_seconds) && (rtc.minutes == ctrl.dcf77_sync_minutes) && (rtc.hours == ctrl.dcf77_sync_hours))
		{
			ctrl.initiate_dcf77 = 1;
		}
		if ((rtc.seconds == ctrl.alarm_seconds) && (rtc.minutes == ctrl.alarm_minutes) && (rtc.hours == ctrl.alarm_hours))
		{
			ctrl.initiate_alarm = 1;
		}
		*/
	}
}

void rtc_stopOsc(rtc_t *rtc)
{
	uint8_t sec = 128 + dec2bcd(rtc->seconds);
	HAL_I2C_Mem_Write(rtc->hi2c, DS1307_ADDRESS, 0x00, I2C_MEMADD_SIZE_8BIT, &sec, 1, 100);
}

uint8_t bcd2dec(uint8_t val)
{
    return ((val >> 4) * 10) + (val % 16);
}

uint8_t dec2bcd(uint8_t val)
{
    return ((val / 10) << 4) + (val % 10);
}
