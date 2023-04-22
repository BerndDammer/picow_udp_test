#include <stdio.h>
#include "pico/stdlib.h"
//#include "hardware/timer.h"
//#include "pico/cyw43_arch.h"

#include "udp_loop.h"



int main()
{
    stdio_init_all();


    puts("\nStarting");
    udp_loop();

    return 0;
}
