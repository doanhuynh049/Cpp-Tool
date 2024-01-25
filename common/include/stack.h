#ifndef STACK_H
#define STACK_H
#include <vector>
using namespace std;

class Stack{
private:
    std::vector <int> vstack = {};
public:
    void push(int value);
    int pop();
    bool empty();
    int size();
};

#endif