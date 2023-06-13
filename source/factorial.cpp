#include <iostream>
#include "../include/factorial.h"
int Factorial(int n){  // Returns the factorial of n
    if (n <= 1) return 1;
    return n*Factorial(n-1);  
}
// int main(){
    
//     printf("%d", Factorial(4));
// }

// in test.cpp file code:
// "#include <iostream>
// #include "../include/factorial.h"
// int main(){
//     printf("%d", Factorial(4));
// }"

// to bukild  g++ factorial.cpp test.cpp 