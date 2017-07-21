/* Application on RASTA specifically for transmission via SpaceWire *
 * use the Makefile (for RTEMS/simulator on RASTA or Tetris)        */


/* Standard C headers */
#include <stdio.h>
#include <stdlib.h>

/* RTEMS header */
#include <rtems/rtems_bsdnet.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* Configuration */
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 20
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM

#define CONFIGURE_EXECUTIVE_RAM_SIZE (512*1024)
#define CONFIGURE_MAXIMUM_SEMAPHORES 20
#define CONFIGURE_MAXIMUM_TASKS      20

#define CONFIGURE_MICROSECONDS_PER_TICK 10000

#define CONFIGURE_INIT_TASK_STACK_SIZE (10*1024)
#define CONFIGURE_INIT_TASK_PRIORITY   120
#define CONFIGURE_MAXIMUM_DRIVERS 16           // grspw_register(&amba_conf)
#define CONFIGURE_INIT_TASK_INITIAL_MODES (RTEMS_PREEMPT | \
                                           RTEMS_NO_TIMESLICE | \
                                           RTEMS_NO_ASR | \
                                           RTEMS_INTERRUPT_LEVEL(0))

#define CONFIGURE_INIT
rtems_task Init (rtems_task_argument argument);

#include <rtems/confdefs.h>

#include "spw.h"
#include "picture.h"


#define MAX_ITERATIONS 100

/*****************************************************************************/

/* RTEMS Startup Task */
rtems_task Init(rtems_task_argument ignored)
{
  /* Variables for image display */
  uint8_t *videoFrame;
  int width = WIDTH;
  int height = HEIGHT;
  int bpl = BPL;
  int count;
  int fd;
  char *dev_name = "/dev/grspw0";
  char *buffer;


  buffer = (char *) malloc(bpl * height); // 1228800

  fd = spw_setup(dev_name);

  /* Receive data via SpaceWire, process the image, and send back */
  videoFrame = (uint8_t*) malloc(width * height * 4); // 1228800

  // main loop repeats MAX_ITERATIONS times
  for(count = 0; count < MAX_ITERATIONS; count++)
  {
    printf("start the iteration %d \n", count);

    rasta_spw_receive(videoFrame, fd);
    process_image_yuv420(videoFrame, width, height, buffer);
    rasta_spw_sendback(buffer, fd);
  }

  /* Stop the SpaceWire link */
  spw_close(fd);

  exit(EXIT_SUCCESS);
}


