/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##   #  # #   #
      #     #  #   #  # #  #   #     #  #  # ## ##
      ####  #   ###   # #   ###   ###    ##  # # #
                           Dialog
 * @author AE, AG
 */
#ifndef DIALOG_C
#define DIALOG_C 1
typedef struct {
	int left;
	int right;
	int top;
	int bottom;
} Box;
int showDialog(char text[], char title[], int optionType)
{
	// Now we have to save the graphic memory
	int oldFix = screen.fix;
	int oldCC;
	//asm("int $0x29" : : "a" (26), "b" (&oldCC)); // Ccounter save... solveIT
	setFix(0);
	char oldVideo[screen.width * screen.height * 2];
	char* video = (char*) 0xb8000;
	int i;
	for (i = 0; i < screen.width * screen.height * 2; i++)
		oldVideo[i] = video[i];
	clearscreen();
	int width = 56, height = 12;
	Box margin;
	margin.left = (screen.width - width) / 2;
	margin.top = (screen.height - height) / 2;
	for (i = 0; i < margin.top; i++)
		kprintf("\n");
	
	// Line 1
	for (i = 0; i < margin.left; i++)
		kprintf(" ");
	kprintf("%c", 201);
	for(i = 0; i < width - 2; i++) 
		kprintf("%c", 205); 
	kprintf("%c\n", 187);
	
	// Line 2
	for (i = 0; i < margin.left; i++)
		kprintf(" ");
	kprintf("%c ", 186); 
	kprintf("%s", title); 
	int titleSpace = width - 3 - strlen(title);
	for(i = 0; i < titleSpace; i++) 
		kprintf(" "); 
	kprintf("%c\n", 186);
	
	// Line 3
	for (i = 0; i < margin.left; i++)
		kprintf(" ");
	kprintf("%c", 204); 
	for(i = 0; i < width - 2; i++) 
		kprintf("%c", 205); 
	kprintf("%c\n", 185);
	
	// Line 4
	for (i = 0; i < margin.left; i++)
		kprintf(" ");
	kprintf("%c", 186); 
	for(i = 0; i < width - 2; i++) 
		kprintf(" "); 
	kprintf("%c\n", 186);
	
	// And now the text...
	for (i = 0; i < margin.left; i++)
		kprintf(" ");
    kprintf("%c  ", 186);
    if (strlen(text) > 52)
    {
    	return -1;
    }
	else 
	{
		kprintf("%s", text);
		for (i = 0; i < width - 5 - strlen(text); i++) kprintf(" ");
	}
    kprintf(" %c\n", 186);
	
	for (i = 0; i < margin.left; i++)
		kprintf(" ");
	kprintf("%c", 186); 
	for(i = 0; i < width - 2; i++) 
		kprintf(" "); 
	kprintf("%c\n", 186);
	
	// The Buttons
	for (i = 0; i < margin.left; i++)
		kprintf(" ");
	kprintf("%c", 186);
	kprintf(" %c", 218); 
	for(i = 0; i < 14; i++) 
		kprintf("%c",196); 
	kprintf("%c", 191); 
	kprintf(" %c", 218); 
	for(i = 0; i < 14; i++) 
		kprintf("%c",196); 
	kprintf("%c", 191); 
	for(i = 0; i < width-4-32; i++) 
		kprintf(" ");
	kprintf("%c\n", 186);
	for (i = 0; i < margin.left; i++)
		kprintf(" ");

	kprintf("%c", 186);
	if (optionType == YES_NO_OPTION)
	{
		kprintf(" %c     Yes      %c", 179,179);
		kprintf(" %c      No      %c", 179,179);
	}
	else 
	{
		kprintf(" %c      OK      %c", 179,179);
		kprintf(" %c    Cancel    %c", 179,179);
	}
	for(i = 1; i<= (width-4)-32 ; i++) kprintf(" ");
	kprintf("%c\n", 186);
	for (i = 0; i < margin.left; i++)
		kprintf(" ");

	kprintf("%c", 186);
	kprintf(" %c", 192); for(i = 1; i<= 14 ; i++) kprintf("%c",196); kprintf("%c", 217); 
	kprintf(" %c", 192); for(i = 1; i<= 14 ; i++) kprintf("%c",196); kprintf("%c", 217); 
	for(i = 1; i<= (width-4)-32 ; i++) kprintf(" ");
	kprintf("%c\n", 186);
	for (i = 0; i < margin.left; i++)
		kprintf(" ");
	
	kprintf("%c", 200);
 	for(i = 0; i < width - 2; i++) 
 		kprintf("%c", 205);
	kprintf("%c\n", 188);
	
	// Restore Graphic Memory...
	char tmp[1];
	if (optionType == YES_NO_OPTION)
		kprintf("\n\n                                Awnser(ENTER, [n]): ");
	else
		kprintf("\n\n                                Awnser(ENTER, [c]): ");
	getString(tmp);
	int ret = 0;
	if (tmp[0] != 0) ret = 1; 
	clearscreen();
	for (i = 0; i < oldCC * 2; i+=2)
	{
		if (i == 80 * 2 * oldFix) setFix(oldFix); 
		//if (oldVideo[i+1] == 0) break; 
		//else 
		{
			chColor(oldVideo[i+1] >> 4, oldVideo[i+1]);
			kprintf("%c", oldVideo[i]);
		}
	}
	
	return ret;
}
#endif
