#include <iostream>
#include <string.h>
#include "../include/class.h"
// class Myclass{
//     int baseValue;
// public:
//     Myclass(int _bv):baseValue(_bv){}
//     void Increment(int byValue){
//             baseValue += byValue;
//     }
//     int getValue(){return baseValue;}
// };
// The header file declares the class and function signatures
// The cpp file implements the functions and includes the header
// The include guard ensures the header can be included multiple times
Myclass::Myclass(int _bv):baseValue(_bv){}
void Myclass::Increment(int byValue){
    baseValue += byValue;
}
int Myclass::getValue(){
    return baseValue;
}

