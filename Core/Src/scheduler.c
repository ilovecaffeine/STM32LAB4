#include "scheduler.h"
#include "main.h"



uint32_t SCH_TIMER_CYCLE = 10;

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
        SCH_tasks_G[i].isActive = false;
    }
}

bool Scheduler_IsTaskScheduled(void (*pF)(void)) {
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks_G[i].pTask == pF && SCH_tasks_G[i].isActive) {
            return true;
        }
    }
    return false;
}

void Scheduler_Update(void) {
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        // Check if there is a task at this location
        if (SCH_tasks_G[i].pTask && SCH_tasks_G[i].isActive) {
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
        if (SCH_tasks_G[i].RunMe > 0 && SCH_tasks_G[i].pTask != NULL && SCH_tasks_G[i].isActive) {
            SCH_tasks_G[i].RunMe--; // Reset / reduce RunMe flag
            
            (*SCH_tasks_G[i].pTask)(); // Run the task
 
            
           
            // - if this is a 'one shot' task, remove it from the array
            if (SCH_tasks_G[i].Period == 0) {
                Scheduler_DeleteTask_byindex(i);
            }
        }
    }
}

bool Scheduler_AddTask(void (*pF)(void), uint32_t Delay, uint32_t Period) {
     // Check if the task is already scheduled
   // if (Scheduler_IsTaskScheduled(pF)) {
    //    return false; // Task is already scheduled
    //}
    
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        // Check if the task is already scheduled
        if (SCH_tasks_G[i].pTask == pF && SCH_tasks_G[i].isActive) {
            // If the task is already scheduled, just update the delay and period
            SCH_tasks_G[i].Delay = Delay / SCH_TIMER_CYCLE; // Update delay
            SCH_tasks_G[i].Period = Period / SCH_TIMER_CYCLE; // Update period
            SCH_tasks_G[i].RunMe = 0;
            return true;
        }


        if (SCH_tasks_G[i].pTask == NULL) {
            SCH_tasks_G[i].pTask = pF;
            SCH_tasks_G[i].Delay = Delay / SCH_TIMER_CYCLE; // Convert delay to ticks
            SCH_tasks_G[i].Period = Period / SCH_TIMER_CYCLE; // Convert period to ticks
            SCH_tasks_G[i].RunMe = 0;
            SCH_tasks_G[i].TaskID = i; // Assign a unique TaskID
            SCH_tasks_G[i].isActive = true;
            return true;
        }
    }
    return false; // No space to add new task
}

// Function to delete a task from the scheduler
void Scheduler_DeleteTask_byindex(uint32_t TaskID) {
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks_G[i].TaskID == TaskID) {
            SCH_tasks_G[i].pTask = NULL;
            SCH_tasks_G[i].Delay = 0;
            SCH_tasks_G[i].Period = 0;
            SCH_tasks_G[i].RunMe = 0;
            SCH_tasks_G[i].TaskID = NO_TASK_ID;
            SCH_tasks_G[i].isActive = false;
            return;
        }
    }
}

void Scheduler_DeleteTask_byname(void (*pF)(void)) {
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks_G[i].pTask == pF) {
            SCH_tasks_G[i].pTask = NULL;
            SCH_tasks_G[i].Delay = 0;
            SCH_tasks_G[i].Period = 0;
            SCH_tasks_G[i].RunMe = 0;
            SCH_tasks_G[i].TaskID = NO_TASK_ID;
            SCH_tasks_G[i].isActive = false;
            return;
        }
    }
}
