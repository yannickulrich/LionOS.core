/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##   #  # #   #
      #     #  #   #  # #  #   #     #  #  # ## ##
      ####  #   ###   # #   ###   ###    ##  # # #
                           Strings
*/

#ifndef UMc_Strings
#define UMc_Strings
#include "datatypes.h"
int strlen( char string[] )
{
	int i;
	for( i = 0; string[i] != '\0'; i++);
	return i;
}
int cpystr(char dest[], char src[])
{
	int length = strlen(dest);
	int nlen = strlen(src);
	if (nlen < length) length = nlen;
	int i;
	for (i = 0; i < length; i++)
		dest[i] = src[i];
	dest[i] = '\0';
	return length;
}
bool cmpstr( char stringOne[], char stringTwo[] )
{
	int i, end, mustadd = 1;
	if ( strlen( stringOne ) != strlen( stringTwo ) ) return false;
	for(i = 0; i < strlen(stringOne); i++)	
	{
		if (stringOne[i] == '\0')
		{
			mustadd = 0;
			break;
		}
	}
	stringOne[strlen(stringOne)] = '\0';
	mustadd = 1;
	for(i = 0; i <= strlen(stringTwo)-1; i++)	
	{
		if (stringTwo[i] == '\0')
		{
			mustadd = 0;
			break;
		}
	}
	stringTwo[strlen(stringTwo)] = '\0';
	end = strlen(stringTwo);
	if (strlen(stringOne) > strlen(stringTwo))
	{
		end = strlen(stringOne);
	}
	
	
	for(i = 0; i <= end; i++)
	{
		if (stringOne[i] == stringTwo[i])
			continue;
		else
			return false;
	}
	return true;
}
bool startsWith(char stringOne[], char start[])
{
	if (strlen(stringOne) < strlen(start)) return false;
	if (strlen(stringOne) == strlen(start)) cmpstr(stringOne, start);
	int i;
	for (i = 0; i < strlen(start); i++)
		if (stringOne[i] != start[i]) return false;
	return true;
}
int indexOf(char src[], char dest[])
{
	int i;
	for (i = 0; i < strlen(src) - strlen(dest); i++)
	{
		int j;
		for (j = 0; j < strlen(dest); j++)
		{
			if (src[i + j] != dest[j]) break;
			if (j == strlen(dest) - 1) return i;
		}
	}
	return -1;
}
int lastIndexOf(char src[], char dest[])
{
	int i;
	for (i = strlen(src) - strlen(dest); i >= 0; i--)
	{
		int j;
		for (j = 0; j < strlen(dest); j++)
		{
			if (src[i + j] != dest[j]) break;
			if (j == strlen(dest) - 1) return i++;
		}
	}
	return -1;
}
bool contains(char src[], char search[])
{
	return indexOf(src, search) != -1;
}
void substringToEnd(char src[], int startpos)
{
	substring(src, startpos, strlen(src));
}
void substring(char src[], int startpos, int stoppos)
{
	int i;
	char ret[stoppos - startpos];
	for (i = 0; i < stoppos - startpos; i++)
	{
		ret[i] = src[startpos + i];
	}
	ret[i] = '\0';
	cpystr(src, ret);
}
#endif
