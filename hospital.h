#ifndef VACCINATION_MANAGEMENT_SYSTEM_H
#define VACCINATION_MANAGEMENT_SYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib> 
#include <vector>
#include <algorithm> 
#include <sstream> 
#include <regex>
#include<iomanip>


class Hospital {
private:
    std::string loggedInUsername;
    std::string loggedInPassword;
    std::string trim(const std::string& str);


    std::map<std::string, int> vaccineStock;

    std::map<std::string, std::string> userVaccinationBookings;

public:

    bool login(const std::string& username, const std::string& password);
    bool validateLogin(const std::string& username, const std::string& password);
    void registerHospital(const std::string& username, const std::string& password);
    void viewCurrentStock();
    void viewUserVaccinationBookings();
    void processVaccinationRequests();
    void requestVaccineStockFromAdmin();
    void viewCurrentVaccinationsDoneDetails();
};

class VaccinationManagementSystem {
public:
    void run();
};

#endif 