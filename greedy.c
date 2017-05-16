#include <cs50.h>
#include <stdio.h>

int main()
{
    printf("O hai!");
    float money = -1.0;
    while(money < 0)
    
    {
    printf("How much change is owed?\n");
    money = get_float();
        
    }
    
     int money_1=money*100+0.5;
   
     int money_25 = money_1/25;
     //printf("%i\n",money_25);
     
     int money_ref =money_1%25;
     //printf("%i\n", money_ref);
     
     int money_10 = money_ref/10;
     int money_ref10 =money_ref%10;
     
     int money_5 = money_ref10/5;
     int money_ref5 =money_ref10%5;
     
     int money1 = money_ref5;
     
     int money_s = money_25 + money_10 + money_5 + money1;
     printf ("%i\n",money_s);
     
}







