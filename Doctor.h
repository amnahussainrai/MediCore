#pragma once
#include "Person.h"
#include <ctime>
class Doctor :public Person
{
private:
	char specialization[20];
	long int contact;
	double fee;
	
public:
	Doctor();
	Doctor(int id, const char* n, const char* p, const char* s, double fee, long int contact);
	int comparison(char a[], char b[]);
	void displaymenu();
	void viewTodaysAppointments();
	void appointmentcomplete();
	void appointmentnoshow();
	void writeprescription();
	void viewPatientHistory();


};
