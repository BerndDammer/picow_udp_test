/*
 * udp_loop.c
 *
 *  Created on: 20.04.2023
 *      Author: manni4
 */

#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/types.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/igmp.h"

#include "udp_loop.h"
#include "parameter.h"

#define CONNECT_TIMEOUT_MS 10000
#define CONSOLE_TIMEOUT 100*1000
#define ECHO_MUL 5

#define UDP_PORT 50000
#define UDP_PORT_TRANS 50100


static void udp_loop_menu(void)
{
	printf("------------------------------------\n");
	printf("LWIP UDP Test1\n");
	printf("-------------------------------------\n");
	printf("1 Start\n");
	printf("0 Stop\n");
	printf("press key to select\n");
	printf("------------------------------------\n");
}

bool cyw_running = false;
static bool blinker_state;

static char msg[] = "ooejtkperjgropejgrep";

static void blinker_toggle(void)
{
	if (cyw_running)
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, blinker_state =
				!blinker_state);
}

static void recCallBack(void *arg, struct udp_pcb *pcb, struct pbuf *p,
	    const ip_addr_t *addr, u16_t port )
{
	printf("\nReceive\n");
}

static void udp_loop_init(void)
{
	struct ip4_addr ipgroup, localIP;
	struct udp_pcb *g_udppcb;

	struct pbuf *p;
	p = pbuf_alloc(PBUF_TRANSPORT, sizeof(msg), PBUF_RAM);
	memcpy(p->payload, msg, sizeof(msg));

	IP4_ADDR(&ipgroup, 224, 0, 0, 1); //Multicast IP address.
	IP4_ADDR(&localIP, 192, 168, 178, 35); //Interface IP address

	if (cyw43_arch_init())
	{
		printf("failed to initialise\n");
		return;
	}

	cyw43_arch_enable_sta_mode();

	printf("Connecting to Wi-Fi...\n");
	if (cyw43_arch_wifi_connect_timeout_ms(PARA_SSID, PARA_PWD,
	CYW43_AUTH_WPA2_AES_PSK, CONNECT_TIMEOUT_MS))
	{
		printf("failed to connect.\n");
		return;
	}
	else
	{
		printf("Connected.\n");
	}

	s8_t iret = igmp_joingroup((ip4_addr_t*) (&localIP),
			(ip4_addr_t*) (&ipgroup));
	g_udppcb = (struct udp_pcb*) udp_new();
//	udp_bind(g_udppcb, &localIP, UDP_PORT); //to allow receiving multicast
	udp_bind(g_udppcb, NULL, UDP_PORT); //to allow receiving multicast
	udp_recv(g_udppcb, recCallBack, NULL); //recCallBack is the callback function that will be called every time you    receive multicast
	udp_sendto(g_udppcb, p, &ipgroup, UDP_PORT_TRANS); //send a multicast packet
}

static void udp_loop_exit(void)
{
	cyw43_arch_deinit();
}

void udp_loop(void)
{
	volatile int c; // make visible in debugger; avoid optimize out
	int counter = 0;
	int mul_counter = 0;


	udp_loop_menu();

	sleep_ms(100);
	sleep_ms(100);

	while (1)
	{
		c = getchar_timeout_us(CONSOLE_TIMEOUT);

		if (c == PICO_ERROR_TIMEOUT)
		{
			if (cyw_running)
			{
				cyw43_arch_poll();
			}
			counter++;
			if (mul_counter-- < 0)
			{
				mul_counter = ECHO_MUL;
				printf("Console Loop %i     running %i\n", counter,
						cyw_running);
				blinker_toggle();
			}
		}
		else
		{
			switch (c)
			{
			case '0':
				if (cyw_running)
				{
					udp_loop_exit();
					cyw_running = false;
				}
				break;
			case '1':
				if (!cyw_running)
				{
					udp_loop_init();
					cyw_running = true;
				}
				break;
			case ' ':
			default:
				udp_loop_menu();
				break;
			}
		}
	}
}
