#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <regex>
#include <cctype>
#include <vector>
#include <iomanip>

#include "user.h"
//-----------------------------------------------------------------------------------------------------------------------

bool User::validateDOB(const std::string &dob)
{
    std::regex pattern("^\\d{2}/\\d{2}/\\d{4}$");
    if (!std::regex_match(dob, pattern))
    {
       return false;
    }

    std::time_t now = std::time(nullptr);
    std::tm *tm_now = std::localtime(&now);
    std::tm tm_dob = {};
    std::istringstream ss(dob);
    int day, month, year;
    char slash;
    ss >> day >> slash >> month >> slash >> year;

    if (ss.fail())
    {
        return false;
    }

    if (year > 2024)
    {
        return false;
    }

    tm_dob.tm_year = year - 1900; 
    tm_dob.tm_mon = month - 1;  
    tm_dob.tm_mday = day;

    if (std::mktime(&tm_dob) > std::mktime(tm_now))
    {
    
        return false;
    }

    return true;
}
bool User::validatePhoneNumber(const std::string &phone)
{
    std::regex pattern("^\\d{10}$");
    return std::regex_match(phone, pattern);
}
bool User::validateName(const std::string &name)
{
    for (char c : name)
    {
        if (!std::isalpha(c))
        {
            return false;
        }
    }
    return true;
}
bool User::validatePassword(const std::string &password)
{
    return password.length() == 8;
}

void User::registerUser()
{
    User user;
    do
    {
        std::cout << "Enter name: ";
        std::getline(std::cin >> std::ws, user.name);
        if (!validateName(user.name))
        {
            std::cout << "Invalid name. Name must contain only alphabetic characters." << std::endl;
        }
    } while (!validateName(user.name));

    std::string dob;
    do
    {
        std::cout << "Enter date of birth (DD/MM/YYYY): ";
        std::cin >> dob;
        if (!validateDOB(dob))
        {
            std::cout << "Invalid date format, date is after today's date, or year is greater than 2024. Please enter in DD/MM/YYYY format." << std::endl;
        }
    } while (!validateDOB(dob));
    user.dob = dob;
    std::istringstream dobStream(dob);
    int day, month, year;
    char slash;
    dobStream >> day >> slash >> month >> slash >> year;
    std::time_t now = std::time(nullptr);
    std::tm tm_now = *std::localtime(&now);
    user.age = tm_now.tm_year + 1900 - year - (tm_now.tm_mon < month || (tm_now.tm_mon == month && tm_now.tm_mday < day));

    std::cout << "Enter address: ";
    std::getline(std::cin >> std::ws, user.address);

    std::string phone;
    do
    {
        std::cout << "Enter phone number (exactly 10 digits): ";
        std::cin >> phone;
        if (!validatePhoneNumber(phone))
        {
            std::cout << "Invalid phone number format. Please enter exactly 10 digits." << std::endl;
        }
    } while (!validatePhoneNumber(phone));
    user.phone = phone;

    do
    {
        std::cout << "Enter password (exactly 8 characters): ";
        std::cin >> user.password;
        if (!validatePassword(user.password))
        {
            std::cout << "Invalid password. Password must be exactly 8 characters long." << std::endl;
        }
    } while (!validatePassword(user.password));

    std::ofstream outputFile("user.txt", std::ios::app); 
    if (outputFile.is_open())
    {
        outputFile << user.name << " " << user.age << " " << user.dob << " " << user.address << " " << user.phone << " " << user.password << std::endl;
        outputFile.close();
        std::cout << "User registered successfully!" << std::endl;
    }
    else
    {
        std::cout<< "Unable to open file." << std::endl;
    }
}
//-------------------------------------------------------------------------------------------------------------------------
bool User::validateCredentials(const std::string &name, const std::string &password)
{
    std::ifstream inputFile("user.txt");
    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            std::istringstream iss(line);
            std::string token;
            std::getline(iss, token, ' '); 
            if (token == name)
            {
                std::getline(iss, token, ' ');
                std::getline(iss, token, ' '); 
                std::getline(iss, token, ' '); 
                std::getline(iss, token, ' ');
                std::getline(iss, token, ' '); 
                if (token == password)
                {
                    inputFile.close();
                    return true;
                }
            }
        }
        inputFile.close();
    }
    return false;
}

bool User::loginUser()
{
    std::string name, password;
    std::cout << "Enter name: ";
    std::cin >> name;
    std::cout << "Enter password: ";
    std::cin >> password;
    if (validateCredentials(name, password))
    {
        std::cout << "Login successful!" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Login failed. Invalid name or password." << std::endl;
        return false;
    }
}
//-----------------------------------------------------------------------------------------------------------------
bool User::validatePincode(const std::string &pincode)
{

    std::regex pattern("^\\d{6}$");
    return std::regex_match(pincode, pattern);
}

