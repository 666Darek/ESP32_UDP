#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "esp_err.h"

/**
 * @brief Inicjalizuje serwer UDP
 *
 * @return ESP_OK jeśli sukces, inny kod błędu w przeciwnym przypadku
 */
esp_err_t udp_server_init(void);
esp_err_t udp_send_to(const char *message, const char *ip_addr, uint16_t port);
#endif // UDP_SERVER_H
