#ifndef USER_H
#define USER_H

#include <string>

class User
{
    std::string name;
    int age;
    std::string dob;
    std::string address;
    std::string phone;
    std::string password;
    std::string mobile_number;

public:
    bool displayMenu();
    void registerUser();
    void bookVaccination();
    void getVaccinationStatus(const std::string &loggedInUser);
    void generateCertificate(const std::string &mobileNumber);;
    bool validateDOB(const std::string &dob);
    bool validatePhoneNumber(const std::string &phone);
    bool validateName(const std::string &name);
    bool validatePassword(const std::string &password);
    bool validateCredentials(const std::string &name, const std::string &password);
    bool loginUser();
    bool validatePincode(const std::string &pincode);
    void listHospitals(const std::string &pincode);
    std::string selectHospital();
    bool checkSuccessfulVaccination(const std::string &phoneNumber);
    bool checkPendingVaccination(const std::string &phoneNumber);
    bool hasDuplicateBooking(const std::string &phone, const std::string &dose);
    void chooseDoseAndVaccineType();
};
struct Hospital1
{
    std::string name;
    std::string id;
    std::string address;
    std::string pincode;
    std::string vaccineType;
    std::string HospitalId;

    int stock;
};
struct Vaccination
{
    std::string hospitalID;
    std::string patientName;
    std::string mobileNumber;
    std::string vaccineName;
    std::string dose;
    std::string date;
};
// void runApp();

#endif // USER_H