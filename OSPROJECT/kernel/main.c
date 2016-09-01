
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"
#include "mystring.h"

/* ==============================下列参数须移动至相关头文件*/
#define KILL_PROCESS_ID 1
#define ADD_PROCESS_ID 2
#define SHOW_PRO_INFO_ID 3
#define CLEAR_ID 4
#define SHUTDOWN_ID 5
#define HELP_ID 6
#define CLOSE_TIME_ID 7
#define OPERN_IME_ID 8
#define ERROR_ID 99
/*===================================*/

/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
        MYclear();
	disp_str("-----\"kernel_main\" begins-----\n");


	default_process_init();
	empty_processList_init();
	k_reenter = 0;
	ticks = 0;

	p_proc_ready	= proc_table;
       

	init_clock();
        init_keyboard();
	showtime = 1;
	hasclosetime = 0;
	restart();
	while(1){}
}


void default_process_init()
{

	TASK*		p_task = task_table;
	PROCESS*	p_proc = proc_table;
	char*		p_task_stack = task_stack + STACK_SIZE_TOTAL;
	u16		selector_ldt = SELECTOR_LDT_FIRST;
	int i;
	u8              privilege;
	u8              rpl;
	int             eflags;
	for (i = 0; i < NR_TASKS + NR_PROCS; i++) {
		if (i < NR_TASKS) {     /* 浠诲姟 */
			p_task = task_table + i;
			privilege = PRIVILEGE_TASK;
			rpl = RPL_TASK;
			eflags = 0x1202; /* IF=1, IOPL=1, bit 2 is always 1 */
		}
		else {                  /* 鐢ㄦ埛杩涚▼ */
			p_task = user_proc_table + (i - NR_TASKS);
			privilege = PRIVILEGE_USER;
			rpl = RPL_USER;
			eflags = 0x202; /* IF=1, bit 2 is always 1 */
		}

		strcpy(p_proc->p_name, p_task->name);	// name of the process
		p_proc->pid = i;			// pid

		p_proc->ldt_sel = selector_ldt;

		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
			sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
			sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;
		p_proc->regs.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;

		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = eflags;

		p_proc->nr_tty = 0;

		p_task_stack -= p_task->stacksize;
		p_proc++;
	//	p_task++;
		selector_ldt += 1 << 3;
	}

	proc_table[0].ticks = proc_table[0].priority = 15;
	proc_table[1].ticks = proc_table[1].priority = 5;
	proc_table[2].ticks = proc_table[2].priority = 5;
	proc_table[3].ticks = proc_table[3].priority = 5;
        proc_table[4].ticks = proc_table[4].priority = 5;

	proc_table[1].nr_tty = 0;
	proc_table[2].nr_tty = 0;
	proc_table[3].nr_tty = 0;
	proc_table[4].nr_tty = 0;
}
PUBLIC void addProcess()
{

	TASK*		p_task = user_proc_table;
	PROCESS*	p_proc = proc_table+(NR_TASKS + NR_PROCS);

	char*		p_task_stack = task_stack + STACK_SIZE_TOTAL;
	u16		selector_ldt = SELECTOR_LDT_FIRST;
	u8              privilege= PRIVILEGE_USER;
	u8              rpl= RPL_USER;
	int             eflags = 0x202;
	for (int i = NR_TASKS + NR_PROCS; i < MAX_NR_PROCS; i++)
	{
		if (proc_table[i].priority == 0)
		{
			char *process_name;
                        printf("please input process's name!\n",0);
			getCommand(process_name);



        

                p_task =  user_proc_table + (i - NR_TASKS);
		strcpy(p_proc->p_name,process_name);	// name of the process
		p_proc->pid = i;			// pid

		p_proc->ldt_sel = selector_ldt;

		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
			sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
			sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;
		p_proc->regs.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;
                
		p_proc->regs.eip = (u32)p_task[i-NR_TASKS].initial_eip;
		
		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack -STACK_SIZE_TTY*i;
		p_proc->regs.eflags = eflags;

		p_proc->nr_tty = 0;


                proc_table[i].ticks = proc_table[i].priority = 5;
	        proc_table[i].nr_tty = 0;



		        printf("add process successfully\n", 0);
			return ;
		}
	   	selector_ldt += 1 << 3;
		p_proc++;
	}
	printf("process list is full", 0);
	return ;

}
void empty_processList_init()
{
	for (int i = NR_TASKS + NR_PROCS; i < MAX_NR_PROCS; i++)
	{
		proc_table[i].priority = 0;
	}
}
/*======================================================================*
                               TestA
 *======================================================================*/


/*======================================================================*
                               TestB
 *======================================================================*/

void main_process()
{
/*	int i = 0x2000;
        char  str[256];

	int number = 2124;*/

        Welcome();
	while(1)
	{
                char *start ;
		write_command();
                getCommand(start);
		int id = getCommandId(start);
		switch (id)
		{
		case KILL_PROCESS_ID: killProcess();break;
		case ADD_PROCESS_ID: addProcess();break;
		case SHOW_PRO_INFO_ID:showProInfo();break;
		case CLEAR_ID:clear();printf("\n",0);break;
		case SHUTDOWN_ID:sys_shutdown(proc_table);break;
		case HELP_ID:help();break;
		case CLOSE_TIME_ID:closetime();break;
		case OPERN_IME_ID:opentime();break;
		case ERROR_ID:printf("input is not command!\n", 0);break;
		}
		milli_delay(200);
	}
}

/*======================================================================*
                               TestB
 *======================================================================*/

void MYclear()
{
	clear_screen(0,console_table[0].cursor);
	console_table[0].current_start_addr = 0;
	console_table[0].cursor = 0;
}






