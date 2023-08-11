/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "drivers/wifi_lib.h"
#include "drivers/udp_server.h"

#include "nvs_flash.h"
static const char *TAG = "example";






void init_gpio(int pin) {
    gpio_reset_pin(pin); 
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}

void blink_task(void *pvParameter) {
    int pin = (int) pvParameter;
    while(1) {
        gpio_set_level(pin, 1); // Włącz diodę
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Odczekaj 1000ms = 1s
        gpio_set_level(pin, 0); // Wyłącz diodę
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Odczekaj 1000ms = 1s
        ESP_LOGI("main", "Starting WiFi...");
    }
}

void run_blink_task(int pin) {
    init_gpio(pin);

    xTaskCreate(
        blink_task,
        "Blinking Task",
        2048,
        (void*) pin,
        5,
        NULL
    );
}





void app_main(void)
{
    // Inicjalizacja NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI("main", "Starting WiFi...");
    
    wifi_lib_init();
    wifi_lib_connect("TBST", "ebs260EBS");
    
    // Odczekaj pewien czas (np. 10 sekund) aby dać czas na uzyskanie adresu IP
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    
    ESP_LOGI(TAG, "Connected to WiFi with IP: %s", get_ip());
udp_send_to("Cześć!", "192.168.10.255", 12345);

    // run_blink_task(4);
   // udp_server_init();

}


