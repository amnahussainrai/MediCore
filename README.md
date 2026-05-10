https://github.com/amnahussainrai/MediCore

MEDICORE

HOSPITAL MANAGEMENT SYSTEM



\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

OVERVIEW

Medicore is a completely functional Hospital Management System built with C++ and designed with SFML Graphical Interface. The console serves as my Final Term Project for OOP course.The console supports three roles- Patient, Doctor and Admin, namely and manages bills, appointments, prescriptions, records, and security etc. All data that is required for basic information and initial record is shared across the program via a .txt file. 



\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_



FEATURES

Role              Capabilities

Patient:        Book/Cancel Appointments, Pay Bills, View Medical Record, 

&#x20;                     View Top-Up Balance.



Doctor:        View Appointments, Mark Appointments as Complete, View

&#x20;                    Patients’ Records, Write Prescription



Admin:        Add/Remove Doctors, Discharge Patient(s), View All Data,

&#x20;                    Generate Daily Report,View Security Log



After three (3) failed login attempts, the account is locked and update is logged into

security\_log.txt file. 



\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_



DEPENDENCIES



C++

SFML 2.5+ 

\-For Windows, download from ( https://www.sfml-dev.org/download.php )

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_



How to Compile \& Run

Visual Studio (Windows) — Recommended

Create a new Empty C++ Project.

Add all .h and .cpp files to the project.

Link SFML:

Go to Project → Properties → C/C++ → Additional Include Directories → add the SFML include/ path.

Go to Linker → Additional Library Directories → add the SFML lib/ path.

Go to Linker → Input → Additional Dependencies → add:

&#x20;   sfml-graphics.lib

&#x20;   sfml-window.lib

&#x20;   sfml-system.lib

&#x20;   sfml-audio.lib

Copy the SFML .dll files into your project's output folder (same folder as the .exe).

Make sure the assets/ folder and all .txt data files are in the same directory as the executable.

Press Ctrl+F5 / F5 to build and run.



\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_



ADDITIONAL NOTES

Std::string or any built in string functions were not allowed hence not used. They are only employed in the SFML/GUI code.

Std::vector is also only implemented in the GUI implementation as permitted.

There are no memory leakages in the program.

