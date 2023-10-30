#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "Librarycode.hpp"
TEST(TestCountPositives, BasicTest)
{
    std::vector<int> inputVector{ 1, -2, 3 - 4, 5, -6, -7 };
    int count = countPositives(inputVector);
    ASSERT_EQ(3, count);
}
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TEST();
}
