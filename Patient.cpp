#include "Person.h"
#include "Patient.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
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
Patient::Patient(int id, const char* n, const char* p, int age, const char* g, double b, int contact) :
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
    cout << "Balance: PKR " << balance << endl;
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
void Patient::bookappointment()//validating left input else cases
{

        char specialization[20];
        cout << "Enter specialization to search (e.g. Cardiology): ";
        cin.ignore(1000, '\n');
        cin.getline(specialization, 20);

        //SPECIALISATION FIND KAR RAHAY DOC KI
        ifstream docfile("doctors.txt");

        if (!docfile)
        {
            cout << "Error opening file!\n";
            return;
        }
       

        char line1[200];

        // skip header line
        //file.getline(line, 200);
        bool found = false;
        double docfee = 0;
        while (docfile.getline(line1, 200))
        {
            int i = 0;

            // --- Extract ID ---
            char id[10];
            int j = 0;
            while (line1[i] != ',' && line1[i] != '\0')
            {
                id[j++] = line1[i++];
            }
            id[j] = '\0';
            i++; // skip comma

            // --- Extract Name ---
            char name[20];
            j = 0;
            while (line1[i] != ',' && line1[i] != '\0')
            {
                name[j++] = line1[i++];
            }
            name[j] = '\0';
            i++;

            // --- Extract Specialization ---
            char spec[20];
            j = 0;
            while (line1[i] != ',' && line1[i] != '\0')
            {
                spec[j++] = line1[i++];
            }
            spec[j] = '\0';
            i++;

            // skip contact
            while (line1[i] != ',' && line1[i] != '\0')
                i++;
            i++;
            

            // skip password
            while (line1[i] != ',' && line1[i] != '\0')
                i++;
            i++;
            

            // --- Extract Fee ---
            char fee[20];
            j = 0;
            while (line1[i] != ',' && line1[i] != '\0')
            {
                fee[j++] = line1[i++];
            }
            fee[j] = '\0';
            

            // --- Compare specialization ---
            bool match = true;
            if (comparison(spec, specialization))
            {
                match = true;
                found = true;
            }
            else {
                match = false;
            }


            // --- Display if match ---
            if (match)
            {
                cout << "ID: " << id << " | " << "Name: " << name << " | " << "Fee: " << fee << endl;
            }

        }
        if (found == false)
        {
            cout << "No doctors of that specialisation found" << endl;
            displaymenu();
            return;
        }

        docfile.close();

        //ID OF DOC
        int selectedID;
        cout << "Enter Doctor ID to book: ";
        cin >> selectedID;

        ifstream docfile2("doctors.txt");

        if (!docfile2)
        {
            cout << "Error opening file!\n";
            return;
        }

        char line2[200];
        //file2.getline(line, 200); // skip header

        bool docfound = false;

        while (docfile2.getline(line2, 200))
        {
            int i = 0;

            // --- Extract ID ---
            char id[10];
            int j = 0;
            while (line2[i] != ',' && line2[i] != '\0')
                id[j++] = line2[i++];
            id[j] = '\0';

            int fileID = atoi(id);   // convert

            i++;

            // --- Extract Name ---
            char name[50];
            j = 0;
            while (line2[i] != ',' && line2[i] != '\0')
                name[j++] = line2[i++];
            name[j] = '\0';

            i++;

            // --- Extract Specialization ---
            char spec[20];
            j = 0;
            while (line2[i] != ',' && line2[i] != '\0')
            {
                spec[j++] = line2[i++];
            }
            spec[j] = '\0';
            i++;

            // skip contact
            while (line2[i] != ',' && line2[i] != '\0')
                i++;
            i++;


            // skip password
            while (line2[i] != ',' && line2[i] != '\0')
                i++;
            i++;


            // --- Extract Fee ---
            char fee[20];
            j = 0;
            while (line2[i] != ',' && line2[i] != '\0')
            {
                fee[j++] = line1[i++];
            }
            fee[j] = '\0';


            // First check specialization
            if (!comparison(spec, specialization))
                continue;

            // Then check ID
            if (fileID == selectedID)
            {
                docfound = true;
                docfee = atof(fee);

                cout << "Doctor found: ";
                cout << "ID: " << fileID << " | ";
                cout << "Name: " << name << endl;

                break; //  stop after finding
            }
        }
        docfile2.close();

        if (!docfound)
        {
            cout << "Invalid ID or doctor does not match specialization.\n";
            displaymenu();
            return;
        }

        

        if (docfound == true)
        {
            char date[11];
            cout << "Enter the date you want to book your appointment on (dd-mm-yyyy): " << endl;
            cin >> date;
            //CHECK FORMAT LATER WHEN YOU'VE MADE THE VALIDATOR
            char time[6];
            cout << "Enter a time slot from the follwing options: 09:00 10:00 11:00 12:00 13:00 14:00 15:00 16:00." << endl;
            cin >> time;
            //CHECK VALIDITY LATER KAY IS IT ONE OF THE 8 SLOTS
            bool available = true;

            ifstream appfile("appointments.txt");

            if (!appfile)
            {
                cout << "Error opening appointments file!\n";
                return;
            }

            char line3[200];

            // skip header
            //file.getline(line, 200);

            while (appfile.getline(line3, 200))
            {
                int i = 0;

                // --- appointment_id (skip) ---
                while (line3[i] != ',') i++;
                i++;

                // --- patient_id (skip) ---
                while (line3[i] != ',') i++;
                i++;

                // --- doctor_id ---
                char docID[10];
                int j = 0;
                while (line3[i] != ',')
                    docID[j++] = line3[i++];
                docID[j] = '\0';
                i++;

                int fileDocID = atoi(docID);

                // --- date ---
                char fileDate[11];
                j = 0;
                while (line3[i] != ',')
                    fileDate[j++] = line3[i++];
                fileDate[j] = '\0';
                i++;

                // --- time slot ---
                char fileTime[10];
                j = 0;
                while (line3[i] != ',')
                    fileTime[j++] = line3[i++];
                fileTime[j] = '\0';
                i++;

                // --- status ---
                char status[20];
                j = 0;
                while (line3[i] != '\0')
                    status[j++] = line3[i++];
                status[j] = '\0';

                char cancelled[10] = "cancelled";
                //  CHECK CONDITIONS
                if (fileDocID == selectedID &&
                    comparison(fileDate, date) &&
                    comparison(fileTime, time) &&
                    !comparison(status,cancelled))
                {
                    available = false;
                    break;
                }
            }

            appfile.close();

            if (available)//slot available
            {
                ifstream inFile("patients.txt");

                char line4[200];
               // inFile.getline(line2, 200);  // copy header
                //tempFile << line2 << endl;

                int patientid = 0;
                char name[20];
                int patientage = 0;
                char gender[10];
                char contact[20];
                char password[20];
                double balance = 0;
                while (inFile.getline(line4, 200))
                {
                    int i = 0;

                    // --- Patient ID ---
                    char id[10];
                    int j = 0;
                    while (line4[i] != ',')
                        id[j++] = line4[i++];
                    id[j] = '\0';
                    int pid = atoi(id);
                    if (pid == getid())//making sure we're storing data of the correct patient
                    {
                        patientid = atoi(id);
                        i++;

                        // --- Name ---
                        j = 0;
                        while (line4[i] != ',')
                            name[j++] = line4[i++];
                        name[j] = '\0';
                        i++;

                        // --- Age ---
                        char age[10];
                        j = 0;
                        while (line4[i] != ',')
                            age[j++] = line4[i++];
                        age[j] = '\0';
                        patientage = atoi(age);
                        i++;

                        // --- Gender ---

                        j = 0;
                        while (line4[i] != ',')
                            gender[j++] = line4[i++];
                        gender[j] = '\0';
                        i++;

                        // --- Contact ---

                        j = 0;
                        while (line4[i] != ',')
                            contact[j++] = line4[i++];
                        contact[j] = '\0';
                        i++;

                        // --- Password ---

                        j = 0;
                        while (line4[i] != ',')
                            password[j++] = line4[i++];
                        password[j] = '\0';
                        i++;

                        // --- Balance ---
                        char balanceStr[20];
                        j = 0;
                        while (line4[i] != '\0')
                            balanceStr[j++] = line4[i++];
                        balanceStr[j] = '\0';
                        balance = atof(balanceStr);
                        break;
                    }
                    
                }
                inFile.close();
                //OVERLOAD BAAD MEIN
                
                if (balance >= docfee)
                {
                    ifstream file("appointments.txt");

                    char line5[200];
                    //file.getline(line, 200); // skip header

                    int maxID = 0;

                    while (file.getline(line5, 200))
                    {
                        int i = 0;
                        char id[10];
                        int j = 0;

                        while (line5[i] != ',')
                            id[j++] = line5[i++];

                        id[j] = '\0';

                        int currentID = atoi(id);

                        if (currentID > maxID)
                            maxID = currentID;
                    }

                    file.close();

                    int newAppointmentID = maxID + 1;
                    ofstream file2("appointments.txt", ios::app);
                    
                    file2 << newAppointmentID << ","
                        << patientid << ","
                        << selectedID << ","
                        << date << ","
                        << time << ","
                        << "pending" << endl;

                    file2.close();
                    ifstream billFile("bills.txt");

                    billFile.getline(line5, 200);

                    int maxBillID = 0;

                    while (billFile.getline(line5, 200))
                    {
                        int i = 0, j = 0;
                        char id[10];

                        while (line5[i] != ',')
                            id[j++] = line5[i++];

                        id[j] = '\0';

                        int currentID = atoi(id);

                        if (currentID > maxBillID)
                            maxBillID = currentID;
                    }

                    billFile.close();

                    int newBillID = maxBillID + 1;
                    ofstream billOut("bills.txt", ios::app);
                    billOut << newBillID << ","
                        << patientid << ","
                        << newAppointmentID << ","
                        << docfee << ","
                        << "unpaid" << ","
                        << date << endl;

                    billOut.close();

                    ifstream pfile("patients.txt");
                    ofstream tempFile("temp.txt");

                    char line6[200];
                   // inFile.getline(line2, 200);  // copy header
                    //tempFile << line2 << endl;

                    while (pfile.getline(line6, 200))
                    {
                        int i = 0;

                        // --- extract patient ID ---
                        char id[10];
                        int j = 0;
                        while (line6[i] != ',')
                            id[j++] = line6[i++];
                        id[j] = '\0';

                        int fileID = atoi(id);

                        cout << "file id: " << fileID << endl;
                        cout << "patient id:" << patientid;
                        if (fileID == patientid)
                        {
                            // adjust balance
                            double newBalance = balance-docfee;
                            cout << "Patient balance:" << balance << endl;
                            cout << "Doctor fee:" << docfee << endl;
                            cout << "New balance" << newBalance << endl;
                            // reconstruct full row manually
                            tempFile << fileID << ","
                                << name << ","
                                << patientage << ","
                                << gender << ","
                                << contact << ","
                                << password << ","
                                << newBalance << endl;
                        }
                        else
                        {
                            tempFile << line6 << endl;
                        }
                    }

                    pfile.close();
                    tempFile.close();

                    // replace file
                    if (remove("patients.txt") != 0)
                    {
                        cout << "Error deleting file!\n";
                    }
                    if (rename("temp.txt", "patients.txt") != 0)
                    {
                        cout << "Error renaming file!\n";
                    }


                    cout << "Appointment booked successfully!\n";
                    cout << "Appointment ID: " << newAppointmentID << endl;
                }
                else
                {
                    cout << "Insufficient Balance" << endl;
                    //displaymenu();
                    return;
                }

               
               

                
            }
            else
            {
                //HANDLE THIS LATER WHEN UVE WRITTEN SLOTUNAVAILABLEEXCEPTION CLASS
            }

        }
         
    

}

