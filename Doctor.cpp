#include <string>
#include "Doctor.h"
#include <ctime>
#include <fstream>
#include <iostream>
using namespace std;
Doctor::Doctor():Person()
{
    contact = 0;
    choice = 0;
    fee = 0;


}
Doctor::Doctor(int id, const char* n, const char* p, const char* s, double fee, long int contact) :Person(id, n, p)
{
    int i = 0;
    while (s[i] != '\0')
    {
        specialization[i] = s[i];
        i++;
    }
    specialization[i] = '\0';
    this->fee = fee;
    this->contact = contact;
    choice = 0;

}

int Doctor::comparison(char a[], char b[])
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

void Doctor::displaymenu()
{
    cout << "Welcome, " << getName() << endl;
    cout << "Specialization: " << specialization << endl;
    cout << "============================" << endl;
    cout << "1. View Today's Appointments" << endl;
    cout << "2. Mark Appointment Complete" << endl;
    cout << "3. Mark Appointment No-Show" << endl;
    cout << "4. Write Prescription" << endl;
    cout << "5. View Patient Medical History" << endl;
    cout << "6. Logout" << endl;
    cin >> choice;

}



void Doctor::viewTodaysAppointments()
{
    // STEP 1: get today’s date
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    char today[20];
    strftime(today, 20, "%d-%m-%Y", &ltm);

    ifstream file("appointments.txt");

    if (!file)
    {
        cout << "Error opening appointments file!\n";
        return;
    }

    // arrays to store filtered appointments
    char times[100][10];
    char names[100][50];
    char statuses[100][20];
    int ids[100];

    int count = 0;
    char line[200];

    while (file.getline(line, 200))
    {
        int i = 0, j;

        // appointment ID
        char appID[10];
        j = 0;
        while (line[i] != ',')
            appID[j++] = line[i++];
        appID[j] = '\0';
        int id = atoi(appID);
        i++;

        // patient ID
        char pid[10];
        j = 0;
        while (line[i] != ',')
            pid[j++] = line[i++];
        pid[j] = '\0';
        int patientID = atoi(pid);
        i++;

        // doctor ID
        char docID[10];
        j = 0;
        while (line[i] != ',')
            docID[j++] = line[i++];
        docID[j] = '\0';
        int fileDocID = atoi(docID);
        i++;

        // date
        char date[20];
        j = 0;
        while (line[i] != ',')
            date[j++] = line[i++];
        date[j] = '\0';
        i++;

        // time
        char timeSlot[10];
        j = 0;
        while (line[i] != ',')
            timeSlot[j++] = line[i++];
        timeSlot[j] = '\0';
        i++;

        // status
        char status[20];
        j = 0;
        while (line[i] != '\0')
            status[j++] = line[i++];
        status[j] = '\0';

        // ✅ filter by doctor + today
        if (fileDocID == getid() && comparison(date, today))
        {
            // get patient name
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
                int pidFile = atoi(id);
                pi++;

                char name[50];
                pj = 0;
                while (pline[pi] != ',')
                    name[pj++] = pline[pi++];
                name[pj] = '\0';

                if (pidFile == patientID)
                {
                    strcpy_s(pname, 50, name);
                    break;
                }
            }
            pfile.close();

            // store in arrays
            ids[count] = id;
            strcpy_s(times[count], 10, timeSlot);
            strcpy_s(names[count], 50, pname);
            strcpy_s(statuses[count], 20, status);
            count++;
        }
    }

    file.close();

    if (count == 0)
    {
        cout << "No appointments scheduled for today.\n";
        return;
    }

    // ✅ STEP 2: SORT BY TIME (bubble sort)
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (strcmp(times[j], times[j + 1]) > 0)
            {
                // swap time
                char tempTime[10];
                strcpy_s(tempTime, 10, times[j]);
                strcpy_s(times[j], 10, times[j + 1]);
                strcpy_s(times[j + 1], 10, tempTime);

                // swap name
                char tempName[50];
                strcpy_s(tempName, 50, names[j]);
                strcpy_s(names[j], 50, names[j + 1]);
                strcpy_s(names[j + 1], 50, tempName);

                // swap status
                char tempStatus[20];
                strcpy_s(tempStatus, 20, statuses[j]);
                strcpy_s(statuses[j], 20, statuses[j + 1]);
                strcpy_s(statuses[j + 1], 20, tempStatus);

                // swap id
                int tempID = ids[j];
                ids[j] = ids[j + 1];
                ids[j + 1] = tempID;
            }
        }
    }

    // ✅ STEP 3: DISPLAY sorted results
    cout << "\nToday's Appointments (" << today << "):\n";

    for (int i = 0; i < count; i++)
    {
        cout << "Appointment ID: " << ids[i]
            << " | Patient: " << names[i]
            << " | Time: " << times[i]
            << " | Status: " << statuses[i] << endl;
    }
}
void Doctor::appointmentcomplete()
{
    // ✅ STEP 1: get today’s date
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    char today[20];
    strftime(today, 20, "%d-%m-%Y", &ltm);

    ifstream file("appointments.txt");

    if (!file)
    {
        cout << "Error opening appointments file!\n";
        return;
    }

    char line[200];
    bool found = false;

    cout << "\nToday's Pending Appointments:\n";

    // ✅ STEP 2: Display today's pending appointments
    while (file.getline(line, 200))
    {
        int i = 0, j;

        // appointment ID
        char appID[10];
        j = 0;
        while (line[i] != ',') appID[j++] = line[i++];
        appID[j] = '\0';
        int id = atoi(appID);
        i++;

        // patient ID
        while (line[i] != ',') i++;
        i++;

        // doctor ID
        char docID[10];
        j = 0;
        while (line[i] != ',') docID[j++] = line[i++];
        docID[j] = '\0';
        int fileDocID = atoi(docID);
        i++;

        // date
        char date[20];
        j = 0;
        while (line[i] != ',') date[j++] = line[i++];
        date[j] = '\0';
        i++;

        // time
        char timeSlot[10];
        j = 0;
        while (line[i] != ',') timeSlot[j++] = line[i++];
        timeSlot[j] = '\0';
        i++;

        // status
        char status[20];
        j = 0;
        while (line[i] != '\0') status[j++] = line[i++];
        status[j] = '\0';

        if (fileDocID == getid() &&
            comparison(date, today) &&
            comparison(status, (char*)"pending"))
        {
            cout << "Appointment ID: " << id
                << " | Time: " << timeSlot << endl;

            found = true;
        }
    }

    file.close();

    if (!found)
    {
        cout << "No pending appointments today.\n";
        return;
    }

    // ✅ STEP 3: Take input
    int chosenID;
    cout << "\nEnter Appointment ID to mark complete: ";
    cin >> chosenID;

    ifstream file2("appointments.txt");
    ofstream temp("temp.txt");

    bool valid = false;

    // ✅ STEP 4: Update file
    while (file2.getline(line, 200))
    {
        int i = 0, j;

        // appointment ID
        char appID[10];
        j = 0;
        while (line[i] != ',') appID[j++] = line[i++];
        appID[j] = '\0';
        int id = atoi(appID);
        i++;

        // patient ID
        char pid[10];
        j = 0;
        while (line[i] != ',') pid[j++] = line[i++];
        pid[j] = '\0';
        i++;

        // doctor ID
        char docID[10];
        j = 0;
        while (line[i] != ',') docID[j++] = line[i++];
        docID[j] = '\0';
        int fileDocID = atoi(docID);
        i++;

        // date
        char date[20];
        j = 0;
        while (line[i] != ',') date[j++] = line[i++];
        date[j] = '\0';
        i++;

        // time
        char timeSlot[10];
        j = 0;
        while (line[i] != ',') timeSlot[j++] = line[i++];
        timeSlot[j] = '\0';
        i++;

        // status
        char status[20];
        j = 0;
        while (line[i] != '\0') status[j++] = line[i++];
        status[j] = '\0';

        // ✅ validate and update
        if (id == chosenID &&
            fileDocID == getid() &&
            comparison(date, today) &&
            comparison(status, (char*)"pending"))
        {
            temp << id << "," << pid << "," << fileDocID << ","
                << date << "," << timeSlot << ",completed" << endl;

            valid = true;
        }
        else
        {
            temp << line << endl;
        }
    }

    file2.close();
    temp.close();

    if (!valid)
    {
        cout << "Invalid appointment ID.\n";
        return;
    }
    if (remove("appointments.txt") != 0)
    {
        cout << "Error deleting file!\n";
    }
    if (rename("temp.txt", "appointments.txt") != 0)
    {
        cout << "Error renaming file!\n";
    }

    

    cout << "\nAppointment marked as completed.\n";
}
void Doctor::appointmentnoshow()
{
    // ✅ STEP 1: get today's date
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    char today[20];
    strftime(today, 20, "%d-%m-%Y", &ltm);

    ifstream file("appointments.txt");

    if (!file)
    {
        cout << "Error opening appointments file!\n";
        return;
    }

    char line[200];
    bool found = false;

    cout << "\nToday's Pending Appointments:\n";

    // ✅ STEP 2: show pending appointments
    while (file.getline(line, 200))
    {
        int i = 0, j;

        char appID[10];
        j = 0;
        while (line[i] != ',') appID[j++] = line[i++];
        appID[j] = '\0';
        int id = atoi(appID);
        i++;

        // skip patient ID
        while (line[i] != ',') i++;
        i++;

        char docID[10];
        j = 0;
        while (line[i] != ',') docID[j++] = line[i++];
        docID[j] = '\0';
        int fileDocID = atoi(docID);
        i++;

        char date[20];
        j = 0;
        while (line[i] != ',') date[j++] = line[i++];
        date[j] = '\0';
        i++;

        char timeSlot[10];
        j = 0;
        while (line[i] != ',') timeSlot[j++] = line[i++];
        timeSlot[j] = '\0';
        i++;

        char status[20];
        j = 0;
        while (line[i] != '\0') status[j++] = line[i++];
        status[j] = '\0';

        if (fileDocID == getid() &&
            comparison(date, today) &&
            comparison(status, (char*)"pending"))
        {
            cout << "Appointment ID: " << id
                << " | Time: " << timeSlot << endl;

            found = true;
        }
    }

    file.close();

    if (!found)
    {
        cout << "No pending appointments today.\n";
        return;
    }

    // ✅ STEP 3: take input
    int chosenID;
    cout << "\nEnter Appointment ID for no-show: ";
    cin >> chosenID;

    ifstream file2("appointments.txt");
    ofstream temp("temp.txt");

    bool valid = false;

    // ✅ STEP 4: update appointment
    while (file2.getline(line, 200))
    {
        int i = 0, j;

        char appID[10];
        j = 0;
        while (line[i] != ',') appID[j++] = line[i++];
        appID[j] = '\0';
        int id = atoi(appID);
        i++;

        char pid[10];
        j = 0;
        while (line[i] != ',') pid[j++] = line[i++];
        pid[j] = '\0';
        int patientID = atoi(pid);
        i++;

        char docID[10];
        j = 0;
        while (line[i] != ',') docID[j++] = line[i++];
        docID[j] = '\0';
        int fileDocID = atoi(docID);
        i++;

        char date[20];
        j = 0;
        while (line[i] != ',') date[j++] = line[i++];
        date[j] = '\0';
        i++;

        char timeSlot[10];
        j = 0;
        while (line[i] != ',') timeSlot[j++] = line[i++];
        timeSlot[j] = '\0';
        i++;

        char status[20];
        j = 0;
        while (line[i] != '\0') status[j++] = line[i++];
        status[j] = '\0';

        if (id == chosenID &&
            fileDocID == getid() &&
            comparison(date, today) &&
            comparison(status, (char*)"pending"))
        {
            temp << id << "," << patientID << "," << fileDocID << ","
                << date << "," << timeSlot << ",no-show" << endl;

            valid = true;
        }
        else
        {
            temp << line << endl;
        }
    }

    file2.close();
    temp.close();

    if (!valid)
    {
        cout << "Invalid appointment ID.\n";
        return;
    }
    if (remove("appointments.txt") != 0)
    {
        cout << "Error deleting file!\n";
    }
    if (rename("temp.txt", "appointments.txt") != 0)
    {
        cout << "Error renaming file!\n";
    }

   

    // ✅ STEP 5: update bill → cancelled
    ifstream bfile("bills.txt");
    ofstream temp2("temp2.txt");

    while (bfile.getline(line, 200))
    {
        int i = 0, j;

        char billID[10];
        j = 0;
        while (line[i] != ',') billID[j++] = line[i++];
        billID[j] = '\0';
        i++;

        char pid[10];
        j = 0;
        while (line[i] != ',') pid[j++] = line[i++];
        pid[j] = '\0';
        i++;

        char appid[10];
        j = 0;
        while (line[i] != ',') appid[j++] = line[i++];
        appid[j] = '\0';
        int appointID = atoi(appid);
        i++;

        char amount[20];
        j = 0;
        while (line[i] != ',') amount[j++] = line[i++];
        amount[j] = '\0';
        double amt = atof(amount);
        i++;

        char status[20];
        j = 0;
        while (line[i] != ',') status[j++] = line[i++];
        status[j] = '\0';
        i++;

        char date[20];
        j = 0;
        while (line[i] != '\0') date[j++] = line[i++];
        date[j] = '\0';

        if (appointID == chosenID)
        {
            temp2 << billID << "," << pid << "," << appointID << ","
                << amt << ",cancelled," << date << endl;
        }
        else
        {
            temp2 << line << endl;
        }
    }

    bfile.close();
    temp2.close();
    if (remove("bills.txt") != 0)
    {
        cout << "Error deleting file!\n";
    }
    if (rename("temp2.txt", "bills.txt") != 0)
    {
        cout << "Error renaming file!\n";
    }

   

    cout << "\nAppointment marked as no-show.\n";
}
void Doctor::writeprescription()
{
    int chosenID;
    cout << "Enter Appointment ID: ";
    cin >> chosenID;

    // ✅ STEP 1: validate appointment
    ifstream file("appointments.txt");
    char line[200];

    bool valid = false;
    int patientID = 0;
    char date[20];

    while (file.getline(line, 200))
    {
        int i = 0, j;

        // appointment ID
        char appID[10];
        j = 0;
        while (line[i] != ',') appID[j++] = line[i++];
        appID[j] = '\0';
        int id = atoi(appID);
        i++;

        // patient ID
        char pid[10];
        j = 0;
        while (line[i] != ',') pid[j++] = line[i++];
        pid[j] = '\0';
        patientID = atoi(pid);
        i++;

        // doctor ID
        char docID[10];
        j = 0;
        while (line[i] != ',') docID[j++] = line[i++];
        docID[j] = '\0';
        int fileDocID = atoi(docID);
        i++;

        // date
        j = 0;
        while (line[i] != ',') date[j++] = line[i++];
        date[j] = '\0';
        i++;

        // skip time
        while (line[i] != ',') i++;
        i++;

        // status
        char status[20];
        j = 0;
        while (line[i] != '\0') status[j++] = line[i++];
        status[j] = '\0';

        if (id == chosenID &&
            fileDocID == getid() &&
            comparison(status, (char*)"completed"))
        {
            valid = true;
            break;
        }
    }

    file.close();

    if (!valid)
    {
        cout << "Invalid appointment ID.\n";
        return;
    }

    // ✅ STEP 2: check if already exists
    ifstream pfile("prescriptions.txt");

    while (pfile.getline(line, 200))
    {
        int i = 0, j;

        char presID[10];
        j = 0;
        while (line[i] != ',') presID[j++] = line[i++];
        presID[j] = '\0';
        i++;

        char appid[10];
        j = 0;
        while (line[i] != ',') appid[j++] = line[i++];
        appid[j] = '\0';
        int existingAppID = atoi(appid);

        if (existingAppID == chosenID)
        {
            cout << "Prescription already written for this appointment.\n";
            pfile.close();
            return;
        }
    }

    pfile.close();

    // ✅ STEP 3: input medicines + notes
    char medicines[500];
    char notes[300];

    cin.ignore(); // clear buffer

    cout << "Enter medicines: ";
    cin.getline(medicines, 500);

    cout << "Enter notes: ";
    cin.getline(notes, 300);

    // ✅ STEP 4: generate new prescription ID
    ifstream countFile("prescriptions.txt");
    int newID = 1;

    while (countFile.getline(line, 200))
        newID++;

    countFile.close();

    // ✅ STEP 5: write to file
    ofstream outfile("prescriptions.txt", ios::app);

    outfile << newID << ","
        << chosenID << ","
        << patientID << ","
        << getid() << ","
        << date << ","
        << medicines << ","
        << notes << endl;

    outfile.close();

    cout << "Prescription saved.\n";
}
void Doctor::viewPatientHistory()
{
    int chosenPID;
    cout << "Enter Patient ID: ";
    cin >> chosenPID;

    char line[200];
    bool exists = false;
    bool hasCompleted = false;

    // ✅ STEP 1: check patient exists
    ifstream pfile("patients.txt");

    while (pfile.getline(line, 200))
    {
        int i = 0, j;

        char id[10];
        j = 0;
        while (line[i] != ',') id[j++] = line[i++];
        id[j] = '\0';

        if (atoi(id) == chosenPID)
        {
            exists = true;
            break;
        }
    }
    pfile.close();

    if (!exists)
    {
        cout << "Access denied.\n";
        return;
    }

    // ✅ STEP 2: check at least one completed appointment with THIS doctor
    ifstream afile("appointments.txt");

    while (afile.getline(line, 200))
    {
        int i = 0, j;

        // skip appointment ID
        while (line[i] != ',') i++;
        i++;

        // patient ID
        char pid[10];
        j = 0;
        while (line[i] != ',') pid[j++] = line[i++];
        pid[j] = '\0';
        int filePID = atoi(pid);
        i++;

        // doctor ID
        char docID[10];
        j = 0;
        while (line[i] != ',') docID[j++] = line[i++];
        docID[j] = '\0';
        int fileDocID = atoi(docID);
        i++;

        // skip date
        while (line[i] != ',') i++;
        i++;

        // skip time
        while (line[i] != ',') i++;
        i++;

        // status
        char status[20];
        j = 0;
        while (line[i] != '\0') status[j++] = line[i++];
        status[j] = '\0';

        if (filePID == chosenPID &&
            fileDocID == getid() &&
            comparison(status, (char*)"completed"))
        {
            hasCompleted = true;
            break;
        }
    }

    afile.close();

    if (!hasCompleted)
    {
        cout << "Access denied, you can only view records of the patients you have already tended to.\n";
        return;
    }

    // ✅ STEP 3: collect prescriptions of this doctor for this patient
    ifstream prfile("prescriptions.txt");

    char dates[100][20];
    char medicines[100][500];
    char notes[100][300];
    int count = 0;

    while (prfile.getline(line, 200))
    {
        int i = 0, j;

        // skip prescription ID
        while (line[i] != ',') i++;
        i++;

        // skip appointment ID
        while (line[i] != ',') i++;
        i++;

        // patient ID
        char pid[10];
        j = 0;
        while (line[i] != ',') pid[j++] = line[i++];
        pid[j] = '\0';
        int filePID = atoi(pid);
        i++;

        // doctor ID
        char docID[10];
        j = 0;
        while (line[i] != ',') docID[j++] = line[i++];
        docID[j] = '\0';
        int fileDocID = atoi(docID);
        i++;

        // date
        char date[20];
        j = 0;
        while (line[i] != ',') date[j++] = line[i++];
        date[j] = '\0';
        i++;

        // medicines
        char meds[500];
        j = 0;
        while (line[i] != ',') meds[j++] = line[i++];
        meds[j] = '\0';
        i++;

        // notes
        char note[300];
        j = 0;
        while (line[i] != '\0') note[j++] = line[i++];
        note[j] = '\0';

        if (filePID == chosenPID && fileDocID == getid())
        {
            strcpy_s(dates[count], 20, date);
            strcpy_s(medicines[count], 500, meds);
            strcpy_s(notes[count], 300, note);
            count++;
        }
    }

    prfile.close();

    if (count == 0)
    {
        cout << "No medical history found.\n";
        return;
    }

    // ✅ STEP 4: sort by date DESCENDING
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (strcmp(dates[j], dates[j + 1]) < 0)
            {
                char tempDate[20];
                strcpy_s(tempDate, 20, dates[j]);
                strcpy_s(dates[j], 20, dates[j + 1]);
                strcpy_s(dates[j + 1], 20, tempDate);

                char tempMeds[500];
                strcpy_s(tempMeds, 500, medicines[j]);
                strcpy_s(medicines[j], 500, medicines[j + 1]);
                strcpy_s(medicines[j + 1], 500, tempMeds);

                char tempNotes[300];
                strcpy_s(tempNotes, 300, notes[j]);
                strcpy_s(notes[j], 300, notes[j + 1]);
                strcpy_s(notes[j + 1], 300, tempNotes);
            }
        }
    }

    // ✅ STEP 5: display
    cout << "\nMedical History:\n";

    for (int i = 0; i < count; i++)
    {
        cout << "Date: " << dates[i] << endl;
        cout << "Medicines: " << medicines[i] << endl;
        cout << "Notes: " << notes[i] << endl;
        cout << "------------------------\n";
    }
}