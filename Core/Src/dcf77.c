/*
 * Author: Florian Simon
 * Institution: Technische Universitaet Berlin
 */

#include "main.h"
#include <stdio.h>
#include <string.h>
#include "dcf77.h"

extern uint8_t state;

void dcf77_init(dcf77_t* dcf77, TIM_HandleTypeDef* htim, GPIO_TypeDef* dcf_out_port, uint16_t dcf_out_pin, GPIO_TypeDef* dcf_sync_g_port, uint16_t dcf_sync_g_pin, GPIO_TypeDef* dcf_sync_y_port, uint16_t dcf_sync_y_pin, GPIO_TypeDef* dcf_sync_r_port, uint16_t dcf_sync_r_pin, GPIO_TypeDef* debug_port, uint16_t debug_pin, IRQn_Type exti_irqn, IRQn_Type tim_irqn)
{
	HAL_NVIC_DisableIRQ(exti_irqn);
	HAL_NVIC_DisableIRQ(tim_irqn);
	HAL_TIM_Base_Stop_IT(htim);
	htim->Instance->CNT = 0;
	dcf_sync_g_port->BSRR = dcf_sync_g_pin << 16;
	dcf_sync_y_port->BSRR = dcf_sync_y_pin << 16;
	dcf_sync_r_port->BSRR = dcf_sync_r_pin;
	dcf77->htim = htim;
	dcf77->tim = htim->Instance;
	dcf77->dcf_out_port = dcf_out_port;
	dcf77->dcf_out_pin = dcf_out_pin;
	dcf77->dcf_sync_g_port = dcf_sync_g_port;
	dcf77->dcf_sync_g_pin = dcf_sync_g_pin;
	dcf77->dcf_sync_y_port = dcf_sync_y_port;
	dcf77->dcf_sync_y_pin = dcf_sync_y_pin;
	dcf77->dcf_sync_r_port = dcf_sync_r_port;
	dcf77->dcf_sync_r_pin = dcf_sync_r_pin;
	dcf77->debug_port = debug_port;
	dcf77->debug_pin = debug_pin;
	dcf77->exti_irqn = exti_irqn;
	dcf77->tim_irqn = tim_irqn;
	memset(dcf77->msg, 255, 60);
	dcf77->msg_idx = 0;
	dcf77->search_active = 0;
	dcf77->bit_detected = 0;
	dcf77->end_of_msg = 0;
	dcf77->sync_status = 0;
	dcf77->error_cnt = 0;
	dcf77->cycle_duration = 0;
	dcf77->cycle_cnt = 0;
	dcf77->cycle_marker = 0;
	dcf77->seconds = 255;
	dcf77->minutes = 255;
	dcf77->hours = 255;
}

void dcf77_reset(dcf77_t* dcf77)
{
	HAL_NVIC_DisableIRQ(dcf77->exti_irqn);
	HAL_NVIC_DisableIRQ(dcf77->tim_irqn);
	HAL_TIM_Base_Stop_IT(dcf77->htim);
	dcf77->tim->CNT = 0;
	memset(dcf77->msg, 255, 60);
	dcf77->msg_idx = 0;
	dcf77->search_active = 0;
	dcf77->bit_detected = 0;
	dcf77->end_of_msg = 0;
	dcf77->error_cnt = 0;
	dcf77->cycle_duration = 0;
	dcf77->cycle_cnt = 0;
	dcf77->cycle_marker = 0;
	dcf77->seconds = 255;
	dcf77->minutes = 255;
	dcf77->hours = 255;
}

void dcf77_start(dcf77_t* dcf77)
{
	dcf77_stop(dcf77);
	dcf77_reset(dcf77);
	dcf77->dcf_sync_y_port->BSRR = dcf77->dcf_sync_y_pin;
	dcf77->search_active = 1;
	HAL_TIM_Base_Start_IT(dcf77->htim);
	dcf77->tim->CNT = 0;
	HAL_NVIC_EnableIRQ(dcf77->tim_irqn);
	HAL_NVIC_EnableIRQ(dcf77->exti_irqn);
}

