#ifdef CHANGED

#include "syscall.h"


void print(char c, int n)
{
	int i;
	for (i = 0; i < n; i++) 
	{
		PutChar(c+i);
	}
	
	PutChar('\n');
}
int main()
{
	//print('a',4);
	//char * test = (char *) malloc (sizeof(char)*120);
	char* temp = "dfcghvjbnksffffffffffffffffffff";
	PutString(temp);
	return 0;
}
#endif