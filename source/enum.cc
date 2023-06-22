#include <iostream>
using namespace std;
void EnumGender(){
    enum Gender {Male, Female};
    Gender gender = Male;
    switch (gender){
        case Male:
            cout<< "Male" <<endl;
            break;
        case Female:
            cout << "Female" <<endl;
            break;
        default:
            cout << "Value can be Male or Female";
        
    }
    
}