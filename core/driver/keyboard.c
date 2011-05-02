/**
 * Adds a KeyListener.
 * @param *h The KeyListener
 */
void addKeyListener(void (*h)(char c, int scan, bool* supress))
{
	keylisteners = h;
}

/**
 * Flush method.
 */
void flush()
{
	int temp;
	do //flush the keyboard controller
	{
		temp = inportb( 0x64 );
		if(temp & 1) inportb( 0x60 );
	}
	while (temp & 2);
	while (inportb(0x64)&1)
		inportb(0x60);
	actualChar = 0;
}

/**
 * The kernel method to install a Keyboard controller.
 */
void installKeyboard()
{
	DEBUG_MSG("IO:       Install Keyboard...");
	while (inportb(0x64)&1)
		inportb(0x60);
    dbg(true);
	DEBUG_MSG("IO:       Init Key Handler...");
	irq_install_handler(1, keyPressed_handler);
	actualChar = 0;
	int i;
	for(i=0;i<110;i++)
	{
		pressedKeys[i] = 0;
	}
    dbg(true);
}

/**
 * The kernel method to Fetch a Scancode from the Keyboard.
 * @return the typed key
 */
unsigned int FetchScancode()//Only kernel
{
    return(inportb(0x60)); // port 0x60: Keyboard
}


/**
 * The kernel method for a pressedkey. It is registered as a Keyhandler.
 * @param *r
 */
void keyPressed_handler(struct regs* r)//Only kernel
{
	unsigned char scancode = FetchScancode();
	KeyPressed = 1;
	// Key Released
	if (scancode & 0x80)
	{
  	    scancode &= 0x7F;
	    pressedKeys[scancode] = 0;
	    KeyPressed = 0;
        if ( scancode == KRLEFT_SHIFT || scancode == KRRIGHT_SHIFT )
            ShiftKeyDown = 0;
	    if (scancode == KRRIGHT_ALT)
			AltGRDown = 0;
        return;
    }
	char retchar;
	if (scancode == KRCAPS_LOCK)
	{
		if (CapsKeyDown) CapsKeyDown = 0;
		else CapsKeyDown = true;
	}
	if(ShiftKeyDown && CapsKeyDown )
		retchar = asciiNonShift[scancode];   // Non-Shift Codes
	else if(ShiftKeyDown && !CapsKeyDown )
		retchar = asciiShift[scancode];      // Shift Codes
	else if(!ShiftKeyDown && CapsKeyDown )
		retchar = asciiShift[scancode];      // Shift Codes
	else if (AltGRDown)
		retchar = asciiAltGR[scancode];      // Alt-Gr Codes
	else
		retchar = asciiNonShift[scancode];   // Non-Shift Codes

	if( KBLayout == 2 && AltGRDown )
		retchar = asciiAltGR[scancode];
	//KeyListener
	void (*handler)(char c, int scan, bool* b);
	bool c = true;
	handler = keylisteners;
	if (handler)
		handler(retchar, scancode, &c); 
	if (!c) return;
	pressedKeys[scancode] = 1;
    if ( scancode == KRLEFT_SHIFT || scancode == KRRIGHT_SHIFT )
    {
        ShiftKeyDown = 1; // Use asciiShift characters
        return;
	}
	if (scancode == KRRIGHT_ALT)
	{
		AltGRDown = 1;
		return;
	}
	if (!scanUse) return;
	switch(scancode) {
		case KLEFT: 
			if (backSpaceEnd > c_counter+1)
				return;
			c_counter--;
			updateCursor();
			return;
		case KUP:
			return;
		case KDOWN: 
			return;
		case KRIGHT: 
			c_counter++;
			updateCursor();
			return;
	}
	if (pressedKeys[KRLEFT_CTRL])
	{
		printChar('^');
		printChar(asciiShift[scancode]);
		return;
	}
	
	if((!(scancode == KRLEFT_SHIFT || scancode == KRRIGHT_SHIFT || scancode == KRCAPS_LOCK)) && (KeyPressed == 1)) //filter Shift Key and Key Release
	{
		if ( retchar == '\b' )
		{
			if (backSpaceEnd > c_counter-1)
				return;
			printChar('\b');
			printChar(' ');
			printChar('\b');
			actualChar = retchar;
			return;
		}
		actualChar = retchar;
		readFromKeyBoard = true;
		if (retchar == '\n')
			readFromKeyBoard = false;
		printChar(retchar);
		return;
	}
	else
		return;
}

