struct AppointmentInfo {
    std::string patientname;
    std::string doctorname;
    std::string date;
    std::string time;
    std::string status;
};

class Appointment {
public:
    Appointment();
    Appointment(int a, int p, int d, const AppointmentInfo& info); // 4 params, all issues fixed

    void cancel();
    void complete();
    void print_row() const;

    int get_AppointmentId() const;
    int get_PatientId()     const;
    int get_DoctorId()      const;

    std::string get_Date()   const;
    std::string get_Time()   const;
    std::string get_Status() const;

private:
    int appointmentid, patientid, doctorid;
    std::string patientname, doctorname, date, time, status;
};
