#pragma once
#include <iostream>
#include "Person.h"

class Patient :public Person
{
private:
    int age;
    char gender[6];
    double balance;
   long  int contact;
   

public:
    Patient();
    Patient(int id, const char* n, const char* p, int age, const char* g, double b, long int contact);
    void displaymenu();
    void bookappointment();
    int comparison(char a[], char b[]);
    void cancelappointment();
    void viewappointments();
    void viewMedicalRecords();
    void viewbills();
    void paybill();
    void topupBalance();
    
};

