#include "driver/gpio.h"
#include "esp_log.h"
#include <stdio.h>

void configure_LED_output(int gpio_num) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << gpio_num),  // Selectează GPIO-ul
        .mode = GPIO_MODE_OUTPUT,           // Setează ca ieșire
        .pull_up_en = GPIO_PULLUP_DISABLE,  // Dezactivează pull-up
        .pull_down_en = GPIO_PULLDOWN_DISABLE,  // Dezactivează pull-down
        .intr_type = GPIO_INTR_DISABLE      // Fără întreruperi
    };
    gpio_config(&io_conf);
    printf("GPIO %d configurat ca ieșire.\n", gpio_num);
}