/**
 * The kernel method to get a char form the keyboard
 * @return The typed char
 */
char getChar()
{	
	asm("sti");
	char c;
	while(actualChar == 0);
	c = actualChar;
	actualChar = 0;
	asm("cli");
	return c;
}




bool editorMode = false;
/**
 * Sets the EditorMode.
 * @use Only use for UserMode
 * @param value Value
 */
void setEditorMode(bool value)
{
	editorMode = value;
}

//void irq_install_handler(int irq, void (*handler)(struct regs* r));

void USscanChar(char* c)
{
	*c = getChar();
}


//#include "../mm.h"
/**
 * Scans a String from the Keyboard.
 * @param *result The pointer for the result.
 * @return The length of the String
 */
int scanString( char * result )
{
	acTask->writing = 1;
	scanUse = true;
	char c = ' ';
	int i = 0;
	while(1)
	{	
		c = getChar();
		if (c == '\n') 
			break;
		else if( c == '\b' )
		{
			i--;
			result[i] = 0;
			continue;
		}
		else
		{
			result[i] = c;
			i++;
		}
	}
	result[i] = '\0';
	scanUse = false;
	acTask->writing = 0;
	return i;
}

/**
 * The kernel intern method to scan an int.
 * @return The scanned int
 */
int scanInt()
{
	acTask->writing = 1;
	scanUse = true;
	char c;
	int akEnter = 0;
	int num = 0;
	int i = 0;
	while(1)
	{
		c = getChar();
		if (c == '\n') break;
		else
		{
			akEnter = scan -1;
			if (scan == 11)
			{
				num *= 10;
				continue;
			}
			num = num * 10 + (akEnter - 48);
			i++;
		}
	}
	scanUse = false;
	acTask->writing = 0;
	return num;
}

/**
 * Scans an integer value from the keyboard to the pointer *p.
 * @param *p The pointer of the result value.
 */
void USscanInt(int *p)
{
	acTask->writing = 1;
	scanUse = true;
	bool invert = false;
	char c;
	int i = 0;
	while(1)
	{
		c = getChar();
		if (c == '\n') break;
		else if (c == '-' && i == 0) invert = true;
		else
		{
			c -= 48;
			*p *= 10;
			*p += c;
			i++;
		}
	}
	if (invert) *p *= -1;
	scanUse = false;
	acTask->writing = 0;
}


#include "strings.h"

/**
 * Scans an bool value from the keyboard to the pointer *p.
 * @param *p The pointer of the result value.
 * @deprecated Is not in use, missing libs
 */
void USscanBool(bool *p)
{
	//char result[5];
	//scanString(&(result[0]));
	//*p = !(result[0] == 'f' && result[0] == 'a' && result[0] == 'l' && result[0] == 's' && result[0] == 'e');
	*p = false;
}



//Note: just for core.c! No use after this anymore!!!

unsigned int GetScancode()//Only Kernel
{
    unsigned int scancode; // For getting the keyboard raw scancode
    while(1) // Loop until a key to be pressed
    {
        // Wait for the key
        while ( !(inportb(0x64)&1) ); // 0x64: read keyboard µC status register
        scancode = FetchScancode();
		KeyPressed = 1;
	
        if ( scancode & 0x80 ){ // Key released? Check bit 7 (10000000b = 0x80) of scan code for this
            scancode &= 0x7F; // Key was released, compare only low seven bits: 01111111b = 0x7F
	    	KeyPressed = 0;

            if ( scancode == KRLEFT_SHIFT || scancode == KRRIGHT_SHIFT )
                ShiftKeyDown = 0;
			if (scancode == KRRIGHT_ALT)
				AltGRDown = 1;
            continue;    // Loop
        }

        // Key was pressed. Capture scan code of shift key, if pressed
        if ( scancode == KRLEFT_SHIFT || scancode == KRRIGHT_SHIFT )
        {
            ShiftKeyDown = 1; // It is down, use asciiShift characters
            continue; // Loop, so it will not return a scan code for the shift key
        }
		if (scancode == KRRIGHT_ALT)
		{
			AltGRDown = 1;
			continue;
		}
        return scancode;
    }
}

