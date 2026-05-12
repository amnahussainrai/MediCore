
#include "FileHandler.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Storage.h"
#include <fstream>
#include <iostream>
using namespace std;

// ─── LOAD PATIENTS ────────────────────────────────────────
void FileHandler::loadPatients(Storage<Patient>& store)
{
    ifstream file("patients.txt");
    if (!file) { cout << "Error opening patients.txt\n"; return; }

    char line[300];
    while (file.getline(line, 300))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;

        char id[10];      j = 0; while (line[i] != ',') id[j++] = line[i++];      id[j] = '\0';      i++;
        char name[20];    j = 0; while (line[i] != ',') name[j++] = line[i++];    name[j] = '\0';    i++;
        char age[5];      j = 0; while (line[i] != ',') age[j++] = line[i++];     age[j] = '\0';     i++;
        char gender[6];   j = 0; while (line[i] != ',') gender[j++] = line[i++];  gender[j] = '\0';  i++;
        char contact[15]; j = 0; while (line[i] != ',') contact[j++] = line[i++]; contact[j] = '\0'; i++;
        char password[20]; j = 0; while (line[i] != ',') password[j++] = line[i++]; password[j] = '\0'; i++;
        char balance[20]; j = 0; while (line[i] != '\0') balance[j++] = line[i++]; balance[j] = '\0';

        Patient p(atoi(id), name, password, atoi(age), gender,
            atof(balance), atol(contact));
        store.add(p);
    }
    file.close();
}

// ─── LOAD DOCTORS ─────────────────────────────────────────
void FileHandler::loadDoctors(Storage<Doctor>& store)
{
    ifstream file("doctors.txt");
    if (!file) { cout << "Error opening doctors.txt\n"; return; }

    char line[300];
    while (file.getline(line, 300))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;

        char id[10];      j = 0; while (line[i] != ',') id[j++] = line[i++];      id[j] = '\0';      i++;
        char name[30];    j = 0; while (line[i] != ',') name[j++] = line[i++];    name[j] = '\0';    i++;
        char spec[20];    j = 0; while (line[i] != ',') spec[j++] = line[i++];    spec[j] = '\0';    i++;
        char contact[15]; j = 0; while (line[i] != ',') contact[j++] = line[i++]; contact[j] = '\0'; i++;
        char password[20]; j = 0; while (line[i] != ',') password[j++] = line[i++]; password[j] = '\0'; i++;
        char fee[20];     j = 0; while (line[i] != '\0') fee[j++] = line[i++];    fee[j] = '\0';

        Doctor d(atoi(id), name, password, spec, atof(fee), atol(contact));
        store.add(d);
    }
    file.close();
}

// ─── LOAD ADMINS ──────────────────────────────────────────
void FileHandler::loadAdmins(Storage<Admin>& store)
{
    ifstream file("admin.txt");
    if (!file) { cout << "Error opening admin.txt\n"; return; }

    char line[200];
    while (file.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;

        char id[10];      j = 0; while (line[i] != ',') id[j++] = line[i++];       id[j] = '\0';      i++;
        char name[20];    j = 0; while (line[i] != ',') name[j++] = line[i++];     name[j] = '\0';    i++;
        char password[20]; j = 0; while (line[i] != '\0') password[j++] = line[i++]; password[j] = '\0';

        Admin a(atoi(id), name, password);
        store.add(a);
    }
    file.close();
}

// ─── GET MAX ID FROM FILE ─────────────────────────────────
int FileHandler::getMaxID(const char* filename)
{
    ifstream file(filename);
    char line[300];
    int maxID = 0;
    while (file.getline(line, 300))
    {
        if (line[0] == '\0') continue;
        int j = 0; char id[10];
        while (line[j] != ',' && line[j] != '\0') id[j] = line[j++];
        id[j] = '\0';
        int cur = atoi(id);
        if (cur > maxID) maxID = cur;
    }
    file.close();
    return maxID;
}

// ─── APPEND APPOINTMENT ───────────────────────────────────
int FileHandler::appendAppointment(int patientID, int doctorID,
    const char* date, const char* time)
{
    int newID = getMaxID("appointments.txt") + 1;
    ofstream file("appointments.txt", ios::app);
    file << newID << "," << patientID << "," << doctorID << ","
        << date << "," << time << ",pending" << endl;
    file.close();
    return newID;
}

