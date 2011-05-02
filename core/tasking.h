/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
               Multitasking
*/
#ifndef MM_H
#define MM_H
#include "mm/pmm.h"
#include "datatypes.h"
#include "time.h"
void idle();
int initTask(void* entry, int priority, pageDir_t page, char *args);
cpu_state* forceTaskSwitch(cpu_state* cpu);
void saveCPU(cpu_state* cpu);
cpu_state* schedule(cpu_state* esp);
void startTasking();

static int currentPID = -1;
task *acTask = null;
int lastTask = 0;
int savingCpuState = 1;
task *firstTask = NIL;
bool killNextTask = false;
/*
 * 0   := less (eg. user space, apps)
 * 100 := high (eg. deamons, firewalls)
 */
static uint32_t tss[32] = { 0, 0, 0x10 };
#include "tasking.c"
#endif
