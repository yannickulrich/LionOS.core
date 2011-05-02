/**
     ####  #   # ##### #     #     
    #      #   # #     #     #
     ####  ##### ####  #     #
         # #   # #     #     #
     ####  #   # ##### ##### #####
             AppLauncher
 * This module includes:
 *  - launchApp()
 *  - initLauncher)
 * Include-Codes: #_APPL___H ; #_APPL___C
 */
#ifndef _APPL___C
#define _APPL___C
void printCharAt(char c, int x, int y);
void initLauncher() {}
void launchApp()
{
	// TODO: PrintAppIcons, Breadcrum-Navi, input, folder management, etc.
	kprintf("LionOS #> ");
	char shell_input[8] = "";
	scanString(&shell_input[0]);
	if (cmpstr(shell_input, "calc"))
	{
		// TODO: Run Calculator App
	}
}
void printCharAt(char c, int x, int y)
{
	asm("int $0x29" : : "a" (0x15), "b" (c), "c" (x), "d" (y));
}
#endif
