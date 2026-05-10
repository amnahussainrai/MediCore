#pragma once
#include <iostream>
#include "Person.h"
using namespace std;

class Patient :public Person
{
private:
    int age;
    char gender[6];
    double balance;
    int contact;
    int choice;

public:
    Patient();
    Patient(int id, const char* n, const char* p, int age, const char* g, double b, int contact);
    void displaymenu();
    void bookappointment();
    int comparison(char a[], char b[]);
    void cancelappointment();
    void viewappointments();
};

