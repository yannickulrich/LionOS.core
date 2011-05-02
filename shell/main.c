	#include <io.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include <datatypes.h>
#include <game.h>
#include <dialog.h>

#define changeColor chColor
#define scanString getString
#define scanInt getInt
//#define cco cheColor
//#define pch printChar
#define uint8 unsigned char

typedef struct {char name[10]; int uscore;} scorelist;
typedef struct{int x; int y; int speedx; int speedy;} pos;
void listDirRec( directory* dir, int level );

int line;
int tempvar;
char bgcolor;

void app_about();
void app_help();
void app_shutdown();
int  app_calc();
void setFullscreen(bool full);
void waitForUserInput();
void setFullscreen(bool full)
{
	if (full)
	{
		if (screen.fix != 0) setFix(0);
		//setFix(0);
		cheColor(toColor(0xF));
		clearscreen();
	}
	else
	{
		setFix(0);
		changeColor((char) 0x0, (char) 0xF);
		clearscreen();

		// CREATE SHELL HEADER LAYOUT
		changeColor((char) 0x0, (char) 0xF);

		// CREATE CONTENT
		kprintf("\n");
		char msg0[] = "Welcome to LionOS\n";
		/*for(line = 1; line <= 31; ++line) kprintf(" ");*/changeColor((char) 0x0, (char) 0xE);palign(&(msg0[0]), screen.center);
		for(line = 1; line <= 22; ++line) kprintf(" ");changeColor((char) 0x0, (char) 0x3);kprintf("Version from %s on %s\n", __TIME__, __DATE__);
		// CREATE BOTTOM LINE
		changeColor((char) 0x0, (char) 0x0);kprintf("     ");
		changeColor((char) 0x0, (char) 0x8);for(tempvar = 1; tempvar <= 3; ++tempvar) kprintf("%c", (char) 196);
		changeColor((char) 0x0, (char) 0x7);for(tempvar = 1; tempvar <= 3; ++tempvar) kprintf("%c", (char) 196);
		changeColor((char) 0x0, (char) 0xF);
		for(tempvar = 1; tempvar <= 58; ++tempvar) kprintf("%c", (char) 196);
		changeColor((char) 0x0, (char) 0x7);for(tempvar = 1; tempvar <= 3; ++tempvar) kprintf("%c", (char) 196);
		changeColor((char) 0x0, (char) 0x8);for(tempvar = 1; tempvar <= 3; ++tempvar) kprintf("%c", (char) 196);
		changeColor((char) 0x0, (char) 0x0);kprintf("     ");
		kprintf("\n"); 
		setFix(5);
	}
}
int b(int a)
{
	return (a > 0) ? a : -a;
}
 
