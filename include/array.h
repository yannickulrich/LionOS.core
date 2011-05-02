#ifndef ARRAY
#define ARRAY
typedef struct {
	char get[32];
	aStringArray next;
	int size;
} aStringArray;
void aget(int element, aStringArray a, char start[]);
void aappend(aStringArray a, char start[]);
void aremove(aStringArray a);
bool acontains(a StringArray a, char start[]);
#endif
