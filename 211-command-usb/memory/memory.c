#include "memory.h"
#include <stdio.h>
#include <stdint.h>
#include "hardware/regs/addressmap.h"
#include "pico/stdlib.h"
#include "command.h"
#include <stdlib.h>
#include "device.h"


extern char __flash_binary_start; // глобльная переменная
extern char __flash_binary_end;
extern char __boot2_start__;
extern char __boot2_end__;
extern char __etext;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __HeapLimit;
extern char __StackBottom;
extern char __StackTop;


static void row(const char *name, uintptr_t start, uintptr_t end)
{
    printf("%-10s 0x%08x 0x%08x %8u\n",
           name, (unsigned)start, (unsigned)end, (unsigned)(end - start));
}

void mem_info(void)
{


    printf("%-10s %-10s %-10s %-10s\n", "area", "start" , "end", "size");
    printf("%-10s 0x%08x 0x%08x %u\n","flash", XIP_BASE,(XIP_BASE + PICO_FLASH_SIZE_BYTES ), PICO_FLASH_SIZE_BYTES);
   printf("%-10s 0x%08x 0x%08x %-10s\n","sram", SRAM_BASE, SRAM_END, "270336");
    printf("%-10s 0x%08x 0x%08x %-10s\n","rom", ROM_BASE, ROM_BASE + 16384 ,"16384");
    row("image", (uintptr_t)&__flash_binary_start, (uintptr_t)&__flash_binary_end);
    row("free", (uintptr_t)&__flash_binary_end, (uintptr_t)(XIP_BASE + PICO_FLASH_SIZE_BYTES ));//
    row("boot2", (uintptr_t)&__boot2_start__, (uintptr_t)&__boot2_end__); 
    row("text", (uintptr_t)&__boot2_end__, (uintptr_t)&__etext); 
    row("data flash",(uintptr_t)&__etext,
    (uintptr_t)&__etext + (uintptr_t)&__data_end__ - (uintptr_t)&__data_start__);
    row("data ram", (uintptr_t)&__data_start__, (uintptr_t)&__data_end__); 
    row("bss", (uintptr_t)&__bss_start__, (uintptr_t)&__bss_end__);
    row("heap", (uintptr_t)&__bss_end__, (uintptr_t)&__HeapLimit);
    row("stack", (uintptr_t)&__StackBottom, (uintptr_t)&__StackTop);


    printf("total\n");

    uint flash_image = (uintptr_t)(&__etext-&__boot2_end__  )+
    (uintptr_t)(&__boot2_end__-&__boot2_start__ )+
    (uintptr_t)(&__data_end__-&__data_start__ );

    uintptr_t ram_used = (uintptr_t)(&__bss_end__ - &__bss_start__)+
    (uintptr_t)(&__data_end__-&__data_start__);

    printf("%-10s %4u %c %s %4u %c %s %4u %c %s %4u\n ","flash image:", flash_image, '=',
         "boot2", (uintptr_t)(&__boot2_end__-&__boot2_start__ ), '+',
        "text", (uintptr_t)(&__etext-&__boot2_end__), '+',
    "data", (uintptr_t)(&__data_end__-&__data_start__ ) );

    printf("%-10s %4u %2s %4u\n","flash free", PICO_FLASH_SIZE_BYTES-flash_image, "of", PICO_FLASH_SIZE_BYTES);
    printf("%-10s %4u %c %s %4u %c %s %4u\n", "ram used", ram_used, '=', "data", (uintptr_t)(&__data_end__-&__data_start__),
    '+', "bss", (uintptr_t)(&__bss_end__ - &__bss_start__));

    printf("%-10s %4u %s %4u %s\n", "ram free", (uintptr_t)(&__HeapLimit-&__bss_end__), "for heap and",
    (uintptr_t)(&__StackTop-&__StackBottom), "for stack" );

}

int main(void);

uint32_t data_variable = 100;
uint32_t bss_variable;

void fw_info(void)
{
    data_variable++;
    bss_variable++;

    uint16_t *main_code = (uint16_t *)((uintptr_t)main);
    uint16_t *main_code1 = (uint16_t *)((uintptr_t)main & ~1u);
    uint16_t *fw_info_code = (uint16_t *)((uintptr_t)fw_info);
    uint16_t *fw_info_code1 = (uint16_t *)((uintptr_t)fw_info& ~1u);


    uint32_t stack_variable = 1946;
    uint32_t *heap_variable = malloc(sizeof(uint32_t));

    if (heap_variable != NULL)
    {
        *heap_variable = 1951;
    }
    
     const struct command_t *commands_address = commands;

     printf("%-10s %-10s %-10s\n", "object", "address" , "value");
     printf ("%-10s %08x %#06x\n", "main", main_code, main_code1[0]);
     printf ("%-10s %08x %#06x\n", "fw_info", fw_info_code, fw_info_code1[0]) ;
    printf ("%-10s %08x\n", "commands", commands_address);
   for (int i = 0; i < command_count; i++)
    {
        printf(" %-10s %08x\n", (commands_address + i)->name, (commands_address + i)->handler);
    }
    printf ("%-10s %08x %-8s\n", "DEVICE_PROJECT", DEVICE_PROJECT, DEVICE_PROJECT);
    printf ("%-10s %08x  %-8s\n", "DEVICE_BOARD", DEVICE_BOARD, DEVICE_BOARD);
    printf ("%-10s %08x %8u\n", "data_variable", (uint32_t)(&data_variable),*(uint32_t *)(&data_variable));
    printf ("%-10s %08x %8u\n", "bss_variable", (uint32_t)(&bss_variable),*(uint32_t *)(&bss_variable));
    printf ("%-10s %08x %8u\n", "stack_variable", (uint32_t)(&stack_variable),*(uint32_t *)(&stack_variable));
    printf ("%-10s %08x %8u\n", "heap_veriable", heap_variable, *heap_variable);

    free(heap_variable);
}