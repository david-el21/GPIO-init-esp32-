idf_component_register(SRCS "gpio_config.c" "main.c"
                    PRIV_REQUIRES nvs_flash driver_gpio
                    INCLUDE_DIRS ".")