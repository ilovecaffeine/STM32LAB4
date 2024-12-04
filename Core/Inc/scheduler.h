#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Define the maximum number of tasks
#define SCH_MAX_TASKS 10

// Define a constant for no task ID
#define NO_TASK_ID 0



// Define the sTask structure
typedef struct {
    // Pointer to the task (must be a 'void (void)' function)
    void (*pTask)(void);
    // Delay (ticks) until the function will (next) be run
    uint32_t Delay;
    // Interval (ticks) between subsequent runs
    uint32_t Period;
    // Incremented (by scheduler) when task is due to execute
    uint8_t RunMe;
    // Task ID
    uint32_t TaskID;

    bool isActive;
} sTask;



// Function to initialize the scheduler
void Scheduler_Init(void);

bool Scheduler_IsTaskScheduled(void (*pF)(void));

// Function to add a task to the scheduler
bool Scheduler_AddTask(void (*pTask)(void), uint32_t Delay, uint32_t Period); //settimer

// Function to remove a task from the scheduler
void Scheduler_DeleteTask_byindex(uint32_t TaskID);

void Scheduler_DeleteTask_byname(void (*pF)(void));

void Scheduler_Update(void); //timerRun

// Function to run the scheduler
void Scheduler_Dispatch_Tasks(void); //timerflag





#endif // SCHEDULER_H