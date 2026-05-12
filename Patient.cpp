#include "Person.h"
#include "Patient.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include "FileHandler.h"
#include "HospitalException.h"
#include "Validator.h"
using namespace std;

Patient::Patient():Person()
{
	age = -1;
    char temp[6] = "Blank";
    int i = 0;
    while (temp[i] != '\0')
    {
        gender[i] = temp[i];
        i++;
    }
    gender[i] = '\0';
    balance = -1;
    contact = 0;
    choice = 0;
}
Patient::Patient(int id, const char* n, const char* p, int age, const char* g, double b, long int contact) :
    Person( id, n, p)
{
    this->age = age;
    int i = 0;
    while (g[i] != '\0')
    {
        gender[i] = g[i];
        i++;
    }
    gender[i] = '\0';
    balance = b;
    this->contact = contact;
    choice = 0;

}

void Patient:: displaymenu()
{
    cout << "Welcome, " << getName() << endl;
    cout << "Balance: PKR " << balance << endl;//load from file
    cout << "============================" << endl;
    cout << "1. Book Appointment" << endl;
    cout << "2. Cancel Appointment" << endl;
    cout << "3. View My Appointments" << endl;
    cout << "4. View My Medical Records" << endl;
    cout << "5. View My Bills" << endl;
    cout << "6. Pay Bill" << endl;
    cout << "7. Top Up Balance" << endl;
    cout << "8. Logout" << endl;
    cin >> choice;
}

    int Patient::comparison(char a[], char b[])
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


