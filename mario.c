#include <cs50.h>
#include <stdio.h>

int main(void)
{
   int height ;
   
   do
    {
        printf("Give a nomber from 0 to 23:");
        height = GetInt();
    }
    while (height < 0 || height > 23);
  
    
  for (int i = 0; i < height; i++)
   {
       int  middle = height + 1 - i - 2;
       for (int j = 0 ; j < middle  ; j++)
       {
             printf (" "); 
       }
          for (int j = middle; j <= height ; j++)
       {
             printf ("#"); 
       }
      printf ("\n");
   }
 return 0;  
}