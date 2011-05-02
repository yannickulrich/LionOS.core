/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##   #  # #   #
      #     #  #   #  # #  #   #     #  #  # ## ##
      ####  #   ###   # #   ###   ###    ##  # # #
                            IO
*/

#ifndef UMc_IO
#define UMc_IO 
#define screen getScreen()
#include "strings.h"

int fix;

Screen getScreen()
{
	Screen s;
	s.width = 80;
	s.height = 24;
	s.fix = fix;
	s.center = 38;
	s.right = 79;
	return s;
}

void kprintf(char *args, ...)
{
	
	va_list ap;
	int i = 0;
	int inC = 0;
	
	va_start(ap, args); 
	int d;
	char c, *s;
	while (args[i])
	{
		switch (args[i])
		{
			case '%':
				switch(args[++i])
				{
					case 'd': 
						d = va_arg(ap, int);
						if (d < 0) printChar('-');
						pn((d >= 0) ? d : -d);
						break;
					case 'b': 
						d = va_arg(ap, int);
						if (d == 1) kprintf("true");
						else kprintf("false");
						break;
					case 'c': 
						c = (char) va_arg(ap, int);
						printChar(c);
						break;
					case 's': 
						s =  va_arg(ap, char *);
						for (inC = 0; s[inC]; inC++) printChar( s[inC] );
						break;
					default: printChar('%');
				}
				break;
			default: 
				printChar(args[i]);
				break;
		}
		i++;
	}
	
	va_end(ap);
	

}
void palign( char *c, int type)
{
	int i;
	if ( type == 38 )
	{
		for( i = 0; i <= 38-strlen( c )/2; i++)
			printChar(' ');
	}
	else if ( type == 79 )
	{
		for( i = 0; i <= 79-strlen( c ); i++)
			printChar(' ');
	}

	kprintf(c);
}


void writeFile( file* f, char *towrite )
{
	asm("int $0x29" : : "a" (0x20), "b" (f), "c" (towrite) );
}

void printChar(char c){
	asm("int $0x29" : : "a" (0x10), "b" (c));
}
void pn(unsigned int input)
{
	int ldb = input % 10;
	int n = (int) input / 10;
	if (input > 9) pn(n);
	printChar(ldb + 48);
}
void clearscreen(){
	asm("int $0x29" : : "a" (0x11));
}
void cheColor(Color c){
	asm("int $0x29" : : "a" (0x12), "b" ( (char) c.fg), "c" ((char) c.bg) );
}
#define chColor changeColor
void changeColor(int fg, int bg){
	asm("int $0x29" : : "a" (0x12), "b" ( (char) fg), "c" ((char) bg) );
}
void getString(char* c)
{
	asm("int $0x29" : : "a" (0x18), "b" (c));
}
int getInt()
{
	int n = 0;
	asm("int $0x29" : : "a" (0x17), "b" (&n));
	return n;
}

directory *listDir( directory *root )
{
	directory *res;
	asm("int $0x29" : : "a" (0x0b), "b" (root), "c" ( &res ));
	return res;
}
char * readFile( file *f, size_t size )
{
	char *res;
	asm("int $0x29" : : "a" (0x0c), "b" (f), "c" ( &res ), "d" (size) );
	return res;
}
file *getFileById( int id, directory *dir)
{
	id += 2;
	file *akfile = dir->first;
	if (akfile == NULL)
		return NULL;
	int i = 0;
	while( akfile->next != NULL && id != i)
	{
		if( (int) akfile->next == -1 ) return NULL;
		i++;
		
		akfile = akfile->next;
	}
	return akfile;
}

directory *getDirById( int id, directory *dir)
{
	//id += 2;
	directory *akfile = dir->firstSub;
	if (akfile == NULL)
		return NULL;
	int i = 0;
	while( akfile->next != NULL && id != i)
	{
		if( akfile->next == NULL ) return NULL;
		i++;
		
		akfile = akfile->next;
	}
	return akfile;
}

void flush()
{
	asm("int $0x29" : : "a" (0x1a) );
}


void addKeyListener( void (*h)(char c, int scan, bool* b) )
{
	asm("int $0x29" : : "a" (0x1b), "b" ( h ) );
}

void setFix(int nfix)
{
	asm("int $0x29" : : "a" (0x13), "b" ( nfix ) );
	fix = nfix;
}

char getChar()
{
	char c;
	asm("int $0x29" : : "a" (0x16), "b" ( &c ) );
	return c;
}
bool getBool()
{
	bool ret;
	asm("int $0x29" : : "a" (0x18), "b" ( &ret ) );
	return ret;
}
#endif
