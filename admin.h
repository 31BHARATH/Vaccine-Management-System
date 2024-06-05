#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>

class Admin
{
private:
    
    std::string username;
    std::string password;

public:
    Admin();
    bool login(const std::string &uname = "admin", const std::string &pwd = "password");
    void loginPage();
    bool displayMenu();
    void addHospital();
    void addVaccine();
    void approveVaccineRequest();
    bool isHospitalIDExists(const std::string& id);
    int idCheckInApproveRequest(const std::string &id);
    void rejectAll();
    void updateStockHospital(const std::string &id);
    void updateVaccineStock(const std::string &vaccineName, int reduction);
    void displayVaccineData();
    void displayVaccineRequestData();
};

#endif // ADMIN_H