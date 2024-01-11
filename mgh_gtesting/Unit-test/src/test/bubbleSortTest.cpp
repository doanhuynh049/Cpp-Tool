#include <gtest/gtest.h>
#include "../bubbleSort.h"

// original array
static const int arr_origin[] = {4, 0, 3, 2, 1};

class BubbleSortTest: public ::testing::Test
{
public:
    int arr_test[5];

protected:
    virtual void SetUp()
    {
        // Initialize before each test
        for(int i = 0; i < 5; i++)
            arr_test[i] = arr_origin[i];
    }

    virtual void TearDown()
    {
        // Deinitialize after each test
    }
};

// Test bubble sort function
TEST_F (BubbleSortTest, bubbleSortReturnSortedArray_TEST)
{
    //int arr_test[] = {4, 0, 2, 1, 3};
    int arr_expected[] =  {0, 1, 2, 3, 4};

    // Run sort
    bubbleSort(arr_test, 5);

    for(int i = 0; i < 5; i++)
        EXPECT_EQ(arr_expected[i], arr_test[i]);
}

