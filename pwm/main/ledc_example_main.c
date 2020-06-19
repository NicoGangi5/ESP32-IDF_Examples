/* LEDC (LED Controller) fade example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"


#define LEDC_LS_CH2_GPIO        (13)
#define DUTY_RESOLUTION         LEDC_TIMER_13_BIT
#define FREQ                    500
#define SPEED_MODE              LEDC_LS_MODE
#define LEDC_LS_TIMER           LEDC_TIMER_1
#define CLK_CFG                 LEDC_AUTO_CLK
#define LEDC_LS_CH2_CHANNEL     LEDC_CHANNEL_2
#define LEDC_DUTY               0
#define LEDC_LS_MODE            LEDC_HIGH_SPEED_MODE
#define LEDC_HPOINT             0


void pwm_init(ledc_timer_config_t _ledc_timer, ledc_channel_config_t _ledc_channel);
void pwm_freq(ledc_timer_config_t _ledc_timer, u_int32_t _freq);
void pwm_duty(ledc_channel_config_t _ledc_channel, u_int32_t _duty);


void app_main(void){

    /*
     * Prepare and set configuration of timers
     * that will be used by LED Controller
     */
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = DUTY_RESOLUTION,     // resolution of PWM duty (0 - 8192)
        .freq_hz = FREQ,                        // frequency of PWM signal
        .speed_mode = SPEED_MODE,               // timer mode
        .timer_num = LEDC_LS_TIMER,             // timer index
        .clk_cfg = CLK_CFG,                     // Auto select the source clock
    };

    /*
     * Prepare individual configuration
     * for each channel of LED Controller
     * by selecting:
     * - controller's channel number
     * - output duty cycle, set initially to 0
     * - GPIO number where LED is connected to
     * - speed mode, either high or low
     * - timer servicing selected channel
     *   Note: if different channels use one timer,
     *         then frequency and bit_num of these channels
     *         will be the same
     */
    ledc_channel_config_t ledc_channel = {
            .channel    = LEDC_LS_CH2_CHANNEL,
            .duty       = LEDC_DUTY,
            .gpio_num   = LEDC_LS_CH2_GPIO,
            .speed_mode = LEDC_LS_MODE,
            .hpoint     = LEDC_HPOINT,
            .timer_sel  = LEDC_LS_TIMER
    };

    pwm_init(ledc_timer, ledc_channel);

    // Initialize fade service.
    ledc_fade_func_install(0);

    while (1) {

        printf("Variable FREQ\n");
        for (size_t i = 100; i < 9000; i++){
            pwm_freq(ledc_timer, i);

            u_int32_t duty = 4300;
            pwm_duty(ledc_channel, duty);

            vTaskDelay(1 / portTICK_PERIOD_MS);
            printf("Freq: %d ", i);
        }

        vTaskDelay(3 / portTICK_PERIOD_MS);

        printf("Variable DUTY\n");
        for (size_t i = 0; i < 8192; i++){
            pwm_freq(ledc_timer, 500);

            pwm_duty(ledc_channel, i);

            vTaskDelay(1 / portTICK_PERIOD_MS);
            //printf("Duty: %d ", i);
        }
        
    }
}



void pwm_init(ledc_timer_config_t _ledc_timer, ledc_channel_config_t _ledc_channel){
    
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&_ledc_timer);

    // Set LED Controller with previously prepared configuration
    ledc_channel_config(&_ledc_channel);
}

void pwm_freq(ledc_timer_config_t _ledc_timer, u_int32_t _freq){

    _ledc_timer.freq_hz = _freq;
    ledc_timer_config(&_ledc_timer);
}

void pwm_duty(ledc_channel_config_t _ledc_channel, u_int32_t _duty){

    ledc_set_duty(_ledc_channel.speed_mode, _ledc_channel.channel, _duty);
    ledc_update_duty(_ledc_channel.speed_mode, _ledc_channel.channel);
}