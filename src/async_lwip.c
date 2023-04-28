/*
 * async_lwip.c
 *
 *  Created on: 25.04.2023
 *      Author: manni4
 */

/************************
 *
 *
 * TRAP QQQQ ?????
 *
 * lwip and cyw43 must be initialized with the same
 * async_context ???
 *
 *
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

#define UDP_PORT 50000
#define UDP_PORT_TRANS 50100
#define HEARTBEAT_TIMEOUT_MS 1000

async_context_t *async_context_lwip;

async_at_time_worker_t s_transmit_heartbeat;

void recCallBack(void *arg, struct udp_pcb *pcb, struct pbuf *p,
		const ip_addr_t *addr, u16_t port)
{
	printf("\nReceive\n");
}

static char msg[] = "ooejtkperjgropejgrep";
struct ip4_addr multicast_destination;
struct udp_pcb *g_udppcb;

void transmit_heartbeat(async_context_t *context,
		struct async_work_on_timeout *worker)
{

	struct pbuf *p;
	p = pbuf_alloc(PBUF_TRANSPORT, sizeof(msg), PBUF_RAM);
	memcpy(p->payload, msg, sizeof(msg));

	udp_sendto(g_udppcb, p, &multicast_destination, UDP_PORT_TRANS); //send a multicast packet




	async_context_add_at_time_worker_in_ms(async_context_lwip,
			&s_transmit_heartbeat, HEARTBEAT_TIMEOUT_MS);
}

void async_lwip_init(async_context_t *asc)
{
	bool success;
	async_context_lwip = asc;

	success = lwip_nosys_init(async_context_lwip);
	if (!success)
	{
		app_panic("lwip init fail");
	}

	//--- add multicast receive
	{
		struct ip4_addr localIP;

		IP4_ADDR(&multicast_destination, 224, 0, 0, 1); //Multicast IP address.
		IP4_ADDR(&localIP, 192, 168, 178, 35); //Interface IP address

		s8_t iret = igmp_joingroup((ip4_addr_t*) (&localIP),
				(ip4_addr_t*) (&multicast_destination));

		g_udppcb = (struct udp_pcb*) udp_new();
		udp_bind(g_udppcb, NULL, UDP_PORT); //to allow receiving multicast
		udp_recv(g_udppcb, recCallBack, NULL); //recCallBack is the callback function that will be called every time you    receive multicast
	}
	{
		s_transmit_heartbeat.do_work = transmit_heartbeat;
		async_context_add_at_time_worker_in_ms(async_context_lwip,
				&s_transmit_heartbeat, HEARTBEAT_TIMEOUT_MS);
	}
}

