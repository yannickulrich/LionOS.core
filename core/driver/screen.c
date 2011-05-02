void pn(unsigned int input);
/**
 * Changes the color for the next print.
	 Code | Color
	--------------------------------------------------
		0 | black (schwarz)
		1 | darkblue (dunkelblau)
		2 | darkgreen (dunkelgrün)
		3 | teal (dunkeltürkis)
		4 | darkred (dunkelrot)
		5 | darkpurple (dunkellila)
		6 | darkyellow/orange (dunkelgelb / orange)
		7 | grey (grau)
		8 | dark grey (dunkelgrau)
		9 | lightblue (hellblau)
		A | lightgreen (hellgrün)
		B | lightturqoise (helltürkis)
		C | lightred (hellrot)
		D | lightpurple (hellila)
		E | lightyellow (hellgelb)
		F | white (weiss)
 */
void changeColor(char bg, char fg)
{
	color = (char *) ((bg << 4) | (fg & 0x0F));
}
int fix = 0;

/**
 * The kernel method to update the cursor and make VideoShift.
 * @param row Zeile
 * @param col Spalte
 */
void updateCursor()
{
	// Do VideoShift
	if ((charCounter - (charCounter % screenwidth)) / screenwidth > 24)
	{
		int i;
		for(i = fix * 2 * 80; i < 2 * (46 - (fix * 2)) * 80; i++)
			video[i] = video[i+160];
		for(i = 2 * 24 * 80; i < (2 * 26 * 80) - 1; i += 2)
		{
			video[i] = 0;
			video[i + 1] = 0x0F;
		}
		c_counter -= 80;
	}
	int tmpc = charCounter;
	if (!cursor) tmpc = 26 * 80;
	// cursor HIGH port to vga INDEX register
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (unsigned char) ((tmpc >> 8) & 0xFF));
	// cursor LOW port to vga INDEX register
	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (unsigned char) (tmpc & 0xFF));
}


void setCursorVisibility(bool visible)
{
	cursor = visible;
}

/**
 * Sets the fixed lines (header lines).
 * @param nfix Count of fixed lines.
 */
void setFix(int nfix)
{
	if (nfix >= 0 && nfix < 23)
	    fix = nfix;
}
/**
 * Clears the screen.
 */
void clearscreen()
{
	int i;
	for (i = fix * 160; i < 25 * 160; i += 2) //Repeating through the graphic memory
	{
		video[i] = ' ';
		video[i + 1] = 0x0F;
	} 
	c_counter = fix * 80;
}

/**
 * Prints a char.
 * @param The Char
 */
void printChar(char hw)
{
	if (hw == '\n')
		c_counter = c_counter + (80 - c_counter % 80);
	else if (hw == '\b')
    {
        c_counter--;
        printChar(' ');
        c_counter--;
    }
    else if (hw == '\t')
        c_counter += (((c_counter % 4) == 0) ? 4 : (c_counter % 4));
	else
	{
		video[c_counter * 2] = hw;
        video[c_counter * 2 + 1] = (int) color;
		c_counter++;
	}
	updateCursor();
    if (!readFromKeyBoard)
		backSpaceEnd = charCounter;
}

void printCharAt(char hw, int x, int y)
{
	if (hw == '\n')
		return;
	else if (hw == '\b')
		return;
    else if (hw == '\t')
		return;
	else
	{
		video[(y * 80 + x) * 2] = hw;
        video[(y * 80 + x) * 2 + 1] = (int) color;
	}
}

/**
 * Prints an integer value
 * @param input The unsigned value.
 */
void pn(unsigned int input)
{
	int ldb = input % 10;
	int n = (int) input / 10;
	if (input > 9) pn(n);
	printChar(ldb + 48);
}

/**
 * The kernel method for the well known function printf in C.
 * Makros:
 *  - %c Prints a char
 *  - %d Prints an integer
 *  - %b Prints a bool
 *  - %s Prints a string
 * @param *args The Arguments
 */
void kprintf(char* args, ...) //Only kernel
{
	writingType = INSDEL;
	PRINTF_STUB();
}

void echo(char * args, ...)
{
	writingType = INSDEL;
	PRINTF_STUB();
	kprintf("\n");
}

char* en(int d)
{
	char ret[7];
	if (d > 99999) ret[0] = ((int) d / 100000) % 10 + 48;
	if (d > 9999) ret[1] = ((int) d / 10000) % 10 + 48;
	if (d > 999) ret[2] = ((int) d / 1000) % 10 + 48;
	if (d > 99) ret[3] = ((int) d / 100) % 10 + 48;
	if (d > 9) ret[4] = ((int) d / 100) % 10 + 48;
	ret[5] = d % 10 + 48;
	ret[6] = '\0';
	char* cc = &(ret[0]);
	return cc;
}

bool keyboardi = false;
void dbg(bool sucess)
{
	if (!DEBUG || POST) return;
	int i = c_counter % 80;
	for (; i < 65; i++)
		kprintf(" ");
	changeColor(0x0, 0xc);
	if (sucess == 1)
	{
		changeColor(0x0, 0xa);
		kprintf("Finish\n");
	}
	else if (sucess == 2)
	{
		changeColor(0x0, 0xe);
		kprintf("Skipped\n");
	}
	else 
	{
		kprintf("Fail\n");
		//getChar();
	}
	changeColor(0x0, 0xf);
}

