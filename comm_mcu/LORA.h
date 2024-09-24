/*
 * LORA.h
 *
 * Created: 2024-08-27 오후 8:10:37
 *  Author: stu11
 */ 
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include "serial.h"

#define LORA_BUFFER_SIZE 256

void make_ATcommand(char* send_command, int32_t lat, int32_t lon, int16_t qtn[4], int16_t atm, int16_t serv_in);
void send_ATcommand(char* send_command);