int main(char * cmd)
{
	changeColor(0x0, 0xF);
	setFix(0);
	clearscreen();
	changeColor((char) 0x0, (char) 0xF);
	clearscreen();

	// CREATE SHELL HEADER LAYOUT
	changeColor((char) 0x0, (char) 0xF);

	// CREATE CONTENT
	kprintf("\n");
	for(line = 1; line <= 31; ++line) kprintf(" ");changeColor((char) 0x0, (char) 0xE);kprintf("Welcome to LionOS\n");
	for(line = 1; line <= 22; ++line) kprintf(" ");changeColor((char) 0x0, (char) 0x3);kprintf("Version from %s on %s\n", __TIME__, __DATE__);
	// CREATE BOTTOM LINE
	changeColor((char) 0x0, (char) 0x0);kprintf("     ");
	changeColor((char) 0x0, (char) 0x8);for(tempvar = 1; tempvar <= 3; ++tempvar) kprintf("%c", (char) 196);
	changeColor((char) 0x0, (char) 0x7);for(tempvar = 1; tempvar <= 3; ++tempvar) kprintf("%c", (char) 196);
	changeColor((char) 0x0, (char) 0xF);
	for(tempvar = 1; tempvar <= 58; ++tempvar) kprintf("%c", (char) 196);
	changeColor((char) 0x0, (char) 0x7);for(tempvar = 1; tempvar <= 3; ++tempvar) kprintf("%c", (char) 196);
	changeColor((char) 0x0, (char) 0x8);for(tempvar = 1; tempvar <= 3; ++tempvar) kprintf("%c", (char) 196);
	changeColor((char) 0x0, (char) 0x0);kprintf("     ");
	kprintf("\n"); 
	setFix(5);
	flush();
	changeColor((char) 0x0, (char) 0xA);kprintf("CMD: %s", cmd);
	while(1) {
		char shell_input[70] = "";
		changeColor((char) 0x0, (char) 0xC);kprintf("LionOS #> ");
		changeColor((char) 0x0, (char) 0xA);scanString(&shell_input[0]);
		
		if (cmpstr(shell_input, "about") || cmpstr(shell_input, "About")) {
			app_about();
		} else if (cmpstr(shell_input, "clear") || cmpstr(shell_input, "Clear")) {
			clearscreen();
		} else if (cmpstr(shell_input, "calc") || cmpstr(shell_input, "Calc")) {
			app_calc();
		} else if (cmpstr(shell_input, "exit") || cmpstr(shell_input, "Exit")) {
			app_shutdown();
		} else if (cmpstr(shell_input, "shutdown") || cmpstr(shell_input, "Shutdown")) {
			app_shutdown();
		} else if (cmpstr(shell_input, "doof") || cmpstr(shell_input, "run")) {
			kprintf("Run App...\n");
			kprintf("List dir\n");
			directory *root = listDir(NULL);
			kprintf("Run file %s\n", root->first->next->name);
			runAPP(root->first->next);
		} else if (cmpstr(shell_input, "str")) {
			char cc[64];
			kprintf("StrinTest 1   Enter a dummy text: ");
			scanString(&(cc[0]));
			kprintf("%s\n", &(cc[0]));
			app_shutdown();
		} else if (cmpstr(shell_input, "help") || cmpstr(shell_input, "Help")) {
			app_help();
		} else if (cmpstr(shell_input, "ls")) {
			printChar((char) 218);
			for(tempvar = 0; tempvar <= 5; tempvar++) printChar((char) 196);
			printChar((char) 194);
			for(tempvar = 0; tempvar <= screen.width-10; tempvar++) printChar((char) 196);
			printChar((char) 191);
			printChar((char) 179);
			kprintf(" ID   %c Filename", (char) 179,(char) 179);
			for(tempvar = 0; tempvar <= 61; tempvar++) printChar(' ');
			printChar((char) 179);
			printChar((char) 192);
			for(tempvar = 0; tempvar <= 5; tempvar++) printChar((char) 196);
			printChar((char) 193);
			for(tempvar = 0; tempvar <= screen.width-10; tempvar++) printChar((char) 196);
			printChar((char) 217);
			directory *root = listDir(NULL);
			int i;
			file *akfile;
			for(i=0;i<=5;i++)
			{
				akfile = getFileById( i, root );
				if (akfile == NULL) continue;
				//if( !cmpstr(akfile->name, getFileById(i-1,root)->name ) )
				{
								
					kprintf("  %d", i);
					if(i<10) printChar(' ');if(i<100) printChar(' ');
					kprintf("    %s.%s\n",akfile->name, akfile->extension);
				}
			}
			directory *akdir;
			for(i=0;i<=5;i++)
			{
				akdir = getDirById( i, root );
				if (akdir == NULL) continue;
				//if( !cmpstr(akfile->name, getFileById(i-1,root)->name ) )
				{
								
					kprintf("  %d", i);
					if(i<10) printChar(' ');if(i<100) printChar(' ');
					changeColor((char) 0x0, (char) 0xC);kprintf("    %s\n",akdir->name);
				}
			}
		} else if (cmpstr(shell_input, "lsR")) {
			listDirRec( (directory *) NULL, 0);
			
		} else if (cmpstr(shell_input, "cat")) {
			printChar((char) 218);
			for(tempvar = 0; tempvar <= 5; tempvar++) printChar((char) 196);
			printChar((char) 194);
			for(tempvar = 0; tempvar <= screen.width-10; tempvar++) printChar((char) 196);
			printChar((char) 191);
			printChar((char) 179);
			kprintf(" ID   %c Filename", (char) 179,(char) 179);
			for(tempvar = 0; tempvar <= 61; tempvar++) printChar(' ');
			printChar((char) 179);
			printChar((char) 192);
			for(tempvar = 0; tempvar <= 5; tempvar++) printChar((char) 196);
			printChar((char) 193);
			for(tempvar = 0; tempvar <= screen.width-10; tempvar++) printChar((char) 196);
			directory *root = listDir(NULL);
			int i;
			file *akfile;
			for(i=0;i<=10;i++)
			{
				akfile = getFileById( i, root );
				
				if( !cmpstr(akfile->name, getFileById(i-1,root)->name ) )
				{
								
					kprintf("  %d", i);
					if(i<10) printChar(' ');if(i<100) printChar(' ');
					kprintf("    %s.%s\n",akfile->name, akfile->extension);
				}
			}
			kprintf("Enter file id: ");
			int n = getInt();
			clearscreen();
			kprintf(readFile( getFileById( n, root ),512 ) );
			char tmp[1];
			scanString(tmp);
			setFullscreen(false);

		} else if (cmpstr(shell_input, "write")) {
			directory *root = listDir(NULL);
			writeFile( root->first->next->next, "AC" );
		
		} else if (cmpstr(shell_input, "time")) {
			int time = getTime();
			kprintf("The OS runs %d Seconds\n", time/10);
		} else if (cmpstr(shell_input, "test")) {
			char swone[] = "Hello World!";
			char swstart[] = "Hello";
			kprintf("Starts With...       (true)      %b\n", startsWith(swone, swstart));
			char sb1one[] = "Hello World!";
			substringToEnd(sb1one, 6);
			kprintf("SubstringToEnd...    (World!)    %s\n", sb1one);
			char sb2one[] = "Hello World!";
			substring(sb2one, 0, 5);
			kprintf("Substring...         (Hello)     %s\n", sb2one);
			char cnone[] = "Hello World!";
			char xnstart[] = "o Worl";
			kprintf("Contains...          (true)      %b\n", contains(cnone, xnstart));
			char cn2one[] = "Hello World!";
			char xn2start[] = "o Worl";
			kprintf("IndexOf...           (4)         %d\n", indexOf(cn2one, xn2start));
			char xn3start[] = "o Wol";
			kprintf("IndexOf...           (-1)        %d\n", indexOf(cn2one, xn3start));
			char cn4one[] = "a.b.c.d";
			char xn4start[] = ".";
			kprintf("LastIndexOf...       (5)         %d\n", lastIndexOf(cn4one, xn4start));
			int mytestvar = false;
			kprintf("Bool printing...     (false)     %b\n", mytestvar);
			int mytestvari = 6822;
			kprintf("Integer printing...  (6822)      %d\n", mytestvari);
		} else if (cmpstr(shell_input, "rand")) {
			kprintf("%d", rand());
		} else if (cmpstr(shell_input, "reboot")) {
			reboot();
		} else if (cmpstr(shell_input, "")) {
		} else {
			kprintf("shell: command not found\n");
		}
		
	}
	exit(0);
	
	return 0;
}
#define    KLEFT       75
#define    KRIGHT      77
#define    KDOWN       80
#define    KUP         72

