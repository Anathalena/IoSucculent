#include "main.h"
#include "sensors.h"
#include "stdio.h"

extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;

uint8_t  ReadSoilMoisture()
{
	uint16_t adc_val;
	uint8_t scaled_val;

	HAL_ADC_Start(&hadc1);
	adc_val = HAL_ADC_GetValue(&hadc1);
	scaled_val = (uint8_t)(100.0f * (1.0f - (float)adc_val/4095.0f));
	HAL_ADC_Stop(&hadc1);

	return scaled_val;
}

void SendSensorReadings()
{
	uint8_t soil_moisture = ReadSoilMoisture();
	printf("Soil moisture: %d\n", soil_moisture);
}
