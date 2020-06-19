/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO18: output
 * GPIO19: output
 * GPIO4:  input, pulled up, interrupt from rising edge and falling edge
 * GPIO5:  input, pulled up, interrupt from rising edge.
 *
 * Test:
 * Connect GPIO18 with GPIO4
 * Connect GPIO19 with GPIO5
 * Generate pulses on GPIO18/19, that triggers interrupt on GPIO4/5
 *
 */

#define GPIO_OUTPUT_IO    15
#define GPIO_INPUT_IO     0

void gpio_setup(uint8_t pin, bool dirrection);       //1 = in - 0 = out

void app_main(void){

    gpio_setup(GPIO_OUTPUT_IO, 0);
    gpio_setup(GPIO_INPUT_IO, 1);

    while(1) {
        if (gpio_get_level (GPIO_INPUT_IO)){
            gpio_set_level (GPIO_OUTPUT_IO, 0);
        }else{
            gpio_set_level (GPIO_OUTPUT_IO, 1);
        }
    }
}



void gpio_setup(uint8_t pin, bool dirrection){

    gpio_pad_select_gpio(pin);
    if (dirrection){
        gpio_set_direction(pin, GPIO_MODE_INPUT);
    }else{
        gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    }
}
