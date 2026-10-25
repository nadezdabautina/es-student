#include "led.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <string.h>
#include "log.h"
#include "device.h"


#define LINE_SIZE 32

char line[LINE_SIZE];
uint line_length = 0;


const uint BUTTON_PIN=15;
const uint DEBOUNCE_MS=20; // задержка чтения 

bool get_button_debounce(uint pin)
{
    bool state = gpio_get(pin); // опрашивает состояние вывода 1 0
    sleep_ms(DEBOUNCE_MS);  // задержка
    return state && gpio_get(pin); // 1 1 - true , false
}

void handle_command(const char *command)
{
     if (strcmp(command, "enable") == 0)
    {
        led_set(true);
        LOG_INF("led on\n");
    }
    else if (strcmp(command, "disable")==0)
    {
        led_set(false);
        LOG_INF("led off\n");
    }

         else if (strcmp(command, "version")==0)
    {
        log_version();
    }

    else if (strcmp(command,"info")==0)
    {
        device_info();
    }

    else
    {
       LOG_ERR("unknown command: %c\n", command);
    }
   
}

void read_line(void)
{
    typedef void (*handle_command_adress)(const char *);
    handle_command_adress handle_command_fun = handle_command;


    int symbol = getchar_timeout_us(0);

    if (symbol == PICO_ERROR_TIMEOUT) // нет команды
    {
        return;
    }

    if (symbol == '\r' || symbol == '\n') // был нажат enter  в терминале
    {
        putchar('\n'); // посимвольный вывод (вывод новой строки)
        line[line_length] = '\0'; // за последним символом стоит ноль , значит строка

        if (line_length > 0)
        {
            LOG_DBG("got %s\n", line); // отладка
           handle_command_fun (line);
        }

        line_length = 0;
        return;
    }

    if (line_length + 1 < LINE_SIZE) 
    {
        line[line_length] = (char)symbol;
        line_length = line_length + 1;
        putchar(symbol);
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
        
        read_line();
    

    }
}