void User::listHospitals(const std::string &pincode)
{
    std::ifstream inputFile("hospitals.txt");
    if (inputFile.is_open())
    {
        std::string line;
        std::vector<Hospital1> hospitals;
        while (std::getline(inputFile, line))
        {
            std::istringstream iss(line);
            Hospital1 hospital;
            std::getline(iss, hospital.name, ',');        
            std::getline(iss, hospital.id, ',');          
            std::getline(iss, hospital.address, ',');     
            std::getline(iss, hospital.pincode, ',');     
            std::getline(iss, hospital.vaccineType, ','); 
            iss >> hospital.stock;                       
            iss.ignore();                                
            if (hospital.pincode == pincode)
            {
                hospitals.push_back(hospital);
            }
        }
        inputFile.close();

        if (!hospitals.empty())
        {
            std::cout << "Hospitals with pincode " << pincode << ":" << std::endl;
            for (size_t i = 0; i < hospitals.size(); ++i)
            {
                std::cout << i + 1 << ". " << hospitals[i].name << " (" << hospitals[i].id << "): " << hospitals[i].address << std::endl;
            }
        }
        else
        {
            std::cout << "No hospitals found with the given pincode." << std::endl;
        }
    }
    else
    {
        std::cout<< "Unable to open file." << std::endl;
    }
}

std::string User::selectHospital()
{
    std::string pincode;
    do
    {
        std::cout << "Enter pincode (exactly 6 digits): ";
        std::cin >> pincode;
        if (!validatePincode(pincode))
        {
            std::cout << "Invalid pincode format. Please enter exactly 6 digits." << std::endl;
        }
    } while (!validatePincode(pincode));

    listHospitals(pincode);

    int choice;
    std::cout << "Enter the number corresponding to the hospital you wish to choose: ";
    std::cin >> choice;
    choice--;
    std::ifstream inputFile("hospitals.txt");
    if (inputFile.is_open())
    {
        std::string line;
        int count = 0;
        while (std::getline(inputFile, line))
        {
            if (count == choice)
            {
                std::istringstream iss(line);
                Hospital1 hospital;
                std::getline(iss, hospital.name, ','); 
                std::getline(iss, hospital.id, ',');   
                inputFile.close();
                return hospital.name + " (" + hospital.id + ")";
            }
            ++count;
        }
        inputFile.close();
    }
    else
    {
        std::cout<< "Unable to open file." << std::endl;
    }

    return "";
}
void User::chooseDoseAndVaccineType()
{
    std::string dose;
    do
    {
        std::cout << "Choose dose (1 or 2): ";
        std::cin >> dose;
    } while (dose != "1" && dose != "2");

    // If dose 1 is selected
    if (dose == "1")
    {
        std::string phone;
        std::cout << "Enter your phone number: ";
        std::cin >> phone;
        if (hasDuplicateBooking(phone, "1")) {
            std::cout << "Duplicate booking found. Booking cannot be made." << std::endl;
            // return;
        }
        std::string hospitalId, patientName, phoneNumber, vaccineName, doseType, dateOfVaccination;
        std::cout << "Enter hospital ID: ";
        std::cin >> hospitalId;
        std::cout << "Enter Patient name: ";
        std::cin >> patientName;
        std::cout << "Enter Patient phoneNumber: ";
        std::cin >> phoneNumber;
        std::cout << "Enter vaccine name: ";
        std::cin >> vaccineName;
        std::cout << "Enter dose type: ";
        std::cin >> doseType;
        std::cout << "Enter date of vaccination (DD/MM/YYYY): ";
        std::cin >> dateOfVaccination;

        // Store dose 1 information in booking.txt file
        std::ofstream outputFile("booking.txt", std::ios::app);
        if (outputFile.is_open())
        {
            outputFile << hospitalId << " " << patientName << " " << phoneNumber << " " << vaccineName << " " << doseType << " " << dateOfVaccination << std::endl;
            outputFile.close();
            std::cout << "Booking for dose 1 successful!" << std::endl;
        }
        else
        {
            std::cerr << "Unable to open file." << std::endl;
        }
    }
    // If dose 2 is selected
    else if (dose == "2") {
        std::string chosenHospitalID;
        std::cout << "Enter the hospital ID for which you want to book Dose 2: ";
        std::cin >> chosenHospitalID;
        std::string phone;
        std::cout << "Enter your phone number: ";
        std::cin >> phone;
        if (hasDuplicateBooking(phone, "2")) {
            std::cout << "Duplicate booking found. Booking cannot be made." << std::endl;
            return;
        }
        
        std::ifstream inputFile("booking.txt");
        bool dose1Found = false;
        std::string dose1Line;
        if (inputFile.is_open()) {
            while (std::getline(inputFile, dose1Line)) {
                std::istringstream iss(dose1Line);
                std::string hospitalID, patientName, phoneNumber, vaccineName, doseType, dateOfVaccination;
                iss >> hospitalID >> patientName >> phoneNumber >> vaccineName >> doseType >> dateOfVaccination;
                if (hospitalID == chosenHospitalID && doseType == "1") {
                    dose1Found = true;
                    break;
                }
            }
            inputFile.close();
            if (dose1Found) {
                std::string vaccineType;
                std::istringstream iss(dose1Line);
                std::string hospitalID, patientName, phoneNumber, vaccineName, doseType, dateOfVaccination;
                iss >> hospitalID >> patientName >> phoneNumber >> vaccineType >> doseType >> dateOfVaccination;
                
                std::tm tm_dateOfVaccination = {};
                std::istringstream(dateOfVaccination) >> std::get_time(&tm_dateOfVaccination, "%d/%m/%Y");
                tm_dateOfVaccination.tm_mday += 60;
                std::mktime(&tm_dateOfVaccination);
                std::ostringstream oss;
                oss << std::put_time(&tm_dateOfVaccination, "%d/%m/%Y");
                std::string dateOfVaccinationDose2 = oss.str();
                
                std::ofstream outputFile("booking.txt", std::ios::app);
                if (outputFile.is_open()) {
                    outputFile << chosenHospitalID << " " << patientName << " " << phoneNumber << " " << vaccineType << " 2 " << dateOfVaccinationDose2 << std::endl;
                    outputFile.close();
                    std::cout << "Booking for dose 2 successful!" << std::endl;
                } else {
                    std::cout << "Unable to open file." << std::endl;
                }
            } else {
                std::cout << "Booking details for Dose 1 not found for the specified hospital ID." << std::endl;
            }
        } else {
            std::cout << "Unable to open file." << std::endl;
        }
    }
}

