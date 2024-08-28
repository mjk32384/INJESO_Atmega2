/*
 * comm_mcu.c
 *
 * Created: 2024-08-27 오후 7:51:41
 * Author : stu11
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serial.h"
#include "GPS.h"
#include "LORA.h"
#include "SPI.h"

char LORA_ATcommand[LORA_BUFFER_SIZE];
char SPI_txbuffer[SPI_BUFFER_SIZE+1];
char SPI_rxbuffer[SPI_BUFFER_SIZE+1];


ISR(USART1_RX_vect){
	
}

ISR(SPI_STC_vect){
	// SPI 전송 완료 인터럽트
	for(spi_index = 0; spi_index<SPI_BUFFER_SIZE; spi_index++){
		
		SPI_rxbuffer[spi_index] = SPDR; // 수신된 데이터 읽기
		SPDR = SPI_txbuffer[spi_index];  // 응답 데이터 설정 (예: 수신된 데이터에 1을 더한 값 전송)
		if(spi_index != SPI_BUFFER_SIZE-1 ) while(!(SPSR & (1 << SPIF)));
	}
	spi_flag = 1;
}


int main(void)
{
	UART0_init();
	UART1_init();
	SPI_SlaveInit();
	
	int16_t qtn[4];
	
	sei();
	
    while (1) 
    {
		
    }
}

