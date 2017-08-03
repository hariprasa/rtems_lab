/* Standard C header files */
#include <stdlib.h> 
#include <assert.h> 
#include <stdio.h> 
#include <stdlib.h> 

/* RTEMS header files */
#include <bsp.h> 
#include <rtems.h>
#include <rtems/bspIo.h> 
#include <rtems/cpuuse.h> 

/* Functions */
rtems_task Init(rtems_task_argument argument);
rtems_task The_Task(unsigned int Task_num);

/****************************************************************************/
/* RTEMS resources configuration                                            */
/****************************************************************************/

#define MAX_TASKS 2
#define MAX_PERIODS 100 

/* RTEMS system configuration informations */
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER 
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS          9
#define CONFIGURE_EXTRA_TASK_STACKS     (5 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_MAXIMUM_PERIODS        10000
//#define CONFIGURE_MICROSECONDS_PER_TICK  100 // this affects the scheduling 

#define CONFIGURE_MAXIMUM_BARRIERS 1

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

/****************************************************************************/
/* Task configuration                                                       */
/****************************************************************************/

rtems_task_priority Priorities[9] = {10, 20, 21, 22, 23, 24, 25, 26, 27}; 
int                 Periods[9]    = {50, 100, 200, 300, 400, 500, 600, 700, 800}; 
rtems_id            Task_id[9]    = {0, 0, 0, 0, 0, 0, 0, 0, 0};    
rtems_name          Task_name[9]  = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int                 Done[9]       = {0, 0, 0, 0, 0, 0, 0, 0, 0}; 
int                 Periods_Done[9]= {0, 0, 0, 0, 0, 0, 0, 0, 0};
int                 Total_Passes  = 0; 
rtems_id            initialization_barrier;
#define MAX_TOTAL_PASSES 10
#define RMS_WORKLOAD 320
/****************************************************************************/
/* The_Task(): body of the task                                             */
/****************************************************************************/

rtems_task The_Task(unsigned int Task_num){ 
  rtems_id          RMS_Id; 
  rtems_status_code Status; 
  rtems_mode        Previous_Mode; 
  int               My_Counter;
  int               My_Period_Passes = 0;
  int               My_Num           = Task_num;
  rtems_name	    name;
  uint32_t          count=0;
  
  printf ("Task %d-- : starting\n", My_Num); 
  /* Set preemption */ 
  Status = rtems_task_mode(RTEMS_PREEMPT, RTEMS_PREEMPT_MASK, &Previous_Mode); 
  if (Status != RTEMS_SUCCESSFUL) 
    printf("Task %d : rtems_task_mode failed\n", My_Num); 
  
  /* Barrier wait */
  printf("Task %d : Task wait for the barrier\n", My_Num);
  Status = rtems_barrier_wait (initialization_barrier, RTEMS_WAIT);
  if (Status != RTEMS_SUCCESSFUL)
  {
     printf("Task %d : Error while waiting for the barrier\n", My_Num);
     return (0);
  }
  printf("Task %d : Task release the barrier\n", My_Num);

  /* Create RMS control block */ 
  name = rtems_build_name('P','E','R','D');
  Status = rtems_rate_monotonic_create(name, &RMS_Id);
  if ( Status != RTEMS_SUCCESSFUL ) {
    printf( "rtems_monotonic_create failed with status of %d.\n", (int)Status );
    exit( 1 );
    }
  
  while (1){ /* RMS  loop */ 
    count++;
  /*Initiate RM Period */
  Status = rtems_rate_monotonic_period(RMS_Id,Periods[My_Num]);
  if(  Status == RTEMS_TIMEOUT )
     	printf("Task %d-- :rtems_monotonic_period missed the Deadline!\n",My_Num);
  else if (Status != RTEMS_SUCCESSFUL) {
      printf("Task %d-- :rtems_monotonic_period failed with status: %d\n",My_Num, Status);
      break;
    }
  
  /* Work to be done (RMS_WORKLOAD is C coefficient)*/ 
  for (My_Counter = 0; My_Counter <= RMS_WORKLOAD; My_Counter++) {};  

  My_Period_Passes++;  

  Total_Passes++; 

  Periods_Done[My_Num] = My_Period_Passes;

  printf("Task %d-- : is running in pass : %d \n",My_Num, My_Period_Passes); 
  
  if ((count%20)==0)
//    rtems_rate_monotonic_report_statistics();

  if (My_Period_Passes >= MAX_TOTAL_PASSES)
      break; 
  }  /* RMS loop end */ 

  Done[My_Num]=1;

  Status = rtems_rate_monotonic_delete(RMS_Id);

  if ( Status != RTEMS_SUCCESSFUL ) {
   printf( "Task %d-- :rtems_rate_monotonic_delete failed with status of %d.\n", My_Num, Status );
   exit( 1 );
  }

  printf( "Task %d-- :RM block Deleted and deleting the task\n", My_Num );

  Status = rtems_task_delete(RTEMS_SELF);    /* should not return */
  printf( "Task %d-- :rtems_task_delete returned with status of %d.\n",My_Num, Status );
  exit( 1 );
}

