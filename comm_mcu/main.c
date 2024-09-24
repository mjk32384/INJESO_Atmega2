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


int spi_flag = 0;
int32_t lat=127; int32_t lon=37;

struct GPS GPS_data;

ISR(USART1_RX_vect){
	
	GPS_receive_and_parse(&GPS_data);
	if(GPS_data.status == 'A'){
		 lat = GPS_data.lat;
		 lon = GPS_data.lon;
	}
	else{
		lat = 0;
		lon = 0;
	}
	
	split_slave_data(lat, lon, SPI_txbuffer);
}

ISR(SPI_STC_vect){
	int spi_index = 0;
	// SPI 전송 완료 인터럽트
	for(spi_index = 0; spi_index<SPI_BUFFER_SIZE; spi_index++){
		SPI_rxbuffer[spi_index] = SPDR; // 수신된 데이터 읽기
		SPDR = SPI_txbuffer[spi_index];  // 응답 데이터 설정 (예: 수신된 데이터에 1을 더한 값 전송)
		if(spi_index != SPI_BUFFER_SIZE-1 ) while(!(SPSR & (1 << SPIF)));
	}
	
	spi_flag=1;
}


int main(void)
{	
	int i;
	int16_t q[4];
	int16_t pressure=500; int16_t servo_input=390;
	for(i = 0; i<SPI_BUFFER_SIZE; i++){	SPI_txbuffer[i] = 0xFF;}
		
	UART0_init();
	UART1_init();
	SPI_SlaveInit();
	
	sei();
	
    while (1) {
		if(spi_flag){
			concat_master_data(q, &servo_input, &pressure, SPI_rxbuffer);
			
			make_ATcommand(LORA_ATcommand, lat, lon, q, pressure, servo_input);
			send_ATcommand(LORA_ATcommand);
			/*UART0_transmit_int16( q[0] ); UART0_transmit(',');
			UART0_transmit_int16( q[1] ); UART0_transmit(',');
			UART0_transmit_int16( q[2] ); UART0_transmit(',');
			UART0_transmit_int16( q[3] ); UART0_transmit('\n');
			*/
			//UART0_transmit_int16( servo_input ); UART0_transmit('\n');
			
			for(i = 0; i<SPI_BUFFER_SIZE; i++){	SPI_txbuffer[i] = 0xFF;}
			
			spi_flag = 0;
		}
		
		
		

		
    }
}

