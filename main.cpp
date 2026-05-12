#include <iostream>
#include <fstream>
#include <ctime>
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "HospitalException.h"
#include "Storage.h"
#include "FileHandler.h"
using namespace std;

void logAttempt(const char* role, int id, const char* result)
{
    time_t t = time(0);
    struct tm now;
    localtime_s(&now, &t);
    char timestamp[25];
    strftime(timestamp, 25, "%d-%m-%Y %H:%M:%S", &now);

    ofstream log("security_log.txt", ios::app);
    log << timestamp << "," << role << "," << id << "," << result << endl;
    log.close();
}

bool loginPatient(int& enteredID)
{
    // load patients from file into Storage
    Storage<Patient> patients;
    FileHandler::loadPatients(patients);

    char enteredPass[20];
    cout << "Enter Patient ID: ";
    cin >> enteredID;
    cout << "Enter Password: ";
    cin >> enteredPass;

    // search Storage instead of file
    Patient* p = patients.findByID(enteredID);

    if (p == nullptr)
    {
        cout << "Invalid ID or password.\n";
        return false;
    }

    // check password
    int k = 0; bool passMatch = true;
    const char* filePass = p->getpass();
    while (enteredPass[k] != '\0' && filePass[k] != '\0')
    {
        if (enteredPass[k] != filePass[k]) { passMatch = false; break; }
        k++;
    }
    if (enteredPass[k] != '\0' || filePass[k] != '\0') passMatch = false;

    if (!passMatch)
    {
        cout << "Invalid ID or password.\n";
        return false;
    }

    logAttempt("Patient", enteredID, "SUCCESS");

    int c = 0;
    do {
        p->displaymenu();
        c = p->getchoice();
        if (c == 1) p->bookappointment();
        else if (c == 2) p->cancelappointment();
        else if (c == 3) p->viewappointments();
        else if (c == 4) p->viewMedicalRecords();
        else if (c == 5) p->viewbills();
        else if (c == 6) p->paybill();
        else if (c == 7) p->topupBalance();
    } while (c != 8);

    return true;
}

bool loginDoctor(int& enteredID)
{
    // load doctors from file into Storage
    Storage<Doctor> doctors;
    FileHandler::loadDoctors(doctors);

    char enteredPass[20];
    cout << "Enter Doctor ID: ";
    cin >> enteredID;
    cout << "Enter Password: ";
    cin >> enteredPass;

    Doctor* d = doctors.findByID(enteredID);

    if (d == nullptr)
    {
        cout << "Invalid ID or password.\n";
        return false;
    }

    int k = 0; bool passMatch = true;
    const char* filePass = d->getpass();
    while (enteredPass[k] != '\0' && filePass[k] != '\0')
    {
        if (enteredPass[k] != filePass[k]) { passMatch = false; break; }
        k++;
    }
    if (enteredPass[k] != '\0' || filePass[k] != '\0') passMatch = false;

    if (!passMatch)
    {
        cout << "Invalid ID or password.\n";
        return false;
    }

    logAttempt("Doctor", enteredID, "SUCCESS");

    int c = 0;
    do {
        d->displaymenu();
        c = d->getchoice();
        if (c == 1) d->viewTodaysAppointments();
        else if (c == 2) d->appointmentcomplete();
        else if (c == 3) d->appointmentnoshow();
        else if (c == 4) d->writeprescription();
        else if (c == 5) d->viewPatientHistory();
    } while (c != 6);

    return true;
}

bool loginAdmin(int& enteredID)
{
    // load admins from file into Storage
    Storage<Admin> admins;
    FileHandler::loadAdmins(admins);

    char enteredPass[20];
    cout << "Enter Admin ID: ";
    cin >> enteredID;
    cout << "Enter Password: ";
    cin >> enteredPass;

    Admin* a = admins.findByID(enteredID);

    if (a == nullptr)
    {
        cout << "Invalid ID or password.\n";
        return false;
    }

    int k = 0; bool passMatch = true;
    const char* filePass = a->getpass();
    while (enteredPass[k] != '\0' && filePass[k] != '\0')
    {
        if (enteredPass[k] != filePass[k]) { passMatch = false; break; }
        k++;
    }
    if (enteredPass[k] != '\0' || filePass[k] != '\0') passMatch = false;

    if (!passMatch)
    {
        cout << "Invalid ID or password.\n";
        return false;
    }

    logAttempt("Admin", enteredID, "SUCCESS");

    int c = 0;
    do {
        a->displaymenu();
        c = a->getchoice();
        if (c == 1) a->addDoctor();
        else if (c == 2) a->removeDoctor();
        else if (c == 3) a-> addPatient();
        else if (c == 4) a-> removePatient();
        else if (c == 5) a->viewAllPatients();
        else if (c == 6) a-> viewAllDoctors();
        else if (c == 7) a->viewAllAppointments();
        else if (c == 8) a->viewUnpaidBills();
        else if (c == 9) a->dischargePatient();
        else if (c == 10) a->viewSecurityLog();
        else if (c == 11) a->generateDailyReport();
    } while (c != 12);

    return true;
}

int main()
{
    int failCount = 0;
    int enteredID = 0;

    while (true)
    {
        cout << "Welcome to MediCore Hospital Management System" << endl;
        cout << "==============================================" << endl;
        cout << "Login as:" << endl;
        cout << "1. Patient" << endl;
        cout << "2. Doctor" << endl;
        cout << "3. Admin" << endl;
        cout << "4. Exit" << endl;

        int roleChoice;
        cin >> roleChoice;

        if (roleChoice == 4) break;

        bool success = false;

        if (roleChoice == 1)      success = loginPatient(enteredID);
        else if (roleChoice == 2) success = loginDoctor(enteredID);
        else if (roleChoice == 3) success = loginAdmin(enteredID);

        if (!success)
        {
            failCount++;
            if (failCount >= 3)
            {
                cout << "Account locked. Contact admin." << endl;
                if (roleChoice == 1)      logAttempt("Patient", enteredID, "FAILED");
                else if (roleChoice == 2) logAttempt("Doctor", enteredID, "FAILED");
                else if (roleChoice == 3) logAttempt("Admin", enteredID, "FAILED");
                return 0;
            }
        }
        else failCount = 0;
    }

    return 0;
}