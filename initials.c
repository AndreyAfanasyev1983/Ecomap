#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main ()
{
	string input = "";
	int input_length = 0;
	
	input = GetString();
	input_length = strlen(input);
	
	printf("%c", toupper(input[0]));
	
	for(int i = 0; i < input_length; i++)
	{
		if(input[i-1] == 32) {
			printf("%c", toupper(input[i]));
		}
	}
	printf("\n");
}