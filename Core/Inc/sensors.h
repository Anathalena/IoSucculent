#ifndef __SENSORS_H__
#define __SENSORS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#define BUFFER_SIZE 5

typedef struct {
	int8_t temperature;
	uint8_t pressure;
	uint8_t humidity;
} bme280_packet;

typedef struct {
	uint8_t soil_moisture;
	bme280_packet air_packet;
} data_packet;

typedef struct {
	uint32_t current_entry;
	data_packet data[BUFFER_SIZE];
} circular_data_buffer;

void sensors_read_soil_data(uint8_t *soil_data);
void sensors_read_air_data(bme280_packet *air_data);
void sensors_save_data_to_global_buffer(void);
void sensors_send_data(void);

#ifdef __cplusplus
}
#endif

#endif // __SENSORS_H__
