#include "Person.h"

Person::Person()
{
	id = 0;

    char temp[6] = "Blank";

    int i = 0;
    while (temp[i] != '\0')
    {
        name[i] = temp[i];
        password[i] = temp[i];
        i++;
    }
    name[i] = '\0';
    password[i] = '\0';

}

Person::Person(int id, const char* n, const char* p)
{
    this->id = id;
    int i = 0;
    while (n[i] != '\0')
    {
        name[i] = n[i];
        i++;
    }
    int j = 0;
    while (p[j] != '\0')
    {
        password[j] = p[j];
        j++;
    }
    name[i] = '\0';
    password[j] = '\0';

}
const char* Person::getpass() 
{
    return password;
}

const char* Person::getName() 
{
    return name;
}

int Person::getid() 
{
    return id;
}
int Person::getchoice()
{
    return choice;
}