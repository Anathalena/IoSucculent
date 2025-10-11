#ifndef __SENSORS_H__
#define __SENSORS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

uint8_t ReadSoilMoisture();
void SendSensorReadings();


#ifdef __cplusplus
}
#endif

#endif // __SENSORS_H__
