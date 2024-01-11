// bubbleSort.cpp
#include "bubbleSort.h"
#include <algorithm> // for std::swap

void bubbleSort(int arr[], int n)
{
    bool swapped;
    for (int i = 0; i < n - 1; i++)
    {
        swapped = false;
        for (int j = n - 1; j > i; j--)
        {
            if (arr[j] < arr[j - 1])
            {
                std::swap(arr[j], arr[j - 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

