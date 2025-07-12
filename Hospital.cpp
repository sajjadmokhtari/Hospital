#include "Hospital.h"
#include <fstream>
#include <sstream>
#include <algorithm>

void Hospital::loadData() {
    ifstream doc("data/doctors.txt");
    string line;

    while (getline(doc, line)) {
    stringstream ss(line);
    string name, spec;
    getline(ss, name, ',');
    getline(ss, spec);
    doctors.push_back({name, spec});
}
    doc.close();

    ifstream nur("data/nurses.txt");
    while (getline(nur, line)) nurses.push_back(line);
    nur.close();

    ifstream pat("data/patients.txt");
    while (getline(pat, line)) patients.push_back({line});
    pat.close();

    ifstream app("data/appointments.txt");
    while (getline(app, line)) {
        stringstream ss(line);
        string d, p, day, hourStr;
        getline(ss, d, ','); getline(ss, p, ','); getline(ss, day, ','); getline(ss, hourStr);
        appointments.push_back({d, p, day, stoi(hourStr)});
    }
    app.close();
}

void Hospital::saveData() {
    ofstream doc("data/doctors.txt");
    for (auto& d : doctors) doc << d.name << "," << d.specialty << endl;
    doc.close();

    ofstream nur("data/nurses.txt");
    for (auto& n : nurses) nur << n << endl;
    nur.close();

    ofstream pat("data/patients.txt");
    for (auto& p : patients) pat << p.name << endl;
    pat.close();

    ofstream app("data/appointments.txt");
    for (auto& a : appointments)
        app << a.doctorName << "," << a.patientName << "," << a.day << "," << a.hour << endl;
    app.close();
}

// Doctor
void Hospital::addDoctor(const string& name, const string& specialty) {
    doctors.push_back({name, specialty});
}
void Hospital::removeDoctor(const string& name) {
    doctors.erase(remove_if(doctors.begin(), doctors.end(), [&](const Doctor& d) {
        return d.name == name;
    }), doctors.end());
}

void Hospital::showDoctors() {
    for (const auto& d : doctors)
        cout << "- " << d.name << " (" << d.specialty << ")" << endl;
}
bool Hospital::doctorExists(const string& name) {
    return any_of(doctors.begin(), doctors.end(), [&](const Doctor& d) {
        return d.name == name;
    });
}


// Nurse
void Hospital::addNurse(const string& name) { nurses.push_back(name); }
void Hospital::removeNurse(const string& name) {
    nurses.erase(remove(nurses.begin(), nurses.end(), name), nurses.end());
}
void Hospital::showNurses() {
    for (const auto& n : nurses) cout << "- " << n << endl;
}



// Patient
void Hospital::addPatient(const string& name) {
    if (none_of(patients.begin(), patients.end(), [&](const Patient& p){ return p.name == name; }))
        patients.push_back({name});
}
void Hospital::removePatient(const string& name) {
    patients.erase(remove_if(patients.begin(), patients.end(), [&](const Patient& p){ return p.name == name; }), patients.end());
}
void Hospital::showPatients() {
    for (auto& p : patients) cout << "- " << p.name << endl;
}
void Hospital::showPatientAppointments(const string& name) {
    for (auto& a : appointments)
        if (a.patientName == name)
            cout << a.day <<  "\t" << a.hour << ":00 o'clock with dr. " << a.doctorName << endl;
}

// Appointment
void Hospital::showAllAppointments() {
    for (auto& a : appointments)
        cout << a.patientName << " -> " << a.doctorName << " in " << a.day << "\t" << a.hour << ":00 o'clock" << endl;
}
bool Hospital::isSlotAvailable(const string& doctorName, const string& day, int hour) {
    return none_of(appointments.begin(), appointments.end(), [&](const Appointment& a) {
        return a.doctorName == doctorName && a.day == day && a.hour == hour;
    });
}
void Hospital::bookAppointment(const Appointment& a) {
    appointments.push_back(a);
}
void Hospital::cancelAppointment(const string& patientName, const string& doctorName, const string& day, int hour) {
    appointments.erase(remove_if(appointments.begin(), appointments.end(), [&](const Appointment& a) {
        return a.doctorName == doctorName && a.patientName == patientName && a.day == day && a.hour == hour;
    }), appointments.end());
}
void Hospital::showAppointmentsForDoctor(const string& doctorName) {
    for (const auto& a : appointments) {
        if (a.doctorName == doctorName)
            cout << a.patientName << " in " << a.day << " hour " << a.hour << ":00\n";
    }
}

void Hospital::writePrescription(const string& patientName, const string& prescription) {
    ofstream file("data/Medicines.txt", ios::app);
    file << patientName << ": " << prescription << endl;
    file.close();
}

void Hospital::searchPatient(const string& name) {
    auto it = find_if(patients.begin(), patients.end(), [&](const Patient& p) {
        return p.name == name;
    });

    if (it != patients.end()) {
        cout << "Patient has been found: " << it->name << endl;
        showPatientAppointments(name);
    } else {
        cout << "Patient not found.\n";
    }
}

void Hospital::editPrescription(const string& patientName, const string& newPrescription) {
    ifstream infile("data/Medicines.txt");
    ofstream temp("data/Medicines_temp.txt");
    string line;
    bool found = false;

    while (getline(infile, line)) {
        size_t pos = line.find(":");
        if (pos != string::npos) {
            string name = line.substr(0, pos);
            if (name == patientName) {
                temp << patientName << ": " << newPrescription << endl;
                found = true;
            } else {
                temp << line << endl;
            }
        } else {
            temp << line << endl;
        }
    }

    infile.close();
    temp.close();

    remove("data/Medicines.txt");
    rename("data/Medicines_temp.txt", "data/prescriptions.txt");

    if (found)
        cout << "Medicine Changed succesfully.\n";
    else
        cout << "Could not find a Medicine for this patient, a New one well be Created.\n";

    if (!found)
        writePrescription(patientName, newPrescription);
}

void Hospital::removePatientCompletely(const string& name) {
    // حذف از لیست بیماران
    removePatient(name);

    // حذف نوبت‌ها
    appointments.erase(remove_if(appointments.begin(), appointments.end(),
        [&](const Appointment& a) { return a.patientName == name; }), appointments.end());

    // حذف نسخه‌ها
    ifstream infile("data/Medicines.txt");
    ofstream temp("data/Medicines_temp.txt");
    string line;

    while (getline(infile, line)) {
        if (line.find(name + ":") != 0)
            temp << line << endl;
    }

    infile.close();
    temp.close();

    remove("data/Medicines.txt");
    rename("data/Medicines_temp.txt", "data/Medicines.txt");

    cout << "Patient and his info deleted succesfully.\n";
}

bool Hospital::validateDoctorLogin(const string& username, const string& password, string& doctorName) {
    ifstream file("data/doctor_credentials.txt");
    string line;
    while (getline(file, line)) {
        string u, p, name;
        stringstream ss(line);
        getline(ss, u, ',');
        getline(ss, p, ',');
        getline(ss, name, ',');

        if (u == username && p == password) {
            doctorName = name;
            return true;
        }
    }
    return false;
}


void Hospital::addDoctorCredential(const string& username, const string& password, const string& doctorName) {
    ofstream file("data/doctor_credentials.txt", ios::app);
    file << username << "," << password << "," << doctorName << endl;
    file.close();
}
