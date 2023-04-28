/*
 * async_main.c
 *
 *  Created on: 24.04.2023
 *      Author: manni4
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//#include "pico/stdlib.h"
#include "pico/types.h"

#include "pico/async_context.h"
#include "pico/async_context_poll.h"

#include "async_main.h"
#include "async_cyw43.h"
#include "async_lwip.h"
#include "async_console.h"
#include "panic.h"


volatile unsigned int poll_counter;
void async_main_loop(void)
{
	async_context_t *async_context_cyw43;
	async_context_t *async_context_lwip;;
	async_context_t *async_context_console;

	printf("\nStarting");
	printf("\nTime ms since start %i", //
			to_ms_since_boot(get_absolute_time()) //
					);

	async_context_cyw43 = async_cyw43_init();
	if (async_context_cyw43 == NULL )
	{
		app_panic("cyw43 iii");
	}
	async_context_lwip = async_lwip_init();
	if (async_context_lwip == NULL )
	{
		app_panic("lwip iii");
	}
	async_context_console = async_console_init();
	if (async_context_console == NULL )
	{
		app_panic("console iii");
	}
	while (true)
	{
		async_context_poll( async_context_cyw43);
		//async_context_poll( async_context_lwip);
		async_context_poll( async_context_console);

		poll_counter++;
	}
}

