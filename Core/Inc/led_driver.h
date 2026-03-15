/*
 * led_driver.h
 *
 *  Created on: 08.02.2026
 *      Author: Anton
 */

#ifndef INC_LED_DRIVER_H_
#define INC_LED_DRIVER_H_

#include "main.h"
#include <stdio.h>
#include <string.h>
#include "tmp126.h"
#include "rtc_ds1307.h"

// HOURS Anzeige Adressen
/* Werte für das 6te-Shift-Register (led_buf[5]) */
#define HOURS_TENS_ZERO		0b00100000
#define HOURS_TENS_ONE		0b00010000
#define HOURS_TENS_TWO		0b00001000
/* Werte für das 7te-Shift-Register (led_buf[6]) */
#define HOURS_TENS_THREE	0b10000000
#define HOURS_TENS_FOUR		-1 // DOES NOT EXIST!!! DO NOT USE!!!
#define HOURS_TENS_FIVE		0b00010000
#define HOURS_TENS_SIX		0b00001000
#define HOURS_TENS_SEVEN	0b00000100
#define HOURS_TENS_EIGHT	0b00000010
#define HOURS_TENS_NINE		0b00000001

/* Werte für das 5te-Shift-Register (led_buf[4]) */
#define HOURS_ONES_ZERO		0b10000000
#define HOURS_ONES_ONE		0b01000000
#define HOURS_ONES_TWO		0b00100000
#define HOURS_ONES_THREE	0b00010000
#define HOURS_ONES_FOUR		0b00001000
#define HOURS_ONES_FIVE		0b00000100
#define HOURS_ONES_SIX		0b00000010
#define HOURS_ONES_SEVEN	0b00000001
/* Werte für das 6te-Shift-Register (led_buf[5]) */
#define HOURS_ONES_EIGHT	0b10000000
#define HOURS_ONES_NINE		0b01000000



// MINS Anzeige Adressen
/* Werte für das 4te-Shift-Register (led_buf[3]) */
#define MINS_TENS_ZERO		0b00100000
#define MINS_TENS_ONE		0b00010000
#define MINS_TENS_TWO		0b00001000
#define MINS_TENS_THREE		0b00000100
#define MINS_TENS_FOUR		0b00000010
#define MINS_TENS_FIVE		0b00000001
#define MINS_TENS_SIX		-1 // DOES NOT EXIST!!! DO NOT USE!!!
#define MINS_TENS_SEVEN		-1 // DOES NOT EXIST!!! DO NOT USE!!!
/* Werte für das 7te-Shift-Register (led_buf[6]) */
#define MINS_TENS_EIGHT		0b01000000
#define MINS_TENS_NINE		0b00100000

/* Werte für das 3te-Shift-Register (led_buf[2]) */
#define MINS_ONES_ZERO		0b10000000
#define MINS_ONES_ONE		0b01000000
#define MINS_ONES_TWO		0b00100000
#define MINS_ONES_THREE		0b00010000
#define MINS_ONES_FOUR		0b00001000
#define MINS_ONES_FIVE		0b00000100
#define MINS_ONES_SIX		0b00000010
#define MINS_ONES_SEVEN		0b00000001
/* Werte für das 4te-Shift-Register (led_buf[3]) */
#define MINS_ONES_EIGHT		0b10000000
#define MINS_ONES_NINE		0b01000000



// SECS Anzeige Adressen
/* Werte für das 2te-Shift-Register (led_buf[1]) */
#define SECS_TENS_ZERO		0b00100000
#define SECS_TENS_ONE		0b00010000
#define SECS_TENS_TWO		0b00001000
#define SECS_TENS_THREE		0b00000100
#define SECS_TENS_FOUR		0b00000010
#define SECS_TENS_FIVE		0b00000001
/* Werte für das 8te-Shift-Register (led_buf[7]) */
#define SECS_TENS_SIX		0b01000000
#define SECS_TENS_SEVEN		-1 // DOES NOT EXIST!!! DO NOT USE!!!
#define SECS_TENS_EIGHT		-1 // DOES NOT EXIST!!! DO NOT USE!!!
#define SECS_TENS_NINE		-1 // DOES NOT EXIST!!! DO NOT USE!!!

/* Werte für das 1te-Shift-Register (led_buf[0]) */
#define SECS_ONES_ZERO		0b10000000
#define SECS_ONES_ONE		0b01000000
#define SECS_ONES_TWO		0b00100000
#define SECS_ONES_THREE		0b00010000
#define SECS_ONES_FOUR		0b00001000
#define SECS_ONES_FIVE		0b00000100
#define SECS_ONES_SIX		0b00000010
#define SECS_ONES_SEVEN		0b00000001
/* Werte für das 2te-Shift-Register (led_buf[1]) */
#define SECS_ONES_EIGHT		0b10000000
#define SECS_ONES_NINE		0b01000000


typedef struct
{
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;

	uint8_t led_buf[8];
} led_driver_t;

void led_init(led_driver_t *led_driver, SPI_HandleTypeDef* hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void led_update(led_driver_t *led_driver);

void led_error(led_driver_t *led_driver);

void led_set_hours(led_driver_t *led_driver, uint8_t hours);

void led_set_mins(led_driver_t *led_driver, uint8_t mins);

void led_set_secs(led_driver_t *led_driver, uint8_t secs);

void led_hours_off(led_driver_t* led_driver);

void led_mins_off(led_driver_t* led_driver);

void led_secs_off(led_driver_t* led_driver);

void led_sync_rtc(led_driver_t* led_driver, rtc_t* rtc);

void led_set_temperture(led_driver_t* led_driver, TMP126_t *tmp126);

#endif /* INC_LED_DRIVER_H_ */
