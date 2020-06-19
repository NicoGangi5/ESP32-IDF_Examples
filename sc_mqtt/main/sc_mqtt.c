/* Esptouch example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"

#include "esp_wifi.h"
#include "esp_wpa2.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_smartconfig.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "driver/gpio.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "mqtt_client.h"

#define BLINK_GPIO 2
uint8_t done = 0;

QueueHandle_t xQueue1;

nvs_handle_t my_handle_ssid;
nvs_handle_t my_handle_pass;

/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
static const int CONNECTED_BIT = BIT0;
static const int ESPTOUCH_DONE_BIT = BIT1;
static const char *TAG = "sc_mqtt_example";

static void smartconfig_example_task(void * parm);
static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
static void initialise_wifi(void);

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

void mqtt_handler(esp_mqtt_event_handle_t event);
void flash_rw(void);


void app_main(void)
{
    ESP_ERROR_CHECK( nvs_flash_init() );

    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(esp_netif_init());

    while(xQueue1 == 0){
        xQueue1= xQueueCreate(1, sizeof( uint8_t ));
    } 
    //flash_rw();
    initialise_wifi();

    while (done == 0){
        if (xQueue1 != NULL) {
           xQueueReceive(xQueue1,&done,(TickType_t )(1000/portTICK_PERIOD_MS)); 
        }
    } 

    esp_mqtt_client_config_t mqtt_cfg = {
        .host = "mqtt.dioty.co",
        .uri = CONFIG_BROKER_URL,
        .port = 1883,
        .username = "1605558@ucc.edu.ar",
        .password = "abef25ba",
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
    
    while (1){
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // const char *top = "/1605558@ucc.udu.ar/pub";
        // const char *dat = "hola manola";

        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        esp_mqtt_client_publish(client, "/1605558@ucc.edu.ar/qos0", "publico_1", 0, 0, 0);
        esp_mqtt_client_subscribe(client, "/1605558@ucc.edu.ar/qos1", 0);

    }
}




//-------------------------------------EVENT_HANDLER-------------------------------------
static void event_handler(void* arg, esp_event_base_t event_base, 
                                int32_t event_id, void* event_data){
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        xTaskCreate(smartconfig_example_task, "smartconfig_example_task", 4096, NULL, 3, NULL);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        xEventGroupClearBits(s_wifi_event_group, CONNECTED_BIT);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        xEventGroupSetBits(s_wifi_event_group, CONNECTED_BIT);
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE) {
        ESP_LOGI(TAG, "Scan done");
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL) {
        ESP_LOGI(TAG, "Found channel");
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD) {
        ESP_LOGI(TAG, "Got SSID and password");

        smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
        wifi_config_t wifi_config;
        uint8_t ssid[33] = { 0 };
        uint8_t password[65] = { 0 };

        bzero(&wifi_config, sizeof(wifi_config_t));
        memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
        memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
        wifi_config.sta.bssid_set = evt->bssid_set;
        if (wifi_config.sta.bssid_set == true) {
            memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
        }

        memcpy(ssid, evt->ssid, sizeof(evt->ssid));
        memcpy(password, evt->password, sizeof(evt->password));
        ESP_LOGI(TAG, "SSID:%s", ssid);
        ESP_LOGI(TAG, "PASSWORD:%s", password);

        // esp_err_t err_ssid = nvs_set_str(my_handle_ssid, "ssid", (char*)ssid);
        // printf((err_ssid != ESP_OK) ? "Failed!\n" : "Done\n");
        // esp_err_t err_pass = nvs_set_str(my_handle_pass, "pass", (char*)password);
        // printf((err_pass != ESP_OK) ? "Failed!\n" : "Done\n");

        ESP_ERROR_CHECK( esp_wifi_disconnect() ); 
        ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
        ESP_ERROR_CHECK( esp_wifi_connect() );
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE) {
        xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_DONE_BIT);
    }
}


//-------------------------------------SMARTCONFIG_EXAMPLE_TASK-------------------------------------
static void smartconfig_example_task(void * parm){
    EventBits_t uxBits;
    ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_ESPTOUCH) );
    smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_smartconfig_start(&cfg) );
    while (1) {
        uxBits = xEventGroupWaitBits(s_wifi_event_group, CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY); 
        if(uxBits & CONNECTED_BIT) {
            ESP_LOGI(TAG, "WiFi Connected to ap");
        }
        if(uxBits & ESPTOUCH_DONE_BIT) {
            ESP_LOGI(TAG, "smartconfig over");
            uint8_t a = 1; 
            xQueueSend(xQueue1,(void *)&a,(TickType_t )10);
            esp_smartconfig_stop();
            vTaskDelete(NULL);
        }
    }
}


//-------------------------------------INITIALISE_WIFI-------------------------------------
static void initialise_wifi(void){
    ESP_ERROR_CHECK(esp_netif_init());
    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

    ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
    ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );
    ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );

    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}


//-------------------------------------MQTT_EVENT_HANDLER-------------------------------------
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    //mqtt_event_handler_cb(event_data);
    mqtt_handler(event_data);
}


//-------------------------------------MQTT_HANDLER-------------------------------------
void mqtt_handler(esp_mqtt_event_handle_t event){
    if (event->event_id == MQTT_EVENT_DATA){
        printf("TOPIC = %.*s\r\n", event->topic_len, event->topic);
        printf("DATA = %.*s\r\n", event->data_len, event->data);
    }
}


//-------------------------------------FLASH_RW-------------------------------------
void flash_rw(void){

    size_t required_size;
    nvs_get_str(my_handle_ssid, "server_name", NULL, &required_size);
    char* ssid = malloc(required_size);
    char* pass = malloc(required_size);
    esp_err_t err_ssid = nvs_open("ssid", NVS_READWRITE, &my_handle_ssid);
    esp_err_t err_pass = nvs_open("pass", NVS_READWRITE, &my_handle_pass);
    if ( (err_ssid != ESP_OK) || (err_pass != ESP_OK) ) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err_ssid));
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err_pass));
    } else {
        printf("Done\n");
        printf("Reading SSID & PASSWORD from NVS ... ");
        err_ssid = nvs_get_str(my_handle_ssid, "ssid", ssid, &required_size);
        err_pass = nvs_get_str(my_handle_pass, "pass", pass, &required_size);
        switch (err_ssid){
            case ESP_OK:
                printf("Done\n");
                printf("SSID = %s\n", ssid);
                printf("PASSWORD = %s\n", pass);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                initialise_wifi();
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err_ssid));
        }
        nvs_close(my_handle_ssid);
        nvs_close(my_handle_pass);
    }
}