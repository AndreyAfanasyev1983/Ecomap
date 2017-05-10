#include <cs50.h>
#include <stdio.h>
int main(void)
{
 
   int n ;
   do 
 {
   printf ("How many minutes did you take shower?\n ");
   n = get_int (); 
       
    }
   while (n < 0);
   printf ("You used %i bottles \n", n * 12 ); 
return 0;
    
}