bool User::hasDuplicateBooking(const std::string &phone, const std::string &dose)
{
    std::ifstream inputFile("booking.txt");
    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            std::istringstream iss(line);
            std::string hospitalID, patientName, phoneNumber, vaccineType, bookedDose, dateOfVaccination;
            iss >> hospitalID >> patientName >> phoneNumber >> vaccineType >> bookedDose >> dateOfVaccination;

            if (phoneNumber == phone && bookedDose == dose)
            {
                inputFile.close();
                return true;
            }
        }
        inputFile.close();
    }
    return false; 
}

//-------------------------------------------------------------------------------------------------------------------
void User::bookVaccination()
{
    std::string hospitalChoice = selectHospital();
    std::cout << "Hospital selected: " << hospitalChoice << std::endl;

    std::string dose;
    User user;
    do
    {
        std::cout << "Choose dose (1 or 2): ";
        std::cin >> dose;
        if (hasDuplicateBooking(user.phone, dose))
        {
            std::cout << "You have already booked a vaccination for dose " << dose << ". Please select a different dose." << std::endl;
        }
    } while (hasDuplicateBooking(user.phone, dose));
    std::ifstream inputFile("hospitals.txt");
    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            std::istringstream iss(line);
            Hospital1 hospital;
            std::getline(iss, hospital.name, ',');        
            std::getline(iss, hospital.id, ',');          
            std::getline(iss, hospital.address, ',');     
            std::getline(iss, hospital.pincode, ',');     
            std::getline(iss, hospital.vaccineType, ','); 
            iss >> hospital.stock;                        
            iss.ignore();                                 

            if (hospital.name + " (" + hospital.id + ")" == hospitalChoice)
            {
    
                if (hospital.stock > 0)
                {
                    std::cout << "Stock available! Proceeding with booking..." << std::endl;
            
                    hospital.stock--;
                    break; 
                }
                else
                {
                    std::cout << "Booking not possible due to insufficient stock." << std::endl;
                    return; 
                }
            }
        }
        inputFile.close();
    }
    else
    {
        std::cout<< "Unable to open file." << std::endl;
        return; 
    }
    chooseDoseAndVaccineType();

    std::cout << "Vaccination booked successfully!" << std::endl;
}

//----------------------------------------------------------------------------------------------------------------------------
struct BookingInfo
{
    std::string hospitalId;
    std::string patientName;
    std::string phoneNumber;
    std::string vaccineName;
    int dose;
    std::string date;
};

