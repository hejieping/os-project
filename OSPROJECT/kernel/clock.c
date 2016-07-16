
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               clock.c
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


PUBLIC void showSystemTime()
{
	disp_pos = 0;
	int seconds = ticks / (HZ*3) ;
	int second = seconds % 60;
	int minutes = seconds / 60;
	int minute = minutes % 60;
	int hour = minutes / 60;
	int text_color = MAKE_COLOR(BLACK,RED)|BRIGHT;
	disp_str("                                                        ");
	disp_color_str("SYstem Time : ",text_color);
	disp_color_int(hour,text_color);
	disp_color_str(" : ",text_color);
	disp_color_int(minute,text_color);
        disp_color_str(" : ",text_color);
	disp_color_int(second,text_color);
	disp_pos = 0;
}
PUBLIC void closeTime()
{
	disp_pos = 0;
	for(int i=0;i<80;i++)disp_str(" ");
	disp_pos = 0;
}
PUBLIC void clock_handler(int irq)
{
	ticks++;
	p_proc_ready->ticks--;

	if (k_reenter != 0) {
		return;
	}

	if (p_proc_ready->ticks > 0) {
		return;
	}
	if(showtime==1)
		{showSystemTime();}
	else if(showtime == 0 && hasclosetime == 0){closeTime();hasclosetime==1;}
	schedule();

}

/*======================================================================*
                              milli_delay
 *======================================================================*/
PUBLIC void milli_delay(int milli_sec)
{
        int t = get_ticks();

        while(((get_ticks() - t) * 1000 / HZ) < milli_sec) {}
}

/*======================================================================*
                           init_clock
 *======================================================================*/
PUBLIC void init_clock()
{
        /* 初始化 8253 PIT */
        out_byte(TIMER_MODE, RATE_GENERATOR);
        out_byte(TIMER0, (u8) (TIMER_FREQ/HZ) );
        out_byte(TIMER0, (u8) ((TIMER_FREQ/HZ) >> 8));

        put_irq_handler(CLOCK_IRQ, clock_handler);    /* 设定时钟中断处理程序 */
        enable_irq(CLOCK_IRQ);                        /* 让8259A可以接收时钟中断 */
}


