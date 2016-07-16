#include "mystring.h"
#include "type.h"
#include "const.h"

PUBLIC char* toLower(char * str)
{
	char *temp = str ;
       while(*str)
	{
	if((*str >= 'A') && (*str <= 'Z'))
		*str += 'a' -'A';
	str++;
	}
	return temp;
}
PUBLIC int compare(char *src,char * dstr)
{
	while(*src&&*dstr)
	{
	   if(*src != *dstr)
	{
		return *src - *dstr;
	}
	src++;
	dstr++;
	}
	return *src - *dstr;
	
}
PUBLIC bool equal(char * src,char *dstr)
{
	if(compare(src,dstr)==0)
		return true ;
	return false ;
}
PUBLIC bool equalwithoutcase(char * src,char *dstr)
{
	char * first = toLower(src);
	char * second = toLower(dstr);
	return equal(first,second);
}
PUBLIC bool parseIntoInt(char *str,int * number)
{
	bool positive = true ;
	*number = 0;
	if(*str==0)return false ;
        if(*str=='-')
		{positive = false ;str++;}
	while(*str!=0)
	{
	
	 if(*str>='0'&&*str<='9')
	 {
		*number = (*number)*10+*str-'0';
	 }
		else return false ;
		str++;
	}
	if(positive==false) *number = -*number;
	return true ;
}