void User::getVaccinationStatus(const std::string &loggedInUser)
{
    std::string phoneNumber;
    std::cout << "Enter patient's phone number: ";
    std::cin >> phoneNumber;
    bool pending = checkPendingVaccination(phoneNumber);

    if (pending)
    {
        std::cout << "Vaccination pending." << std::endl;
        return;
    }
    bool successful = checkSuccessfulVaccination(phoneNumber);

    if (successful)
    {
        std::cout << "Vaccination successful." << std::endl;
    }
    else
    {
        std::cout << "Vaccination denied." << std::endl;
    }
}

bool User::checkPendingVaccination(const std::string &phoneNumber)
{
    std::ifstream bookingFile("booking.txt");
    if (!bookingFile.is_open())
    {
        std::cout<< "Unable to open Booking.txt." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(bookingFile, line))
    {
        std::istringstream iss(line);
        BookingInfo bookingInfo;
        iss >> bookingInfo.hospitalId >> bookingInfo.patientName >> bookingInfo.phoneNumber >> bookingInfo.vaccineName >> bookingInfo.dose >> bookingInfo.date;
        if (bookingInfo.phoneNumber == phoneNumber && bookingInfo.dose == 1)
        {
            bookingFile.close();
            return true;
        }
    }

    bookingFile.close();
    return false; 
}

bool User::checkSuccessfulVaccination(const std::string &phoneNumber)
{
    std::ifstream approvedVaccineFile("approvedvaccine.txt");
    if (!approvedVaccineFile.is_open())
    {
        std::cout<< "Unable to open approvedvaccine.txt." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(approvedVaccineFile, line))
    {
        std::istringstream iss(line);
        std::string hospitalId, patientName, phoneNumberFromFile, vaccineName;
        int dose;
        std::string date;
        iss >> hospitalId >> patientName >> phoneNumberFromFile >> vaccineName >> dose >> date;
        if (phoneNumberFromFile == phoneNumber && dose == 1)
        {
            approvedVaccineFile.close();
            return true; 
        }
    }

    approvedVaccineFile.close();
    return false; 
}


//------------------------------------------------------------------------------------------------------------------------
void User::generateCertificate(const std::string &mobileNumber)
{
    std::string mobile_Number;
    std::cout << "Enter mobile number: ";
    std::cin >> mobile_Number;
    std::ifstream approvedFile("approvedvaccine.txt");
    bool found = false;

    std::string hospitalID, patientName, vaccineName, dose, date;

    if (approvedFile.is_open())
{
    Vaccination approvedData;
    while (approvedFile >> approvedData.hospitalID >> approvedData.patientName >> approvedData.mobileNumber >> approvedData.vaccineName >> approvedData.dose >> approvedData.date)
    {
        if (approvedData.mobileNumber == mobile_Number)
        {
            found = true;
            hospitalID = approvedData.hospitalID;
            patientName = approvedData.patientName;
            vaccineName = approvedData.vaccineName;
            dose = approvedData.dose;
            date = approvedData.date;

            std::cout << "Certificate\n"
                      << std::endl;
            std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ " << std::endl;
            std::cout << "Name: " << patientName << std::endl;
            std::cout << "Vaccine Type: " << vaccineName << std::endl;
            std::cout << "Dose Number: " << dose << std::endl;
            std::cout << "Dose Date: " << date << std::endl;
            std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ " << std::endl;
        }
    }
    approvedFile.close();

    if (!found)
    {
        std::cout << "No vaccination record found for the provided phone number." << std::endl;
        return;
    }
}
else
{
    std::cout << "Unable to open approved vaccine file." << std::endl;
    return;
}
}

//----------------------------------------------------------------------------------------------------------------------------
bool User::displayMenu()
{
    int choice;
    std::string loggedInUser;
    bool loggedIn = false;
    do
    {
        if (!loggedIn)
        {
            std::cout << "1. Register User\n2. Login\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                registerUser();
                break;
            case 2:
                loggedIn = loginUser();
                break;
            default:
                std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
                break;
            }
        }
        else
        {
            int innerChoice;
            std::string mobile_number;
            std::cout << "1. Book Vaccination\n2. Check Vaccination Status\n3. Display Certificate\n4. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> innerChoice;

            switch (innerChoice)
            {
            case 1:
                bookVaccination();
                break;
            case 2:
                getVaccinationStatus(loggedInUser);
                break;
            case 3:

                generateCertificate(mobile_number);
                break;
            case 4:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 4." << std::endl;
                break;
            }
            if (innerChoice == 4)
            {
                break;
            }
        }
    } while (true); 
    return 0;
}
// int main() {
//     runApp(); // Call the function to run the program
//     return 0;
// }