// ─── APPEND BILL ──────────────────────────────────────────
void FileHandler::appendBill(int patientID, int appointmentID,
    double amount, const char* date)
{
    int newID = getMaxID("bills.txt") + 1;
    ofstream file("bills.txt", ios::app);
    file << newID << "," << patientID << "," << appointmentID << ","
        << amount << ",unpaid," << date << endl;
    file.close();
}

// ─── APPEND PATIENT ───────────────────────────────────────
void FileHandler::appendPatient(int id, const char* name, int age,
    const char* gender, long int contact,
    const char* password, double balance)
{
    ofstream file("patients.txt", ios::app);
    file << id << "," << name << "," << age << "," << gender << ","
        << contact << "," << password << "," << balance << endl;
    file.close();
}

// ─── APPEND DOCTOR ────────────────────────────────────────
void FileHandler::appendDoctor(int id, const char* name, const char* spec,
    long int contact, const char* password, double fee)
{
    ofstream file("doctors.txt", ios::app);
    file << id << "," << name << "," << spec << "," << contact << ","
        << password << "," << fee << endl;
    file.close();
}

// ─── UPDATE PATIENT BALANCE ───────────────────────────────
void FileHandler::updatePatientBalance(int patientID, double newBalance,
    const char* name, int age,
    const char* gender, long int contact,
    const char* password)
{
    ifstream file("patients.txt");
    ofstream temp("temp.txt");
    char line[300];
    while (file.getline(line, 300))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;
        char id[10]; j = 0;
        while (line[i] != ',') id[j++] = line[i++]; id[j] = '\0';

        if (atoi(id) == patientID)
            temp << patientID << "," << name << "," << age << ","
            << gender << "," << contact << "," << password << ","
            << newBalance << endl;
        else
            temp << line << endl;
    }
    file.close(); temp.close();
    if (remove("patients.txt") != 0)
        cout << "Error deleting file!\n";
    if (rename("temp.txt", "patients.txt") != 0)
        cout << "Error renaming file!\n";
    
}

// ─── UPDATE BILL STATUS ───────────────────────────────────
void FileHandler::updateBillStatus(int billID, const char* newStatus)
{
    ifstream file("bills.txt");
    ofstream temp("temp.txt");
    char line[200];
    while (file.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;

        // extract id
        char id[10]; j = 0;
        while (line[i] != ',') id[j++] = line[i++]; id[j] = '\0'; i++;

        if (atoi(id) == billID)
        {
            // patientID
            char patID[10]; j = 0;
            while (line[i] != ',') patID[j++] = line[i++]; patID[j] = '\0'; i++;
            // appointmentID
            char appID[10]; j = 0;
            while (line[i] != ',') appID[j++] = line[i++]; appID[j] = '\0'; i++;
            // amount
            char amount[20]; j = 0;
            while (line[i] != ',') amount[j++] = line[i++]; amount[j] = '\0'; i++;
            // skip old status
            while (line[i] != ',') i++; i++;
            // date
            char date[15]; j = 0;
            while (line[i] != '\0') date[j++] = line[i++]; date[j] = '\0';

            temp << id << "," << patID << "," << appID << ","
                << amount << "," << newStatus << "," << date << endl;
        }
        else
            temp << line << endl;
    }
    file.close(); temp.close();
    if (remove("bills.txt") != 0)
        cout << "Error deleting file!\n";
    if (rename("temp.txt", "bills.txt") != 0)
        cout << "Error renaming file!\n";
    
}

// ─── UPDATE APPOINTMENT STATUS ────────────────────────────
void FileHandler::updateAppointmentStatus(int appointmentID, const char* newStatus)
{
    ifstream file("appointments.txt");
    ofstream temp("temp.txt");
    char line[200];
    while (file.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;

        char id[10]; j = 0;
        while (line[i] != ',') id[j++] = line[i++]; id[j] = '\0'; i++;

        if (atoi(id) == appointmentID)
        {
            char patID[10]; j = 0;
            while (line[i] != ',') patID[j++] = line[i++]; patID[j] = '\0'; i++;
            char docID[10]; j = 0;
            while (line[i] != ',') docID[j++] = line[i++]; docID[j] = '\0'; i++;
            char date[11]; j = 0;
            while (line[i] != ',') date[j++] = line[i++]; date[j] = '\0'; i++;
            char time[6]; j = 0;
            while (line[i] != ',') time[j++] = line[i++]; time[j] = '\0'; i++;
            // skip old status

            temp << id << "," << patID << "," << docID << ","
                << date << "," << time << "," << newStatus << endl;
        }
        else
            temp << line << endl;
    }
    file.close(); temp.close();
    if (remove("appointments.txt") != 0)
        cout << "Error deleting file!\n";
    if (rename("temp.txt", "appointments.txt") != 0)
        cout << "Error renaming file!\n";
    
}