void dcf77_EXTI_callback(dcf77_t* dcf77)
{
	if (dcf77->search_active)
	{
		dcf77_search(dcf77);
	}
	else
	{
		dcf77_read(dcf77);
	}
	if (dcf77->end_of_msg)
	{
		dcf77_stop(dcf77);
		dcf77_decode(dcf77);
		dcf77_synchronize(dcf77);
	}
}

void dcf77_search(dcf77_t* dcf77)
{
	// Rising edge
	if (dcf77->dcf_out_port->IDR & dcf77->dcf_out_pin)
	{
		// If at least 1.9 s since last rising edge: Search successful
		if ((dcf77->tim->CNT - dcf77->cycle_marker) > 1900000)
		{
			dcf77->debug_port->BSRR = dcf77->debug_pin; // DEBUG
			dcf77->cycle_marker = dcf77->tim->CNT;
			dcf77->search_active = 0;
			dcf77->cycle_duration = 0;
			dcf77_read(dcf77);
			return;
		}
		else
		{
			dcf77->cycle_marker = dcf77->tim->CNT;
		}
	}
}

void dcf77_read(dcf77_t* dcf77)
{
	// Rising edge
	if (dcf77->dcf_out_port->IDR & dcf77->dcf_out_pin)
	{
		// If at least 0.925 s since last valid rising edge...
		if ((dcf77->tim->CNT - dcf77->cycle_marker) > 925000)
		{
			// New valid rising edge
			dcf77->cycle_marker = dcf77->tim->CNT;
			// Toggle yellow LED
			dcf77->dcf_sync_y_port->ODR ^= dcf77->dcf_sync_y_pin;
			// If no valid bit detected during previous second...
			if (dcf77->bit_detected == 0)
			{
				dcf77->msg[dcf77->msg_idx] = 255;
			}
			// Reset bit_detected flag for new second
			dcf77->bit_detected = 0;
			// Increase msg index, f msg reached the end...
			if (++dcf77->msg_idx > 58)
			{
				dcf77->end_of_msg = 1;
				return;
			}
			dcf77->debug_port->BSRR = dcf77->debug_pin; // DEBUG
		}
		else
		{
			dcf77->error_cnt++;
		}
	}
	// Falling edge
	else
	{
		// If valid 0 detected...
		if (((dcf77->tim->CNT - dcf77->cycle_marker) > 50000) && ((dcf77->tim->CNT - dcf77->cycle_marker) < 150000) && (dcf77->bit_detected != 1)) // bit_detected condition prevents multiple 0 detected executions
		{
			dcf77->debug_port->BSRR = dcf77->debug_pin << 16; // DEBUG
			dcf77->msg[dcf77->msg_idx] = 0;
			dcf77->bit_detected = 1;
		}
		// Else if valid 1 detected (previous 0 is overwritten)...
		else if (((dcf77->tim->CNT - dcf77->cycle_marker) >= 150000) && ((dcf77->tim->CNT - dcf77->cycle_marker) < 250000) && (dcf77->bit_detected != 2))
		{
			dcf77->debug_port->BSRR = dcf77->debug_pin << 16; // DEBUG
			dcf77->msg[dcf77->msg_idx] = 1;
			dcf77->bit_detected = 2;
		}
		// If invalid pulse duration (measured w.r.t. last valid rising edge)...
		else
		{
			dcf77->error_cnt++;
		}
	}
}

void dcf77_stop(dcf77_t* dcf77)
{
	HAL_NVIC_DisableIRQ(dcf77->exti_irqn);
	HAL_NVIC_DisableIRQ(dcf77->tim_irqn);
	HAL_TIM_Base_Stop_IT(dcf77->htim);
	dcf77->tim->CNT = 0;
	dcf77->debug_port->BSRR = dcf77->debug_pin << 16; // DEBUG
	dcf77->dcf_sync_y_port->BSRR = dcf77->dcf_sync_y_pin << 16;
}

