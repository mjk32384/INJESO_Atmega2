/*
 * SPI.h
 *
 * Created: 2024-08-27 오후 8:27:04
 *  Author: stu11
 */ 

#include <avr/io.h>
#include <stdlib.h>

#define SPI_BUFFER_SIZE 12


void SPI_MasterInit(void);
void SPI_SlaveInit(void);
void SPI_MasterTransfer(char* SPI_txbuffer, char* SPI_rxbuffer);

void split_master_data(int16_t qtn[4], int16_t atm, int16_t serv_in, uint8_t spi_buffer[12]);
void concat_master_data(int16_t qtn[4], int16_t atm, int16_t serv_in, uint8_t spi_buffer[12]);
void split_slave_data(int32_t lat, int32_t lon, uint8_t spi_buffer[12]);
void concat_slave_data(int32_t lat, int32_t lon, uint8_t spi_buffer[12]);