/*
 * led_driver.c
 *
 *  Created on: 08.02.2026
 *      Author: Anton
 */

#include "led_driver.h"

void led_init(led_driver_t *led_driver, SPI_HandleTypeDef* hspi, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	led_driver->hspi = hspi;
	led_driver->GPIOx = GPIOx;
	led_driver->GPIO_Pin = GPIO_Pin;

	memset(led_driver->led_buf, 0, 8);
	/* RGB-Colour initialisieren
	   RGB-Blau: 	0b10111001
	   RGB-Grün: 	0b10110101
	   RGB-Rot:  	0b10110011
	*/
	led_driver->led_buf[7] = 0b10110101;

	led_driver->led_buf[5] = HOURS_TENS_ZERO;
	led_driver->led_buf[4] = HOURS_ONES_ZERO;
	led_driver->led_buf[3] = MINS_TENS_ZERO;
	led_driver->led_buf[2] = MINS_ONES_ZERO;
	led_driver->led_buf[1] = SECS_TENS_ZERO;
	led_driver->led_buf[0] = SECS_ONES_ZERO;

	// Nützlich beim LED Löten
	led_hours_off(led_driver);
	led_mins_off(led_driver);
	led_secs_off(led_driver);
	led_driver->led_buf[6] = 0b11111111;
	led_driver->led_buf[5] = 0b11111111;
	led_driver->led_buf[4] = 0b11111111;
	led_driver->led_buf[3] = 0b11111111;
	led_driver->led_buf[2] = 0b11111111;
	led_driver->led_buf[1] = 0b11111111;
	led_driver->led_buf[0] = 0b11111111;

	led_update(led_driver);
}