void listDirRec( directory * dir, int level )
{
	directory *root = listDir(dir);
	int i, j;
	file *akfile;
	for(i=0;i<=5;i++)
	{
		akfile = getFileById( i, root );
		if (akfile == NULL) continue;
		for(j=0;j<=level;j++) printChar('-');
		printChar('>');
		kprintf("%s.%s\n",akfile->name, akfile->extension);
	
	}
	directory *akdir;
	for(i=0;i<=5;i++)
	{
		akdir = getDirById( i, root );
		if (akdir == NULL) continue;
		for(j=0;j<=level;j++) printChar('-');
		printChar('>');
		changeColor((char) 0x0, (char) 0xC);kprintf("%s\n",akdir->name); changeColor( (char) 0x0, (char) 0xA);
		listDirRec( akdir, level+1 );
		
	}
}


void app_screensaver()
{
	static int srandom = 0;

	while(1){
		int rnd = ( getTime() + srandom) % /*( 0x1337 * 0x1337 )*/ 255;
		srandom += rnd;
		if ((srandom % 255) == '\n') continue;
		kprintf("%c", rnd );
		char tmp[1];
		scanString(tmp);
	}
}

void app_about() {
	setFullscreen(true);
	kprintf("\n\n\n\n\n\n\n\n");
	changeColor(0x0, 0xE );
	for(line = 1; line <= 26; ++line) kprintf(" ");kprintf("#    #            ###   ### \n");		changeColor(0x0, 0xC );
	for(line = 1; line <= 26; ++line) kprintf(" ");kprintf("#                #   # #    \n");		changeColor(0x0, 0xB );
	for(line = 1; line <= 26; ++line) kprintf(" ");kprintf("#    #  ###  ##  #   #  ##  \n");		changeColor(0x0, 0xA );
	for(line = 1; line <= 26; ++line) kprintf(" ");kprintf("#    # #   # # # #   #    # \n");		changeColor(0x0, 0xD );
	for(line = 1; line <= 26; ++line) kprintf(" ");kprintf("#### #  ###  # #  ###  ###  \n\n\n");	changeColor(0x0, 0xF );
	for(line = 1; line <= 34; ++line) kprintf(" ");
	kprintf("%c About %c\n\n", (char) 174, (char) 175);
	kprintf(" This OS is created by SpaceEmotion and Alexis Engelke.\n\n");
	kprintf("   We got a lot of help from\n");
	kprintf("    %c www.henkessoft.de\n", 7);
	kprintf("    %c www.lowlevel.org\n\n", 7);
	for(line = 1; line <= 160-31; ++line) kprintf(" ");
	kprintf("Press any key to continue...");
	changeColor(0x0, 0x0);kprintf(" \b");
	waitForUserInput();
	setFullscreen(false);
	changeColor(0x0, 0xF);kprintf(" \b");
}

