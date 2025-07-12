#include "Hospital.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    system("mkdir data");
    ofstream("data/doctors.txt", ios::app).close();
    ofstream("data/nurses.txt", ios::app).close();
    ofstream("data/patients.txt", ios::app).close();
    ofstream("data/appointments.txt", ios::app).close();
    ofstream("data/doctor_credentials.txt", ios::app).close();

    Hospital h;
    h.loadData();

    while (true) {
        int role;
        cout << "\n*** Hospital Management System ***\n";
        cout << "1. Enter as Manager (Admin)\n";
        cout << "2. Enter as Doctor\n";
        cout << "3. Enter as Patient\n";
        cout << "0. Exit\n";
        cout << "Your Choice: ";
        cin >> role;
        cin.ignore();

        if (role == 0) {
            cout << "Exiting out...\n";
            break;
        } else if (role == 1) {
            string u, p;
            cout << "Username: "; getline(cin, u);
            cout << "Password: "; getline(cin, p);
            if (u != "admin" || p != "1234") {
                cout << "Access Denied!\n";
                continue;
            }

            int ch;
            do {
                cout << "\n--- Admin Menu ---\n";
                cout << "1. Add a Doctor\n";
                cout << "2. Delete a Doctor\n";
                cout << "3. Add a Nurse\n";
                cout << "4. Delete a Nurse\n";
                cout << "5. Patient list\n";
                cout << "6. Visits List\n";
                cout << "0. Back\n";
                cout << "Your choice: ";
                cin >> ch;
                cin.ignore();
                string name;
                switch (ch) {
                    case 1:
                        {
                            string specialty;
                            cout << "Doctor name: "; getline(cin, name);
                            cout << "Specialty: "; getline(cin, specialty);
                            h.addDoctor(name, specialty);
                            string uname, pwd;
                            cout << "Set username for doctor: "; getline(cin, uname);
                            cout << "Set password for doctor: "; getline(cin, pwd);
                            h.addDoctorCredential(uname, pwd, name);
                        }
                        break;
                    case 2: cout << "Doctor name: "; getline(cin, name); h.removeDoctor(name); break;
                    case 3: cout << "Nurse name: "; getline(cin, name); h.addNurse(name); break;
                    case 4: cout << "Nurse name: "; getline(cin, name); h.removeNurse(name); break;
                    case 5: h.showPatients(); break;
                    case 6: h.showAllAppointments(); break;
                }
            } while (ch != 0);

        } else if (role == 2) {
            string doctorName, username, password;
            cout << "Doctor name: "; getline(cin, doctorName);
            cout << "Username: "; getline(cin, username);
            cout << "Password: "; getline(cin, password);

            string verifiedDoctorName;
            if (!h.validateDoctorLogin(username, password, verifiedDoctorName) || verifiedDoctorName != doctorName) {
                cout << "Login failed or name mismatch.\n";
                continue;
            }

            int docChoice;
            do {
                cout << "\n--- Doctor menu ---\n";
                cout << "1. My Visits\n";
                cout << "2. Add a patient\n";
                cout << "3. Search patient\n";
                cout << "4. Write a prescription for a patient\n";
                cout << "5. See list of all patients\n";
                cout << "6. See visits of a patient\n";
                cout << "7. Edit patient prescription\n";
                cout << "8. Delete patient\n";
                cout << "0. Back\n";
                cout << "Your choice: ";
                cin >> docChoice;
                cin.ignore();

                string patientName, prescription;
                switch (docChoice) {
                    case 1: h.showAppointmentsForDoctor(doctorName); break;
                    case 2: cout << "Patient name: "; getline(cin, patientName); h.addPatient(patientName); break;
                    case 3: cout << "Patient name: "; getline(cin, patientName); h.searchPatient(patientName); break;
                    case 4: cout << "Patient name: "; getline(cin, patientName); cout << "Prescription: "; getline(cin, prescription); h.writePrescription(patientName, prescription); break;
                    case 5: h.showPatients(); break;
                    case 6: cout << "Patient name: "; getline(cin, patientName); h.showPatientAppointments(patientName); break;
                    case 7: cout << "Patient name: "; getline(cin, patientName); cout << "New prescription: "; getline(cin, prescription); h.editPrescription(patientName, prescription); break;
                    case 8: cout << "Patient name: "; getline(cin, patientName); h.removePatientCompletely(patientName); break;
                }
            } while (docChoice != 0);

        } else if (role == 3) {
            string patientName;
            cout << "Patient name: ";
            getline(cin, patientName);
            h.addPatient(patientName);

            int ch;
            do {
                cout << "\n--- Patient Menu ---\n";
                cout << "1. Set a visit\n";
                cout << "2. Cancel a visit\n";
                cout << "3. See my visits\n";
                cout << "0. Back\n";
                cout << "Your choice: ";
                cin >> ch;
                cin.ignore();

                string doc, day;
                int hour;

                if (ch == 1) {
                    h.showDoctors();
                    cout << "Doctor name: "; getline(cin, doc);
                    if (!h.doctorExists(doc)) {
                        cout << "This doctor does not exist.\n";
                        continue;
                    }
                    cout << "Day (Saturday to Wednesday): "; getline(cin, day);
                    cout << "Hour (7 to 19): "; cin >> hour; cin.ignore();
                    if (h.isSlotAvailable(doc, day, hour)) {
                        h.bookAppointment({doc, patientName, day, hour});
                        cout << "Visit successful.\n";
                    } else {
                        cout << "This time slot is not available.\n";
                    }
                } else if (ch == 2) {
                    cout << "Doctor name: "; getline(cin, doc);
                    cout << "Day: "; getline(cin, day);
                    cout << "Hour: "; cin >> hour; cin.ignore();
                    h.cancelAppointment(patientName, doc, day, hour);
                    cout << "Visit canceled.\n";
                } else if (ch == 3) {
                    h.showPatientAppointments(patientName);
                }
            } while (ch != 0);
        }
    }

    h.saveData();
    return 0;
}
