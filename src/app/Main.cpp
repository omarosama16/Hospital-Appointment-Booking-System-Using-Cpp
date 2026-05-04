#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include "system.h"
#include "user.h"
#include "Appointment.h"
#include "Doctor.h"
#include "patient.h"   // Fixed: was "core/Patient.h" — file is patient.h (lowercase)

using namespace std;

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

// Fixed: use get_role() / get_email() (snake_case) — these match user.cpp
string getRoleByEmail(HospitalSystem &sys, const string &email)
{
    vector<User *> users = sys.adminViewAllUsers();
    for (auto u : users)
        if (u->get_email() == email)
            return u->get_role();
    return "";
}

void listDoctors(HospitalSystem &sys)
{
    vector<User *> users = sys.adminViewAllUsers();
    cout << "\n"
         << left
         << setw(6)  << "ID"
         << setw(25) << "Doctor Name"
         << "Specialization\n";
    printLine(55, '-');

    bool found = false;
    for (auto u : users)
    {
        if (u->get_role() == "doctor")
        {
            // Cast to Doctor* to show specialization
            Doctor *d = static_cast<Doctor *>(u);
            cout << setw(6)  << u->get_id()
                 << setw(25) << u->get_name()
                 << d->getSpecialization() << "\n";
            found = true;
        }
    }
    if (!found)
        cout << "  No doctors registered yet.\n";
}

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

        if (choice == 1)
        {
            printHeader("MY APPOINTMENTS");
            vector<Appointment> appts = sys.viewMyAppointments();
            if (appts.empty())
                cout << "  You have no appointments.\n";
            else
            {
                printApptHeader();
                for (auto &a : appts)
                    a.print_row();
            }
            pause();
        }
        else if (choice == 2)
        {
            printHeader("BOOK AN APPOINTMENT");
            listDoctors(sys);

            int docId;
            string date, time;
            cout << "\n  Enter Doctor ID  : ";
            cin >> docId;
            cin.ignore();
            cout << "  Enter Date       : (e.g. 2025-06-01) : ";
            getline(cin, date);
            cout << "  Enter Time slot  : (e.g. 10AM)       : ";
            getline(cin, time);

            if (sys.bookAppointment(docId, date, time))
                cout << "\n  Appointment booked successfully!\n";
            else
                cout << "\n  Booking failed.\n"
                     << "  The slot may already be taken or the doctor ID is invalid.\n";
            pause();
        }
        else if (choice == 3)
        {
            printHeader("CANCEL AN APPOINTMENT");
            vector<Appointment> appts = sys.viewMyAppointments();
            if (appts.empty())
            {
                cout << "  You have no appointments to cancel.\n";
                pause();
                continue;
            }
            printApptHeader();
            for (auto &a : appts)
                a.print_row();

            int id;
            cout << "\n  Enter Appointment ID to cancel: ";
            cin >> id;
            cin.ignore();

            if (sys.cancelAppointmentPatient(id))
                cout << "  Appointment cancelled successfully.\n";
            else
                cout << "  Cancellation failed.\n"
                     << "  Check the ID or the appointment may already be cancelled/completed.\n";
            pause();
        }

    } while (choice != 0);
}

void doctorMenu(HospitalSystem &sys)
{
    int choice;
    do
    {
        printHeader("DOCTOR MENU");
        cout << "  1. View my schedule\n";
        cout << "  2. Mark appointment as Completed\n";
        cout << "  0. Logout\n";
        printLine();
        cout << "  Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1)
        {
            printHeader("MY SCHEDULE");
            vector<Appointment> appts = sys.viewDoctorSchedule();
            if (appts.empty())
                cout << "  No appointments in your schedule.\n";
            else
            {
                printApptHeader();
                for (auto &a : appts)
                    a.print_row();
            }
            pause();
        }
        else if (choice == 2)
        {
            printHeader("MARK APPOINTMENT AS COMPLETED");
            vector<Appointment> appts = sys.viewDoctorSchedule();
            if (appts.empty())
            {
                cout << "  No appointments in your schedule.\n";
                pause();
                continue;
            }
            printApptHeader();
            for (auto &a : appts)
                a.print_row();

            int id;
            cout << "\n  Enter Appointment ID to complete: ";
            cin >> id;
            cin.ignore();

            if (sys.completeAppointmentDoctor(id))
                cout << "  Appointment marked as Completed.\n";
            else
                cout << "  Failed. Check the ID or appointment may not be Scheduled.\n";
            pause();
        }

    } while (choice != 0);
}

