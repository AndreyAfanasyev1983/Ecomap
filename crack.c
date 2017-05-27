#define _XOPEN_SOURCE
#include <unistd.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool bruteforce(string hash, string salt, int index, string password)
{
    if(index >=4)
    {
       return false; 
    }
    
    char symbols[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for(int i = 0, n = strlen(symbols); i < n; i++)
    {
        password[index] = symbols[i];
        password[index +1] = '\0';
        string new_hash = crypt(password, salt);
        printf("%s has hash %s\n", password, new_hash);
        if(strcmp(hash, new_hash) == 0)
        {
            return true;
        }
        else if(bruteforce(hash, salt, index +1, password))
        {
            return true;
        }
   
    }
    
    return false;
}

int main(int argc, string argv[])
{
    if(argc != 2)
    {
        printf("Eror\n");
        return 1;
    }
    
    string hash = argv[1];
    char salt[] = "50";
    char password[5];
    
    bool ok = bruteforce(hash, salt, 0, password);
    if(ok)
    {
        printf("%s\n", password);
    }
    
    return 0;
}