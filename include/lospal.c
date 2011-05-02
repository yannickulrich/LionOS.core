/*
 * LOSPaL - LionOS Programming Language
 * Interpreter Implementation
 */
#ifndef LOSPaL
#define LOSPaL
bool isInt(char number[]);

void execute(char start[][])
{
	if (!validateSyntax(start))
	{
		kprintf("Syntax error\n");
		return;
	}
	// TODO: Execute Code
}
aStringArray jumpmarks;
aStringArray vars;
bool validateSyntax(char start[])
{
	// TODO: Split into lines and validate them
	return true;
}
bool validateSyntaxLine(char start[])
{
	// TODO: Validate Syntax
	if (cmpstr("clear", start)) return true;
	if (startsWith(start, "//")) return true;
	if (startsWith(start, ": "))
	{
		if (!acontains(jumpmarks, substringToEnd(start, 2)) && !isInt(substringToEnd(start, 2)))
			aappend(jumpmarks, substringToEnd(start, 2));
		else return false; // ...............................Mark already exists
	}
	if (startsWith(start, "var "))
	{
		if (!acontains(vars, substringToEnd(start, 4)) && !isInt(substringToEnd(start, 4)))
			aappend(vars, substringToEnd(start, 4));
		else return false; // ...............................Mark already exists
	}
	if (startsWith(start, "jump "))
		if (!acontains(jumpmarks, substringToEnd(start, 5))) return false;
	if (startsWith(start, "input "))
		if (!acontains(vars, substringToEnd(start, 6))) return false;
	if (startsWith(start, "fix "))
	{
		bool r = false;
		if (acontains(vars, substringToEnd(start, 4))) r = true;
		if (isInt(substringToEnd(start, 4))) r = true;
		return r;
	}
	return true;
}
bool isInt(char number[])
{
	int i; 
	for (i = 0; i < strlen(number); i++)
	{
		if (!(number[i] != '\0' || (number[i] >= 48 && number[i] < 58))) return false;
	}
	return true;
}
#endif
