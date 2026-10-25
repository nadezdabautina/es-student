#include "hardware/gpio.h"
#include "led.h"

const uint LED_PIN =25; // вывод светодиода

static bool led_state = false;

void led_init(void) // направление диода 
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    led_state=false;
}

void led_set(bool on) // выставляет уровень и запоминает состояние
{
     led_state=on;
      gpio_put(LED_PIN, led_state); // включение или выключение
}

void led_toggle( void)
{

    led_set(!led_state);

}

bool led_is_on(void)
{
return led_state;

}