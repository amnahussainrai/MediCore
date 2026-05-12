#pragma once
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include <fstream>
#include <iostream>
using namespace std;

class FileHandler
{
public:
    // --- LOAD ---
    static void loadPatients(Storage<Patient>& store);
    static void loadDoctors(Storage<Doctor>& store);
    static void loadAdmins(Storage<Admin>& store);

    // --- APPEND ---
    static int  appendAppointment(int patientID, int doctorID, const char* date, const char* time);
    static void appendBill(int patientID, int appointmentID, double amount, const char* date);
    static void appendPatient(int id, const char* name, int age, const char* gender, long int contact, const char* password, double balance);
    static void appendDoctor(int id, const char* name, const char* spec, long int contact, const char* password, double fee);

    // --- UPDATE ---
    static void updatePatientBalance(int patientID, double newBalance, const char* name, int age, const char* gender, long int contact, const char* password);
    static void updateBillStatus(int billID, const char* newStatus);
    static void updateAppointmentStatus(int appointmentID, const char* newStatus);

    // --- DELETE ---
    static void deletePatient(int patientID);
    static void deleteDoctor(int doctorID);

    // --- CASCADE DELETE ---
    static void cascadeDeletePatient(int patientID);  // deletes appointments, bills, prescriptions too

    // --- HELPERS ---
    static int getMaxID(const char* filename);
};