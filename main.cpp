#include <iostream>
#include "Patient.h"   // include derived class
using namespace std;

int main()
{
    // Example: create patient object
    Patient p(1, "Ahmed Ali", "pass123", 28, "Male", 5000, 03001234567);

    // Call menu OR directly booking function
    p.displaymenu();
   // p.bookappointment();//menu does get displayed again but it does not really work figure that out later
    return 0;
}
