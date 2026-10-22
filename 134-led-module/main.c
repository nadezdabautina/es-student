#include "led.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include "log.h"
#include "device.h"


const uint BUTTON_PIN=15;
const uint DEBOUNCE_MS=20; // задержка чтения 

bool get_button_debounce(uint pin)
{
    bool state = gpio_get(pin); // опрашивает состояние вывода 1 0
    sleep_ms(DEBOUNCE_MS);  // задержка
    return state && gpio_get(pin); // 1 1 - true , false
}


void handle_command(int command)
{
    if (command == 'e')
    {
        led_set(true);
        LOG_INF("led on\n");
    }
    else if (command == 'd')
    {
        led_set(false);
        LOG_INF("led off\n");
    }

         else if (command == 'v')
    {
        log_version();
    }

    else if (command=='i')
    {
        device_info();
    }

    else
    {
       LOG_ERR("unknown command: %c\n", command);
    }
   
}

int main()
{

    led_init();

     gpio_init(BUTTON_PIN);
    stdio_init_all();
    
    gpio_set_dir(BUTTON_PIN, GPIO_IN); // на вход
    gpio_pull_up(BUTTON_PIN); // подтягивающий резистор


    bool led=false;
    bool previous = false;
    

    while (1)
    {
        bool current = get_button_debounce(BUTTON_PIN);


        if (previous == true && current == false)
        {
         
            led_toggle();
             LOG_INF("led on\n");
        
        }

        previous = current;
    

      int command = getchar_timeout_us(0);

        if (command == PICO_ERROR_TIMEOUT)
        {
            continue;
        }

        LOG_DBG ("got %c\n", command);
         handle_command(command);
    }
}