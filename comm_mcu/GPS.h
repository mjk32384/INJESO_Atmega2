/*
 * GPS.h
 *
 * Created: 2024-08-27 오후 5:11:36
 *  Author: jhyeo
 */ 


#ifndef GPS_H_
#define GPS_H_

#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "serial.h"

#define MAX_FIELDS 20
#define MAX_FIELD_LENGTH 20

#define UTC 0
#define STATUS 1
#define LAT 2
#define LAT_HEMI 3
#define LON 4
#define LON_HEMI 5
#define SPEED 6
#define COG 7
#define DATE 8
#define MAG 9
#define MAG_DIR 10

// GPS는 UART0랑 연결
// print_GPS_data 함수는 UART1을 통해서 정해진 형식으로 파싱한 데이터 출력

struct GPS {
	int32_t time;     // UTC, hhmmss
	char status;      // A:OK, V:warning
	int32_t lat;      // +:north, -:south, micro deg
	int32_t lon;      // +:east, -:west, micro deg
	int32_t speed;    // m/s * 10000
	int32_t cog;      // deg * 10000
	int32_t date;     // ddmmyy
	int32_t mag;      // +:east, -:west, deg * 10000
};

void GPS_receive_and_parse(struct GPS *GPS_data);
void GPRMC_parse(char* raw_data, struct GPS *GPS_data);
void print_GPS_data(const struct GPS *data);

#endif /* GPS_H_ */