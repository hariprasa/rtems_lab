/*
 * Network configuration
 */

#ifndef _RTEMS_NETWORKCONFIG_H_
#define _RTEMS_NETWORKCONFIG_H_

#include <rtems/rtems_bsdnet.h>
#include <bsp.h> 

/* Default network interface */
static struct rtems_bsdnet_ifconfig netdriver_config = {
        RTEMS_BSP_NETWORK_DRIVER_NAME,          /* name */
        RTEMS_BSP_NETWORK_DRIVER_ATTACH,        /* attach function */
        NULL,                           /* next interfaces */
        "192.168.0.80",                 /* IP address */
        "255.255.255.0",                /* IP net mask */
        NULL,                           /* Driver supplies hardware PHY address */
        0,                              /* ignore broadcast */
        0,                              /* mtu */
        0,                              /* rbuf_count */
        0,                              /* xbuf_count */
        0,                              /* port */
        0,                              /* irq */
        0,                              /* bpar */
        NULL                            /* driver control pointer */
};

/*
 * Network configuration
 */
struct rtems_bsdnet_config rtems_bsdnet_config = {
	&netdriver_config,	/* link to next interface */
	NULL,			/* BOOTP */
	100,			/* Default network task priority */
	128*1024,		/* Default mbuf capacity */
	256*1024,		/* Default mbuf cluster capacity */
	"rtems_host",		/* Host name */
	"localnet",		/* Domain name */
	"192.168.0.1",		/* Gateway */
	"192.168.0.1",		/* Log host */
	{"192.168.0.1" },	/* Name server(s) */
	{"192.168.0.1" },	/* NTP server(s) */
	0,                      /* sb_efficiency */
	0,                      /* udp_tx_buf_size */
	0,                      /* udp_rx_buf_size */
	0,                      /* tcp_tx_buf_size */
	0                       /* tcp_rx_buf_size */
};

#endif /* _RTEMS_NETWORKCONFIG_H_ */
