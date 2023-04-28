/*
 * async_lwip.c
 *
 *  Created on: 25.04.2023
 *      Author: manni4
 */


#include "pico/stdlib.h"
#include "pico/types.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/igmp.h"

#include "pico/async_context.h"
#include "pico/async_context_poll.h"
#include "pico/lwip_nosys.h"

#include "panic.h"
#include "async_lwip.h"

async_context_poll_t async_context_lwip;

async_context_t *async_lwip_init(void)
{
	if (!async_context_poll_init_with_defaults(&async_context_lwip))
	{
		app_panic("Async context lwip init fail");
		return NULL;
	}

	bool success;
	success = lwip_nosys_init(&async_context_lwip.core);
	if(!success)
	{
		app_panic("lwip init fail");
		return NULL;
	}

	return &async_context_lwip.core;
}

