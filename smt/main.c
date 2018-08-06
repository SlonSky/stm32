/*
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

int main(void)
{
	// Rx - 9
	// Tx - 10
  // Настраиваем ножку PC8 в режим выхода на светодиод на плате
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Включаем модули USART1 и GPIOA, а также включаем альтернативные функции выходов
  RCC->APB2ENR|= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
  // Контакт PA9 будет выходом с альтернативной функцией, а контакт PA10 - входом
  GPIOA->CRH &= !GPIO_CRH_CNF9;
  GPIOA->CRH  |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_CNF10_0;
  // Настраиваем регистр тактирования, скорость составит 9600 бод (при тактовой частоте 24 МГц)
  USART1->BRR = 0x9C4;
  // Выключаем TxD и RxD USART
  USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
  // Запускаем модуль USART
  USART1->CR1 |= USART_CR1_UE;
  // Разрешаем прерывание по приёму информации с RxD
  USART1->CR1 |= USART_CR1_RXNEIE;
  // Назначаем обработчик для всех прерываний от USART1
  NVIC_EnableIRQ(USART1_IRQn);
  USART1_Send_String("Start\r\n");
  // Бесконечный цикл
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

// Обработчик всех прерываний от USART1
void USART1_IRQHandler(void) {
  // Выясняем, какое именно событие вызвало прерывание. Если это приём байта в RxD - обрабатываем.
  if (USART1->SR & USART_SR_RXNE) {
    // Сбрасываем флаг прерывания
    USART1->SR&=~USART_SR_RXNE;

    // Получаем принятый байт
    if(USART1->DR=='1') {
      GPIO_SetBits(GPIOC, GPIO_Pin_9);
      // Отправляем обратно строку "On" с переводом строки
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

	//Настраиваем ноги PC8 и PC9 на выход.
	PORT.GPIO_Pin = (GPIO_Pin_9 | GPIO_Pin_8);
	PORT.GPIO_Mode = GPIO_Mode_Out_PP;
	PORT.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init( GPIOC , &PORT);

	// Инициализуем периферию
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
		// Если дек не заполнен
		if(signalCount != width - 1) {
			signalCount++;
		} else {

			// Убрать первый элемент, сместить все влево
			for(int i = 0; i < width - 1; i++) {
				myoData[i] = myoData[i + 1];
			}
		}

		// Добавить измерение в конец
		myoData[signalCount] = adcGet();

		if(adcGet() > 2900) {
			delay(100);
		}
		// Найти среднее
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
		// Если порог превышен
		if(mean > threshold) {

			// Двигать платформу
			GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_SET);
			USART1_Send_uint32(1);

			USART1_Send('\n');
		} else {

			// Остановить платформу
			GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_RESET);
			USART1_Send_uint32(0);

			USART1_Send('\n');
		}

		delay(100);

	}
}



//		delay(200);
//		adc_res=adcGet();
//		delay(adc_res); //Исходя из значения АЦП делаем задержку
//		GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_RESET); //Гасим диод...
//		GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_SET); // Зажигаем диод...
//
//		adc_res=adcGet();
//		delay(adc_res); //Всё повторяется...
//		GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_RESET);
//		GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_SET);