// ─── DELETE PATIENT ───────────────────────────────────────
void FileHandler::deletePatient(int patientID)
{
    ifstream file("patients.txt");
    ofstream temp("temp.txt");
    char line[300];
    while (file.getline(line, 300))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;
        char id[10]; j = 0;
        while (line[i] != ',') id[j++] = line[i++]; id[j] = '\0';
        if (atoi(id) != patientID)
            temp << line << endl;
    }
    file.close(); temp.close();
    if (remove("patients.txt") != 0)
        cout << "Error deleting file!\n";
    if (rename("temp.txt", "patients.txt") != 0)
        cout << "Error renaming file!\n";
  
}

// ─── DELETE DOCTOR ────────────────────────────────────────
void FileHandler::deleteDoctor(int doctorID)
{
    ifstream file("doctors.txt");
    ofstream temp("temp.txt");
    char line[300];
    while (file.getline(line, 300))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;
        char id[10]; j = 0;
        while (line[i] != ',') id[j++] = line[i++]; id[j] = '\0';
        if (atoi(id) != doctorID)
            temp << line << endl;
    }
    file.close(); temp.close();
    if (remove("doctors.txt") != 0)
        cout << "Error deleting file!\n";
    if (rename("temp.txt", "doctors.txt") != 0)
        cout << "Error renaming file!\n";
    
}

// ─── CASCADE DELETE PATIENT ───────────────────────────────
void FileHandler::cascadeDeletePatient(int patientID)
{
    // 1. delete from patients.txt
    deletePatient(patientID);

    // 2. delete from appointments.txt
    {
        ifstream file("appointments.txt");
        ofstream temp("temp.txt");
        char line[200];
        while (file.getline(line, 200))
        {
            if (line[0] == '\0') continue;
            int i = 0, j = 0;
            // skip appointment id
            while (line[i] != ',') i++; i++;
            // get patient id
            char pid[10]; j = 0;
            while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0';
            if (atoi(pid) != patientID)
                temp << line << endl;
        }
        file.close(); temp.close();
        if (remove("appointments.txt") != 0)
            cout << "Error deleting file!\n";
        if (rename("temp.txt", "appointments.txt") != 0)
            cout << "Error renaming file!\n";
       
    }

    // 3. delete from bills.txt
    {
        ifstream file("bills.txt");
        ofstream temp("temp.txt");
        char line[200];
        while (file.getline(line, 200))
        {
            if (line[0] == '\0') continue;
            int i = 0, j = 0;
            // skip bill id
            while (line[i] != ',') i++; i++;
            // get patient id
            char pid[10]; j = 0;
            while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0';
            if (atoi(pid) != patientID)
                temp << line << endl;
        }
        file.close(); temp.close();
        if (remove("bills.txt") != 0)
            cout << "Error deleting file!\n";
        if (rename("temp.txt", "bills.txt") != 0)
            cout << "Error renaming file!\n";
      
    }

    // 4. delete from prescriptions.txt
    {
        ifstream file("prescriptions.txt");
        ofstream temp("temp.txt");
        char line[500];
        while (file.getline(line, 500))
        {
            if (line[0] == '\0') continue;
            int i = 0, j = 0;
            // skip prescription id
            while (line[i] != ',') i++; i++;
            // skip appointment id
            while (line[i] != ',') i++; i++;
            // get patient id
            char pid[10]; j = 0;
            while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0';
            if (atoi(pid) != patientID)
                temp << line << endl;
        }
        file.close(); temp.close();
        if (remove("prescriptions.txt") != 0)
            cout << "Error deleting file!\n";
        if (rename("temp.txt", "prescriptions.txt") != 0)
            cout << "Error renaming file!\n";
      
    }
}