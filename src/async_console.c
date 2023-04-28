/*
 * async_console.c
 *
 *  Created on: 27.04.2023
 *      Author: manni4
 */

#include <stdio.h>
#include "async_console.h"
#include "panic.h"

#include "pico/stdlib.h"
#include "pico/types.h"
#include "pico/async_context.h"
#include "pico/async_context_poll.h"

async_context_poll_t async_context_console;

struct char_callback_para_t
{
	char c;
	bool has;
} cbp;

async_when_pending_worker_t process_char_worker;

//void chars_available_callback(struct char_callback_para_t *char_callback_para)
void chars_available_callback(void *char_callback_para)
{
	cbp.c = getchar_timeout_us(1);
	cbp.has = true;
	async_context_set_work_pending(&async_context_console.core,
			&process_char_worker);
}

void process_char(async_context_t *context,
		struct async_when_pending_worker *worker)
{
	printf("\nGot char %i", cbp.c);
	cbp.has = false;
}

async_context_t* async_console_init(void)
{
	if (!async_context_poll_init_with_defaults(&async_context_console))
	{
		app_panic("Async context console init fail");
		return NULL ;
	}
	stdio_set_chars_available_callback(chars_available_callback, (void*) &cbp);

	process_char_worker.do_work = process_char;
	async_context_add_when_pending_worker(&async_context_console.core,
			&process_char_worker);

	return &async_context_console.core;
}

