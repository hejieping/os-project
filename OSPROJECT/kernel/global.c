
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "console.h"
#include "proc.h"
#include "global.h"
#include "proto.h"


PUBLIC	PROCESS	proc_table[MAX_NR_PROCS];

PUBLIC	TASK	task_table[NR_TASKS] = {
	{task_tty, STACK_SIZE_TTY, "tty"}};

PUBLIC  TASK    user_proc_table[MAX_NR_PROCS-NR_TASKS] = {
	{process_1, STACK_SIZE_PROCESS, "process_1"},
	{main_process, STACK_SIZE_PROCESS, "main_process"},
	{process_2, STACK_SIZE_PROCESS, "process_2"},
        {process_3,STACK_SIZE_PROCESS,"process_3"},
        {process_4,STACK_SIZE_PROCESS,"process_4"},
	{process_5,STACK_SIZE_PROCESS,"process_5"},
	{process_6,STACK_SIZE_PROCESS,"process_6"},
	{process_7,STACK_SIZE_PROCESS,"process_7"},
	{process_8,STACK_SIZE_PROCESS,"process_8"},
	{process_9,STACK_SIZE_PROCESS,"process_9"},
	{process_10,STACK_SIZE_PROCESS,"process_10"}};

PUBLIC	char		task_stack[STACK_SIZE_TOTAL];

PUBLIC	TTY		tty_table[NR_CONSOLES];
PUBLIC	CONSOLE		console_table[NR_CONSOLES];

PUBLIC	irq_handler	irq_table[NR_IRQ];

PUBLIC	system_call	sys_call_table[NR_SYS_CALL] = {sys_get_ticks, sys_write , sys_clear,sys_getcharfromtty , sys_write_command};