/* Sende Initialiesierung an das LED-Board */
void led_update(led_driver_t *led_driver) {
	HAL_GPIO_WritePin(led_driver->GPIOx, led_driver->GPIO_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(led_driver->hspi, led_driver->led_buf, 8, 100);
	HAL_GPIO_WritePin(led_driver->GPIOx, led_driver->GPIO_Pin, GPIO_PIN_SET);
}

/* LEDs auf ROT gesetzt im Fall eines Fehlers */
void led_error(led_driver_t *led_driver) {
	led_driver->led_buf[7] = 0x40;
	printf("LED ERROR!\n");
}

/* @brief Registerwert für HOURS setzen */
void led_set_hours(led_driver_t* led_driver, uint8_t hours){
	uint8_t hours_tens = 0;
	uint8_t hours_ones = 0;

	hours_tens = hours / 10;
	hours_ones = hours % 10;

	led_hours_off(led_driver);

	switch (hours_tens) {
		case 0:
			led_driver->led_buf[5] = led_driver->led_buf[5] | HOURS_TENS_ZERO;
			break;
		case 1:
			led_driver->led_buf[5] = led_driver->led_buf[5] | HOURS_TENS_ONE;
			break;
		case 2:
			led_driver->led_buf[5] = led_driver->led_buf[5] | HOURS_TENS_TWO;
			break;
		case 3:
			led_driver->led_buf[6] = led_driver->led_buf[6] | HOURS_TENS_THREE;
			break;
		case 4:
			led_driver->led_buf[6] = led_driver->led_buf[6] | HOURS_TENS_FIVE;
			break;
		case 5:
			led_driver->led_buf[6] = led_driver->led_buf[6] | HOURS_TENS_SIX;
			break;
		case 6:
			led_driver->led_buf[6] = led_driver->led_buf[6] | HOURS_TENS_SEVEN;
			break;
		case 7:
			led_driver->led_buf[6] = led_driver->led_buf[6] | HOURS_TENS_EIGHT;
			break;
		case 8:
			led_driver->led_buf[6] = led_driver->led_buf[6] | HOURS_TENS_NINE;
			break;
		case 9:
//			led_driver->led_buf[6] = led_driver->led_buf[6] | HOURS_TENS_FOUR;
			led_error(led_driver);
			printf("LED HOURS_TENS_4 is not connected...\n");
			break;
		default:
			led_error(led_driver);
	}

	switch (hours_ones) {
		case 0:
			led_driver->led_buf[4] = led_driver->led_buf[4] | HOURS_ONES_ZERO;
			break;
		case 1:
			led_driver->led_buf[4] = led_driver->led_buf[4] | HOURS_ONES_ONE;
			break;
		case 2:
			led_driver->led_buf[4] = led_driver->led_buf[4] | HOURS_ONES_TWO;
			break;
		case 3:
			led_driver->led_buf[4] = led_driver->led_buf[4] | HOURS_ONES_THREE;
			break;
		case 4:
			led_driver->led_buf[4] = led_driver->led_buf[4] | HOURS_ONES_FOUR;
			break;
		case 5:
			led_driver->led_buf[4] = led_driver->led_buf[4] | HOURS_ONES_FIVE;
			break;
		case 6:
			led_driver->led_buf[4] = led_driver->led_buf[4] | HOURS_ONES_SIX;
			break;
		case 7:
			led_driver->led_buf[4] = led_driver->led_buf[4] | HOURS_ONES_SEVEN;
			break;
		case 8:
			led_driver->led_buf[5] = led_driver->led_buf[5] | HOURS_ONES_EIGHT;
			break;
		case 9:
			led_driver->led_buf[5] = led_driver->led_buf[5] | HOURS_ONES_NINE;
			break;
		default:
			led_error(led_driver);
	}
	led_update(led_driver);
}

/* @brief Registerwert für MINS setzen */
void led_set_mins(led_driver_t* led_driver, uint8_t mins){
	uint8_t mins_tens = 0;
	uint8_t mins_ones = 0;

	mins_tens = mins / 10;
	mins_ones = mins % 10;

	led_mins_off(led_driver);

	switch (mins_tens) {
		case 0:
			led_driver->led_buf[3] = led_driver->led_buf[3] | MINS_TENS_ZERO;
			break;
		case 1:
			led_driver->led_buf[3] = led_driver->led_buf[3] | MINS_TENS_ONE;
			break;
		case 2:
			led_driver->led_buf[3] = led_driver->led_buf[3] | MINS_TENS_TWO;
			break;
		case 3:
			led_driver->led_buf[3] = led_driver->led_buf[3] | MINS_TENS_THREE;
			break;
		case 4:
			led_driver->led_buf[3] = led_driver->led_buf[3] | MINS_TENS_FOUR;
			break;
		case 5:
			led_driver->led_buf[3] = led_driver->led_buf[3] | MINS_TENS_FIVE;
			break;
		case 6:
//			led_driver->led_buf[6] = led_driver->led_buf[6] | MINS_TENS_SIX;
			led_error(led_driver);
			printf("LED MINS_TENS_6 is not connected...\n");
			break;
		case 7:
//			led_driver->led_buf[6] = led_driver->led_buf[6] | MINS_TENS_SEVEN;
			led_error(led_driver);
			printf("LED MINS_TENS_7 is not connected...\n");
			break;
		case 8:
			led_driver->led_buf[6] = led_driver->led_buf[6] | MINS_TENS_EIGHT;
			break;
		case 9:
			led_driver->led_buf[6] = led_driver->led_buf[6] |MINS_TENS_NINE;
			break;
		default:
			led_error(led_driver);
	}

	switch (mins_ones) {
		case 0:
			led_driver->led_buf[2] = led_driver->led_buf[2] | MINS_ONES_ZERO;
			break;
		case 1:
			led_driver->led_buf[2] = led_driver->led_buf[2] | MINS_ONES_ONE;
			break;
		case 2:
			led_driver->led_buf[2] = led_driver->led_buf[2] | MINS_ONES_TWO;
			break;
		case 3:
			led_driver->led_buf[2] = led_driver->led_buf[2] | MINS_ONES_THREE;
			break;
		case 4:
			led_driver->led_buf[2] = led_driver->led_buf[2] | MINS_ONES_FOUR;
			break;
		case 5:
			led_driver->led_buf[2] = led_driver->led_buf[2] | MINS_ONES_FIVE;
			break;
		case 6:
			led_driver->led_buf[2] = led_driver->led_buf[2] | MINS_ONES_SIX;
			break;
		case 7:
			led_driver->led_buf[2] = led_driver->led_buf[2] | MINS_ONES_SEVEN;
			break;
		case 8:
			led_driver->led_buf[3] = led_driver->led_buf[3] | MINS_ONES_EIGHT;
			break;
		case 9:
			led_driver->led_buf[3] = led_driver->led_buf[3] | MINS_ONES_NINE;
			break;
		default:
			led_error(led_driver);
	}
	led_update(led_driver);
}

/* @brief Registerwert für SECS setzen */
void led_set_secs(led_driver_t* led_driver, uint8_t secs){
	uint8_t secs_tens = 0;
	uint8_t secs_ones = 0;

	secs_tens = secs / 10;
	secs_ones = secs % 10;

	led_secs_off(led_driver);

	printf("%u %u\n", secs_tens, secs_ones);

	switch (secs_tens) {
		case 0:
			led_driver->led_buf[1] = led_driver->led_buf[1] | SECS_TENS_ZERO;
			break;
		case 1:
			led_driver->led_buf[1] = led_driver->led_buf[1] | SECS_TENS_ONE;
			break;
		case 2:
			led_driver->led_buf[1] = led_driver->led_buf[1] | SECS_TENS_TWO;
			break;
		case 3:
			led_driver->led_buf[1] = led_driver->led_buf[1] | SECS_TENS_THREE;
			break;
		case 4:
			led_driver->led_buf[1] = led_driver->led_buf[1] | SECS_TENS_FOUR;
			break;
		case 5:
			led_driver->led_buf[1] = led_driver->led_buf[1] | SECS_TENS_FIVE;
			break;
		case 6:
			led_driver->led_buf[7] = led_driver->led_buf[7] | SECS_TENS_SIX;
			break;
		case 7:
			// led_driver->led_buf[6] |= SECS_TENS_SEVEN;
			led_error(led_driver);
			printf("LED SECS_TENS_7 is not connected...\n");
			break;
		case 8:
			// led_driver->led_buf[6] |= SECS_TENS_EIGHT;
			led_error(led_driver);
			printf("LED SECS_TENS_8 is not connected...\n");
			break;
		case 9:
			// led_driver->led_buf[6] |= SECS_TENS_NINE;
			led_error(led_driver);
			printf("LED SECS_TENS_9 is not connected...\n");
			break;
		default:
			led_error(led_driver);
	}

	switch (secs_ones) {
		case 0:
			led_driver->led_buf[0] = led_driver->led_buf[0] | SECS_ONES_ZERO;
			break;
		case 1:
			led_driver->led_buf[0] = led_driver->led_buf[0] | SECS_ONES_ONE;
			break;
		case 2:
			led_driver->led_buf[0] = led_driver->led_buf[0] | SECS_ONES_TWO;
			break;
		case 3:
			led_driver->led_buf[0] = led_driver->led_buf[0] | SECS_ONES_THREE;
			break;
		case 4:
			led_driver->led_buf[0] = led_driver->led_buf[0] | SECS_ONES_FOUR;
			break;
		case 5:
			led_driver->led_buf[0] = led_driver->led_buf[0] | SECS_ONES_FIVE;
			break;
		case 6:
			led_driver->led_buf[0] = led_driver->led_buf[0] | SECS_ONES_SIX;
			break;
		case 7:
			led_driver->led_buf[0] = led_driver->led_buf[0] | SECS_ONES_SEVEN;
			break;
		case 8:
			led_driver->led_buf[1] = led_driver->led_buf[1] | SECS_ONES_EIGHT;
			break;
		case 9:
			led_driver->led_buf[1] = led_driver->led_buf[1] | SECS_ONES_NINE;
			break;
		default:
			led_error(led_driver);
	}
	led_update(led_driver);
}

void led_hours_off(led_driver_t* led_driver) {
	led_driver->led_buf[4] &= 0b00000000;
	led_driver->led_buf[5] &= 0b00000111;
	led_driver->led_buf[6] &= 0b01100000;
}

void led_mins_off(led_driver_t* led_driver) {
	led_driver->led_buf[2] &= 0b00000000;
	led_driver->led_buf[3] &= 0b00000000;
	led_driver->led_buf[6] &= 0b10011111;
}

void led_secs_off(led_driver_t* led_driver) {
	led_driver->led_buf[0] &= 0b00000000;
	led_driver->led_buf[1] &= 0b00000000;
	led_driver->led_buf[7] &= 0b10111111;
}

void led_sync_rtc(led_driver_t* led_driver, rtc_t* rtc){
	led_set_secs(led_driver, rtc->seconds);
	led_set_mins(led_driver, rtc->minutes);
	led_set_hours(led_driver, rtc->hours);
}

void led_set_temperture(led_driver_t* led_driver, TMP126_t *tmp126){
	uint8_t temp = 0;

	temp = (int8_t) tmp126->Temperature;

	if(temp > 0 && temp < 90){
		led_set_secs(led_driver, 0);
		led_set_mins(led_driver, 90); // Debug LED für °C-Scheibe
		led_set_hours(led_driver, temp);
	} else {
		led_error(led_driver);
		printf("Temperature out of range!\n");
	}
}

