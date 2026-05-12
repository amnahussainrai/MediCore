#pragma once
#include "Person.h"
class Admin :public Person
{
private:

public:
	Admin();
	Admin(int id, const char* n, const char* p);
	void addDoctor();
	void removeDoctor();
	void addPatient();
	void removePatient();
	int comparison(char a[], char b[]);
	void viewAllPatients();
	void viewAllDoctors();
	void viewAllAppointments();
	void viewUnpaidBills();
	void dischargePatient();
	void displaymenu();
	void viewSecurityLog();
	void generateDailyReport();















};
