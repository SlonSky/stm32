/*
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

int main(void)
{
	// Rx - 9
	// Tx - 10
  // ����������� ����� PC8 � ����� ������ �� ��������� �� �����
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // �������� ������ USART1 � GPIOA, � ����� �������� �������������� ������� �������
  RCC->APB2ENR|= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
  // ������� PA9 ����� ������� � �������������� ��������, � ������� PA10 - ������
  GPIOA->CRH &= !GPIO_CRH_CNF9;
  GPIOA->CRH  |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_CNF10_0;
  // ����������� ������� ������������, �������� �������� 9600 ��� (��� �������� ������� 24 ���)
  USART1->BRR = 0x9C4;
  // ��������� TxD � RxD USART
  USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
  // ��������� ������ USART
  USART1->CR1 |= USART_CR1_UE;
  // ��������� ���������� �� ����� ���������� � RxD
  USART1->CR1 |= USART_CR1_RXNEIE;
  // ��������� ���������� ��� ���� ���������� �� USART1
  NVIC_EnableIRQ(USART1_IRQn);
  USART1_Send_String("Start\r\n");
  // ����������� ����
  while(1);
}

void USART1_Send(char chr) {
  while(!(USART1->SR & USART_SR_TC));
  USART1->DR = chr;
}

void USART1_Send_String(char* str) {
  int i=0;
  while(str[i])
    USART1_Send(str[i++]);
}

// ���������� ���� ���������� �� USART1
void USART1_IRQHandler(void) {
  // ��������, ����� ������ ������� ������� ����������. ���� ��� ���� ����� � RxD - ������������.
  if (USART1->SR & USART_SR_RXNE) {
    // ���������� ���� ����������
    USART1->SR&=~USART_SR_RXNE;

    // �������� �������� ����
    if(USART1->DR=='1') {
      GPIO_SetBits(GPIOC, GPIO_Pin_9);
      // ���������� ������� ������ "On" � ��������� ������
      USART1_Send_String("On\r\n");
    }

    if(USART1->DR=='2') {
      GPIO_ResetBits(GPIOC, GPIO_Pin_9);
      USART1_Send_String("Off\r\n");
    }

  }
}
*/

#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include "stm32f10x.h"

#include "adc.h"
#include "usart1.h"

void delay(uint32_t i) {
	volatile uint32_t j;
	for (j = 0; j != i * 1000; j++);
}

int main(void) {

	GPIO_InitTypeDef PORT;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//����������� ���� PC8 � PC9 �� �����.
	PORT.GPIO_Pin = (GPIO_Pin_9 | GPIO_Pin_8);
	PORT.GPIO_Mode = GPIO_Mode_Out_PP;
	PORT.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init( GPIOC , &PORT);

	// ������������ ���������
	adcInit(1);
	USART1_Init();

	delay(500);
	USART1_Send('0');
	delay(200);


	int threshold = 160;
	int width = 20;

	int myoData[width];
	for(int i = 0; i < width; i++) {
		myoData[i] = 0;
	}

	int signalCount = 0;

	while(1)
	{
		// ���� ��� �� ��������
		if(signalCount != width - 1) {
			signalCount++;
		} else {

			// ������ ������ �������, �������� ��� �����
			for(int i = 0; i < width - 1; i++) {
				myoData[i] = myoData[i + 1];
			}
		}

		// �������� ��������� � �����
		myoData[signalCount] = adcGet();

		if(adcGet() > 2900) {
			delay(100);
		}
		// ����� �������
		float mean = 0;
		for(int i = 0; i < width; i++) {
			mean += myoData[i];
		}
		mean /= width;


		GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_SET);
		delay((int)5);
		GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_RESET);
//		USART1_Send_uint32(adcGet());
//		USART1_Send('\n');
		// ���� ����� ��������
		if(mean > threshold) {

			// ������� ���������
			GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_SET);
			USART1_Send_uint32(1);

			USART1_Send('\n');
		} else {

			// ���������� ���������
			GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_RESET);
			USART1_Send_uint32(0);

			USART1_Send('\n');
		}

		delay(100);

	}
}



//		delay(200);
//		adc_res=adcGet();
//		delay(adc_res); //������ �� �������� ��� ������ ��������
//		GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_RESET); //����� ����...
//		GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_SET); // �������� ����...
//
//		adc_res=adcGet();
//		delay(adc_res); //�� �����������...
//		GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_RESET);
//		GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_SET);

