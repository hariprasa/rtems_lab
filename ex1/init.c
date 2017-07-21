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
rtems_task The_Task1(rtems_task_argument unused);
rtems_task The_Task2(rtems_task_argument unused);
rtems_task The_Task3(rtems_task_argument unused);

/****************************************************************************/
/* RTEMS resources configuration                                            */
/****************************************************************************/

#define MAX_TASKS 5
#define MAX_PERIODS 100 

/* RTEMS system configuration informations */
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER 
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS          9
#define CONFIGURE_EXTRA_TASK_STACKS     (10 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_MAXIMUM_PERIODS        10000
#define CONFIGURE_MICROSECONDS_PER_TICK  100

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

/****************************************************************************/
/* Task configuration                                                       */
/****************************************************************************/

rtems_task_priority Priorities[9] = {10, 20, 21, 22, 23, 24, 25, 26, 27}; 
int                 Periods[9]    = {50, 50, 200, 300, 400, 500, 600, 700, 800}; 
rtems_id            Task_id[9]    = {0, 0, 0, 0, 0, 0, 0, 0, 0};    
rtems_id            Sem_id;
rtems_name          Task_name[9]  = {0, 0, 0, 0, 0, 0, 0, 0, 0};
rtems_name          Sem_name;
int                 Done[9]       = {0, 0, 0, 0, 0, 0, 0, 0, 0}; 
int                 Periods_Done[9]= {0, 0, 0, 0, 0, 0, 0, 0, 0};
int                 Total_Passes  = 0; 

#define MAX_TOTAL_PASSES 100
#define RMS_WORKLOAD 32
/****************************************************************************/
/* The_Task1(): body of the task                                             */
/****************************************************************************/

rtems_task The_Task1(rtems_task_argument unused){ 

  rtems_status_code Status; 
  rtems_mode        Previous_Mode; 
  int               My_Counter;
  rtems_time_of_day    time; 

  printf ("Task 1-- : starting\n"); 

  /* Set preemption */ 
  Status = rtems_task_mode(RTEMS_PREEMPT, RTEMS_PREEMPT_MASK, &Previous_Mode); 
  if (Status != RTEMS_SUCCESSFUL) 
    printf("Task 1-- : rtems_task_mode failed\n"); 

  /* waiting for time */
  rtems_task_wake_after(1500);

  /* Obtain Semaphore */
  rtems_clock_get_tod(&time); 
  // rtems_clock_get(RTEMS_CLOCK_GET_TOD, &time); /* not supported for RTEMS 4.12 */
  printf("Task 1-- : semaphore wait started in %d us\n", (int)time.ticks); 
  Status = rtems_semaphore_obtain(Sem_id,RTEMS_WAIT,10000);
  rtems_clock_get_tod(&time);  
  printf("Task 1-- : semaphore obtain in %d us\n", (int)time.ticks); 

  if(  Status == RTEMS_TIMEOUT )
     	printf("Task 1-- :rtems_semaphore_obtain is Timeout!\n");
  else if (Status != RTEMS_SUCCESSFUL) {
      printf("Task 1-- :rtems_semaphore_obtain failed with status: %d\n",Status);
      exit(1);
  }

  for (My_Counter = 0; My_Counter <= RMS_WORKLOAD; My_Counter++) {
    rtems_task_wake_after(100);
  };  
  rtems_clock_get_tod(&time); 
  printf("Task 1-- : semaphore released in %d us\n", (int)time.ticks); 
  rtems_semaphore_release(Sem_id);


  for (My_Counter = 0; My_Counter <= RMS_WORKLOAD; My_Counter++) {
    rtems_task_wake_after(100);
  };  

  //  while (1)
  printf( "Task 1-- :Task_delete\n");
  Status = rtems_task_delete(RTEMS_SELF);    /* should not return */
  printf( "Task 1-- :rtems_task_delete returned with status of %d.\n", Status );
  exit( 1 );
}
/****************************************************************************/
/* The_Task2(): body of the task                                             */
/****************************************************************************/

