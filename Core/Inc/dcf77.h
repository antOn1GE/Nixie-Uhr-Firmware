/*
 * Author: Florian Simon
 * Institution: Technische Universitaet Berlin
 */

#ifndef INC_DCF77_H_
#define INC_DCF77_H_

/* How to use:
 * Configure GPIO "DCF_OUT" as external interrupt with falling and rising edge trigger, enable interrupt, disable enable-generation in init function
 * Configure 32-bit timer as Output Compare no Output with 1 us tick, set counter period to (x * 65 s + 65 s) (x = number of search re-tries), enable trigger event: update event and enable interrupt, disable enable-generation in init function
 * Enable HAL drivers
 *
 * Define global dcf77_t instance
 * In main, call dcf77_init()
 * In EXTI callback, call: dcf_search(), if search_active_flag, else dcf_read(), then dcf_stop() and dcf_decode(), if end_of_message_flag (do not use else statements)
 * In TIM callback, call: dcf_stop(), then dcf_reset(), then set sync failed indicators
 *
 * In main, call dcf77_start() for sync attempt
 * After x failed attempts to locate minute marker, process stops via TIM interrupt
 */

typedef struct
{
	// Peripherals
	TIM_HandleTypeDef* htim;
	TIM_TypeDef* tim;
	GPIO_TypeDef* dcf_out_port;
	uint16_t dcf_out_pin;
	GPIO_TypeDef* dcf_sync_g_port;
	uint16_t dcf_sync_g_pin;
	GPIO_TypeDef* dcf_sync_y_port;
	uint16_t dcf_sync_y_pin;
	GPIO_TypeDef* dcf_sync_r_port;
	uint16_t dcf_sync_r_pin;
	GPIO_TypeDef* debug_port;
	uint16_t debug_pin;
	IRQn_Type exti_irqn;
	IRQn_Type tim_irqn;

	// DCF77 message
	uint8_t msg[60];
	uint8_t msg_idx;

	// Flags and indicators
	uint8_t search_active;
	uint8_t bit_detected;
	uint8_t end_of_msg;
	uint8_t sync_status;

	// Counters
	uint32_t error_cnt;
	uint32_t cycle_duration;
	uint8_t cycle_cnt;
	uint32_t cycle_marker;

	// DCF77 time
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
} dcf77_t;

void dcf77_init(dcf77_t* dcf77, TIM_HandleTypeDef* htim, GPIO_TypeDef* dcf_out_port, uint16_t dcf_out_pin, GPIO_TypeDef* dcf_sync_g_port, uint16_t dcf_sync_g_pin, GPIO_TypeDef* dcf_sync_y_port, uint16_t dcf_sync_y_pin, GPIO_TypeDef* dcf_sync_r_port, uint16_t dcf_sync_r_pin, GPIO_TypeDef* debug_port, uint16_t debug_pin, IRQn_Type exti_irqn, IRQn_Type tim_irqn);
void dcf77_reset(dcf77_t* dcf77);
void dcf77_start(dcf77_t* dcf77);
void dcf77_EXTI_callback(dcf77_t* dcf77);
void dcf77_search(dcf77_t* dcf77);
void dcf77_read(dcf77_t* dcf77);
void dcf77_stop(dcf77_t* dcf77);
void dcf77_decode(dcf77_t* dcf77);
void dcf77_synchronize(dcf77_t* dcf77);
void dcf77_timeout(dcf77_t* dcf77);

#endif /* INC_DCF77_H_ */
