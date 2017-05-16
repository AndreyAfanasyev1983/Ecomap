#include <cs50.h>
#include <stdio.h>

int main(void)
{
   int height;
   do
    {
        printf(" Give  integer no greater than 23: "); 
        height = get_int();  
    }
    while (height <= 0 || height > 23);
    printf("Valid integer!\nHere is Mario's half-pyramid of height %i:\n", height);
    
  for (int i = 0; i < height; i++)
   {
       int  middle = height + 1 - i - 2;
       for (int j = 0 ; j < middle  ; j++)
       {
             printf (" "); 
       }
          for (int j = middle; j <= height   ; j++)
       {
             printf ("#"); 
       }
      printf ("\n");
   }
 return 0;  
}