//choice 1
    void Patient::bookappointment()
    {
        // --- SPECIALIZATION ---
        char specialization[20];
        cout << "Enter specialization to search (e.g. Cardiology): ";
        cin.ignore(1000, '\n');
        cin.getline(specialization, 20);

        ifstream docfile("doctors.txt");
        if (!docfile)
        {
            throw FileNotFoundException();
            return;
        }

        char line1[200];
        bool found = false;

        while (docfile.getline(line1, 200))
        {
            int i = 0;
            char id[10]; int j = 0;
            while (line1[i] != ',' && line1[i] != '\0') id[j++] = line1[i++]; id[j] = '\0'; i++;

            char name[20]; j = 0;
            while (line1[i] != ',' && line1[i] != '\0') name[j++] = line1[i++]; name[j] = '\0'; i++;

            char spec[20]; j = 0;
            while (line1[i] != ',' && line1[i] != '\0') spec[j++] = line1[i++]; spec[j] = '\0'; i++;

            while (line1[i] != ',' && line1[i] != '\0') i++; i++;  // skip contact
            while (line1[i] != ',' && line1[i] != '\0') i++; i++;  // skip password

            char fee[20]; j = 0;
            while (line1[i] != ',' && line1[i] != '\0') fee[j++] = line1[i++]; fee[j] = '\0';

            if (comparison(spec, specialization))
            {
                found = true;
                cout << "ID: " << id << " | Name: " << name << " | Fee: " << fee << endl;
            }
        }
        docfile.close();

        if (!found)
        {
            cout << "No doctors available for that specialization." << endl;
            return;
        }

        // --- DOCTOR ID ---
        int selectedID;
        cout << "Enter Doctor ID to book: ";
        cin >> selectedID;

        ifstream docfile2("doctors.txt");
        if (!docfile2) { throw FileNotFoundException(); return; }

        char line2[200];
        bool docfound = false;
        double docfee = 0;

        while (docfile2.getline(line2, 200))
        {
            int i = 0;
            char id[10]; int j = 0;
            while (line2[i] != ',' && line2[i] != '\0') id[j++] = line2[i++]; id[j] = '\0'; i++;
            int fileID = atoi(id);

            char name[50]; j = 0;
            while (line2[i] != ',' && line2[i] != '\0') name[j++] = line2[i++]; name[j] = '\0'; i++;

            char spec[20]; j = 0;
            while (line2[i] != ',' && line2[i] != '\0') spec[j++] = line2[i++]; spec[j] = '\0'; i++;

            while (line2[i] != ',' && line2[i] != '\0') i++; i++;  // skip contact
            while (line2[i] != ',' && line2[i] != '\0') i++; i++;  // skip password

            char fee[20]; j = 0;
            while (line2[i] != ',' && line2[i] != '\0') fee[j++] = line2[i++]; fee[j] = '\0';
            double money = atof(fee);

            if (!comparison(spec, specialization)) continue;

            if (fileID == selectedID)
            {
                docfound = true;
                docfee = money;
                cout << "Doctor found: ID: " << fileID << " | Name: " << name << " | Fee: " << docfee << endl;
                break;
            }
        }
        docfile2.close();

        if (!docfound)
        {
            cout << "Doctor not found." << endl;
            return;
        }

        // --- DATE with 3 attempts ---
        char date[11];
        int dateAttempts = 0;
        bool validDate = false;

        while (dateAttempts < 3)
        {
            cout << "Enter date (DD-MM-YYYY): ";
            cin >> date;

            if (Validator::validateDate(date))
            {
                validDate = true;
                break;
            }
            else
            {
                dateAttempts++;
                if (dateAttempts < 3)
                    cout << "Invalid date. Use format DD-MM-YYYY." << endl;
            }
        }

        if (!validDate)
        {
            cout << "Invalid date. Use format DD-MM-YYYY." << endl;
            return;
        }

        // --- DISPLAY AVAILABLE SLOTS ---
        const char* allSlots[8] = {
            "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00"
        };

        // check which slots are taken
        bool taken[8] = { false };

        ifstream appfile("appointments.txt");
        if (!appfile) { throw FileNotFoundException(); return; }

        char line3[200];
        while (appfile.getline(line3, 200))
        {
            if (line3[0] == '\0') continue;
            int i = 0, j = 0;

            while (line3[i] != ',') i++; i++;  // skip appointment_id
            while (line3[i] != ',') i++; i++;  // skip patient_id

            char docID[10]; j = 0;
            while (line3[i] != ',') docID[j++] = line3[i++]; docID[j] = '\0'; i++;
            int fileDocID = atoi(docID);

            char fileDate[11]; j = 0;
            while (line3[i] != ',') fileDate[j++] = line3[i++]; fileDate[j] = '\0'; i++;

            char fileTime[6]; j = 0;
            while (line3[i] != ',') fileTime[j++] = line3[i++]; fileTime[j] = '\0'; i++;

            char status[20]; j = 0;
            while (line3[i] != '\0') status[j++] = line3[i++]; status[j] = '\0';

            char cancelled[10] = "cancelled";
            char noshow[9] = "no-show";

            if (fileDocID == selectedID &&
                comparison(fileDate, date) &&
                !comparison(status, cancelled) &&
                !comparison(status, noshow))
            {
                // mark this slot as taken
                for (int s = 0; s < 8; s++)
                {
                    if (comparison(fileTime, (char*)allSlots[s]))
                    {
                        taken[s] = true;
                        break;
                    }
                }
            }
        }
        appfile.close();

        cout << "Available time slots: ";
        bool anyAvailable = false;
        for (int s = 0; s < 8; s++)
        {
            if (!taken[s])
            {
                cout << allSlots[s] << " ";
                anyAvailable = true;
            }
        }
        cout << endl;

        if (!anyAvailable)
        {
            cout << "No available slots for this doctor on this date." << endl;
            return;
        }

        // --- TIME SLOT with SlotUnavailableException ---
        char time[6];
        bool slotBooked = false;

        while (!slotBooked)
        {
            cout << "Enter time slot (e.g. 09:00): ";
            cin >> time;

            // validate it's one of the 8 slots
            if (!Validator::validateTimeSlot(time))
            {
                try { throw InvalidInputException(); }
                catch (InvalidInputException& e) { cout << e.what() << endl; }
                continue;
            }

            // check if slot is available
            bool slotTaken = false;
            for (int s = 0; s < 8; s++)
            {
                if (comparison(time, (char*)allSlots[s]) && taken[s])
                {
                    slotTaken = true;
                    break;
                }
            }

            if (slotTaken)
            {
                try { throw SlotUnavailableException(); }
                catch (SlotUnavailableException& e)
                {
                    cout << e.what() << endl;
                    // re-display available slots
                    cout << "Available time slots: ";
                    for (int s = 0; s < 8; s++)
                        if (!taken[s]) cout << allSlots[s] << " ";
                    cout << endl;
                }
            }
            else
            {
                slotBooked = true;
            }
        }

        // --- BOOK APPOINTMENT using FileHandler ---
        int newAppointmentID = FileHandler::appendAppointment(getid(), selectedID, date, time);
        FileHandler::appendBill(getid(), newAppointmentID, docfee, date);

        cout << "Appointment booked successfully!" << endl;
        cout << "Appointment ID: " << newAppointmentID << endl;
    }

    void Patient::cancelappointment()
    {
        ifstream file("appointments.txt");
        if (!file) { throw FileNotFoundException(); return; }

        char line[200];
        bool found = false;

        cout << "\nPending Appointments:\n";

        while (file.getline(line, 200))
        {
            if (line[0] == '\0') continue;
            int i = 0, j = 0;

            char appID[10]; j = 0;
            while (line[i] != ',') appID[j++] = line[i++]; appID[j] = '\0'; i++;
            int id = atoi(appID);

            char pid[10]; j = 0;
            while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0'; i++;
            int filePID = atoi(pid);

            char docID[10]; j = 0;
            while (line[i] != ',') docID[j++] = line[i++]; docID[j] = '\0'; i++;
            int doctorID = atoi(docID);

            char date[20]; j = 0;
            while (line[i] != ',') date[j++] = line[i++]; date[j] = '\0'; i++;

            char time[10]; j = 0;
            while (line[i] != ',') time[j++] = line[i++]; time[j] = '\0'; i++;

            char status[20]; j = 0;
            while (line[i] != '\0') status[j++] = line[i++]; status[j] = '\0';

            if (filePID == getid() && comparison(status, (char*)"pending"))
            {
                // get doctor name
                char docName[30] = "Unknown";
                ifstream dfile("doctors.txt");
                char dline[200];
                while (dfile.getline(dline, 200))
                {
                    if (dline[0] == '\0') continue;
                    int x = 0, k = 0;
                    char did[10]; k = 0;
                    while (dline[x] != ',') did[k++] = dline[x++]; did[k] = '\0'; x++;
                    if (atoi(did) == doctorID)
                    {
                        k = 0;
                        while (dline[x] != ',') docName[k++] = dline[x++]; docName[k] = '\0';
                        break;
                    }
                }
                dfile.close();

                cout << "Appointment ID: " << id
                    << " | Doctor: " << docName
                    << " | Date: " << date
                    << " | Time: " << time << endl;
                found = true;
            }
        }
        file.close();

        if (!found)
        {
            cout << "You have no pending appointments.\n";
            return;
        }

        // --- get appointment ID ---
        int chosenID;
        cout << "\nEnter the ID of Appointment you want to cancel: ";
        cin >> chosenID;

        // --- validate it belongs to this patient and is pending ---
        ifstream file2("appointments.txt");
        bool valid = false;
        int selectedDoctorID = 0;

        while (file2.getline(line, 200))
        {
            if (line[0] == '\0') continue;
            int i = 0, j = 0;

            char appID[10]; j = 0;
            while (line[i] != ',') appID[j++] = line[i++]; appID[j] = '\0'; i++;
            int id = atoi(appID);

            char pid[10]; j = 0;
            while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0'; i++;
            int filePID = atoi(pid);

            char docID[10]; j = 0;
            while (line[i] != ',') docID[j++] = line[i++]; docID[j] = '\0'; i++;

            while (line[i] != ',') i++; i++;  // skip date
            while (line[i] != ',') i++; i++;  // skip time

            char status[20]; j = 0;
            while (line[i] != '\0') status[j++] = line[i++]; status[j] = '\0';

            if (id == chosenID && filePID == getid() && comparison(status, (char*)"pending"))
            {
                valid = true;
                selectedDoctorID = atoi(docID);
                break;
            }
        }
        file2.close();

        if (!valid)
        {
            cout << "Invalid appointment ID.\n";
            return;
        }

        // --- get doctor fee ---
        double docfee = 0;
        ifstream docfile("doctors.txt");
        while (docfile.getline(line, 200))
        {
            if (line[0] == '\0') continue;
            int i = 0, j = 0;

            char id[10]; j = 0;
            while (line[i] != ',') id[j++] = line[i++]; id[j] = '\0'; i++;

            while (line[i] != ',') i++; i++;  // skip name
            while (line[i] != ',') i++; i++;  // skip spec
            while (line[i] != ',') i++; i++;  // skip contact
            while (line[i] != ',') i++; i++;  // skip password

            char fee[20]; j = 0;
            while (line[i] != '\0') fee[j++] = line[i++]; fee[j] = '\0';

            if (atoi(id) == selectedDoctorID)
            {
                docfee = atof(fee);
                break;
            }
        }
        docfile.close();

        // --- check bill status ---
        char billStat[20] = "";
        int billID = 0;
        ifstream bifile("bills.txt");
        while (bifile.getline(line, 200))
        {
            if (line[0] == '\0') continue;
            int i = 0, j = 0;

            char bid[10]; j = 0;
            while (line[i] != ',') bid[j++] = line[i++]; bid[j] = '\0'; i++;

            char pid[10]; j = 0;
            while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0'; i++;
            int PID = atoi(pid);

            char appid[10]; j = 0;
            while (line[i] != ',') appid[j++] = line[i++]; appid[j] = '\0'; i++;
            int appoint = atoi(appid);

            while (line[i] != ',') i++; i++;  // skip amount

            char status[20]; j = 0;
            while (line[i] != ',') status[j++] = line[i++]; status[j] = '\0';

            if (appoint == chosenID && PID == getid())
            {
                strcpy_s(billStat, 20, status);
                billID = atoi(bid);
                break;
            }
        }
        bifile.close();

        // --- update appointment status using FileHandler ---
        FileHandler::updateAppointmentStatus(chosenID, "cancelled");

        // --- update bill status using FileHandler ---
        FileHandler::updateBillStatus(billID, "cancelled");

        // --- refund if bill was paid ---
        if (comparison(billStat, (char*)"paid"))
        {
            double newBalance = balance + docfee;
            FileHandler::updatePatientBalance(getid(), newBalance, getName(),
                age, gender, contact, getpass());
            this->balance = newBalance;
        }

        cout << "Appointment cancelled. PKR " << docfee << " refunded to your balance if you paid.\n";
    }
