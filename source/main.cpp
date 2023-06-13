#include <iostream>
#include <gtest/gtest.h>
#include "../include/factorial.h"
using namespace std;
// TEST( Testname, Subtest_1){
//     ASSERT_TRUE(1 == 2);
//     cout<<"After assertion 1"<<endl;
// }
// TEST( Testname2, Subtest_2){
//     ASSERT_FALSE(1 == 2);
//     cout<<"After assertion 2"<<endl;
// }
// TEST( Testname, Subtest_2){
//     EXPECT_EQ(1 ,2);
//     cout<<"After assertion 2"<<endl;
// }
// TEST(CustomFailureMessages, Vectors) {
//   std::vector<int> x = {1, 2, 3};
//   std::vector<int> y = {1, 2, 4};
  
//   ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";
  
//   for (int i = 0; i < x.size(); i++) {
//     EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;  
//   }
// }
TEST(Factorial_test, Handle_zeroInput){
    ASSERT_EQ(Factorial(0),3);
    EXPECT_EQ(Factorial(0),1);
}
TEST(Factorial_test, Handle_positiveInput){
    EXPECT_EQ(Factorial(1),1);
    EXPECT_EQ(Factorial(2),2);
    EXPECT_EQ(Factorial(3),6);
    EXPECT_EQ(Factorial(8),40320);
}
int main(int argc, char **argv){
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}