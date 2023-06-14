#ifndef MYCLASS_H
#define MYCLASS_H

class Myclass{
    int baseValue;
public:
    Myclass(int _bv);       //public constructor
    void Increment(int byValue);
    int getValue();
};

#endif