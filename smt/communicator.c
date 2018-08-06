/*
 *
 #include "communicator.h"

//‘ункци€ отправл€юща€ байт в UART
void send_to_uart(uint8_t data_uart)
{
	while(!(USART1->SR & USART_SR_TC)); //∆дем пока бит TC в регистре SR станет 1
		USART1->DR=data_uart; //ќтсылаем байт через UART
}

void send_message(char message[], int size) {
	for(int i = 0; i < size; i++) {
		send_to_uart(message[i]);
	}
}
*/