void dcf77_decode(dcf77_t* dcf77)
{
 	uint8_t error_flag_minutes = 0;
 	uint8_t error_flag_hours = 0;
 	uint8_t minutes_parity = 0;
 	uint8_t hours_parity = 0;
 	uint8_t enable_valid = 1;
 	uint8_t enable_parity = 1;
 	uint8_t minutes_overflow = 0;
 	// Seconds
 	dcf77->seconds = 1;
 	// Minutes, hours
 	for (uint8_t i = 0; i < 4; i++)
 	{
 		if (dcf77->msg[21 + i] == 255) error_flag_minutes = 1;
		dcf77->minutes += dcf77->msg[21 + i] * (1 << i);
		minutes_parity += dcf77->msg[21 + i];
		if (dcf77->msg[29 + i] == 255) error_flag_hours = 1;
		dcf77->hours += dcf77->msg[29 + i] * (1 << i);
		hours_parity += dcf77->msg[29 + i];
 	}
 	for (uint8_t i = 0; i < 3; i++)
 	{
 		if (dcf77->msg[25 + i] == 255) error_flag_minutes = 1;
		dcf77->minutes += dcf77->msg[25 + i] * (1 << i) * 10;
		minutes_parity += dcf77->msg[25 + i];
	}
	for (uint8_t i = 0; i < 2; i++)
	{
		if (dcf77->msg[33 + i] == 255) error_flag_hours = 1;
		dcf77->hours += dcf77->msg[33 + i] * (1 << i) * 10;
		hours_parity += dcf77->msg[33 + i];
	}
	// Valid value check
	if (enable_valid)
	{
		if (dcf77->minutes > 59) dcf77->minutes = 255;
		if (dcf77->hours > 23) dcf77->hours = 255;
	}
	// Parity check
	if (enable_parity)
	{
		if ((minutes_parity % 2) != dcf77->msg[28]) error_flag_minutes = 1;
		if ((hours_parity % 2) != dcf77->msg[35]) error_flag_hours = 1;
		if (error_flag_minutes) dcf77->minutes = 255;
		if (error_flag_hours) dcf77->hours = 255;
	}
	// For some reason time reads are offset by -1
	if (!error_flag_minutes && (dcf77->minutes < 60))
	{
	    dcf77->minutes++;
	    if (dcf77->minutes == 60)
	    {
	        dcf77->minutes = 0;
	        minutes_overflow = 1;
	    }
	}
	if (!error_flag_hours && (dcf77->hours < 24))
	{
	    dcf77->hours++;
	    if (dcf77->hours == 24) dcf77->hours = 0;
	    if (minutes_overflow)
	    {
	        dcf77->hours++;
	        if (dcf77->hours == 24) dcf77->hours = 0;
	    }
	}
}

void dcf77_synchronize(dcf77_t* dcf77)
{
	// TO-DO: COMPARE TIMES, SYNCHRONIZE, SET/RESET LEDS
	// If successful...
	if ((dcf77->minutes > 59) || (dcf77->hours > 23))
	{
		dcf77->dcf_sync_g_port->BSRR = dcf77->dcf_sync_g_pin << 16;
		dcf77->dcf_sync_r_port->BSRR = dcf77->dcf_sync_r_pin;
		dcf77->sync_status = 0;
	}
	else
	{
		dcf77->dcf_sync_g_port->BSRR = dcf77->dcf_sync_g_pin;
		dcf77->dcf_sync_r_port->BSRR = dcf77->dcf_sync_r_pin << 16;
		dcf77->sync_status = 1;
	}
}

void dcf77_timeout(dcf77_t* dcf77)
{
	dcf77_stop(dcf77);
	dcf77_reset(dcf77);
	dcf77->dcf_sync_g_port->BSRR = dcf77->dcf_sync_g_pin << 16;
	dcf77->dcf_sync_r_port->BSRR = dcf77->dcf_sync_r_pin;
	dcf77->sync_status = 0;
}
