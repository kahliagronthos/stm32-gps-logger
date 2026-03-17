/*
 * nmea_parser.c
 *
 *  Created on: Mar 16, 2026
 *      Author: kahliagronthos
 */

#include "nmea_parser.h"
#include <string.h>
#include <stdlib.h>

/*
$GNRMC sentence format:
0  - Sentence ID ($GNRMC or $GPRMC)
1  - UTC time (hhmmss.ss)
2  - Status (A = valid fix, V = void (no fix))
3  - Latitude (DDMM.MMMM)
4  - Latitude hemisphere (N = North, S = South)
5  - Longitude (DDDMM.MMMM)
6  - Longitude hemisphere (E = East, W = West)
7  - Speed over ground (knots)
8  - Course over ground (degrees true)
9  - Date (ddmmyy)
10 - Magnetic variation (degrees)
11 - Magnetic variation direction (E = East, W = West)
12 - Mode indicator (A = Autonomous, D = Differential, E = Estimated, N = No fix)
*  - Checksum (two-digit hexadecimal XOR of characters between '$' and '*')
*/

static uint8_t hex_val(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

static int validate_checksum(const char *sentence)
{
    if (sentence[0] != '$') return 0; // sentence must start with '$'

    const char *p = sentence + 1;
    uint8_t calc = 0;

    while (*p && *p != '*')
    {
        calc ^= (uint8_t)*p; // XOR all chars between '$' and '*' into running total
        p++;
    }

    if (*p != '*') return 0; // sentence must contain checksum delimiter '*'

    uint8_t expected = (hex_val(*(p + 1)) << 4) | hex_val(*(p + 2));
    return calc == expected;
}

static int get_field(const char *sentence, int field_num, char *buf, int buf_len)
{
    const char *p = sentence;
    int current_field = 0;

    while (*p && current_field < field_num) // advance thru str until field_num is reached
    {
        if (*p == ',') current_field++;
        p++;
    }

    if (!*p) return 0;

    int i = 0;
    while (*p && *p != ',' && *p != '*' && i < buf_len - 1)
    {
        buf[i++] = *p++; // copy chars of field until next comma, delimeter or buffer full
    }
    buf[i] = '\0';
    return i > 0;
}

static float nmea_coord_to_decimal(const char *coord_str, char hemisphere)
{
    // NMEA format is ddmm.mmmm for latitude and dddmm.mmmm for longitude.
    if (coord_str[0] == '\0') return 0.0f;

    float raw = strtof(coord_str, NULL);

    int degrees = (int)(raw / 100); // integer division to get degrees component
    float minutes = raw - (degrees * 100); // remainder is minutes component
    float decimal = degrees + (minutes / 60.0f);

    if (hemisphere == 'S' || hemisphere == 'W')
        decimal = -decimal; // sign flip bc southern and western hemispheres are negative coordinates in decimal degrees

    return decimal;
}

static int parse_gnrmc(const char *sentence, GPS_Data_t *out)
{
    char field[20];
    char hem[4];

    // check status field to determine if fix is valid, no point parsing other fields if fix is void
    if (!get_field(sentence, 2, field, sizeof(field))) return 0;
    out->fix_valid = (field[0] == 'A') ? 1 : 0;
    if (!out->fix_valid) return 0;

    // parse remaining fields if fix is valid
    if (get_field(sentence, 1, field, sizeof(field)))
        strncpy(out->time_utc, field, sizeof(out->time_utc) - 1);

    if (get_field(sentence, 3, field, sizeof(field)) &&
        get_field(sentence, 4, hem,   sizeof(hem)))
        out->latitude = nmea_coord_to_decimal(field, hem[0]);

    if (get_field(sentence, 5, field, sizeof(field)) &&
        get_field(sentence, 6, hem,   sizeof(hem)))
        out->longitude = nmea_coord_to_decimal(field, hem[0]);

    if (get_field(sentence, 7, field, sizeof(field)))
        out->speed_knots = strtof(field, NULL);

    return 1;
}

int nmea_parse(const char *sentence, GPS_Data_t *out)
{
    if (!sentence || !out) return 0;
    if (!validate_checksum(sentence)) return 0;

    char id[12];
    get_field(sentence, 0, id, sizeof(id));

    if (strcmp(id, "$GNRMC") == 0 || strcmp(id, "$GPRMC") == 0)
        return parse_gnrmc(sentence, out);

    return 0;
}