rtems_task The_Task2(rtems_task_argument unused){ 

  rtems_status_code Status; 
  rtems_mode        Previous_Mode; 
  int               My_Counter;
  rtems_time_of_day    time; 
  
  printf ("Task 2-- : starting\n"); 

  /* Set preemption */ 
  Status = rtems_task_mode(RTEMS_PREEMPT, RTEMS_PREEMPT_MASK, &Previous_Mode); 
  if (Status != RTEMS_SUCCESSFUL) 
    printf("Task 2 : rtems_task_mode failed\n"); 

  /* waiting for time */
  rtems_task_wake_after(1000);

  /* Obtain Semaphore */
  rtems_clock_get_tod(&time); 
  printf("Task 2-- : semaphore wait started in %d us\n", (int)time.ticks); 
  Status = rtems_semaphore_obtain(Sem_id,RTEMS_WAIT,10000);
  rtems_clock_get_tod(&time); 
  printf("Task 2-- : semaphore obtain in %d us\n", (int)time.ticks); 

  if(  Status == RTEMS_TIMEOUT )
      printf("Task 2-- :rtems_semaphore_obtain is Timeout!\n");
  else if (Status != RTEMS_SUCCESSFUL) {
      printf("Task 2-- :rtems_semaphore_obtain failed with status: %d\n",Status);
      exit(1);
  }

  for (My_Counter = 0; My_Counter <= RMS_WORKLOAD; My_Counter++) {
    rtems_task_wake_after(100);
  };  
  rtems_clock_get_tod(&time); 
  printf("Task 2-- : semaphore released in %d us\n", (int)time.ticks); 
  rtems_semaphore_release(Sem_id);


  for (My_Counter = 0; My_Counter <= RMS_WORKLOAD; My_Counter++) {
    rtems_task_wake_after(100);
  };  

  //  while (1)
  printf( "Task 2-- :Task_delete\n");
  Status = rtems_task_delete(RTEMS_SELF);    /* should not return */
  printf( "Task 2-- :rtems_task_delete returned with status of %d.\n", Status );
  exit( 1 );
}
/****************************************************************************/
/* The_Task3(): body of the task                                             */
/****************************************************************************/

rtems_task The_Task3(rtems_task_argument unused){ 

  rtems_status_code Status; 
  rtems_mode        Previous_Mode; 
  int               My_Counter;
  rtems_time_of_day    time; 

  printf ("Task 3-- : starting\n"); 

  /* Set preemption */ 
  Status = rtems_task_mode(RTEMS_PREEMPT, RTEMS_PREEMPT_MASK, &Previous_Mode); 
  if (Status != RTEMS_SUCCESSFUL) 
    printf("Task 3 : rtems_task_mode failed\n"); 

  /* Obtain Semaphore */
  rtems_clock_get_tod(&time); 
  printf("Task 3-- : semaphore wait started in %d us\n", (int)time.ticks); 
  Status = rtems_semaphore_obtain(Sem_id,RTEMS_WAIT,10000);
  rtems_clock_get_tod(&time);   
  printf("Task 3-- : semaphore obtain in %d us\n", (int)time.ticks); 

  if(  Status == RTEMS_TIMEOUT )
     	printf("Task 3-- :rtems_semaphore_obtain is Timeout!\n");
  else if (Status != RTEMS_SUCCESSFUL) {
      printf("Task 3-- :rtems_semaphore_obtain failed with status: %d\n",Status);
  }

  for (My_Counter = 0; My_Counter <= RMS_WORKLOAD; My_Counter++) {
    rtems_task_wake_after(100);
  };  
  rtems_clock_get_tod(&time); 
  printf("Task 3-- : semaphore released in %d us\n", (int)time.ticks); 
  rtems_semaphore_release(Sem_id);

  for (My_Counter = 0; My_Counter <= RMS_WORKLOAD; My_Counter++) {
    rtems_task_wake_after(100);
  };  

  //  while (1)
  printf( "Task 3-- :Task_delete\n");
  Status = rtems_task_delete(RTEMS_SELF);    /* should not return */
  printf( "Task 3-- :rtems_task_delete returned with status of %d.\n", Status );
  exit( 1 );
}

/****************************************************************************/
/* Init(): main entrypoint                                                  */
/****************************************************************************/

