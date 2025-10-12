#include "main.h"
#include "sensors.h"
#include "bmp280.h"
#include "stdio.h"

extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;
extern BMP280_HandleTypedef bme280;

void  ReadSoilData(uint8_t *data)
{
	uint16_t adc_val;

	HAL_ADC_Start(&hadc1);
	adc_val = HAL_ADC_GetValue(&hadc1);
	*data = (uint8_t)(100.0f * (1.0f - (float)adc_val/4095.0f));
	HAL_ADC_Stop(&hadc1);
}

void ReadAirData(bme280_packet *data)
{
	int32_t temperature;
	uint32_t pressure;
	uint32_t humidity;

	if(!bmp280_read_fixed(&bme280, &temperature, &pressure, &humidity)) {
		printf("BME280 reading failed\n");
	}

	data->temperature = temperature / 100;
	data->humidity = (uint8_t)(humidity >> 10);
	data->pressure = (uint8_t)(pressure >> 8);
}

void ReadSensorData()
{
	ReadSoilData(&data.soil_moisture);
	ReadAirData(&data.air_packet);
}

void SendSensorData()
{
	ReadSensorData();
	printf("Soil moisture: %d%%\n", data.soil_moisture);
	printf("Air temperature: %dC; Air humidity: %d%%\n", data.air_packet.temperature, data.air_packet.humidity);
}
