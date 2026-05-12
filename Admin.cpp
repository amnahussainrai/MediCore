#include "Admin.h"
#include <fstream>
#include <iostream>
#include<string>
#include <cstring>
#include "FileHandler.h"
#include "HospitalException.h"
#include "Validator.h"
using namespace std;
Admin::Admin():Person()
{

}
Admin::Admin(int id, const char* n, const char* p) :Person(id,n,p)
{

}
void Admin::displaymenu()
{
    cout << "Admin Panel-MediCore" <<  endl;
    cout << "============================" << endl;
    cout << "1. Add Doctor" << endl;
    cout << "2. Remove Doctor" << endl;
    cout << "3. Add Patient" << endl;
    cout << "4. Remove Patient" << endl;
    cout << "5. View all Patients" << endl;
    cout << "6. View all Doctors" << endl;
    cout << "7. View all Appointments" << endl;
    cout << "8. View Unpaid Bills" << endl;
    cout << "9. Discharge Patient" << endl;
    cout << "10. View Security Log" << endl;
    cout << "11. Generate Daily Report" << endl;
    cout << "12. Log Out" << endl;
    cin >> choice;
}
void Admin::viewSecurityLog()
{
    
        ifstream file("security_log.txt");
        if (!file)
        {
            cout << "Error opening security log!\n";
            return;
        }

        char line[200];
        bool empty = true;

        while (file.getline(line, 200))
        {
            if (line[0] == '\0') continue;  // skip blank lines
            empty = false;
            cout << line << endl;
        }

        if (empty)
            cout << "No security logs found." << endl;

        file.close();
    
}
int Admin::comparison(char a[], char b[])
{
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0')
    {
        if (tolower(a[i]) != tolower(b[i]))
            return 0;
        i++;
    }

    // ignore trailing spaces or hidden chars
    while (a[i] == ' ') i++;
    while (b[i] == ' ') i++;

    if (a[i] == '\0' && b[i] == '\0')
        return 1;

    return 0;
}

