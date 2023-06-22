#include <gtest/gtest.h>

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
// TEST(Factorial_test, Handle_zeroInput){
//     ASSERT_EQ(Factorial(0),3);
//     EXPECT_EQ(Factorial(0),1);
// }
// TEST(Factorial_test, Handle_positiveInput){
//     EXPECT_EQ(Factorial(1),1);
//     EXPECT_EQ(Factorial(2),2);
//     EXPECT_EQ(Factorial(3),6);
//     EXPECT_EQ(Factorial(8),40320);
// }



struct ClassTest:public testing::Test{
    Myclass *mc;
    void SetUp(){
        mc = new Myclass(100);
    }
    void TearDown(){
        delete mc;
    }
};
TEST_F(ClassTest, increment_by_5){
    //Arrange 
    // Myclass mc(100);
    //actor
    mc->Increment(5);
    //assert
    ASSERT_EQ(mc->getValue(),105);
}
TEST_F(ClassTest, increment_by_10){
    //Arrange 
    // Myclass mc(100);
     //actor
    mc->Increment(5);
    //assert
    ASSERT_EQ(mc->getValue(),105);
}

struct StackTest:public testing::Test{
    Stack s1;
    void SetUp(){
        int value[]={1,2,3,4,5,6,7,8,9};
        for (auto &val: value){
            s1.push(val);
        }
    }
    void TearDown(){
    }
};

TEST_F(StackTest, PopTest){
    int lastValue = 9;
    while (lastValue != 1){
        ASSERT_EQ(s1.pop(), lastValue--);
    }
}

TEST_F(StackTest, PopTest1){
    int lastValue = 10;
    while (lastValue != 1){
        EXPECT_EQ(s1.pop(), lastValue--);
    }
}

int MainTest(int argc, char **argv){
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}