void Patient::viewappointments()
{
    ifstream file("appointments.txt");

    if (!file)
    {
        cout << "Error opening appointments file!\n";
        return;
    }

    // arrays to store data
    int appID[100], docID[100];
    char date[100][11], time[100][10], status[100][20];

    int count = 0;
    char line[200];

    // STEP 1: Read appointments of THIS patient
    while (file.getline(line, 200))
    {
        int i = 0, j = 0;

        // appointment id
        char id[10];
        j = 0;
        while (line[i] != ',')
            id[j++] = line[i++];
        id[j] = '\0';
        int aID = atoi(id);
        i++;

        // patient id
        char pid[10];
        j = 0;
        while (line[i] != ',')
            pid[j++] = line[i++];
        pid[j] = '\0';
        int pID = atoi(pid);
        i++;

        // doctor id
        char did[10];
        j = 0;
        while (line[i] != ',')
            did[j++] = line[i++];
        did[j] = '\0';
        int dID = atoi(did);
        i++;

        // date
        j = 0;
        while (line[i] != ',')
            date[count][j++] = line[i++];
        date[count][j] = '\0';
        i++;

        // time
        j = 0;
        while (line[i] != ',')
            time[count][j++] = line[i++];
        time[count][j] = '\0';
        i++;

        // status
        j = 0;
        while (line[i] != '\0')
            status[count][j++] = line[i++];
        status[count][j] = '\0';

        if (pID == getid())
        {
            appID[count] = aID;
            docID[count] = dID;
            count++;
        }
    }

    file.close();

    if (count == 0)
    {
        cout << "No appointments found.\n";
        return;
    }

    //  STEP 2: SORT by date (dd-mm-yyyy -> convert to yyyy mm dd)
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            int d1 = (date[i][0] - '0') * 10 + (date[i][1] - '0');
            int m1 = (date[i][3] - '0') * 10 + (date[i][4] - '0');
            int y1 = (date[i][6] - '0') * 1000 +
                (date[i][7] - '0') * 100 +
                (date[i][8] - '0') * 10 +
                (date[i][9] - '0');

            int d2 = (date[j][0] - '0') * 10 + (date[j][1] - '0');
            int m2 = (date[j][3] - '0') * 10 + (date[j][4] - '0');
            int y2 = (date[j][6] - '0') * 1000 +
                (date[j][7] - '0') * 100 +
                (date[j][8] - '0') * 10 +
                (date[j][9] - '0');

            if (y1 > y2 ||
                (y1 == y2 && m1 > m2) ||
                (y1 == y2 && m1 == m2 && d1 > d2))
            {
                //  swap everything
                swap(appID[i], appID[j]);
                swap(docID[i], docID[j]);

                char temp[20];
                strcpy_s(temp, 20, date[i]);
                strcpy_s(date[i], 11, date[j]);
                strcpy_s(date[j], 11, temp);

                strcpy_s(temp, 20, time[i]);
                strcpy_s(time[i], 10, time[j]);
                strcpy_s(time[j], 10, temp);

                strcpy_s(temp, 20, status[i]);
                strcpy_s(status[i], 20, status[j]);
                strcpy_s(status[j], 20, temp);

                
            }
        }
    }

    // STEP 3: Display with doctor info
    cout << "\nMy Appointments:\n";

    for (int i = 0; i < count; i++)
    {
        ifstream dfile("doctors.txt");

        char dline[200];
        char docName[50], spec[20];

        while (dfile.getline(dline, 200))
        {
            int x = 0, k = 0;

            char id[10];
            while (dline[x] != ',')
                id[k++] = dline[x++];
            id[k] = '\0';

            int did = atoi(id);
            x++;

            // name
            k = 0;
            while (dline[x] != ',')
                docName[k++] = dline[x++];
            docName[k] = '\0';
            x++;

            // specialization
            k = 0;
            while (dline[x] != ',')
                spec[k++] = dline[x++];
            spec[k] = '\0';

            if (did == docID[i])
            {
                break;
            }
        }

        dfile.close();

        cout << "ID: " << appID[i]
            << " | Doctor: " << docName
            << " | Spec: " << spec
            << " | Date: " << date[i]
            << " | Time: " << time[i]
            << " | Status: " << status[i] << endl;
    }
}

