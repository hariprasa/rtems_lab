#include <rtems.h>

/* RTEMS Configuration information */
#include <bsp.h> /* for device driver prototypes */

#define CONFIGURE_MAXIMUM_DRIVERS           32   //grspw_register, 
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK


#define CONFIGURE_MAXIMUM_TASKS             20
#define CONFIGURE_MAXIMUM_SEMAPHORES        20
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES    20
#define CONFIGURE_EXTRA_TASK_STACKS         (1024*1024)

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 32
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM

#define CONFIGURE_STACK_CHECKER_ENABLED
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MALLOC_STATISTICS

#define CONFIGURE_UNIFIED_WORK_AREAS
#define CONFIGURE_UNLIMITED_OBJECTS

#include <rtems/confdefs.h>

/* RTEMS shell configuration*/

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL_NETWORKING
#define CONFIGURE_SHELL_COMMANDS_ALL
//#define CONFIGURE_SHELL_COMMAND_DRVMGR

#include <rtems/shellconfig.h>



//for spacewire
#define CONFIGURE_INIT_TASK_STACK_SIZE (10*1024)
#define CONFIGURE_INIT_TASK_PRIORITY   120
#define CONFIGURE_INIT_TASK_INITIAL_MODES (RTEMS_PREEMPT | \
                                           RTEMS_NO_TIMESLICE | \
                                           RTEMS_NO_ASR | \
                                           RTEMS_INTERRUPT_LEVEL(0))

#define CONFIGURE_EXECUTIVE_RAM_SIZE    (1024*1024)

//#include <drvmgr/drvmgr.h>
//#define CONFIGURE_DRIVER_AMBAPP_GAISLER_GRSPW
//#include <drvmgr/drvmgr_confdefs.h>

/* end of include file */

#if 0
/* GRSPW0 resources https://devel.rtems.org/wiki/TBR/UserManual/DriverManager */
struct rtems_drvmgr_key grlib_grspw0_res[] =

{
	{"txDesc", KEY_TYPE_INT, {(unsigned int)16}},
	{"rxDesc", KEY_TYPE_INT, {(unsigned int)32}},
	KEY_EMPTY
};
/* GRSPW1 and GRSPW2 resources */
struct rtems_drvmgr_key grlib_grspw1_res[] =

{
	{"txDesc", KEY_TYPE_INT, {(unsigned int)32}},
	{"rxDesc", KEY_TYPE_INT, {(unsigned int)64}},
	KEY_EMPTY
};
/* GRLIB Plug & Play bus driver resources */
struct rtems_drvmgr_drv_res grlib_drv_resources[] =

{
	{DRIVER_AMBAPP_GAISLER_GRSPW_ID, 0, &grlib_grspw0_res[0]},
	{DRIVER_AMBAPP_GAISLER_GRSPW_ID, 1, &grlib_grspw1_res[0]},
	{DRIVER_AMBAPP_GAISLER_GRSPW_ID, 2, &grlib_grspw1_res[0]},
	RES_EMPTY
};
#endif
