# GPIO Initialization

Acest modul configurează pini GPIO ca ieșire pentru LED-uri sau ca intrare pentru butoane, cu suport pentru pull-up.

## Fișiere
- `gpio_config.c` – funcții de inițializare GPIO
- `gpio_config.h` – declarații publice

## Funcții disponibile
```c
void configure_LED_output(int gpio_num);
void configure_BUTTON_input(int gpio_num);
## Example folder contents

The project **sample_project** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.
##Exemplu de utilizare
#include "gpio_config.h"

#define LED_GPIO    2
#define BUTTON_GPIO 0

void app_main() {
    configure_LED_output(LED_GPIO);
    configure_BUTTON_input(BUTTON_GPIO);
}

```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
```
Additionally, the sample project contains Makefile and component.mk files, used for the legacy Make based build system. 
They are not used or needed when building with CMake and idf.py.
