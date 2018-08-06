/*
#include "sensor.h"

void delay(uint32_t del){
	for(volatile uint32_t i = 0; i<del; i++);
}


void send_presence()
{
	GPIOA->ODR = GPIO_Pin_3;
	delay(25);
	GPIOA->ODR = 0;
	delay(1100); //420us
	GPIOA->ODR = GPIO_Pin_3;
}

void one_wire_write_bit(uint8_t bit)
{
	GPIOA->ODR = 0;
	delay(bit ? 20 : 190);//"1"
	GPIOA->ODR = GPIO_Pin_3;
	delay(bit ? 190 : 20);//"0"
}

uint8_t one_wire_read_bit()
{
	uint8_t bit = 0;
	GPIOA->ODR = 0;
	delay(20);
	GPIOA->ODR = GPIO_Pin_3;
	delay(12);
	GPIOA->CRL &= ~GPIO_CRL_MODE3;
	GPIOA->CRL &= ~GPIO_CRL_CNF3;
	GPIOA->CRL |=  GPIO_CRL_CNF3_0;
	bit = (GPIOA->IDR&GPIO_Pin_3?1:0);
	GPIOA->CRL |=  GPIO_CRL_MODE3;
	GPIOA->CRL |=  GPIO_CRL_CNF3_0;
	delay(150);
	return bit;
}

void one_wire_write_byte(uint8_t data)
{
	for(uint8_t i = 0; i<8; i++)
		one_wire_write_bit(data>>i & 1);
}

float getCurrentTemperature() {
	send_presence();
	delay(1600);
	one_wire_write_byte(0xCC);
	one_wire_write_byte(0x4E);
	one_wire_write_byte(0x4B);
	one_wire_write_byte(0x46);
	one_wire_write_byte(0x5F);

	send_presence();
	delay(1400);
	one_wire_write_byte(0xCC);
	one_wire_write_byte(0x44);
	delay(1500000);

	send_presence();
	delay(1400);
	one_wire_write_byte(0xCC);
	one_wire_write_byte(0xBE);
	delay(1000);
	uint16_t data = 0;
	for(uint8_t i = 0; i<16; i++)
		data += (uint16_t)one_wire_read_bit()<<i;
	float currentTemperature = data/16.0;

	return currentTemperature;
}
*/
