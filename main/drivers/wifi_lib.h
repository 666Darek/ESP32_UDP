#ifndef WIFI_LIB_H
#define WIFI_LIB_H

#include "esp_err.h"

esp_err_t wifi_lib_init(void);
esp_err_t wifi_lib_connect(const char *ssid, const char *password);
const char* get_ip(void);

#endif // WIFI_LIB_H