void adminMenu(HospitalSystem &sys)
{
    int choice;
    do
    {
        printHeader("ADMIN MENU");
        cout << "  1. View all users\n";
        cout << "  2. View all appointments\n";
        cout << "  3. Register a new doctor\n";
        cout << "  4. Register a new patient\n";
        cout << "  0. Logout\n";
        printLine();
        cout << "  Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1)
        {
            printHeader("ALL USERS");
            vector<User *> users = sys.adminViewAllUsers();
            cout << left
                 << setw(5)  << "ID"
                 << setw(25) << "Name"
                 << setw(30) << "Email"
                 << "Role\n";
            printLine(70, '-');
            for (auto u : users)
                cout << setw(5)  << u->get_id()
                     << setw(25) << u->get_name()
                     << setw(30) << u->get_email()
                     << u->get_role() << "\n";
            pause();
        }
        else if (choice == 2)
        {
            printHeader("ALL APPOINTMENTS");
            vector<Appointment> appts = sys.adminViewAllAppointments();
            if (appts.empty())
                cout << "  No appointments in the system yet.\n";
            else
            {
                printApptHeader();
                for (auto &a : appts)
                    a.print_row();
            }
            pause();
        }
        else if (choice == 3)
        {
            printHeader("REGISTER NEW DOCTOR");
            string name, email, spec;
            cout << "  Name           : ";
            getline(cin, name);
            cout << "  Email          : ";
            getline(cin, email);
            cout << "  Specialization : ";
            getline(cin, spec);

            sys.registerNewDoctor(name, email, spec);
            cout << "\n  Doctor registered successfully!\n"
                 << "  Default password: default\n";
            pause();
        }
        else if (choice == 4)
        {
            printHeader("REGISTER NEW PATIENT");
            string name, email, phone;
            cout << "  Name  : ";
            getline(cin, name);
            cout << "  Email : ";
            getline(cin, email);
            cout << "  Phone : ";
            getline(cin, phone);

            sys.registerNewPatient(name, email, phone);
            cout << "\n  Patient registered successfully!\n"
                 << "  Default password: default\n";
            pause();
        }

    } while (choice != 0);
}

int main()
{
    HospitalSystem sys;
    int choice;

    cout << "\n";
    printLine(55, '*');
    cout << "   HOSPITAL APPOINTMENT BOOKING SYSTEM\n";
    cout << "   Cairo University\n";
    printLine(55, '*');
    cout << "\n  Default Admin  ->  admin@mail.com  /  admin123\n";

    do
    {
        printHeader("MAIN MENU");
        cout << "  1. Login\n";
        cout << "  2. Register as Patient\n";
        cout << "  0. Exit\n";
        printLine();
        cout << "  Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1)
        {
            printHeader("LOGIN");
            string email, password;
            cout << "  Email    : ";
            getline(cin, email);
            cout << "  Password : ";
            getline(cin, password);

            if (sys.login(email, password))
            {
                string role = getRoleByEmail(sys, email);
                cout << "\n  Login successful!  Role: " << role << "\n";

                // Fixed: role strings match what constructors set
                // patient.cpp  → "Patient"
                // Doctor.cpp   → "doctor"
                // admin.cpp    → "Admin"
                if (role == "Patient")
                    patientMenu(sys);
                else if (role == "doctor")
                    doctorMenu(sys);
                else if (role == "Admin")
                    adminMenu(sys);
                else
                    cout << "  Unknown role. Logging out.\n";
            }
            else
            {
                cout << "\n  Invalid email or password.\n";
                pause();
            }
        }
        else if (choice == 2)
        {
            printHeader("PATIENT SELF-REGISTRATION");
            string name, email, phone;
            cout << "  Name  : ";
            getline(cin, name);
            cout << "  Email : ";
            getline(cin, email);
            cout << "  Phone : ";
            getline(cin, phone);

            sys.registerNewPatient(name, email, phone);
            cout << "\n  Registration successful!\n"
                 << "  You can now login with your email.\n"
                 << "  Default password: default\n";
            pause();
        }

    } while (choice != 0);

    cout << "\n  Thank you for using the Hospital Booking System. Goodbye!\n\n";
    return 0;
}
