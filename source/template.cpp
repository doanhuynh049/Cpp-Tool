#include <iostream>
#include "../include/template.h"

// using namespace std;
// Remove command above. Instead, we use prefix std:: to define the source of objects and classes

void myMaxTemplate()
{
    std::cout << myMax(3, 7) << std::endl;
    std::cout << myMax(3.2, 5.3) << std::endl;
    std::cout << myMax('g', 'a') << std::endl;
}
