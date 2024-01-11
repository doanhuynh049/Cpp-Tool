cd src/test
g++ -c bubbleSortTest.cpp -o bubbleSortTest.o -I./.. -I/usr/src/gtest/include -L/usr/src/gtest/lib -lgtest -lgtest_main -lpthread
g++ bubbleSortTest.o ../bubbleSort.o -I.. -I/usr/src/gtest/include -L/usr/src/gtest/lib -lgtest -lgtest_main -lpthread -o outputTest
#./outputTest
./outputTest --gtest_output='xml:./outputTest.xml' #create report
cat outputTest.xml
