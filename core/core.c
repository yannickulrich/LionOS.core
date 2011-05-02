/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                   Core
*/
void USgetKernel(char* start)
{
	start = "Kernel v0.1.2 nightly";
}

bool askKeycodes = false;
void printLogo()
{
	int line;
	if (!DEBUG)
		kprintf("\n\n\n\n\n\n\n");
	else kprintf("\n\n");
	changeColor(0x0, 0xE);
	for(line = 1; line <= 26; line++) kprintf(" ");
	kprintf("#    #            ###   ###\n");
	changeColor(0x0, 0xC);
	for(line = 1; line <= 26; line++) kprintf(" ");
	kprintf("#                #   # #\n");
	changeColor(0x0, 0xB);
	for(line = 1; line <= 26; line++) kprintf(" ");
	kprintf("#    #  ###  ##  #   #  ##\n");
	changeColor(0x0, 0xA);
	for(line = 1; line <= 26; line++) kprintf(" ");
	kprintf("#    # #   # # # #   #    #\n");
	changeColor(0x0, 0xD);
	for(line = 1; line <= 26; line++) kprintf(" ");
	kprintf("#### #  ###  # #  ###  ###\n\n\n");
	changeColor(0x0, 0xF);
}
void setAskKeycodes(bool value)
{
	askKeycodes = value;
}
void initScreen()
{
	setFix(0);
	changeColor(0x0, 0xF);
	clearscreen();
}
void initKeycodes()
{
	pc("[06.000050] -initKeycodes() called");
	int line;
	int scan;
	if (askKeycodes)
	{
		pc("[06.000055] +Ask User");
		pc("[06.000056] +Attemp Overlay - Debug Output frozen");
		char vm[25 * 2 * 80];
		kmemcpy(&(vm[0]), &(video[0]), 25 * 80 * 2);
		setFix(0);
		clearscreen();
		printLogo();
		for(line = 1; line <= 23; line++) kprintf(" ");
		kprintf("%c Please choose Keyboard layout %c\n\n", (char) 174, (char) 175);
		for(line = 1; line <= 26; line++) kprintf(" ");
		kprintf("<1> QWERTY (English Layout)\n");
		for(line = 1; line <= 26; line++) kprintf(" ");
		kprintf("<2> QWERTZ (German Layout)\n");
		changeColor(0x0, 0x0);
		kprintf(" \b");
		scan = FetchAndAnalyzeScancode();
		changeColor(0x0, 0xF);
		kmemcpy(&(video[0]), &(vm[0]), 25 * 80 * 2);
		setFix(10);
		pc("[06.000056] +overlay finished");
	}
	else
	{ 
		PS("[06.%s] +echo \"Using Keyboard Layout QWERTZ (GERMAN)", dl);
		DEBUG_MSG("IO:       Using Keyboard Layout QWERTZ (German)...");
		scan = 3;
		dbg(true);
	}
	pc("[06.000081] +setIOKbLeyout to %s", (scan == 2) ? "QWERTY - US" : "QWERTZ - DE");
	if (scan == 2) // QWERTY -- US
	{
		unsigned char tasciiNonShift[100] = {
		0, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BACKSPACE,
		TAB, 'q', 'w',   'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',   '[', ']', ENTER, 0,
		'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
		'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ', 0,
		KF1, KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
		KHOME, KUP, KPGUP,'-', KLEFT, '5', KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12 };
		tasciiNonShift[86] = '<';

		unsigned char tasciiShift[100] = {
		0, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', BACKSPACE,
		TAB, 'Q', 'W',   'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',   '{', '}', ENTER, 0,
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' ', 0,
		KF1,   KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
		KHOME, KUP, KPGUP, '-', KLEFT, '5',   KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12 };
		tasciiShift[86] = '>';
		
		unsigned char tasciiAltGR[100] = {
 			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			 0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 
		};
		tasciiAltGR[86] = '|';
		int i;
		for(i = 0; i <= 100; i++)
		{
			asciiNonShift[i] = tasciiNonShift[i];
			asciiShift[i] = tasciiShift[i];
			asciiAltGR[i] = tasciiAltGR[i];
		}
	}
	else // QWERTZ -- Germany
	{
		unsigned char tasciiNonShift[100] = {
		0, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 225,0, BACKSPACE,
		TAB, 'q', 'w',   'e', 'r', 't', 'z', 'u', 'i', 'o', 'p',   0, '+', ENTER, 0,//29
		'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0,0, '#', 0, '#',
		'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', '<', 0, 0, ' ', 0,
		KF1, KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
		KHOME, KUP, KPGUP,'-', KLEFT, '5', KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12 };
		tasciiNonShift[86] = '<';
		tasciiNonShift[26] = (char) 129;
		tasciiNonShift[39] = (char) 148;
		tasciiNonShift[40] = (char) 132;
		tasciiNonShift[41] = (char) 95;
		
		unsigned char tasciiShift[100] = {
		0, ESC, '!', '\"', 0, '$', '%', '&', '/', '(', ')', '=', '?', '`', BACKSPACE,
		TAB, 'Q', 'W',   'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',   0, '*', ENTER, 0,
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0,0, '\'', 0, '\'',
		'Y', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', '>', 0, 0, ' ', 0,
		KF1,   KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
		KHOME, KUP, KPGUP, '-', KLEFT, '5',   KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12 };
		tasciiShift[86] = '>';
		tasciiShift[26] = (char) 154;
		tasciiShift[39] = (char) 153;
		tasciiShift[40] = (char) 142;
		tasciiShift[41] = (char) 248;
		
		unsigned char tasciiAltGR[100] = {
			0, 170, 173,253,0, 0,0,0, '{', '[', ']', '}', '\\', 0, 0,
			0, '@', 0,   0, 0, 0,0,0, 0,237,0,   0, '~', 0, 0,
			145, 0, 0, 0,0,0, 0, 0,0,0, '^', '`', 0, 0,
			0,0,0,0,0,0, 0,0,0, 0, '|', 0, 0, 0, 0,
			0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};
		tasciiAltGR[86] = '|';
		
		int i;
		for(i = 0; i < 100; i++)
		{
			asciiNonShift[i] = tasciiNonShift[i];
			asciiShift[i] = tasciiShift[i];
			asciiAltGR[i] = tasciiAltGR[i];
		}
	}
	pc("[06.000165] +ret");
}
