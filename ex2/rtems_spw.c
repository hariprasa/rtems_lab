

/* Standard C headers */
#include <stdio.h>      // printf()
#include <fcntl.h>      // open()
#include <errno.h>      // errno
#include <stdlib.h>     // abort()
#include <sys/ioctl.h>  // ioctl()
#include <sched.h>      // sched_yield()


/* SpaceWire on RASTA */
#include <rtems.h>      // define all RTEMS specific types
#include <bsp/grspw.h>      // grspw_register()
#include <amba.h>       // amba_conf

#include "spw.h"

void print_config(spw_config *cnf);
#define OPTION_INFO_ALL   0xffffffff

/*****************************************************************************/
int spw_setup(char *dev_name)
{
  int ret_val;
  int fd;
  spw_config *config = (spw_config *)malloc(sizeof(spw_config));


  //ret_val = grspw_register(&amba_conf); /*RTEMS 4.10*/

  //grspw_register_drv(); /*RTEMS 4.12*/
  grspw_print(1);
  printf("grspw_register_drv(): finished\n");
  rtems_task_wake_after(1000);
  grspw2_register_drv(); /*RTEMS 4.12*/
  grspw_print(1);
  printf("grspw2_register_drv(): finished\n");
  rtems_task_wake_after(1000);

  /* Open device */
  fd = open(dev_name, O_RDWR);
  if (fd < 0)
  {
    printf("Error Opening %s, errno: %d \n",dev_name, errno);
    abort();
  }

  printf("/dev/grspw0 is opened \n");

  printf("**** Checking initial configuration for /dev/grspw0 ****\n");
  printf("\n");
  if (ioctl(fd, SPACEWIRE_IOCTRL_GET_CONFIG, config) == -1) {
      printf("ioctl failed: SPACEWIRE_IOCTRL_GET_CONFIG \n");
      exit(0);
  }
  print_config(config);

  /* Make sure link is up */
  while( ioctl(fd, SPACEWIRE_IOCTRL_START, 0) == -1 )
  {
    //printf("SPACEWIRE_IOCTRL_START failed, errno: %d \n", errno);
    sched_yield();
  }

  printf("/dev/grspw0: link is started \n");

  /* Set blocking mode for transmissions */
  if ( ioctl(fd, SPACEWIRE_IOCTRL_SET_TXBLOCK, 1) == -1 )  // blocking
    printf("SPACEWIRE_IOCTRL_SET_TXBLOCK, errno: %d \n", errno);

  /* Set the node address of the device, VERY IMPORTANT for read() */
  if (ioctl(fd, SPACEWIRE_IOCTRL_SET_NODEADDR, NODE_ADR_RX) == -1)
  {
    printf("ioctl failed: SPACEWIRE_IOCTRL_SET_NODEADDR \n");
    exit(0);
  }

  /* Set blocking receiving mode */
  if ( ioctl(fd, SPACEWIRE_IOCTRL_SET_RXBLOCK, 1) == -1 )  // blocking
    printf("SPACEWIRE_IOCTRL_SET_RXBLOCK, errno: %d \n", errno);

  return(fd);
}


void spw_close(int fd)
{
  close(fd);
}


/* SpaceWire RECEIVE */
void rasta_spw_receive(uint8_t *videoFrame, int fd)
{
  int len;
  int i;

  for(i = 0; i < 450; i++)
  {
    len = read(fd, (char*)videoFrame + 1024 * i, 1024);
    // packetSize = 1024 (bytes), (460800 = 1024 * 450 packets)
    // only receive 1023 useful bytes per pack

    if (len < 0)
    {
      printf("Error during read, errno: %d \n", errno);
      printf("Received %d bytes \n", len);
      // to show incorrect result
    }
    else
    {
      printf("Reception successful (i = %d), received %d bytes \n", i, len);
      // the string starts from buf2[0]
    }
  }
}


/* SpaceWire SEND BACK */
void rasta_spw_sendback(char *buffer, int fd)
{
  int result;
  int i;


  // 1228800 bytes = 1024 bytes per pack * 1200 packets
  for(i = 0; i < 1200; i++)
  {
    result = write(fd, buffer + i*1024, 1024); // 1024 bytes per pack
                                               // without NODE_ADR_RX
    if (result < 0)
      printf("Transmission failed at /dev/grspw0 \n");
    else
      printf("Transmission successful (i = %d), sent %d bytes \n", i, result);
  }

  if (buffer != NULL)
    free (buffer);
}

/* SpaceWire Configuration */
void print_config(spw_config *cnf) 
{
        printf("\n");
        printf(" ******** CONFIG ********  \n");
        printf("Node Address: %i\n", cnf->nodeaddr);
        printf("Destination Key: %i\n", cnf->destkey);
        printf("Clock Divider: %i\n", cnf->clkdiv);
        printf("Rx Maximum Packet: %i\n", cnf->rxmaxlen);
        printf("Timer: %i\n", cnf->timer);
        printf("Linkdisabled: %i\n", cnf->linkdisabled);
        printf("Linkstart: %i\n", cnf->linkstart);
        printf("Disconnect: %i\n", cnf->disconnect);
        printf("Promiscuous: %i\n", cnf->promiscuous);
        printf("RMAP Enable: %i\n", cnf->rmapen);
        printf("RMAP Buffer Disable: %i\n", cnf->rmapbufdis);
        printf("Check Rmap Error: %i\n", cnf->check_rmap_err);
        printf("Remove Protocol ID: %i\n", cnf->rm_prot_id);
        printf("Blocking Transmit: %i\n", cnf->tx_blocking);
        printf("Disable when Link Error: %i\n", cnf->disable_err);
        printf("Link Error IRQ Enabled: %i\n", cnf->link_err_irq);
        printf("Link Error Event Task ID: %i\n", cnf->event_id);
        printf("RMAP Available: %i\n", cnf->is_rmap);
        printf("RMAP CRC Available: %i\n", cnf->is_rmapcrc);
        printf("Unaligned Receive Buffer Support: %i\n", cnf->is_rxunaligned);
        printf("Read Timeout: %i\n", cnf->rtimeout);
        printf("Keep source address in userbuffer: %i\n", cnf->keep_source);
        printf("\n");
}