/****************************************************************************/
/* Init(): main entrypoint                                                  */
/****************************************************************************/

rtems_task Init( rtems_task_argument ignored ){ 
  rtems_status_code    status; 
  rtems_task_priority  the_priority; 
  rtems_id             id; 
  rtems_time_of_day    time; 
  rtems_mode           Previous_Mode; 
  rtems_interval       ticks_per_second;


  int j;  
  char Char_Name; 
  int Main_Done =0; 
  uint32_t count=0;
	
  /* Clock value */ 
  time.year   = 2011; 
  time.month  = 10; 
  time.day    = 11; 
  time.hour   = 9; 
  time.minute = 0; 
  time.second = 0; 
  time.ticks  = 0; 

//>   rtems_interval ticks;
//> -  rtems_clock_get(RTEMS_CLOCK_GET_TICKS_SINCE_BOOT,&ticks);
//> +  ticks = rtems_clock_get_ticks_since_boot();

  status = rtems_clock_set( &time );  
  
  printf ("main-- starting\n"); 

  ticks_per_second = rtems_clock_get_ticks_per_second(); //for rtems 4.12
  //rtems_clock_get(RTEMS_CLOCK_GET_TICKS_PER_SECOND, &ticks_per_second); 
  printf("main-- ticks_per_second : %d\n", (int)ticks_per_second); 

  status = rtems_task_set_priority(RTEMS_SELF, 50 , &the_priority); 

  /* Check current priority  */ 
  status = rtems_task_set_priority
    (RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &the_priority); 
  printf("main-- initial current priority : %d\n", (int)the_priority); 

  /* Change current task priority  */ 
  status = rtems_task_set_priority(RTEMS_SELF, 10, &the_priority); 

  /* Check new current priority  */ 
  status = rtems_task_set_priority
    (RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &the_priority); 
  printf("main-- actual current priority:%d\n", (int)the_priority); 

  /* Barrier manager */
  Task_name[5] = rtems_build_name('B', 'A', 'R', 'M'); 
  status = rtems_barrier_create (Task_name[5], RTEMS_BARRIER_AUTOMATIC_RELEASE, (uint32_t)3, &initialization_barrier);
  if (status != RTEMS_SUCCESSFUL)
  {
    printf("main-- failed in barrier manager : %d \n", status);
    return(0);
  }  
  
  /* Create tasks with right parameters */ 

  for (j = 1; j <= MAX_TASKS; j++){ 
    Task_name[j] = rtems_build_name('T','S','K','0'+j); 
    
    status = rtems_task_create(Task_name[j],Priorities[j], RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &Task_id[j]);
    
    printf("main-- Task nb. %d created, status = %d, priority = %d, id = %x\n",
	   j, (int)status, (int)Priorities[j], (int)Task_id[j]); 
  } 

 // status = rtems_task_mode(RTEMS_PREEMPT, RTEMS_PREEMPT_MASK, &Previous_Mode); 
 // if(status != RTEMS_SUCCESSFUL)
 //   printf("main-- rtems_task_mode failed\n"); 

  status = rtems_clock_get_tod(&time); // for rtems 4.12
  //status = rtems_clock_get(RTEMS_CLOCK_GET_TOD, &time); 
  printf("main-- start time(s) : %d\n", (int)time.second);

  for (j = 1; j <= MAX_TASKS; j++){
    printf("main-- Task %d is started\n", j);
    rtems_task_start(Task_id[j],The_Task, j);
  }


  /* Barrier wait */
  printf("main-- : Task wait for the barrier\n");
  status = rtems_barrier_wait (initialization_barrier, RTEMS_WAIT);
  if (status != RTEMS_SUCCESSFUL)
  {
     printf("main-- : Error while waiting for the barrier\n");
     return (0);
  }
  printf("main-- : Task release the barrier\n");

  /* Set current task priority lower than RMS tasks priorities,
   * so they begin running right now 
   */
  status = rtems_task_set_priority(RTEMS_SELF, 50, &the_priority);
  printf("main-- current priority : %d, will be set to 50\n", (int)the_priority);
  status = rtems_task_set_priority(RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &the_priority); 
  printf("main-- check current priority:%d\n", (int)the_priority); 

 while(1){
  rtems_task_wake_after(RTEMS_YIELD_PROCESSOR);
  count++;
  for (j = 1; j <= MAX_TASKS; j++)
   {
    if (Done[j] == 1) {
        Done[j]= 0;
        Main_Done++;
       }
   }
//  printf("main--: maindone %d\n",Main_Done);  
//  if ((count%10000)==0)
//    rtems_cpu_usage_report();
  if(Main_Done>= MAX_TASKS)
    break; 
  }

 for (j = 1; j <= MAX_TASKS; j++)
  printf("main-- Periods done for task %d : %d\n", j, Periods_Done[j]);
 printf("main-- Total Passses: %d \n", Total_Passes);
  
  printf("main-- exiting\n");
  rtems_task_delete(RTEMS_SELF);
  exit(1);
}