void Patient::viewMedicalRecords()
{
    ifstream file("prescriptions.txt");

    if (!file)
    {
        cout << "Error opening prescriptions file!\n";
        return;
    }

    // store data
    static int appID[100], docID[100];
    static char date[100][11];
    static char meds[100][500];
    static char notes[100][300];

    int count = 0;
    char line[300];

    // STEP 1: Read only CURRENT PATIENT records
    while (file.getline(line, 300))
    {
        int i = 0, j;

        // prescription_id (skip)
        while (line[i] != ',') i++;
        i++;

        // appointment_id
        char aid[10];
        j = 0;
        while (line[i] != ',')
            aid[j++] = line[i++];
        aid[j] = '\0';
        int aID = atoi(aid);
        i++;

        // patient_id
        char pid[10];
        j = 0;
        while (line[i] != ',')
            pid[j++] = line[i++];
        pid[j] = '\0';
        int pID = atoi(pid);
        i++;

        // doctor_id
        char did[10];
        j = 0;
        while (line[i] != ',')
            did[j++] = line[i++];
        did[j] = '\0';
        int dID = atoi(did);
        i++;

        // date
        j = 0;
        while (line[i] != ',')
            date[count][j++] = line[i++];
        date[count][j] = '\0';
        i++;

        // medicines
        j = 0;
        while (line[i] != ',' && line[i] != '\0' && j < 499)
            meds[count][j++] = line[i++];
        meds[count][j] = '\0';
       if(line[i]==',') i++;

        // notes
        j = 0;
        while (line[i] != '\0' && j < 299)
            notes[count][j++] = line[i++];
        notes[count][j] = '\0';

        if (pID == getid())
        {
            
            appID[count] = aID;
            docID[count] = dID;
            count++;
            
        }
    }

    file.close();
   
    if (count == 0)
    {
        cout << "No medical records found.\n";
        return;
    }

    // STEP 2: SORT (DESCENDING = most recent first)
   
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            int d1 = (date[i][0] - '0') * 10 + (date[i][1] - '0');
            int m1 = (date[i][3] - '0') * 10 + (date[i][4] - '0');
            int y1 = (date[i][6] - '0') * 1000 +
                     (date[i][7] - '0') * 100 +
                     (date[i][8] - '0') * 10 +
                     (date[i][9] - '0');

            int d2 = (date[j][0] - '0') * 10 + (date[j][1] - '0');
            int m2 = (date[j][3] - '0') * 10 + (date[j][4] - '0');
            int y2 = (date[j][6] - '0') * 1000 +
                     (date[j][7] - '0') * 100 +
                     (date[j][8] - '0') * 10 +
                     (date[j][9] - '0');

            // reverse condition for DESCENDING
            if (y1 < y2 ||
                (y1 == y2 && m1 < m2) ||
                (y1 == y2 && m1 == m2 && d1 < d2))
            {
                // swap ints
                int tempInt;

                tempInt = appID[i]; appID[i] = appID[j]; appID[j] = tempInt;
                tempInt = docID[i]; docID[i] = docID[j]; docID[j] = tempInt;

                char temp[500]; // make temp big enough for largest array

                strcpy_s(temp, 500, meds[i]);
                strcpy_s(meds[i], 500, meds[j]);
                strcpy_s(meds[j], 500, temp);

                strcpy_s(temp, 300, notes[i]);
                strcpy_s(notes[i], 300, notes[j]);
                strcpy_s(notes[j], 300, temp);

                strcpy_s(temp, 11, date[i]);      // date is only 11
                strcpy_s(date[i], 11, date[j]);
                strcpy_s(date[j], 11, temp);
            }
        }
    }

    //  STEP 3: DISPLAY
    cout << "\nMedical Records:\n";

    for (int i = 0; i < count; i++)
    {
        // find doctor name
        ifstream dfile("doctors.txt");
        char line2[200], name[50];

        while (dfile.getline(line2, 200))
        {
            int x = 0, k = 0;

            char id[10];
            while (line2[x] != ',')
                id[k++] = line2[x++];
            id[k] = '\0';

            int did = atoi(id);
            x++;

            k = 0;
            while (line2[x] != ',')
                name[k++] = line2[x++];
            name[k] = '\0';

            if (did == docID[i])
                break;
        }

        dfile.close();

        cout << "Date: " << date[i]
             << " | Doctor: " << name << endl;
        cout << "Medicines: " << meds[i] << endl;
        cout << "Notes: " << notes[i] << endl;
        cout << "-----------------------------\n";
    }
}
void Patient::viewbills()
{
    ifstream file("bills.txt");

    if (!file)
    {
        cout << "Error opening bills file!\n";
        return;
    }

    char line[200];

    int billID, appID, patientID;
    double amount;
    char status[20], date[20];

    bool found = false;
    double unpaidTotal = 0;

    cout << "\nMy Bills:\n";

    while (file.getline(line, 200))
    {
        int i = 0, j = 0;

        // --- bill ID ---
        char bid[10];
        j = 0;
        while (line[i] != ',' && line[i] != '\0')
            bid[j++] = line[i++];
        bid[j] = '\0';
        billID = atoi(bid);
        i++;

        // --- patient ID ---
        char pid[10];
        j = 0;
        while (line[i] != ',' && line[i] != '\0')
            pid[j++] = line[i++];
        pid[j] = '\0';
        patientID = atoi(pid);
        i++;

        // --- appointment ID ---
        char aid[10];
        j = 0;
        while (line[i] != ',' && line[i] != '\0')
            aid[j++] = line[i++];
        aid[j] = '\0';
        appID = atoi(aid);
        i++;

        // --- amount ---
        char amt[20];
        j = 0;
        while (line[i] != ',' && line[i] != '\0')
            amt[j++] = line[i++];
        amt[j] = '\0';
        amount = atof(amt);
        i++;

        // --- status ---
        j = 0;
        while (line[i] != ',' && line[i] != '\0')
            status[j++] = line[i++];
        status[j] = '\0';
        i++;

        // --- date ---
        j = 0;
        while (line[i] != '\0')
            date[j++] = line[i++];
        date[j] = '\0';

        //  only show THIS patient’s bills
        if (patientID == getid())
        {
            found = true;

            cout << "Bill ID: " << billID
                << " | Appointment ID: " << appID
                << " | Amount: PKR " << amount
                << " | Status: " << status
                << " | Date: " << date << endl;

            // calculate unpaid total
            if (comparison(status, (char*)"unpaid"))
            {
                unpaidTotal += amount;
            }
        }
    }

    file.close();

    if (!found)
    {
        cout << "No bills found.\n";
        return;
    }

    //  show total unpaid
    cout << "\nTotal Outstanding (Unpaid): PKR " << unpaidTotal << endl;
}
void Patient::paybill()
{
    ifstream file("bills.txt");
    if (!file) { throw FileNotFoundException(); return; }

    char line[200];
    bool found = false;

    cout << "\nUnpaid Bills:\n";

    while (file.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j;

        char bid[10]; j = 0;
        while (line[i] != ',') bid[j++] = line[i++]; bid[j] = '\0'; i++;
        int billID = atoi(bid);

        char pid[10]; j = 0;
        while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0'; i++;
        int pID = atoi(pid);

        while (line[i] != ',') i++; i++;  // skip appointment ID

        char amt[20]; j = 0;
        while (line[i] != ',') amt[j++] = line[i++]; amt[j] = '\0'; i++;
        double amount = atof(amt);

        char status[20]; j = 0;
        while (line[i] != ',' && line[i] != '\0') status[j++] = line[i++]; status[j] = '\0';

        if (pID == getid() && comparison(status, (char*)"unpaid"))
        {
            cout << "Bill ID: " << billID << " | Amount: PKR " << amount << endl;
            found = true;
        }
    }
    file.close();

    if (!found)
    {
        cout << "No unpaid bills.\n";
        return;
    }

    // --- get bill ID ---
    int chosenID;
    cout << "\nEnter Bill ID to pay: ";
    cin >> chosenID;

    // --- validate bill belongs to patient and is unpaid ---
    ifstream file2("bills.txt");
    bool valid = false;
    double billAmount = 0;

    while (file2.getline(line, 200))
    {
        if (line[0] == '\0') continue;
        int i = 0, j;

        char bid[10]; j = 0;
        while (line[i] != ',') bid[j++] = line[i++]; bid[j] = '\0'; i++;
        int bID = atoi(bid);

        char pid[10]; j = 0;
        while (line[i] != ',') pid[j++] = line[i++]; pid[j] = '\0'; i++;
        int pID = atoi(pid);

        while (line[i] != ',') i++; i++;  // skip appointment ID

        char amt[20]; j = 0;
        while (line[i] != ',') amt[j++] = line[i++]; amt[j] = '\0'; i++;
        double amount = atof(amt);

        char status[20]; j = 0;
        while (line[i] != ',' && line[i] != '\0') status[j++] = line[i++]; status[j] = '\0';

        if (bID == chosenID && pID == getid() && comparison(status, (char*)"unpaid"))
        {
            valid = true;
            billAmount = amount;
            break;
        }
    }
    file2.close();

    if (!valid)
    {
        cout << "Invalid bill ID.\n";
        return;
    }

    // --- check balance, throw InsufficientFundsException if not enough ---
    try
    {
        if (balance < billAmount)
            throw InsufficientFundsException();
    }
    catch (InsufficientFundsException& e)
    {
        cout << e.what() << endl;
        return;
    }

    // --- deduct balance ---
    double newBalance = balance - billAmount;
    this->balance = newBalance;

    // --- update bill status using FileHandler ---
    FileHandler::updateBillStatus(chosenID, "paid");

    // --- update patient balance using FileHandler ---
    FileHandler::updatePatientBalance(getid(), newBalance, getName(),
        age, gender, contact, getpass());

    cout << "\nBill paid successfully!" << endl;
    cout << "Remaining balance: PKR " << newBalance << endl;
}
void Patient::topupBalance()
{
    double amount;
    int attempts = 0;
    bool valid = false;

    // --- validate amount with 3 attempts ---
    while (attempts < 3)
    {
        cout << "Enter amount to add (PKR): ";
        cin >> amount;

        try
        {
            if (!Validator::validateAmount(amount))
                throw InvalidInputException();
            valid = true;
            break;
        }
        catch (InvalidInputException& e)
        {
            cout << e.what() << endl;
            attempts++;
            if (attempts < 3)
                cout << "Please try again.\n";
        }
    }

    if (!valid)
    {
        cout << "Too many invalid attempts. Returning to menu.\n";
        return;
    }

    // --- add to balance ---
    double newBalance = balance + amount;
    this->balance = newBalance;

    // --- update patients.txt using FileHandler ---
    FileHandler::updatePatientBalance(getid(), newBalance, getName(),
        age, gender, contact, getpass());

    cout << "Balance updated." << endl;
    cout << "New balance: PKR " << newBalance << endl;
}