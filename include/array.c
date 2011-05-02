#ifndef ARRAY_C
#define ARRAY_C
void aget(int element, aStringArray a, char start[])
{
	StringArray e = a;
	int i;
	for (i = 0; i < element; i++) e = e.next;
	cpystr(start, e.get);
}
void aappend(aStringArray a, char start[])
{
	StringArray e = a;
	int i;
	for (i = 0; i < a.size; i++) e = e.next;
	StringArray n;
	cpystr(n.get, start);
	e.next = n;
	a.size++;
}
void aremove(aStringArray a)
{
	StringArray e = a;
	int i;
	for (i = 0; i < a.size-1; i++) e = e.next;
	e.next = 0;
	a.size--;
}
bool acontains(aStringArray a, char start[])
{
	StringArray e = a;
	int i;
	for (i = 0; i < a.size; i++)
	{
		if (cmpstr(e.get, start)) return true;
		e = e.next;
	}
	return false;
}
#endif
