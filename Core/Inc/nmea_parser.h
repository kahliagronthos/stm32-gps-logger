/*
 * nmea_parser.h
 *
 *  Created on: Mar 16, 2026
 *      Author: kahliagronthos
 */

#ifndef INC_NMEA_PARSER_H_
#define INC_NMEA_PARSER_H_

#include <stdint.h>

typedef struct {
    float   latitude;
    float   longitude;
    float   speed_knots;
    uint8_t fix_valid;
    char    time_utc[12];
} GPS_Data_t;

int nmea_parse(const char *sentence, GPS_Data_t *out);

#endif /* INC_NMEA_PARSER_H_ */
