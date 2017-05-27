#include<stdio.h>
#include<cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main (int argc, string argv[])

{ 
    if (argc != 2)
    {
        printf("Error\n");
        return 1;
    }
    
    int key = atoi(argv[1]);
    key = key % 26;
    
    printf("plaintext:");
    string plain_text = get_string();
    
    printf("ciphertext:");
    for(int i = 0, n = strlen(plain_text); i < n; i++)
    {
        if (isalpha(plain_text[i]))
        {
            if (plain_text[i] >= 'a' && plain_text[i] <= 'z')
            {
                int crypted = plain_text[i] + key;
                if (crypted > 'z')
                {
                    int shift = (crypted - 'a') % 26;
                    crypted = 'a' + shift;
                }
                printf("%c", crypted);
            }
            
            if (plain_text[i] >= 'A' && plain_text[i] <= 'Z')
            {
                int crypted = plain_text[i] + key;
                if (crypted > 'Z')
                {
                    int shift = (crypted - 'A') % 26;
                    crypted = 'A' + shift;
                }
                printf("%c", crypted);
            }
        }
        else
        {
            printf("%c", plain_text[i]);
        }
    }
    
    printf("\n");
    
   
    return 0;
}