rtems_task Init( rtems_task_argument ignored ){ 
  rtems_status_code    status; 
  rtems_task_priority  the_priority; 
  rtems_time_of_day    time; 
  rtems_mode           Previous_Mode; 
  rtems_interval       ticks_per_second;

  /* Clock value */ 
  time.year   = 2011; 
  time.month  = 10; 
  time.day    = 11; 
  time.hour   = 9; 
  time.minute = 0; 
  time.second = 0; 
  time.ticks  = 0; 

  status = rtems_clock_set( &time );  
  rtems_task_wake_after(1000);
  printf ("main-- starting\n"); 

  ticks_per_second = rtems_clock_get_ticks_per_second();
//  rtems_clock_get(RTEMS_CLOCK_GET_TICKS_PER_SECOND, &ticks_per_second); 
  printf("main-- ticks_per_second : %d\n", (int)ticks_per_second); 

  /* task priority  */ 
  status = rtems_task_set_priority(RTEMS_SELF, 10, &the_priority); 

  /* Check current priority  */ 
  status = rtems_task_set_priority(RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &the_priority); 
  printf("main-- actual current priority:%d\n", (int)the_priority); 

  /* Creating Semaphore */
  Sem_name = rtems_build_name('P','C','P','M') ;
  status = rtems_semaphore_create(Sem_name,1,RTEMS_PRIORITY| RTEMS_BINARY_SEMAPHORE| RTEMS_PRIORITY_CEILING| RTEMS_GLOBAL,20,&Sem_id ) ;
  //  status = rtems_semaphore_create(Sem_name,1,RTEMS_DEFAULT_ATTRIBUTES,20,&Sem_id ) ;
  printf("main-- Semaphore created, status = %d, id = %x\n",(int)status,(int)Sem_id); 
  
  /* Create tasks with right parameters */ 
    Task_name[1] = rtems_build_name('T','A','1','-'); 
    Task_name[2] = rtems_build_name('T','A','2','-'); 
    Task_name[3] = rtems_build_name('T','A','3','-'); 
    status = rtems_task_create(Task_name[1],21, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &Task_id[1]);
    printf("main-- Task nb. %d created, status = %d, priority = %d, id = %x\n",1, (int)status, 21, (int)Task_id[1]); 
    status = rtems_task_create(Task_name[2],22, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &Task_id[2]);  
    printf("main-- Task nb. %d created, status = %d, priority = %d, id = %x\n",2, (int)status, 22, (int)Task_id[2]); 
    status = rtems_task_create(Task_name[3],23, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &Task_id[3]);  
    printf("main-- Task nb. %d created, status = %d, priority = %d, id = %x\n",3, (int)status, 23, (int)Task_id[3]); 

  rtems_clock_get_tod(&time); 
  printf("main-- start time(s) : %d us\n", (int)time.ticks);




  /* Starting tasks*/
  rtems_task_start(Task_id[1],The_Task1, 0);
  rtems_task_start(Task_id[2],The_Task2, 0);
  rtems_task_start(Task_id[3],The_Task3, 0);

  status = rtems_task_mode(RTEMS_PREEMPT, RTEMS_PREEMPT_MASK, &Previous_Mode); 
    printf("main-- Enable PREEMPT mode\n");
  if(status != RTEMS_SUCCESSFUL)
    printf("main-- rtems_task_mode failed\n");
 
  /* Lower the priority of current task to 40*/
  status = rtems_task_set_priority(RTEMS_SELF, 40, &the_priority);
  printf("main-- current priority : %d, will be set to 40\n", (int)the_priority);
  status = rtems_task_set_priority(RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &the_priority); 
  printf("main-- check current priority:%d\n", (int)the_priority); 





  while(1){
  rtems_task_wake_after(RTEMS_YIELD_PROCESSOR);
  }
  /*  rtems_task_wake_after(7000);
  rtems_clock_get(RTEMS_CLOCK_GET_TOD, &time); 
  printf("main-- : semaphore wait started in %d us\n", (int)time.ticks); 
  status = rtems_semaphore_obtain(Sem_id,RTEMS_WAIT,10000);
  rtems_clock_get(RTEMS_CLOCK_GET_TOD, &time); 
  printf("main-- : semaphore obtain in %d us\n", (int)time.ticks); 
  */
  printf("main-- Deleting Semaphore\n");
  rtems_semaphore_delete(Sem_id);

  printf("main-- exiting\n");
  rtems_task_delete(RTEMS_SELF);
  exit(1);
}
