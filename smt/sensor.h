#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void delay(uint32_t del);
void send_presence();
void one_wire_write_bit(uint8_t bit);
uint8_t one_wire_read_bit();
void one_wire_write_byte(uint8_t data);
float getCurrentTemperature();
