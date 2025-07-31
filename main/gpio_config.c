#include "driver/gpio.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include <stdio.h>

#define TAG "gpio_config"
/**
 * @brief Definit of GPIO in use
 *
 */

// Funcție pentru configurarea unui GPIO ca ieșire
void configure_LED_output(int gpio_num) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << gpio_num),  // Selectează GPIO-ul
        .mode = GPIO_MODE_OUTPUT,           // Setează ca ieșire
        .pull_up_en = GPIO_PULLUP_DISABLE,  // Dezactivează pull-up
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  // Dezactivează pull-down
        .intr_type = GPIO_INTR_DISABLE      // Fără întreruperi
    };
    gpio_config(&io_conf);
    ESP_LOGI(TAG,"GPIO %d configurat ca ieșire.", gpio_num);
}
// Funcite pentru configurarea unui GPIO ca intrare
// Aceasta funcție configurează un GPIO specificat ca intrare, cu pull-up activ
void configure_BUTTON_input(int gpio_num) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << gpio_num),  // Selectează GPIO-ul
        .mode = GPIO_MODE_INPUT,           // Setează ca intrare
        .pull_up_en = GPIO_PULLUP_ENABLE,  // Activează pull-up
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  // Dezactivează pull-down
        .intr_type = GPIO_INTR_DISABLE      // Fără întreruperi
    };
    gpio_config(&io_conf);
    ESP_LOGI(TAG,"GPIO %d configurat ca intrare.", gpio_num);
}

/**
 * @brief Configure GPIO's function
 *
 */

// void configure_GPIO_output(int gpio_num) {
//     gpio_config_t io_conf = {
//         .pin_bit_mask = (1ULL << gpio_num),  // Selectează GPIO-ul
//         .mode = GPIO_MODE_OUTPUT,           // Setează ca ieșire
//         .pull_up_en = GPIO_PULLUP_DISABLE,  // Dezactivează pull-up
//         .pull_down_en = GPIO_PULLDOWN_DISABLE,  // Dezactivează pull-down
//         .intr_type = GPIO_INTR_DISABLE      // Fără întreruperi
//     };
//     gpio_config(&io_conf);
//     printf("GPIO %d configurat ca ieșire.\n", gpio_num);
// }

// void configure_GPIO_input(int gpio_num) {
//     gpio_config_t io_conf = {
//         .pin_bit_mask = (1ULL << gpio_num),  // Selectează GPIO-ul
//         .mode = GPIO_MODE_INPUT,           // Setează ca intrare
//         .pull_up_en = GPIO_PULLUP_ENABLE,  // Activează pull-up
//         .pull_down_en = GPIO_PULLDOWN_DISABLE,  // Dezactivează pull-down
//         .intr_type = GPIO_INTR_DISABLE      // Fără întreruperi
//     };
//     gpio_config(&io_conf);
//     printf("GPIO %d configurat ca intrare.\n", gpio_num);
// }
/**
 * @brief Add user defined handler for gpio config
 *
 * @param gpio_handle handle of GPIO
 * @param event_handler user defined event handler
 * @param handler_args handler specific arguments
 * @return esp_err_t
 *  - ESP_OK: Success
 *  - ESP_ERR_NO_MEM: Cannot allocate memory for the handler
 *  - ESP_ERR_INVALIG_ARG: Invalid combination of event base and event id
 *  - Others: Fail
 */
