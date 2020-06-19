/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define DEFAULT_VREF    3250                //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64                  //Multisampling
#define ADC_CHANNEL     ADC_CHANNEL_6       //ADC channel (GPIO34 in defkit)
#define ADC_WIDTH       ADC_WIDTH_BIT_12
#define ADC_ATTEN       ADC_ATTEN_DB_11

void adc1_init(adc_channel_t _channel, adc_bits_width_t _width, adc_atten_t _atten);
uint32_t adc1_get_value (adc_channel_t _channel, uint8_t _samples);

adc_channel_t channel = ADC_CHANNEL;
adc_bits_width_t width = ADC_WIDTH;
adc_atten_t atten = ADC_ATTEN;

void app_main(void){

    adc1_init(channel, width, atten);

    //Continuously sample ADC1
    while (1) {
        
        uint32_t value = 0;
        value = adc1_get_value(channel, NO_OF_SAMPLES);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}



void adc1_init(adc_channel_t _channel, adc_bits_width_t _width, adc_atten_t _atten){ 

    //Configure ADC
    adc1_config_width(_width);
    adc1_config_channel_atten(_channel, _atten);
}


uint32_t adc1_get_value (adc_channel_t _channel, uint8_t _samples){

    uint32_t adc_reading = 0;
    //Multisampling (saca un promedio de 64 muestras)
    for (int i = 0; i < _samples; i++) {
        adc_reading += adc1_get_raw((adc1_channel_t)_channel);
    }
    adc_reading /= _samples;

    //Convert adc_reading to voltage in mV
    uint32_t v = (adc_reading * DEFAULT_VREF) / 4095;
    printf("\nRaw: %d            Vol: %dmV\n", adc_reading, v);

    return v;
}