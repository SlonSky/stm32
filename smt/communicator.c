/*
 *
 #include "communicator.h"

//������� ������������ ���� � UART
void send_to_uart(uint8_t data_uart)
{
	while(!(USART1->SR & USART_SR_TC)); //���� ���� ��� TC � �������� SR ������ 1
		USART1->DR=data_uart; //�������� ���� ����� UART
}

void send_message(char message[], int size) {
	for(int i = 0; i < size; i++) {
		send_to_uart(message[i]);
	}
}
*/
