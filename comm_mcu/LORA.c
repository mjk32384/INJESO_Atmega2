/*
 * LORA.c
 *
 * Created: 2024-08-27 오후 7:53:55
 *  Author: stu11
 */ 


#include "LORA.h"


void make_ATcommand(char* send_command, int16_t gps[3], int16_t qtn[4], int16_t atm, int16_t serv_in) {	
	/*	
		<input parameter>
		gps:		gps data.					[lat, lon, vel]
		qtn:		quaternion data.			[q0, q1, q2, q3]
		atm:		atmospheric pressure data.
		serv_in:	servo motor input data.
		
		<send command>
		AT+SEND=<Address>,<Payload Length>,<Data>
	*/
	
	// static char send_command[LORA_BUFFER_SIZE];
	char data[LORA_BUFFER_SIZE];
	char temp[16];	// temporary buffer to hold each number as a string
	int payload_length = 0;

	// Initialize the data string
	data[0] = '\0';

	// Concatenate gps data
	for (int i = 0; i < 3; i++) {
		itoa(gps[i], temp, 10);             // Convert int to string
		strcat(data, temp);             // Concatenate
		strcat(data, "/");              // Add a space
	}

	// Concatenate qtn data
	for (int i = 0; i < 4; i++) {
		itoa(qtn[i], temp, 10);
		strcat(data, temp);
		strcat(data, "/");
	}

	// Concatenate atm data
	itoa(atm, temp, 10);
	strcat(data, temp);
	strcat(data, "/");

	// Concatenate serv_in data
	itoa(serv_in, temp, 10);
	strcat(data, temp);

	// Calculate the payload length (number of characters in data)
	payload_length = strlen(data);
	
	// Manually build the send_command string
	strcpy(send_command, "AT+SEND=0,");  // Start with the command prefix
	itoa(payload_length, temp, 10);          // Convert payload length to string
	strcat(send_command, temp);          // Append payload length
	strcat(send_command, ",");           // Append a comma
	strcat(send_command, data);          // Append the data
	strcat(send_command, "\r\n");          // Append the data
}

void send_ATcommand(char* send_command){
	UART0_transmit_string(send_command);
}