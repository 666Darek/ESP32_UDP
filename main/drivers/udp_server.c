#include "udp_server.h"
#include "esp_log.h"
#include "lwip/sockets.h"

#define TAG "UDP_SERVER"
#define UDP_SERVER_PORT 12345  // Możesz zmienić na inny port

static void udp_server_task(void *pvParameters)
{
    char rx_buffer[128];
    struct sockaddr_in server_addr, client_addr;
    int sock, len;
    socklen_t client_addr_len = sizeof(client_addr);

    // Tworzenie gniazda
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        ESP_LOGE(TAG, "Nie można utworzyć gniazda: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }

    // Ustawianie adresu dla serwera
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Wiązanie gniazda z adresem
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        ESP_LOGE(TAG, "Błąd podczas wiązania: errno %d", errno);
        close(sock);
        vTaskDelete(NULL);
        return;
    }

    ESP_LOGI(TAG, "Serwer UDP nasłuchuje na porcie: %d", UDP_SERVER_PORT);

    while (1) {
        // Odbieranie danych
        len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&client_addr, &client_addr_len);

        // Wypisanie otrzymanych danych
        if (len > 0) {
            rx_buffer[len] = 0;  // Dodanie znaku końca ciągu
            ESP_LOGI(TAG, "Otrzymano od %s:", inet_ntoa(client_addr.sin_addr));
            ESP_LOGI(TAG, "%s", rx_buffer);
        }
    }
}

esp_err_t udp_server_init(void)
{
    xTaskCreate(udp_server_task, "udp_server_task", 4096, NULL, 5, NULL);
    return ESP_OK;
}
esp_err_t udp_send_to(const char *message, const char *ip_addr, uint16_t port)
{
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        ESP_LOGE(TAG, "Nie można utworzyć gniazda: errno %d", errno);
        return ESP_FAIL;
    }

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    inet_aton(ip_addr, &dest_addr.sin_addr.s_addr);

    int len = sendto(sock, message, strlen(message), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    close(sock);
    ESP_LOGE(TAG, "Poszło: errno %d", errno);
    if (len < 0) {
        ESP_LOGE(TAG, "Nie można wysłać wiadomości: errno %d", errno);
        return ESP_FAIL;
    }

    return ESP_OK;
}
