
/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */

bool binary_search(int value, int values[], int start, int stop)
{
    int middle= (start + stop)*0.5;
    if (values[middle]==value)
    {
      return true;   
    }
    if (start==stop)
    {
        return false;
    }
    if (values[middle]<value)
    {
        return binary_search(value,  values, middle+1, stop);
    }
    else
    {
         return binary_search(value,  values, start, middle);
    }
    return false;
}

bool search(int value, int values[], int n)
{ 
    if (n<0)
    {
        return false;
    }
    return binary_search(value, values, 0, n);
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{   
    bool swapped = false;
    for(int i = 0; i < n - 1; i ++)
    {
        for(int j = 0; j < n - 1; j ++)
        {
            if(values[j] > values[j + 1])
            {
                int tmp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = tmp;
                swapped = true;
            }
        }
        if (swapped != false)
        {
            break;
        }
        // for(int i = 0; i < n; i++)
        // {
        // printf("[%i],", values[i]);
        // }
    }
}