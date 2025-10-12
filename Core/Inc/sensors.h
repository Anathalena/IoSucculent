#ifndef __SENSORS_H__
#define __SENSORS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef struct {
	int8_t temperature;
	uint8_t pressure;
	uint8_t humidity;
} bme280_packet;

typedef struct {
	uint8_t soil_moisture;
	bme280_packet air_packet;
} data_packet;

data_packet data;

void ReadSoilData();
void ReadAirData();
void ReadSensorData();
void SendSensorData();

#ifdef __cplusplus
}
#endif

#endif // __SENSORS_H__
