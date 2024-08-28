/*
 * GPS.c
 *
 * Created: 2024-08-27 오후 5:11:56
 *  Author: jhyeo
 */ 

#include "GPS.h"


void GPS_receive_and_parse(struct GPS *GPS_data) {
	char target_nmea[] = "GPRMC,";
	char received_nmea[7] = "";
	char raw_data[100];
	char ch  = '0';
	
	while (1) {
		ch = UART1_receive();
		
		if (ch == '$') {
			for (int i = 0; i < 6; i++) {
				received_nmea[i] = UART1_receive();
			}
			received_nmea[6] = '\0';
			
			if (strcmp(received_nmea, target_nmea) == 0) {
				UART1_receive_string(raw_data, '\n');
				GPRMC_parse(raw_data, GPS_data);
				return;
			}
			ch = '0';
		}
	}
}

void GPRMC_parse(char* raw_data, struct GPS *GPS_data) {
	int i = 0;
	char *start = raw_data;
	char *end = strchr(start, ',');
	
	while (end != NULL && i < MAX_FIELDS) {
		char temp[MAX_FIELD_LENGTH] = "";
		
		if (end == start) {
			strcpy(temp, "");
			} else {
			strncpy(temp, start, end - start);
			temp[end - start] = '\0';
		}
		
		char *dot_position;
		char deg[4] = "";
		char *min;
		
		switch (i) {
			case UTC:
			dot_position = strchr(temp, '.');
			if (dot_position != NULL) {
				*dot_position = '\0';
			}
			GPS_data->time = atol(temp);
			break;
			case STATUS:
			GPS_data->status = temp[0];
			break;
			case LAT:
			strncpy(deg, temp, 2);
			deg[2] = '\0';
			min = temp + 2;
			GPS_data->lat = (int32_t)((atof(deg) + atof(min)/60.0) * 1000000);
			break;
			case LAT_HEMI:
			if (strcmp(temp, "S") == 0) {
				GPS_data->lat *= -1;
			}
			break;
			case LON:
			strncpy(deg, temp, 3);
			deg[3] = '\0';
			min = temp + 3;
			GPS_data->lon = (int32_t)((atof(deg) + atof(min)/60.0) * 1000000);
			break;
			case LON_HEMI:
			if (strcmp(temp, "W") == 0) {
				GPS_data->lon *= -1;
			}
			break;
			case SPEED:
			GPS_data->speed = (int32_t)(atof(temp) * 10000 * 0.51444);
			break;
			case COG:
			GPS_data->cog = (int32_t)(atof(temp) * 10000);
			break;
			case DATE:
			GPS_data->date = atol(temp);
			break;
			case MAG:
			GPS_data->mag = (int32_t)(atof(temp) * 10000);
			break;
			case MAG_DIR:
			if (strcmp(temp, "W") == 0) {
				GPS_data->mag *= -1;
			}
			break;
		}
		
		i++;
		start = end + 1;
		end = strchr(start, ',');
	}
}

void print_GPS_data(const struct GPS *data) {
	char buffer[50];

	if (data->time != 0) {
		sprintf(buffer, "UTC Time: %06ld", data->time);
		} else {
		sprintf(buffer, "UTC Time: N/A");
	}
	UART0_transmit_string_LF(buffer);

	if (data->status != '\0') {
		sprintf(buffer, "Status: %c", data->status);
		} else {
		sprintf(buffer, "Status: N/A");
	}
	UART0_transmit_string_LF(buffer);

	if (data->lat != 0) {
		sprintf(buffer, "Latitude: %ld micro deg", data->lat);
		} else {
		sprintf(buffer, "Latitude: N/A");
	}
	UART0_transmit_string_LF(buffer);

	if (data->lon != 0) {
		sprintf(buffer, "Longitude: %ld micro deg", data->lon);
		} else {
		sprintf(buffer, "Longitude: N/A");
	}
	UART0_transmit_string_LF(buffer);

	if (data->speed != 0) {
		sprintf(buffer, "Speed: %ld *10^-4 m/s", data->speed);
		} else {
		sprintf(buffer, "Speed: N/A");
	}
	UART0_transmit_string_LF(buffer);

	if (data->cog != 0) {
		sprintf(buffer, "Course Over Ground: %ld *10^-4 deg", data->cog);
		} else {
		sprintf(buffer, "Course Over Ground: N/A");
	}
	UART0_transmit_string_LF(buffer);

	if (data->date != 0) {
		sprintf(buffer, "Date: %06ld", data->date);
		} else {
		sprintf(buffer, "Date: N/A");
	}
	UART0_transmit_string_LF(buffer);

	if (data->mag != 0) {
		sprintf(buffer, "Magnetic Variation: %ld *10^-4 deg", data->mag);
		} else {
		sprintf(buffer, "Magnetic Variation: N/A");
	}
	UART0_transmit_string_LF(buffer);
	
	UART0_transmit_string_LF("******************************");
}