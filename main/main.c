#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio_config.h"

#define GPIO_LED 2  // Definește GPIO-ul pentru LED

void app_main(void) {
    configure_LED_output(GPIO_LED)

    while (1) {
        gpio_set_level(GPIO_LED, 1);  // Aprinde LED-ul
        printf("LED aprins\n");
        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(GPIO_LED, 0);  // Stinge LED-ul
        printf("LED stins\n");
        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}