#include <iostream>
#include <vector>
#include "../include/stack.h"

void Stack::push(int value){
    vstack.push_back(value);
}
int Stack::pop(){
    if (Stack::size() >= 1){
        int value = vstack.back();
        vstack.pop_back();
        return value;
    }
    else{
        return -1;
    }
}
bool Stack::empty(){
    if(vstack.size() > 0) return true;
    else return false;
}
int Stack::size(){
    return vstack.size();
}