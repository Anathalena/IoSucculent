#include "sensors.h"
#include "main.h"
#include "usart.h"
#include "adc.h"
#include "bmp280.h"
#include "stdio.h"

extern BMP280_HandleTypedef bme280;
circular_data_buffer data_buffer;

void  sensors_read_soil_data(uint8_t *soil_data)
{
	uint16_t adc_val;

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 50);
	adc_val = HAL_ADC_GetValue(&hadc1);
	*soil_data = (uint8_t)(100.0f * (1.0f - (float)adc_val/4095.0f));
	HAL_ADC_Stop(&hadc1);
}

void sensors_read_air_data(bme280_packet *air_data)
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

void sensors_save_data_to_global_buffer(void)
{
	sensors_read_soil_data(&data_buffer.data[data_buffer.current_entry].soil_moisture);
	sensors_read_air_data(&data_buffer.data[data_buffer.current_entry].air_packet);
	data_buffer.current_entry++;
	data_buffer.current_entry = data_buffer.current_entry % BUFFER_SIZE;
}

void sensors_send_data(void)
{
	for(int idx=0; idx<BUFFER_SIZE; idx++){
		printf("Soil moisture: %d%%\n", data_buffer.data[idx].soil_moisture);
		printf("Air temperature: %dC; Air humidity: %d%%\n", data_buffer.data[idx].air_packet.temperature, data_buffer.data[idx].air_packet.humidity);
	}
}
