#include <rtems.h>
#include <rtems/rtems_bsdnet.h>
#include <rtems/shell.h>
#include "spw.h"
#include <errno.h>     
#define DEBUG_SPACEWIRE_ONOFF

/*******************************************************************************
 * RTEMS Startup Task
 ******************************************************************************/
rtems_task Init (rtems_task_argument ignored)
{
  /* Initialize networking */
  rtems_bsdnet_initialize_network (); //tcp
  rtems_bsdnet_show_inet_routes();    //tcp
  
  int fd; 
  char *dev_name = "/dev/grspw0";

  fd = spw_setup(dev_name);

  /* Initialize shell */
  rtems_shell_init(
    "SHLL",                          /* task_name */
    512*1024,//RTEMS_MINIMUM_STACK_SIZE * 4,    /* task_stacksize */
    100,                             /* task_priority */
    "/dev/console",                   /* devname */
    /* device is currently ignored by the shell if it is not a pty */
    false,                           /* forever */
    true,                            /* wait */
    NULL                             /* login rtems_shell_login_check */
  );

  spw_close(fd);
}


/*******************************************************************************
 * RTEMS Configuration
 ******************************************************************************/

#define CONFIGURE_INIT
rtems_task Init (rtems_task_argument argument);
#include <bsp.h> /* for device driver prototypes */
#include "system.h"
#include "networkconfig.h"



