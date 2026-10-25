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


void cmd_enable (void)
{
        led_set(true);
        LOG_INF("led on\n");
}

void cmd_disable (void)
{
        led_set(false);
        LOG_INF("led off\n");
}

void cmd_info (void)
{
    device_info();
}

void cmd_version (void)
{
    log_version();
}

void cmd_ping(void)
{
    printf("pong\n");
}

typedef void (*command_handler_t)(void);

struct command_t // описание (название, адрес функции)
{
    const char *name;
    command_handler_t handler;
};



const struct command_t commands[] = { // таблица функций 
    { "enable", cmd_enable },
    { "disable", cmd_disable },
    { "info", cmd_info },
    { "version", cmd_version },
    {"ping", cmd_ping}
};

#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

void handle_command(const char *command)
{
    for (uint i = 0; i < COMMAND_COUNT; i++)
    {
        if (strcmp(command, commands[i].name) == 0)
        
        {
            if (commands[i].handler != NULL)
            {
                commands[i].handler();
            }

            return;
        }
    }

    LOG_ERR("unknown command: %s\n", command);
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