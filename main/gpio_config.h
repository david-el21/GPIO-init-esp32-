#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#include "driver/gpio.h"
/**
 * @brief Configure OUTPUT PIN for LED 
 * 
* @param gpio_num GPIO PIN_NUM
 */
void configure_LED_output(int gpio_num);
/**
 * @brief Config INPUT PIN for BUTTON
 * 
* @param gpio_num GPIO PIN_NUM
 */
void configure_BUTTON_input(int gpio_num);

/**
 * @brief Config user GPIO PIN
 * 
 *
 *
 *
 *
 */
#endif // GPIO_CONFIG_H
