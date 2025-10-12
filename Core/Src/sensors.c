#include "sensors.h"
#include "main.h"
#include "usart.h"
#include "adc.h"
#include "bmp280.h"
#include "stdio.h"

extern BMP280_HandleTypedef bme280;
data_packet data[BUFFER_SIZE];

void  ReadSoilData(uint8_t *soil_data)
{
	uint16_t adc_val;

	HAL_ADC_Start(&hadc1);
	adc_val = HAL_ADC_GetValue(&hadc1);
	*soil_data = (uint8_t)(100.0f * (1.0f - (float)adc_val/4095.0f));
	HAL_ADC_Stop(&hadc1);
}

void ReadAirData(bme280_packet *air_data)
{
	int32_t temperature;
	uint32_t pressure;
	uint32_t humidity;

	if(!bmp280_read_fixed(&bme280, &temperature, &pressure, &humidity)) {
		printf("BME280 reading failed\n");
	}

	air_data->temperature = temperature / 100;
	air_data->humidity = (uint8_t)(humidity >> 10);
	air_data->pressure = (uint8_t)(pressure >> 8);
}

void ReadSensorData()
{
	static unsigned int idx = 0;
	ReadSoilData(&data[idx].soil_moisture);
	ReadAirData(&data[idx].air_packet);
	idx += 1;

	if(idx == BUFFER_SIZE){
		idx = 0;
	}
}

void SendSensorData()
{
	for(int idx=0; idx<BUFFER_SIZE; idx++){
		printf("Soil moisture: %d%%\n", data[idx].soil_moisture);
		printf("Air temperature: %dC; Air humidity: %d%%\n", data[idx].air_packet.temperature, data[idx].air_packet.humidity);
	}
}
