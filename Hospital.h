#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Appointment {
    string doctorName;
    string patientName;
    string day;
    int hour;
};

struct Patient {
    string name;
    vector<Appointment> appointments;
};

struct Doctor {
    string name;
    string specialty;

};

class Hospital {
private:
    vector<Doctor> doctors;
    vector<string> nurses;
    vector<Appointment> appointments;
    vector<Patient> patients;

public:
    void loadData();
    void saveData();

    // Doctor
    void addDoctor(const string& name, const string& specialty);
    void removeDoctor(const string& name);
    void showDoctors();
    bool doctorExists(const string& name);

    // Nurse
    void addNurse(const string& name);
    void removeNurse(const string& name);
    void showNurses();

    // Appointment
    void showAllAppointments();
    bool isSlotAvailable(const string& doctorName, const string& day, int hour);
    void bookAppointment(const Appointment& a);
    void cancelAppointment(const string& patientName, const string& doctorName, const string& day, int hour);
    void showAppointmentsForDoctor(const string& doctorName);

    // Patient
    void addPatient(const string& name);
    void removePatient(const string& name);
    void showPatients();
    void showPatientAppointments(const string& name);

    // Patient prescriptions
    void writePrescription(const string& patientName, const string& prescription);
    void searchPatient(const string& name);

    void editPrescription(const string& patientName, const string& newPrescription);
    void removePatientCompletely(const string& name);

    bool validateDoctorLogin(const string& username, const string& password, string& doctorName);
    void addDoctorCredential(const string& username, const string& password, const string& doctorName);



};

#endif