void Patient::cancelappointment()
{
        ifstream file("appointments.txt");

        if (!file)
        {
            cout << "Error opening appointments file!\n";
            return;
        }

        char line[200];
        bool found = false;

        cout << "\nPending Appointments:\n";

        //  Display pending appointments
        while (file.getline(line, 200))
        {
            int i = 0;

            // appointment ID
            char appID[10];
            int j = 0;
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
            int filePID = atoi(pid);
            i++;

            // doctor ID
            char docID[10];
            j = 0;
            while (line[i] != ',')
                docID[j++] = line[i++];
            docID[j] = '\0';
            int doctorID = atoi(docID);
            i++;

            // date
            char date[20];
            j = 0;
            while (line[i] != ',')
                date[j++] = line[i++];
            date[j] = '\0';
            i++;

            // time
            char time[10];
            j = 0;
            while (line[i] != ',')
                time[j++] = line[i++];
            time[j] = '\0';
            i++;

            // status
            char status[20];
            j = 0;
            while (line[i] != '\0')
                status[j++] = line[i++];
            status[j] = '\0';

            // show only pending appointments of this patient
            if (filePID == getid() && comparison(status, (char*)"pending"))
            {
                cout << "Appointment ID: " << id
                    << " | Doctor ID: " << doctorID
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

        // Step 2: Take input
        int chosenID;
        cout << "\nEnter the ID of Appointment you want to cancel: ";
        cin >> chosenID;

        ifstream file2("appointments.txt");
        ofstream temp("temp.txt");

        bool valid = false;
        int selectedDoctorID = 0;
        char selectedDate[20], selectedTime[10];

        while (file2.getline(line, 200))
        {
            int i = 0;

            char appID[10];
            int j = 0;
            while (line[i] != ',')
                appID[j++] = line[i++];
            appID[j] = '\0';
            int id = atoi(appID);
            i++;

            char pid[10];
            j = 0;
            while (line[i] != ',')
                pid[j++] = line[i++];
            pid[j] = '\0';
            int filePID = atoi(pid);
            i++;

            char docID[10];
            j = 0;
            while (line[i] != ',')
                docID[j++] = line[i++];
            docID[j] = '\0';
            int doctorID = atoi(docID);
            i++;

            char date[20];
            j = 0;
            while (line[i] != ',')
                date[j++] = line[i++];
            date[j] = '\0';
            i++;

            char time[10];
            j = 0;
            while (line[i] != ',')
                time[j++] = line[i++];
            time[j] = '\0';
            i++;

            char status[20];
            j = 0;
            while (line[i] != '\0')
                status[j++] = line[i++];
            status[j] = '\0';

            if (id == chosenID && filePID == getid() && comparison(status, (char*)"pending"))
            {
                valid = true;

                //  mark cancelled
                temp << id << "," << filePID << "," << doctorID << ","
                    << date << "," << time << ",cancelled" << endl;

                selectedDoctorID = doctorID;
                strcpy(selectedDate, date);
                strcpy(selectedTime, time);
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
       

        // Step 3: Get doctor fee
        ifstream docfile("doctors.txt");
        double docfee = 0;

        while (docfile.getline(line, 200))
        {
            int i = 0;

            char id[10];
            int j = 0;
            while (line[i] != ',')
                id[j++] = line[i++];
            id[j] = '\0';
            int fileID = atoi(id);
            i++;

            // skip name
            while (line[i] != ',') i++;
            i++;

            // skip specialization
            while (line[i] != ',') i++;
            i++;

            // skip contact
            while (line[i] != ',') i++;
            i++;

            // skip password
            while (line[i] != ',') i++;
            i++;

            char fee[20];
            j = 0;
            while (line[i] != '\0')
                fee[j++] = line[i++];
            fee[j] = '\0';

            if (fileID == selectedDoctorID)
            {
                docfee = atof(fee);
                break;
            }
        }

        docfile.close();

        //  Step 4: REFUND (update patient balance)
        ifstream pfile("patients.txt");
        ofstream temp2("temp2.txt");

        while (pfile.getline(line, 200))
        {
            int i = 0;

            char id[10];
            int j = 0;
            while (line[i] != ',')
                id[j++] = line[i++];
            id[j] = '\0';

            int fileID = atoi(id);

            if (fileID == getid())
            {
                double newBalance = balance + docfee;

                temp2 << fileID << "," << getName() << ","
                    << age << "," << gender << ","
                    << contact << "," << getpass() << ","
                    << newBalance << endl;

                balance = newBalance;
            }
            else
            {
                temp2 << line << endl;
            }
        }

        pfile.close();
        temp2.close();
        if (remove("patients.txt") != 0)
        {
            cout << "Error deleting file!\n";
        }
        if (rename("temp2.txt", "patients.txt") != 0)
        {
            cout << "Error renaming file!\n";
        }


        // Step 5: update bills
        ifstream bfile("bills.txt");
        ofstream temp3("temp3.txt");

        while (bfile.getline(line, 200))
        {
            int i = 0;

            char billID[10];
            int j = 0;
            while (line[i] != ',')
                billID[j++] = line[i++];
            billID[j] = '\0';
            i++;

            // skip rest until status
            int commaCount = 0;
            char buffer[200];
            int k = 0;

            while (line[i] != '\0')
                buffer[k++] = line[i++];
            buffer[k] = '\0';

            // replace "unpaid/pending" → "cancelled"
            temp3 << billID << "," << buffer << endl;
        }

        bfile.close();
        temp3.close();
        if (remove("bills.txt") != 0)
        {
            cout << "Error deleting file!\n";
        }
        if (rename("temp3.txt", "bills.txt") != 0)
        {
            cout << "Error renaming file!\n";
        }


        cout << "\nAppointment cancelled.\nPKR " << docfee << " refunded to your balance.\n";
    
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

                strcpy(temp, date[i]);
                strcpy(date[i], date[j]);
                strcpy(date[j], temp);

                strcpy(temp, time[i]);
                strcpy(time[i], time[j]);
                strcpy(time[j], temp);

                strcpy(temp, status[i]);
                strcpy(status[i], status[j]);
                strcpy(status[j], temp);
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
    int appID[100], docID[100];
    char date[100][11], meds[100][100], notes[100][100];

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
        while (line[i] != ',')
            meds[count][j++] = line[i++];
        meds[count][j] = '\0';
        i++;

        // notes
        j = 0;
        while (line[i] != '\0')
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

                char temp[150];

                strcpy(temp, date[i]);
                strcpy(date[i], date[j]);
                strcpy(date[j], temp);

                strcpy(temp, meds[i]);
                strcpy(meds[i], meds[j]);
                strcpy(meds[j], temp);

                strcpy(temp, notes[i]);
                strcpy(notes[i], notes[j]);
                strcpy(notes[j], temp);
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
            if (!comparison(status, (char*)"paid"))
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

    if (!file)
    {
        cout << "Error opening bills file!\n";
        return;
    }

    char line[200];
    bool found = false;

    cout << "\nUnpaid Bills:\n";

    // ✅ STEP 1: Show unpaid bills
    while (file.getline(line, 200))
    {
        int i = 0, j;

        // bill ID
        char bid[10];
        j = 0;
        while (line[i] != ',' && line[i] != '\0')
            bid[j++] = line[i++];
        bid[j] = '\0';
        int billID = atoi(bid);
        i++;

        // patient ID
        char pid[10];
        j = 0;
        while (line[i] != ',' && line[i] != '\0')
            pid[j++] = line[i++];
        pid[j] = '\0';
        int pID = atoi(pid);
        i++;

        // appointment ID
        while (line[i] != ',' && line[i] != '\0') i++;
        i++;

        // amount
        char amt[20];
        j = 0;
        while (line[i] != ',' && line[i] != '\0')
            amt[j++] = line[i++];
        amt[j] = '\0';
        double amount = atof(amt);
        i++;

        // status
        char status[20];
        j = 0;
        while (line[i] != ',' && line[i] != '\0')
            status[j++] = line[i++];
        status[j] = '\0';

        // ✅ show only unpaid bills of THIS patient
        if (pID == getid() && !comparison(status, (char*)"paid"))
        {
            cout << "Bill ID: " << billID
                << " | Amount: PKR " << amount
                << endl;
            found = true;
        }
    }

    file.close();

    if (!found)
    {
        cout << "No unpaid bills.\n";
        return;
    }

    // ✅ STEP 2: Ask for ID
    int chosenID;
    cout << "\nEnter Bill ID to pay: ";
    cin >> chosenID;

    // ✅ STEP 3: Find bill + validate
    ifstream file2("bills.txt");
    bool valid = false;
    double billAmount = 0;

    while (file2.getline(line, 200))
    {
        int i = 0, j;

        // bill ID
        char bid[10];
        j = 0;
        while (line[i] != ',')
            bid[j++] = line[i++];
        bid[j] = '\0';
        int bID = atoi(bid);
        i++;

        // patient ID
        char pid[10];
        j = 0;
        while (line[i] != ',')
            pid[j++] = line[i++];
        pid[j] = '\0';
        int pID = atoi(pid);
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
        while (line[i] != ',' && line[i] != '\0')
            status[j++] = line[i++];
        status[j] = '\0';

        if (bID == chosenID && pID == getid() && !comparison(status, (char*)"paid"))
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

    // ✅ STEP 4: Check balance
    if (balance < billAmount)
    {
        cout << "Insufficient balance.\n";
        return;
    }

    // ✅ deduct balance
    balance -= billAmount;

    // ✅ STEP 5: Update bills.txt
    ifstream inFile("bills.txt");
    ofstream tempFile("temp.txt");

    while (inFile.getline(line, 200))
    {
        int i = 0, j;

        char bid[10];
        j = 0;
        while (line[i] != ',')
            bid[j++] = line[i++];
        bid[j] = '\0';
        int bID = atoi(bid);

        if (bID == chosenID)
        {
            // rewrite line as paid
            tempFile << bID;

            // copy rest until amount
            int commaCount = 0;
            i = 0;

            // copy until status
            while (commaCount < 4)
            {
                if (line[i] == ',') commaCount++;
                tempFile << line[i++];
            }

            // skip old status
            while (line[i] != ',' && line[i] != '\0') i++;

            tempFile << "paid";

            // copy remaining (date)
            while (line[i] != '\0')
                tempFile << line[i++];

            tempFile << endl;
        }
        else
        {
            tempFile << line << endl;
        }
    }

    inFile.close();
    tempFile.close();

    remove("bills.txt");
    rename("temp.txt", "bills.txt");

    // ✅ STEP 6: Update patient balance
    ifstream pfile("patients.txt");
    ofstream temp2("temp2.txt");

    while (pfile.getline(line, 200))
    {
        int i = 0, j;

        char id[10];
        j = 0;
        while (line[i] != ',')
            id[j++] = line[i++];
        id[j] = '\0';

        int fileID = atoi(id);

        if (fileID == getid())
        {
            temp2 << fileID << ","
                << getName() << ","
                << age << ","
                << gender << ","
                << contact << ","
                << getpass() << ","
                << balance << endl;
        }
        else
        {
            temp2 << line << endl;
        }
    }

    pfile.close();
    temp2.close();

    remove("patients.txt");
    rename("temp2.txt", "patients.txt");

    cout << "\nBill paid successfully!\n";
    cout << "Remaining balance: PKR " << balance << endl;
}

