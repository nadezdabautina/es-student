#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

const uint LED_PIN =25; // вывод светодиода
const uint BUTTON_PIN=15;
const uint DEBOUNCE_MS=20; // задержка чтения 

bool get_button_debounce(uint pin)
{
    bool state = gpio_get(pin); // опрашивает состояние вывода 1 0
    sleep_ms(DEBOUNCE_MS);  // задержка
    return state && gpio_get(pin); // 1 1 - true , false
}

void set_led(bool on)
{
    gpio_put(LED_PIN, on);
    printf("led %s\n", on ? "on" : "off");
}

int main()
{
     gpio_init(BUTTON_PIN);
    stdio_init_all();
    gpio_init(LED_PIN);

    gpio_set_dir(BUTTON_PIN, GPIO_IN); // на вход
    gpio_pull_up(BUTTON_PIN); // подтягивающий резистор
    gpio_set_dir(LED_PIN, GPIO_OUT); // на выход

    bool led=false;
    bool previous = false;
    

    while (1)
    {
        bool current = get_button_debounce(BUTTON_PIN);

        if (previous == true && current == false)
        {
         
            led=!led;
            set_led(led);
        
        }

        previous = current;
    }
}