void app_help() {
	printChar((char) 218);
	for(tempvar = 0; tempvar <= 8; tempvar++) printChar((char) 196);
	printChar((char) 194);
	for(tempvar = 0; tempvar <= screen.width-13; tempvar++) printChar((char) 196);
	printChar((char) 191);
	printChar((char) 179);
	kprintf(" Command %c Description", (char) 179,(char) 179);
	for(tempvar = 0; tempvar <= 55; tempvar++) printChar(' ');
	printChar((char) 179);
	printChar((char) 192);
	for(tempvar = 0; tempvar <= 8; tempvar++) printChar((char) 196);
	printChar((char) 193);
	for(tempvar = 0; tempvar <= screen.width-13; tempvar++) printChar((char) 196);
	printChar((char) 217);
	//kprintf("  hello\t\tSimple Hello World output command\n");
	kprintf("  about\t\tabout this OS\n");
	kprintf("  clear\t\tclears the screen\n");
	kprintf("  shutdown\thaltes the system\n");
	kprintf("  help \t\tshows this helpfile\n");
	kprintf("  ls   \t\tlists root directory'\n");
	kprintf("  cat  \t\tshows the content of a text file\n");
	kprintf("  calc \t\tA simple Calculator\n");
}

void app_shutdown() {
	// Hide Cursor
	changeColor((char) 0x0, (char) 0x0);
	kprintf(" \b");
	// Unfix the screen and clear
	setFix(0);
	clearscreen();
	// Print logo
	kprintf("\n\n\n\n\n\n\n");
	changeColor(0x0, 0xE );
	for(line = 1; line <= 26; ++line) kprintf(" ");kprintf("#    #            ###   ### \n");	changeColor(0x0, 0xC );
	for(line = 1; line <= 26; ++line) kprintf(" ");kprintf("#                #   # #    \n");	changeColor(0x0, 0xB );
	for(line = 1; line <= 26; ++line) kprintf(" ");kprintf("#    #  ###  ##  #   #  ##  \n");	changeColor(0x0, 0xA );
	for(line = 1; line <= 26; ++line) kprintf(" ");kprintf("#    # #   # # # #   #    # \n");	changeColor(0x0, 0xD );
	for(line = 1; line <= 26; ++line) kprintf(" ");kprintf("#### #  ###  # #  ###  ###  \n\n\n");	changeColor(0x0, 0xF );
	for(line = 1; line <= 31; ++line) kprintf(" ");
	kprintf("%c System halted %c\n\n", (char) 174, (char) 175);
	changeColor(0,0);
	kprintf(" \b");
	asm("cli");
	// Halt
	asm("hlt");
}
void waitForUserInput()
{
	char tmp[1];
	scanString(tmp);
}

int app_calc() {
	kprintf("     %c%c%c%c%c%c%c\n", 0xdc, 0xdc, 0xdc, 0xdc, 0xdc, 0xdc, 0xdc);
	kprintf("     %c + - %c\n", 0xdb, 0xdb);
	kprintf("     %c     %c\n", 0xdb, 0xdb);
	kprintf("     %c * = %c\n", 0xdb, 0xdb);
	kprintf("     %c%c%c%c%c%c%c\n\n", 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf);
	kprintf("   Calculator\n\n");

	char op;
	kprintf("<+> Addition\n<-> Subtraction\n</> Division\n<*> Multiplication\n<%%> Modulo\n");
	kprintf("Enter A: ");
	int a = getInt();
	kprintf("Enter Operation: ");
	scanString(&op);
	kprintf("Enter B: ");
	int b = getInt();
	
	switch(op)
	{
		case '+': kprintf("a+b = %d \n", a+b);
			break;
		case '-': kprintf("a-b=%d\n", a-b);
			break;
		case '/': kprintf("a/b=%d\n", a/b);
			break;
		case '*': kprintf("a*b=%d\n", a*b);
			break;
		case '%': kprintf("a%%b=%d\n", a%b);
			break;
		default: kprintf("Invalid Operation!\n");
			return 0;
	}
	return 0;
}

#include <io.c>
#include <stdlib.c>
#include <strings.c>
#include <time.c>
