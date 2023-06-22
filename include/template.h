#ifndef MY_MAX_TEMPLATE_H
#define MY_MAX_TEMPLATE_H 
template <typename T>
T myMax(const T &x, const T &y) // use const keyword to ensure we don't modify in function
{
    return (x > y) ? x : y;
}
void myMaxTemplate();
#endif