#include "wifi_lib.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include <string.h>
#include "lwip/ip4_addr.h"
#include <inttypes.h>
#include "esp_netif.h"




static const char *TAG = "wifi_lib";

static ip4_addr_t s_ip_addr;  // przechowuje adres IP

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "WIFI_EVENT_STA_START triggered. Trying to connect...");
    } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "WIFI_EVENT_STA_DISCONNECTED triggered. Reconnecting...");
    } else if (event_id == WIFI_EVENT_STA_CONNECTED) {
        ESP_LOGI(TAG, "Successfully connected to the WiFi network");
    } else if (event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        s_ip_addr.addr = event->ip_info.ip.addr;
        ESP_LOGI(TAG, "IP_EVENT_STA_GOT_IP triggered. Got IP: %s", ip4addr_ntoa(&s_ip_addr));
    } else {
        ESP_LOGI(TAG, "Unhandled event ID: %" PRId32, event_id);
    }
}


const char* get_ip(void)
{
    return ip4addr_ntoa(&s_ip_addr);
}


esp_err_t wifi_lib_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());  // Używaj tego zamiast tcpip_adapter_init()
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    // Tworzenie domyślnego obiektu esp_netif dla stacji WiFi
    esp_netif_create_default_wifi_sta();
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));
    return ESP_OK;
}



esp_err_t wifi_lib_connect(const char *ssid, const char *password)
{
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "",
            .password = ""
        },
    };
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());
    return ESP_OK;
}