PUBLIC void showProInfo()
{
	int k;
	printf("=======================================================\n", 0);
	printf("  name                    priority       id          \n", 0);
	for (k = 0; k<MAX_NR_PROCS; k++)
	{
         if(proc_table[k].priority!=0)
         {
	printf("  %s                        %d           %d \n", proc_table[k].p_name, proc_table[k].priority, proc_table[k].pid);
	 }

	}
	printf("=======================================================\n", 0);
}
PUBLIC void private_killProcess(int pid)
{
	if (proc_table[pid].priority == 0)
	{
		printf("process isn't exist\n", 0);
		return ;
	}
	else
	{
		proc_table[pid].priority = 0;
		{
			printf("kill process successfully\n", 0);
			return ;
		}
	}
}

PUBLIC void killProcess()
{
	char* ctr_number[256];
        char* temp[256];
        printf("please input process's id!\n",0);
	getCommand(ctr_number);
        strcpy(temp,ctr_number);
        

	int number;
	if (!parseIntoInt(ctr_number, & number))
	{
		printf("pid is error\n", 0);
		return ;
	}
	else
	{
		if (number >= MAX_NR_PROCS)
		{
			printf("pid is out of range!\n",0);
                        return ;

		}
		if (number < NR_TASKS + NR_PROCS)
		{
			printf("process is system_process,can't be kill!\n",0);
                        return ;
		}
		private_killProcess(number);
	}

}


PUBLIC void help()
{
	printf('\n');
	printf("=======================================================================\n", 0);
	printf("Command list:\n", 0);
	printf("showproinfo:   A process manage, show you all process-info here\n", 0);
	printf("addprocess:    create new process\n", 0);
	printf("killprocess:   kill a process\n", 0);
	printf("closetimer:    close the show of time\n", 0);
	printf("opentimer:     open the show of time\n", 0);
	printf("help:          Show this help message\n", 0);
	printf("clear:         Clear the screen\n", 0);
	printf("shutdown:      Shut down the OS\n", 0);
	printf("=======================================================================\n", 0);
}
PUBLIC void getCommand(char *str)
{
	private_getCommand(str);
	while (*str == 0)
	{
		write_command();
		private_getCommand(str);
	}
}
 void private_getCommand(char * str)
{
	while (1)
	{
		char ch;
		ch = getcharfromtty();
		if (ch == 0) continue;
		if (ch == '\n'){ *str = 0; break; }
		*str++ = ch;
	}
}
 PUBLIC void closetime()
 {
	 showtime = 0;
 }
 PUBLIC void opentime()
 {
	 showtime = 1;
 }

PUBLIC  void Welcome()
 {
	 int color = 0x7f;
	 clear();
	 printf('\n');
	 printf("=======================================================================\n", 0);
	 printf("                       virtual S_Linux 1\n", 0);
	 printf("                     kernel 1.0.3 on a i386\n", 0);
	 printf("                            Welcome!\n", 0);
	 printf("You can input: help, to get some messages about this system. \n", 0);
	 printf("=======================================================================\n", 0);
	 milli_delay(1000);
 }

PUBLIC void sys_shutdown(PROCESS *p_pro)
{
        for(int i = 0;i<MAX_NR_PROCS;i++)
 	{
		p_pro->priority = 0;
		p_pro++;
	}

        clear();
        
        printf("\nclosing system",0);
        milli_delay(1000);
        clear();
        


     /*
	int i;
	disable_int();
	for (i = 0; i<16; ++i)
	{
		disable_irq(i);
	}
	while (1) {}*/
}
int getCommandId(char *str)
{
	int id;
	if (equalwithoutcase(str, "killprocess"))
	{
		id = KILL_PROCESS_ID;
	}
	else if (equalwithoutcase(str, "addprocess"))
	{
		id = ADD_PROCESS_ID;
	}
	else if (equalwithoutcase(str, "showproinfo"))
	{
		id = SHOW_PRO_INFO_ID;
	}
	else if (equalwithoutcase(str, "clear"))
	{
		id = CLEAR_ID;
	}
	else if (equalwithoutcase(str, "shutdown"))
	{
		id = SHUTDOWN_ID;
	}
	else if (equalwithoutcase(str, "help"))
	{
		id = HELP_ID;
	}
	else if (equalwithoutcase(str, "closetimer"))
	{
		id = CLOSE_TIME_ID;
	}
	else if (equalwithoutcase(str, "opentimer"))
	{
		id = OPERN_IME_ID;
	}
	else
	{
		id = ERROR_ID;
	}
	return id;
}


void process_1()
{
	clear();
//	readcommand(str);
//	printf(str,0);
	while (1) {

		milli_delay(200);
		
	}
}
void process_2()
{

	while(1){

		milli_delay(200);
	}
}
void process_3()
{

	while(1){

		milli_delay(200);
	}
}

void process_4()
{


	while (1) {
		disp_str("funciont2");
		milli_delay(200);
		
	}
}
void process_5()
{


	while (1) {
		disp_str("funciont3");
		milli_delay(200);
		
	}
}
void process_6()
{


	while (1) {
		disp_str("funciont4");
		milli_delay(200);
		
	}
}

void process_7()
{


	while (1) {
		disp_str("funciont5");
		milli_delay(200);
		
	}
}
void process_8()
{


	while (1) {
		disp_str("funciont6");
		milli_delay(200);
		
	}
}
void process_9()
{


	while (1) {
		disp_str("funciont7");
		milli_delay(200);
		
	}
}

void process_10()
{



	while(1){
		disp_str("funciont1");
		milli_delay(200);
	}
}















