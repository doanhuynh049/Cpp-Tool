// main.cpp
#include <stdio.h>
#include "bubbleSort.h"


// Function to print an array
void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
         printf("%d ", arr[i]);
    }
    printf("\n");
}

int main()
{
    printf("Start main funtion\n");

    int arr[5] = {0, 3, 1, 4, 2};
    int len = sizeof(arr) / sizeof(arr[0]);

    bubbleSort(arr, len); // run sort

    printf("Sorted array: \n");
    printArray(arr, len);
    return 0;
}

