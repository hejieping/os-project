
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                              vsprintf.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "string.h"

/*
 *  为更好地理解此函数的原理，可参考 printf 的注释部分。
 */

/*======================================================================*
                                vsprintf
 *======================================================================*/
int vsprintf(char *buf, const char *fmt, va_list args)
{
	char*	p;
	char	tmp[256];
	va_list	p_next_arg = args;

	for (p=buf;*fmt;fmt++) {
		if (*fmt != '%') {
			*p++ = *fmt;
			continue;
		}

		fmt++;
 		int lenth;
		switch (*fmt) {
		case 'd':
			itoa(tmp, *((int*)p_next_arg));
			strcpy(p, tmp);
			lenth = strlen(tmp);
			p_next_arg += 4;
			p += lenth;
			
			break;
		case 's':
			strcpy(p,*(char**)(p_next_arg));
			lenth = strlen(*(char **)(p_next_arg));
			p[lenth] = 0;
			p += lenth ;
			p_next_arg += 4;
			break;
		case 'c':
			*p++ = *p_next_arg ;
			p_next_arg +=4;
			break ;
		default:
			break;
		}
	}

	return (p - buf);
}

