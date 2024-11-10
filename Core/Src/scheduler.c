#include "scheduler.h"
#include "main.h"



uint32_t TIMER_CYCLE = 10;

// Array to hold tasks
static sTask SCH_tasks_G[SCH_MAX_TASKS];

// // Variable to keep track of the number of tasks
// static uint8_t taskCount = 0;

// // Variable to keep track of the system tick count
// static uint32_t tickCount = 0;

// Function to initialize the scheduler
void Scheduler_Init(void) {
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks_G[i].pTask = NULL;
        SCH_tasks_G[i].Delay = 0;
        SCH_tasks_G[i].Period = 0;
        SCH_tasks_G[i].RunMe = 0;
        SCH_tasks_G[i].TaskID = 0;
    }
}

void Scheduler_Update(void) {
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        // Check if there is a task at this location
        if (SCH_tasks_G[i].pTask) {
            if (SCH_tasks_G[i].Delay == 0) {
                // The task is due to run
                // Increment the 'RunMe' flag
                SCH_tasks_G[i].RunMe++;; //flag_on
                if (SCH_tasks_G[i].Period) {
                    // Schedule periodic tasks to run again
                    SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period;
                }
            } else {
                // Not yet ready to run: just decrement the delay
                SCH_tasks_G[i].Delay--;
            }
        }
    }
}

void Scheduler_Dispatch_Tasks(void) {
    // Dispatches (runs) the next task (if one is ready)
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks_G[i].RunMe > 0 && SCH_tasks_G[i].pTask != NULL) {
            SCH_tasks_G[i].RunMe--; // Reset / reduce RunMe flag
            
            (*SCH_tasks_G[i].pTask)(); // Run the task
 
            
           
            // - if this is a 'one shot' task, remove it from the array
            if (SCH_tasks_G[i].Period == 0) {
                Scheduler_DeleteTask(i);
            }
        }
    }
}

void Scheduler_AddTask(void (*pF)(void), uint32_t Delay, uint32_t Period) {
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks_G[i].pTask == NULL) {
            SCH_tasks_G[i].pTask = pF;
            SCH_tasks_G[i].Delay = Delay / TIMER_CYCLE; // Convert delay to ticks
            SCH_tasks_G[i].Period = Period / TIMER_CYCLE; // Convert period to ticks
            SCH_tasks_G[i].RunMe = 0;
            SCH_tasks_G[i].TaskID = i; // Assign a unique TaskID
            return;
        }
    }
}

// Function to delete a task from the scheduler
void Scheduler_DeleteTask(uint32_t TaskID) {
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks_G[i].TaskID == TaskID) {
            SCH_tasks_G[i].pTask = NULL;
            SCH_tasks_G[i].Delay = 0;
            SCH_tasks_G[i].Period = 0;
            SCH_tasks_G[i].RunMe = 0;
            SCH_tasks_G[i].TaskID = NO_TASK_ID;
            return;
        }
    }
}