void Admin::addDoctor()
{
    char name[50];
    char specialization[50];
    char contact[20];
    char password[20];
    double fee;

    cin.ignore(1000, '\n');

    cout << "Enter name: ";
    cin.getline(name, 50);

    cout << "Enter specialization: ";
    cin.getline(specialization, 50);

    // --- contact validation with 3 attempts ---
    int attempts = 0;
    bool valid = false;
    while (attempts < 3)
    {
        cout << "Enter contact (11 digits): ";
        cin.getline(contact, 20);
        try
        {
            if (!Validator::validateContact(contact))
                throw InvalidInputException();
            valid = true;
            break;
        }
        catch (InvalidInputException& e)
        {
            cout << e.what() << endl;
            attempts++;
        }
    }
    if (!valid) { cout << "Too many invalid attempts. Returning to menu.\n"; return; }

    // --- password validation with 3 attempts ---
    attempts = 0; valid = false;
    while (attempts < 3)
    {
        cout << "Enter password (min 6 characters): ";
        cin.getline(password, 20);
        try
        {
            if (!Validator::validatePassword(password))
                throw InvalidInputException();
            valid = true;
            break;
        }
        catch (InvalidInputException& e)
        {
            cout << e.what() << endl;
            attempts++;
        }
    }
    if (!valid) { cout << "Too many invalid attempts. Returning to menu.\n"; return; }

    // --- fee validation with 3 attempts ---
    attempts = 0; valid = false;
    while (attempts < 3)
    {
        cout << "Enter consultation fee: ";
        cin >> fee;
        try
        {
            if (!Validator::validateAmount(fee))
                throw InvalidInputException();
            valid = true;
            break;
        }
        catch (InvalidInputException& e)
        {
            cout << e.what() << endl;
            attempts++;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    if (!valid) { cout << "Too many invalid attempts. Returning to menu.\n"; return; }

    // --- generate new ID using FileHandler ---
    int newID = FileHandler::getMaxID("doctors.txt") + 1;

    // --- append using FileHandler ---
    FileHandler::appendDoctor(newID, name, specialization,
        atol(contact), password, fee);

    cout << "Doctor added successfully. ID: " << newID << endl;
}
void Admin::removeDoctor()
{
    char line[200];
    ifstream file("doctors.txt");
    if (!file) { throw FileNotFoundException(); return; }

    bool foundAny = false;
    cout << "\nDoctors List:\n";

    while (file.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j;

        char id[10]; j = 0;
        while (line[i] != ',') id[j++] = line[i++]; id[j] = '\0'; i++;

        char name[50]; j = 0;
        while (line[i] != ',') name[j++] = line[i++]; name[j] = '\0'; i++;

        char spec[50]; j = 0;
        while (line[i] != ',') spec[j++] = line[i++]; spec[j] = '\0'; i++;

        while (line[i] != ',') i++; i++;  // skip contact
        while (line[i] != ',') i++; i++;  // skip password

        char fee[20]; j = 0;
        while (line[i] != '\0') fee[j++] = line[i++]; fee[j] = '\0';

        cout << "ID: " << id
            << " | Name: " << name
            << " | Specialization: " << spec
            << " | Fee: " << fee << endl;
        foundAny = true;
    }
    file.close();

    if (!foundAny) { cout << "No doctors available.\n"; return; }

    int chosenID;
    cout << "\nEnter Doctor ID to remove: ";
    cin >> chosenID;

    // --- check pending appointments ---
    ifstream apfile("appointments.txt");
    while (apfile.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j;

        while (line[i] != ',') i++; i++;  // skip appointment ID
        while (line[i] != ',') i++; i++;  // skip patient ID

        char docID[10]; j = 0;
        while (line[i] != ',') docID[j++] = line[i++]; docID[j] = '\0'; i++;

        while (line[i] != ',') i++; i++;  // skip date
        while (line[i] != ',') i++; i++;  // skip time

        char status[20]; j = 0;
        while (line[i] != '\0') status[j++] = line[i++]; status[j] = '\0';

        if (atoi(docID) == chosenID && comparison(status, (char*)"pending"))
        {
            cout << "Cannot remove doctor with pending appointments.\n";
            cout << "Cancel or reassign them first.\n";
            apfile.close();
            return;
        }
    }
    apfile.close();

    // --- delete using FileHandler ---
    FileHandler::deleteDoctor(chosenID);

    cout << "Doctor removed.\n";
}
void Admin::addPatient()
{
    char name[50];
    char genderStr[6];
    char contact[20], password[20];
    int age;
    double balance;

    cin.ignore(1000, '\n');

    cout << "Enter name (max 50 chars): ";
    cin.getline(name, 50);

    // --- age validation with 3 attempts ---
    int attempts = 0; bool valid = false;
    while (attempts < 3)
    {
        cout << "Enter age: ";
        cin >> age;
        try
        {
            if (!Validator::validateAge(age))
                throw InvalidInputException();
            valid = true; break;
        }
        catch (InvalidInputException& e)
        {
            cout << e.what() << endl;
            attempts++;
        }
    }
    if (!valid) { cout << "Too many invalid attempts. Returning to menu.\n"; return; }

    // --- gender validation with 3 attempts ---
    attempts = 0; valid = false;
    while (attempts < 3)
    {
        cout << "Enter gender (M/F): ";
        cin >> genderStr;
        try
        {
            if (!Validator::validateGender(genderStr))
                throw InvalidInputException();
            valid = true; break;
        }
        catch (InvalidInputException& e)
        {
            cout << e.what() << endl;
            attempts++;
        }
    }
    if (!valid) { cout << "Too many invalid attempts. Returning to menu.\n"; return; }

    // --- contact validation with 3 attempts ---
    attempts = 0; valid = false;
    while (attempts < 3)
    {
        cout << "Enter contact (11 digits): ";
        cin >> contact;
        try
        {
            if (!Validator::validateContact(contact))
                throw InvalidInputException();
            valid = true; break;
        }
        catch (InvalidInputException& e)
        {
            cout << e.what() << endl;
            attempts++;
        }
    }
    if (!valid) { cout << "Too many invalid attempts. Returning to menu.\n"; return; }

    // --- password validation with 3 attempts ---
    attempts = 0; valid = false;
    while (attempts < 3)
    {
        cout << "Enter password (min 6 chars): ";
        cin >> password;
        try
        {
            if (!Validator::validatePassword(password))
                throw InvalidInputException();
            valid = true; break;
        }
        catch (InvalidInputException& e)
        {
            cout << e.what() << endl;
            attempts++;
        }
    }
    if (!valid) { cout << "Too many invalid attempts. Returning to menu.\n"; return; }

    // --- balance validation with 3 attempts ---
    attempts = 0; valid = false;
    while (attempts < 3)
    {
        cout << "Enter initial balance: ";
        cin >> balance;
        try
        {
            if (!Validator::validateAmount(balance))
                throw InvalidInputException();
            valid = true; break;
        }
        catch (InvalidInputException& e)
        {
            cout << e.what() << endl;
            attempts++;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    if (!valid) { cout << "Too many invalid attempts. Returning to menu.\n"; return; }

    // --- generate new ID using FileHandler ---
    int newID = FileHandler::getMaxID("patients.txt") + 1;

    // --- append using FileHandler ---
    FileHandler::appendPatient(newID, name, age, genderStr,
        atol(contact), password, balance);

    cout << "\nPatient added successfully. ID: " << newID << endl;
}
void Admin::removePatient()
{
    ifstream file("patients.txt");
    if (!file) { throw FileNotFoundException(); return; }

    char line[200];
    cout << "\nAll Patients:\n";

    while (file.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j;

        char id[10], name[50], age[10], gender[10], contact[20], balance[20];

        j = 0; while (line[i] != ',') id[j++] = line[i++];      id[j] = '\0';      i++;
        j = 0; while (line[i] != ',') name[j++] = line[i++];    name[j] = '\0';    i++;
        j = 0; while (line[i] != ',') age[j++] = line[i++];     age[j] = '\0';     i++;
        j = 0; while (line[i] != ',') gender[j++] = line[i++];  gender[j] = '\0';  i++;
        j = 0; while (line[i] != ',') contact[j++] = line[i++]; contact[j] = '\0'; i++;
        while (line[i] != ',') i++; i++;  // skip password
        j = 0; while (line[i] != '\0') balance[j++] = line[i++]; balance[j] = '\0';

        cout << "ID: " << id
            << " | Name: " << name
            << " | Age: " << age
            << " | Gender: " << gender
            << " | Contact: " << contact
            << " | Balance: " << balance << endl;
    }
    file.close();

    int chosenID;
    cout << "\nEnter Patient ID to remove: ";
    cin >> chosenID;

    // --- check pending appointments ---
    ifstream afile("appointments.txt");
    while (afile.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j;

        while (line[i] != ',') i++; i++;  // skip appointment ID

        char pid[10]; j = 0;
        while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0'; i++;

        while (line[i] != ',') i++; i++;  // skip doctor ID
        while (line[i] != ',') i++; i++;  // skip date
        while (line[i] != ',') i++; i++;  // skip time

        char status[20]; j = 0;
        while (line[i] != '\0') status[j++] = line[i++]; status[j] = '\0';

        if (atoi(pid) == chosenID && comparison(status, (char*)"pending"))
        {
            cout << "Cannot remove patient with pending appointments.\n";
            afile.close();
            return;
        }
    }
    afile.close();

    // --- check unpaid bills ---
    ifstream bfile("bills.txt");
    while (bfile.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j;

        while (line[i] != ',') i++; i++;  // skip bill ID

        char pid[10]; j = 0;
        while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0'; i++;

        while (line[i] != ',') i++; i++;  // skip appointment ID
        while (line[i] != ',') i++; i++;  // skip amount

        char status[20]; j = 0;
        while (line[i] != ',' && line[i] != '\0') status[j++] = line[i++]; status[j] = '\0';

        if (atoi(pid) == chosenID &&
            !comparison(status, (char*)"paid") &&
            !comparison(status, (char*)"cancelled"))
        {
            cout << "Cannot remove patient with unpaid bills.\n";
            bfile.close();
            return;
        }
    }
    bfile.close();

    // --- cascade delete using FileHandler ---
    FileHandler::cascadeDeletePatient(chosenID);

    cout << "\nPatient removed successfully.\n";
}
void Admin::viewAllPatients()
{
    ifstream pfile("patients.txt");

    if (!pfile)
    {
        cout << "Error opening patients file!\n";
        return;
    }

    char line[200];

    cout << "\nAll Patients:\n";

    while (pfile.getline(line, 200))
    {
        int i = 0, j;

        // ✅ patient ID
        char id[10];
        j = 0;
        while (line[i] != ',') id[j++] = line[i++];
        id[j] = '\0';
        int patientID = atoi(id);
        i++;

        // ✅ name
        char name[50];
        j = 0;
        while (line[i] != ',') name[j++] = line[i++];
        name[j] = '\0';
        i++;

        // ✅ age
        char age[10];
        j = 0;
        while (line[i] != ',') age[j++] = line[i++];
        age[j] = '\0';
        i++;

        // ✅ gender
        char gender[10];
        j = 0;
        while (line[i] != ',') gender[j++] = line[i++];
        gender[j] = '\0';
        i++;

        // ✅ contact
        char contact[20];
        j = 0;
        while (line[i] != ',') contact[j++] = line[i++];
        contact[j] = '\0';
        i++;

        // skip password
        while (line[i] != ',') i++;
        i++;

        // ✅ balance
        char balance[20];
        j = 0;
        while (line[i] != '\0') balance[j++] = line[i++];
        balance[j] = '\0';

        // ✅ STEP 2: count unpaid bills
        ifstream bfile("bills.txt");
        char bline[200];
        int unpaidCount = 0;

        while (bfile.getline(bline, 200))
        {
            int k = 0, l;

            // skip bill ID
            while (bline[k] != ',') k++;
            k++;

            // patient ID in bill
            char pid[10];
            l = 0;
            while (bline[k] != ',') pid[l++] = bline[k++];
            pid[l] = '\0';
            int billPID = atoi(pid);
            k++;

            // skip appointment ID
            while (bline[k] != ',') k++;
            k++;

            // skip amount
            while (bline[k] != ',') k++;
            k++;

            // ✅ status
            char status[20];
            l = 0;
            while (bline[k] != ',') status[l++] = bline[k++];
            status[l] = '\0';

            if (billPID == patientID && (!comparison(status, (char*)"paid")&&!comparison(status,(char*)"cancelled")))
            {
                unpaidCount++;
            }
        }

        bfile.close();

        // ✅ STEP 3: display
        cout << "ID: " << patientID
            << " | Name: " << name
            << " | Age: " << age
            << " | Gender: " << gender
            << " | Contact: " << contact
            << " | Balance: " << balance
            << " | Unpaid Bills: " << unpaidCount
            << endl;
    }

    pfile.close();
}
void Admin::viewAllDoctors()
{
    ifstream file("doctors.txt");

    if (!file)
    {
        cout << "Error opening doctors file!\n";
        return;
    }

    char line[200];

    cout << "\nAll Doctors:\n";

    while (file.getline(line, 200))
    {
        int i = 0, j;

        char id[10], name[50], spec[50], contact[20], password[20], fee[20];

        // ID
        j = 0;
        while (line[i] != ',')
            id[j++] = line[i++];
        id[j] = '\0';
        i++;

        // Name
        j = 0;
        while (line[i] != ',')
            name[j++] = line[i++];
        name[j] = '\0';
        i++;

        // Specialization
        j = 0;
        while (line[i] != ',')
            spec[j++] = line[i++];
        spec[j] = '\0';
        i++;

        // Contact
        j = 0;
        while (line[i] != ',')
            contact[j++] = line[i++];
        contact[j] = '\0';
        i++;

        // Skip password
        while (line[i] != ',')
            i++;
        i++;

        // Fee
        j = 0;
        while (line[i] != '\0')
            fee[j++] = line[i++];
        fee[j] = '\0';

        cout << "ID: " << id
            << " | Name: " << name
            << " | Specialization: " << spec
            << " | Contact: " << contact
            << " | Fee: PKR " << fee << endl;
    }

    file.close();
}
void Admin::viewAllAppointments()
{
    ifstream file("appointments.txt");

    if (!file)
    {
        cout << "Error opening appointments file!\n";
        return;
    }

    char line[200];

    // arrays
    int ids[100];
    char patientNames[100][50];
    char doctorNames[100][50];
    char dates[100][20];
    char times[100][10];
    char statuses[100][20];

    int count = 0;

    while (file.getline(line, 200))
    {
        int i = 0, j;

        // Appointment ID
        char id[10];
        j = 0;
        while (line[i] != ',')
            id[j++] = line[i++];
        id[j] = '\0';
        int appID = atoi(id);
        i++;

        // Patient ID
        char pid[10];
        j = 0;
        while (line[i] != ',')
            pid[j++] = line[i++];
        pid[j] = '\0';
        int patientID = atoi(pid);
        i++;

        // Doctor ID
        char did[10];
        j = 0;
        while (line[i] != ',')
            did[j++] = line[i++];
        did[j] = '\0';
        int doctorID = atoi(did);
        i++;

        // Date
        char date[20];
        j = 0;
        while (line[i] != ',')
            date[j++] = line[i++];
        date[j] = '\0';
        i++;

        // Time
        char time[10];
        j = 0;
        while (line[i] != ',')
            time[j++] = line[i++];
        time[j] = '\0';
        i++;

        // Status
        char status[20];
        j = 0;
        while (line[i] != '\0')
            status[j++] = line[i++];
        status[j] = '\0';

        // ✅ Get patient name
        ifstream pfile("patients.txt");
        char pname[50] = "Unknown";
        char pline[200];

        while (pfile.getline(pline, 200))
        {
            int pi = 0, pj;

            char id[10];
            pj = 0;
            while (pline[pi] != ',')
                id[pj++] = pline[pi++];
            id[pj] = '\0';
            int fileID = atoi(id);
            pi++;

            char name[50];
            pj = 0;
            while (pline[pi] != ',')
                name[pj++] = pline[pi++];
            name[pj] = '\0';

            if (fileID == patientID)
            {
                strcpy_s(pname, 50, name);
                break;
            }
        }
        pfile.close();

        // ✅ Get doctor name
        ifstream dfile("doctors.txt");
        char dname[50] = "Unknown";
        char dline[200];

        while (dfile.getline(dline, 200))
        {
            int di = 0, dj;

            char id[10];
            dj = 0;
            while (dline[di] != ',')
                id[dj++] = dline[di++];
            id[dj] = '\0';
            int fileID = atoi(id);
            di++;

            char name[50];
            dj = 0;
            while (dline[di] != ',')
                name[dj++] = dline[di++];
            name[dj] = '\0';

            if (fileID == doctorID)
            {
                strcpy_s(dname, 50, name);
                break;
            }
        }
        dfile.close();

        // store in arrays
        ids[count] = appID;
        strcpy_s(patientNames[count], 50, pname);
        strcpy_s(doctorNames[count], 50, dname);
        strcpy_s(dates[count], 20, date);
        strcpy_s(times[count], 10, time);
        strcpy_s(statuses[count], 20, status);
        count++;
    }

    file.close();

    if (count == 0)
    {
        cout << "No appointments found.\n";
        return;
    }

    // ✅ SORT BY DATE DESCENDING
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (strcmp(dates[j], dates[j + 1]) < 0) // descending
            {
                // swap all fields

                // ID
                int tempID = ids[j];
                ids[j] = ids[j + 1];
                ids[j + 1] = tempID;

                // patient
                char tempP[50];
                strcpy_s(tempP, 50, patientNames[j]);
                strcpy_s(patientNames[j], 50, patientNames[j + 1]);
                strcpy_s(patientNames[j + 1], 50, tempP);

                // doctor
                char tempD[50];
                strcpy_s(tempD, 50, doctorNames[j]);
                strcpy_s(doctorNames[j], 50, doctorNames[j + 1]);
                strcpy_s(doctorNames[j + 1], 50, tempD);

                // date
                char tempDate[20];
                strcpy_s(tempDate, 20, dates[j]);
                strcpy_s(dates[j], 20, dates[j + 1]);
                strcpy_s(dates[j + 1], 20, tempDate);

                // time
                char tempTime[10];
                strcpy_s(tempTime, 10, times[j]);
                strcpy_s(times[j], 10, times[j + 1]);
                strcpy_s(times[j + 1], 10, tempTime);

                // status
                char tempStatus[20];
                strcpy_s(tempStatus, 20, statuses[j]);
                strcpy_s(statuses[j], 20, statuses[j + 1]);
                strcpy_s(statuses[j + 1], 20, tempStatus);
            }
        }
    }

    // ✅ DISPLAY
    cout << "\nAll Appointments:\n";

    for (int i = 0; i < count; i++)
    {
        cout << "ID: " << ids[i]
            << " | Patient: " << patientNames[i]
            << " | Doctor: " << doctorNames[i]
            << " | Date: " << dates[i]
            << " | Time: " << times[i]
            << " | Status: " << statuses[i] << endl;
    }
}


void Admin::viewUnpaidBills()
{
    ifstream file("bills.txt");

    if (!file)
    {
        cout << "Error opening bills file!\n";
        return;
    }

    char line[200];
    bool found = false;

    // ✅ get today's date
    time_t now = time(0);
    tm today_tm;
    localtime_s(&today_tm, &now);

    cout << "\nUnpaid Bills:\n";

    while (file.getline(line, 200))
    {
        int i = 0, j;

        // bill ID
        char bid[10];
        j = 0;
        while (line[i] != ',')
            bid[j++] = line[i++];
        bid[j] = '\0';
        int billID = atoi(bid);
        i++;

        // patient ID
        char pid[10];
        j = 0;
        while (line[i] != ',')
            pid[j++] = line[i++];
        pid[j] = '\0';
        int patientID = atoi(pid);
        i++;

        // skip appointment ID
        while (line[i] != ',') i++;
        i++;

        // amount
        char amt[20];
        j = 0;
        while (line[i] != ',')
            amt[j++] = line[i++];
        amt[j] = '\0';
        double amount = atof(amt);
        i++;

        // status
        char status[20];
        j = 0;
        while (line[i] != ',')
            status[j++] = line[i++];
        status[j] = '\0';
        i++;

        // date
        char date[20];
        j = 0;
        while (line[i] != '\0')
            date[j++] = line[i++];
        date[j] = '\0';

        // ✅ check unpaid
        if (!comparison(status, (char*)"paid")&&!comparison(status,(char*)"cancelled"))
        {
            found = true;

            // ✅ get patient name
            ifstream pfile("patients.txt");
            char pname[50] = "Unknown";
            char pline[200];

            while (pfile.getline(pline, 200))
            {
                int pi = 0, pj;

                char id[10];
                pj = 0;
                while (pline[pi] != ',')
                    id[pj++] = pline[pi++];
                id[pj] = '\0';
                int fileID = atoi(id);
                pi++;

                char name[50];
                pj = 0;
                while (pline[pi] != ',')
                    name[pj++] = pline[pi++];
                name[pj] = '\0';

                if (fileID == patientID)
                {
                    strcpy_s(pname, 50, name);
                    break;
                }
            }
            pfile.close();

            // ✅ convert bill date to tm
            int day, month, year;
            sscanf_s(date, "%d-%d-%d", &day, &month, &year);

            tm bill_tm = {};
            bill_tm.tm_mday = day;
            bill_tm.tm_mon = month - 1;
            bill_tm.tm_year = year - 1900;

            time_t bill_time = mktime(&bill_tm);

            // ✅ difference in days
            double diff = difftime(now, bill_time) / (60 * 60 * 24);

            // ✅ display
            cout << "Bill ID: " << billID
                << " | Patient: " << pname
                << " | Amount: PKR " << amount
                << " | Date: " << date;

            if (diff > 7)
            {
                cout << " [OVERDUE]";
            }

            cout << endl;
        }
    }

    file.close();

    if (!found)
    {
        cout << "No unpaid bills found.\n";
    }
}
void Admin::dischargePatient()
{
    int chosenID;
    cout << "Enter Patient ID: ";
    cin >> chosenID;

    char line[200];

    // --- check unpaid bills ---
    ifstream bfile("bills.txt");
    while (bfile.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j;

        while (line[i] != ',') i++; i++;  // skip bill id

        char pid[10]; j = 0;
        while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0'; i++;

        while (line[i] != ',') i++; i++;  // skip appointment id
        while (line[i] != ',') i++; i++;  // skip amount

        char status[20]; j = 0;
        while (line[i] != ',' && line[i] != '\0') status[j++] = line[i++]; status[j] = '\0';

        if (atoi(pid) == chosenID &&
            !comparison(status, (char*)"paid") &&
            !comparison(status, (char*)"cancelled"))
        {
            cout << "Cannot discharge patient with unpaid bills.\n";
            bfile.close();
            return;
        }
    }
    bfile.close();

    // --- check pending appointments ---
    ifstream afile("appointments.txt");
    while (afile.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j;

        while (line[i] != ',') i++; i++;  // skip appointment ID

        char pid[10]; j = 0;
        while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0'; i++;

        for (int k = 0; k < 3; k++) { while (line[i] != ',') i++; i++; }

        char status[20]; j = 0;
        while (line[i] != '\0') status[j++] = line[i++]; status[j] = '\0';

        if (atoi(pid) == chosenID && comparison(status, (char*)"pending"))
        {
            cout << "Cannot discharge patient with pending appointments.\n";
            afile.close();
            return;
        }
    }
    afile.close();

    // --- copy records to discharged.txt FIRST ---
    ofstream discharged("discharged.txt", ios::app);

    // copy from patients.txt
    ifstream pfile("patients.txt");
    while (pfile.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;
        char id[10]; j = 0;
        while (line[i] != ',') id[j++] = line[i++]; id[j] = '\0';
        if (atoi(id) == chosenID)
            discharged << line << endl;
    }
    pfile.close();

    // copy from appointments.txt
    ifstream a2("appointments.txt");
    while (a2.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;
        while (line[i] != ',') i++; i++;  // skip appointment id
        char pid[10]; j = 0;
        while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0';
        if (atoi(pid) == chosenID)
            discharged << line << endl;
    }
    a2.close();

    // copy from bills.txt
    ifstream b2("bills.txt");
    while (b2.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;
        while (line[i] != ',') i++; i++;  // skip bill id
        char pid[10]; j = 0;
        while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0';
        if (atoi(pid) == chosenID)
            discharged << line << endl;
    }
    b2.close();

    // copy from prescriptions.txt
    ifstream pr("prescriptions.txt");
    while (pr.getline(line, 500))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;
        while (line[i] != ',') i++; i++;  // skip prescription id
        while (line[i] != ',') i++; i++;  // skip appointment id
        char pid[10]; j = 0;
        while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0';
        if (atoi(pid) == chosenID)
            discharged << line << endl;
    }
    pr.close();
    discharged.close();

    // --- now delete using FileHandler cascade ---
    FileHandler::cascadeDeletePatient(chosenID);

    cout << "\nPatient discharged and archived successfully.\n";
}
void Admin::generateDailyReport()
{
    // get today's date
    time_t t = time(0);
    struct tm now;
    localtime_s(&now, &t);
    char today[11];
    strftime(today, 11, "%d-%m-%Y", &now);

    cout << "Daily Report for: " << today << endl;
    cout << "========================================" << endl;

    // ─── SECTION 1: APPOINTMENT COUNTS ───────────────────
    int total = 0, pending = 0, completed = 0, noshow = 0, cancelled = 0;

    ifstream appFile("appointments.txt");
    char line[200];
    while (appFile.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;

        while (line[i] != ',') i++; i++;  // skip appointment_id
        while (line[i] != ',') i++; i++;  // skip patient_id
        while (line[i] != ',') i++; i++;  // skip doctor_id

        // date
        char date[11]; j = 0;
        while (line[i] != ',') date[j++] = line[i++]; date[j] = '\0'; i++;

        // time slot (skip)
        while (line[i] != ',') i++; i++;

        // status
        char status[20]; j = 0;
        while (line[i] != '\0') status[j++] = line[i++]; status[j] = '\0';

        if (!comparison(date, today)) continue;

        total++;
        if (comparison(status, (char*)"pending"))   pending++;
        else if (comparison(status, (char*)"completed")) completed++;
        else if (comparison(status, (char*)"no-show"))   noshow++;
        else if (comparison(status, (char*)"cancelled")) cancelled++;
    }
    appFile.close();

    cout << "Total appointments today: " << total
        << " (Pending: " << pending
        << " Completed: " << completed
        << " No-show: " << noshow
        << " Cancelled: " << cancelled << ")" << endl;

    // ─── SECTION 2: REVENUE FROM PAID BILLS TODAY ────────
    double revenue = 0;

    ifstream billFile("bills.txt");
    while (billFile.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;

        while (line[i] != ',') i++; i++;  // skip bill_id
        while (line[i] != ',') i++; i++;  // skip patient_id
        while (line[i] != ',') i++; i++;  // skip appointment_id

        // amount
        char amount[20]; j = 0;
        while (line[i] != ',') amount[j++] = line[i++]; amount[j] = '\0'; i++;

        // status
        char status[20]; j = 0;
        while (line[i] != ',') status[j++] = line[i++]; status[j] = '\0'; i++;

        // date
        char date[11]; j = 0;
        while (line[i] != '\0') date[j++] = line[i++]; date[j] = '\0';

        if (comparison(status, (char*)"paid") && comparison(date, today))
            revenue += atof(amount);
    }
    billFile.close();

    cout << "Revenue collected today (paid bills): PKR " << revenue << endl;

    // ─── SECTION 3: PATIENTS WITH UNPAID BILLS ───────────
    cout << "Patients with outstanding unpaid bills:" << endl;

    ifstream billFile2("bills.txt");
    while (billFile2.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j = 0;

        while (line[i] != ',') i++; i++;  // skip bill_id

        // patient_id
        char patID[10]; j = 0;
        while (line[i] != ',') patID[j++] = line[i++]; patID[j] = '\0'; i++;

        while (line[i] != ',') i++; i++;  // skip appointment_id

        // amount
        char amount[20]; j = 0;
        while (line[i] != ',') amount[j++] = line[i++]; amount[j] = '\0'; i++;

        // status
        char status[20]; j = 0;
        while (line[i] != ',') status[j++] = line[i++]; status[j] = '\0'; i++;

        if (!comparison(status, (char*)"unpaid")) continue;

        // find patient name
        char patName[20] = "Unknown";
        ifstream pFile("patients.txt");
        char pline[300];
        while (pFile.getline(pline, 300))
        {
            if (pline[0] == '\0') continue;
            int x = 0, k = 0;
            char pid[10]; k = 0;
            while (pline[x] != ',') pid[k++] = pline[x++]; pid[k] = '\0'; x++;
            if (comparison(pid, patID))
            {
                k = 0;
                while (pline[x] != ',') patName[k++] = pline[x++]; patName[k] = '\0';
                break;
            }
        }
        pFile.close();

        cout << "Patient: " << patName << " | Amount Owed: PKR " << amount << endl;
    }
    billFile2.close();

    // ─── SECTION 4: DOCTOR-WISE SUMMARY ──────────────────
    cout << "Doctor-wise summary for today:" << endl;

    // load doctors first to get names
    ifstream dFile("doctors.txt");
    char docIDs[100][10];
    char docNames[100][30];
    int docCount = 0;

    char dline[300];
    while (dFile.getline(dline, 300))
    {
        if (dline[0] == '\0') continue;
        int i = 0, j = 0;

        char id[10]; j = 0;
        while (dline[i] != ',') id[j++] = dline[i++]; id[j] = '\0'; i++;

        char name[30]; j = 0;
        while (dline[i] != ',') name[j++] = dline[i++]; name[j] = '\0';

        strcpy_s(docIDs[docCount], 10, id);
        strcpy_s(docNames[docCount], 30, name);
        docCount++;
    }
    dFile.close();

    // for each doctor count completed/pending/noshow today
    for (int d = 0; d < docCount; d++)
    {
        int dCompleted = 0, dPending = 0, dNoshow = 0;

        ifstream aFile("appointments.txt");
        char aline[200];
        while (aFile.getline(aline, 200))
        {
            if (aline[0] == '\0') continue;
            int i = 0, j = 0;

            while (aline[i] != ',') i++; i++;  // skip appointment_id
            while (aline[i] != ',') i++; i++;  // skip patient_id

            // doctor_id
            char did[10]; j = 0;
            while (aline[i] != ',') did[j++] = aline[i++]; did[j] = '\0'; i++;

            // date
            char date[11]; j = 0;
            while (aline[i] != ',') date[j++] = aline[i++]; date[j] = '\0'; i++;

            while (aline[i] != ',') i++; i++;  // skip time

            // status
            char status[20]; j = 0;
            while (aline[i] != '\0') status[j++] = aline[i++]; status[j] = '\0';

            if (!comparison(did, docIDs[d])) continue;
            if (!comparison(date, today)) continue;

            if (comparison(status, (char*)"completed")) dCompleted++;
            else if (comparison(status, (char*)"pending"))   dPending++;
            else if (comparison(status, (char*)"no-show"))   dNoshow++;
        }
        aFile.close();

        cout << "Doctor: " << docNames[d]
            << " | Completed: " << dCompleted
            << " | Pending: " << dPending
            << " | No-show: " << dNoshow << endl;
    }

    cout << "========================================" << endl;
}

























































