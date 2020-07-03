/* esp_timer (high resolution timer) example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "sdkconfig.h"

#include "driver/gpio.h"

#define BLINK_GPIO 2
u_int8_t flag = 0;

static void periodic_timer_callback(void* arg);

static const char* TAG = "example";

void app_main(void){
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    /* Create two timers:
     * 1. a periodic timer which will run every 0.5s, and print a message
     */

    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &periodic_timer_callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = "periodic"
    };

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    /* The timer has been created but is not running yet */


    /* Start the timers */
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 500000));
    ESP_LOGI(TAG, "Started timers, time since boot: %lld us", esp_timer_get_time());

}

static void periodic_timer_callback(void* arg){
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGI(TAG, "Periodic timer called, time since boot: %lld us", time_since_boot);

    printf("\nEstoy en el Timer: %d\n", flag);
    if (flag==1){
        gpio_set_level(BLINK_GPIO, 1);
        flag = 0;
    }else{
        gpio_set_level(BLINK_GPIO, 0);
        flag = 1;
    }
}
