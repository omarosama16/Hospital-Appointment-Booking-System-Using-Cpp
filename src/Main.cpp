#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include "system.h"
#include "user.h"
#include "Appointment.h"

using namespace std;

/* ===================== UTILITY FUNCTIONS ===================== */

void printLine(int n = 55, char c = '-')
{
    cout << string(n, c) << "\n";
}

void printHeader(const string &title)
{
    cout << "\n";
    printLine(55, '=');
    cout << "   " << title << "\n";
    printLine(55, '=');
}

void pause()
{
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/* ===================== APPOINTMENT DISPLAY ===================== */

void printApptHeader()
{
    cout << "\n"
         << left
         << setw(5)  << "ID"
         << setw(20) << "Patient"
         << setw(20) << "Doctor"
         << setw(12) << "Date"
         << setw(10) << "Time"
         << setw(12) << "Status" << "\n";
    printLine(79, '-');
}

/* ===================== USER HELPERS ===================== */

string getRole(const User *u)
{
    if (!u)
        return "";
    return u->get_role();
}

// Fix: const ref — only calls const method adminViewAllUsers(), never modifies sys
void listDoctors(const HospitalSystem &sys)
{
    vector<User *> users = sys.adminViewAllUsers();

    cout << "\n"
         << left
         << setw(6)  << "ID"
         << setw(25) << "Doctor Name"
         << "Specialization\n";
    printLine(55, '-');

    bool found = false;
    for (const auto *u : users)
    {
        if (u->get_role() == "doctor")
        {
            cout << setw(6)  << u->get_id()
                 << setw(25) << u->get_name()
                 << "\n";
            found = true;
        }
    }

    if (!found)
        cout << "  No doctors registered yet.\n";
}

/* ===================== PATIENT MENU HANDLERS ===================== */

// Fix: const ref — only calls const method viewMyAppointments()
void handleViewAppointments(const HospitalSystem &sys)
{
    printHeader("MY APPOINTMENTS");

    if (auto appts = sys.viewMyAppointments(); appts.empty())
    {
        cout << "  No appointments found.\n";
    }
    else
    {
        printApptHeader();
        for (const auto &a : appts)
            a.print_row();
    }

    pause();
}

void handleBookAppointment(HospitalSystem &sys)
{
    printHeader("BOOK APPOINTMENT");
    listDoctors(sys);

    int    docId;
    string date;
    string time;

    cout << "\n  Doctor ID: ";
    cin >> docId;
    cin.ignore();
    cout << "  Date (YYYY-MM-DD): ";
    getline(cin, date);
    cout << "  Time: ";
    getline(cin, time);

    if (sys.bookAppointment(docId, date, time))
        cout << "\n  Booked successfully!\n";
    else
        cout << "\n  Booking failed.\n";

    pause();
}

// Returns false when there are no appointments to cancel (caller should continue)
bool handleCancelAppointment(HospitalSystem &sys)
{
    printHeader("CANCEL APPOINTMENT");

    if (auto appts = sys.viewMyAppointments(); appts.empty())
    {
        cout << "  No appointments to cancel.\n";
        pause();
        return false;
    }
    else
    {
        printApptHeader();
        for (const auto &a : appts)
            a.print_row();
    }

    int id;
    cout << "\n  Enter Appointment ID: ";
    cin >> id;
    cin.ignore();

    if (sys.cancelAppointmentPatient(id))
        cout << "  Cancelled successfully.\n";
    else
        cout << "  Cancel failed.\n";

    pause();
    return true;
}

/* ===================== PATIENT MENU ===================== */

void patientMenu(HospitalSystem &sys)
{
    int choice;
    do
    {
        printHeader("PATIENT MENU");
        cout << "  1. View my appointments\n";
        cout << "  2. Book an appointment\n";
        cout << "  3. Cancel an appointment\n";
        cout << "  0. Logout\n";
        printLine();
        cout << "  Choice: ";
        cin >> choice;
        cin.ignore();

        if      (choice == 1) handleViewAppointments(sys);
        else if (choice == 2) handleBookAppointment(sys);
        else if (choice == 3) handleCancelAppointment(sys);

    } while (choice != 0);
}

/* ===================== DOCTOR MENU HANDLERS ===================== */

// Fix: const ref — only calls const method viewDoctorSchedule()
void handleViewSchedule(const HospitalSystem &sys)
{
    printHeader("MY SCHEDULE");

    if (auto appts = sys.viewDoctorSchedule(); appts.empty())
    {
        cout << "  No appointments.\n";
    }
    else
    {
        printApptHeader();
        for (const auto &a : appts)
            a.print_row();
    }

    pause();
}

// Returns false when there are no appointments to complete (caller should continue)
bool handleCompleteAppointment(HospitalSystem &sys)
{
    printHeader("COMPLETE APPOINTMENT");

    if (auto appts = sys.viewDoctorSchedule(); appts.empty())
    {
        cout << "  No appointments.\n";
        pause();
        return false;
    }
    else
    {
        printApptHeader();
        for (const auto &a : appts)
            a.print_row();
    }

    int id;
    cout << "\n  Appointment ID: ";
    cin >> id;
    cin.ignore();

    if (sys.completeAppointmentDoctor(id))
        cout << "  Completed successfully.\n";
    else
        cout << "  Failed.\n";

    pause();
    return true;
}

/* ===================== DOCTOR MENU ===================== */

void doctorMenu(HospitalSystem &sys)
{
    int choice;
    do
    {
        printHeader("DOCTOR MENU");
        cout << "  1. View schedule\n";
        cout << "  2. Complete appointment\n";
        cout << "  0. Logout\n";
        printLine();
        cout << "  Choice: ";
        cin >> choice;
        cin.ignore();

        if      (choice == 1) handleViewSchedule(sys);
        else if (choice == 2) handleCompleteAppointment(sys);

    } while (choice != 0);
}

/* ===================== ADMIN MENU HANDLERS ===================== */

// Fix: const ref — only calls const methods adminViewAllUsers()
void handleViewUsers(const HospitalSystem &sys)
{
    printHeader("USERS");
    auto users = sys.adminViewAllUsers();

    cout << left
         << setw(5)  << "ID"
         << setw(20) << "Name"
         << setw(25) << "Email"
         << "Role\n";
    printLine(70, '-');

    for (const auto *u : users)
        cout << setw(5)  << u->get_id()
             << setw(20) << u->get_name()
             << setw(25) << u->get_email()
             << u->get_role() << "\n";

    pause();
}

// Fix: const ref — only calls const method adminViewAllAppointments()
void handleViewAllAppointments(const HospitalSystem &sys)
{
    printHeader("APPOINTMENTS");

    if (auto appts = sys.adminViewAllAppointments(); appts.empty())
    {
        cout << "  No appointments.\n";
    }
    else
    {
        printApptHeader();
        for (const auto &a : appts)
            a.print_row();
    }

    pause();
}

void handleAddDoctor(HospitalSystem &sys)
{
    printHeader("ADD DOCTOR");
    string name;
    string email;
    string pass;
    string spec;

    cout << " Name: ";           getline(cin, name);
    cout << " Email: ";          getline(cin, email);
    cout << " Password: ";       getline(cin, pass);
    cout << " Specialization: "; getline(cin, spec);

    sys.registerNewDoctor(name, email, pass, spec);
    cout << "\n  Doctor added.\n";
    pause();
}

void handleAddPatient(HospitalSystem &sys)
{
    printHeader("ADD PATIENT");
    string name;
    string email;
    string pass;
    string phone;

    cout << " Name: ";     getline(cin, name);
    cout << " Email: ";    getline(cin, email);
    cout << " Password: "; getline(cin, pass);
    cout << " Phone: ";    getline(cin, phone);

    sys.registerNewPatient(name, email, pass, phone);
    cout << "\n  Patient added.\n";
    pause();
}

/* ===================== ADMIN MENU ===================== */

void adminMenu(HospitalSystem &sys)
{
    int choice;
    do
    {
        printHeader("ADMIN MENU");
        cout << "  1. View users\n";
        cout << "  2. View appointments\n";
        cout << "  3. Add doctor\n";
        cout << "  4. Add patient\n";
        cout << "  0. Logout\n";
        printLine();
        cout << "  Choice: ";
        cin >> choice;
        cin.ignore();

        if      (choice == 1) handleViewUsers(sys);
        else if (choice == 2) handleViewAllAppointments(sys);
        else if (choice == 3) handleAddDoctor(sys);
        else if (choice == 4) handleAddPatient(sys);

    } while (choice != 0);
}

/* ===================== MAIN MENU HANDLERS ===================== */

void handleLogin(HospitalSystem &sys)
{
    string email;
    string pass;
    printHeader("LOGIN");
    cout << " Email: ";    getline(cin, email);
    cout << " Password: "; getline(cin, pass);

    if (!sys.login(email, pass))
    {
        cout << "\n Login failed.\n";
        pause();
        return;
    }

    string role = getRole(sys.getCurrentUser());
    cout << "\n Logged in as: " << role << "\n";

    if      (role == "patient") patientMenu(sys);
    else if (role == "doctor")  doctorMenu(sys);
    else if (role == "admin")   adminMenu(sys);
}

void handleRegisterPatient(HospitalSystem &sys)
{
    string name;
    string email;
    string pass;
    string phone;
    printHeader("REGISTER");
    cout << " Name: ";     getline(cin, name);
    cout << " Email: ";    getline(cin, email);
    cout << " Password: "; getline(cin, pass);
    cout << " Phone: ";    getline(cin, phone);

    sys.registerNewPatient(name, email, pass, phone);
    cout << "\n Registered successfully.\n";
    pause();
}

/* ===================== MAIN PROGRAM ===================== */

int main()
{
    HospitalSystem sys;

    cout << "\n===== HOSPITAL SYSTEM =====\n";
    cout << "Admin: admin@mail.com / admin123\n";

    int choice;
    do
    {
        printHeader("MAIN MENU");
        cout << "  1. Login\n";
        cout << "  2. Register Patient\n";
        cout << "  0. Exit\n";
        printLine();
        cout << " Choice: ";
        cin >> choice;
        cin.ignore();

        if      (choice == 1) handleLogin(sys);
        else if (choice == 2) handleRegisterPatient(sys);

    } while (choice != 0);

    cout << "\n Goodbye!\n";
}
