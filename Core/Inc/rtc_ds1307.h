/*
 * Author: Feng-Shou Hsieh
 * Institution: Technische Universitaet Berlin
 */

#ifndef INC_RTC_DS1307_H_
#define INC_RTC_DS1307_H_

#define DS1307_ADDRESS 0xD0

#include "tmp126.h"
#include "rnbd451.h"

typedef struct
{
	I2C_HandleTypeDef* hi2c;
	DMA_HandleTypeDef* hdma_i2c_rx;
	DMA_HandleTypeDef* hdma_i2c_tx;
	TIM_HandleTypeDef* htim;
	TIM_TypeDef* tim;

	uint8_t get_time_buf[7];
	uint8_t set_time_buf[7];

	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
} rtc_t;

void rtc_init(rtc_t* rtc, TIM_HandleTypeDef* htim, I2C_HandleTypeDef* hi2c, DMA_HandleTypeDef* hdma_i2c_rx, DMA_HandleTypeDef* hdma_i2c_tx);
void rtc_reset(rtc_t *rtc);
void rtc_setTime(rtc_t *rtc, uint8_t sec, uint8_t min, uint8_t hr, uint8_t day, uint8_t date, uint8_t month, uint8_t year);
void rtc_getTime(rtc_t *rtc);
void rtc_I2C_Mem_Read_callback(rtc_t *rtc, TMP126_t *tmp126, rnbd451_t *rnbd451);
void rtc_stopOsc(rtc_t *rtc);
uint8_t bcd2dec(uint8_t val);
uint8_t dec2bcd(uint8_t val);

#endif /* INC_RTC_DS1307_H_ */
