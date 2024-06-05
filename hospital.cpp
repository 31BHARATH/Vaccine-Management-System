#include "hospital.h"


bool Hospital::login(const std::string& username, const std::string& password) {
    std::string inputUsername = username; 
    std::string inputPassword = password;
    
    while (true) {
        if (validateLogin(inputUsername, inputPassword)) {
            loggedInUsername = inputUsername;
            loggedInPassword = inputPassword;
            std::cout << "Login successful. Welcome, " << inputUsername << "!" << std::endl;
            return true;
        } else {
            std::cout << "Invalid username or password. Please try again." << std::endl;
            std::cout << "Enter hospital ID: ";
            std::cin >> inputUsername; 
            std::cout << "Enter password: ";
            std::cin >> inputPassword; 
        }
    }
}

bool Hospital::validateLogin(const std::string& username, const std::string& password) {

    std::ifstream inFile("hospital.txt");
    if (inFile.is_open()) {
        std::string storedUsername, storedPassword;
        while (inFile >> storedUsername >> storedPassword) {
            if (username == storedUsername && password == storedPassword) {
                if (std::regex_match(username, std::regex("^[a-zA-Z0-9#]+$")) &&
                    std::regex_match(password, std::regex("^[\\w!@#$%^&*()]+$"))) {
                    inFile.close();
                    return true;
                }
            }
        }
        inFile.close();
    }
    return false;
}

void Hospital::registerHospital(const std::string& username, const std::string& password) {
    std::regex hospitalIDRegex("^H#[0-9]{4}$");
    if (!std::regex_match(username, hospitalIDRegex)) {
        std::cout << "Invalid hospital ID format. Hospital ID should be in the format 'H#xxxx' where 'x' represents digits." << std::endl;
        return;
    }

    std::ifstream inFile("hospitals.txt");
    if (!inFile.is_open()) {
        std::cerr << "Unable to open file: hospitals.txt" << std::endl;
        return;
    }

    std::string line;
    bool hospitalIDExists = false;

    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string hospitalName, hospitalID, address, pincode;

        std::getline(iss, hospitalName, ',');
        std::getline(iss, hospitalID, ','); 
        std::getline(iss, address, ',');
        std::getline(iss, pincode, ',');

        if (hospitalID == username) {
            hospitalIDExists = true;
            break;
        }
    }

    inFile.close();

    if (hospitalIDExists) {
        std::ofstream outFile("hospital.txt", std::ios::app);
        if (outFile.is_open()) {
            outFile << username << " " << password << std::endl;
            std::cout << "Hospital registered successfully." << std::endl;
            outFile.close();
        } else {
            std::cerr << "Unable to open file: hospital.txt" << std::endl;
        }
    } else {
        std::cout << "Hospital ID doesn't exist in the system. Please contact the admin." << std::endl;
    }
}


void Hospital::viewCurrentStock() {
    std::ifstream inFile("hospitals.txt");

    if (!inFile) {
        std::cerr << "Unable to open file: hospitals.txt" << std::endl;
        return;
    }

    std::string line;
    bool dataFound = false;

    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string hospitalName, hospitalID, hospitalAddress, pincode;
        std::getline(iss, hospitalName, ',');      
        std::getline(iss, hospitalID, ',');        
        std::getline(iss, hospitalAddress, ',');   
        std::getline(iss, pincode, ',');           

        if (hospitalID == loggedInUsername) {
            std::cout << "Vaccine Stock Information for Hospital " << hospitalID << ":" << std::endl;

            std::string vaccineType;
            int stock;

            while (std::getline(iss, vaccineType, ',') && std::getline(iss, line, ',')) {
                stock = std::stoi(line);  
                std::cout << vaccineType << "    : " << stock << std::endl;
            }

            dataFound = true;
            break;
        }
    }

    if (!dataFound) {
        std::cout << "No vaccine stock information found for the logged-in hospital." << std::endl;
    }

    inFile.close();

}

void Hospital::viewUserVaccinationBookings() {
    std::ifstream inFile("booking.txt");

    if (!inFile) {
        std::cerr << "Unable to open file: booking.txt" << std::endl;
        return;
    }

    std::string hospitalId, patientName, phoneNumber, vaccineName, vaccineDose, dateOfVaccine;
    int bookingNumber = 1;
    bool dataFound = false;

    while (inFile >> hospitalId >> patientName >> phoneNumber >> vaccineName >> vaccineDose >> dateOfVaccine) {
         if (hospitalId == loggedInUsername) {
            std::cout << "Booking " << bookingNumber << ":" << std::endl;
            std::cout << "Hospital Id        : " << hospitalId << std::endl;
            std::cout << "Patient name       : " << patientName << std::endl;
            std::cout << "Phone number       : " << phoneNumber << std::endl;
            std::cout << "Vaccine name       : " << vaccineName << std::endl;
            std::cout << "Dose               : " << vaccineDose << std::endl;
            std::cout << "Date of vaccination: " << dateOfVaccine << std::endl;
            std::cout << std::endl;
            dataFound = true;
            bookingNumber++;
        }
    }

    if (!dataFound) {
        std::cout << "No vaccination booking information found for the logged-in hospital." << std::endl;
    }

    inFile.close();
}
void Hospital::processVaccinationRequests() {
    std::ifstream inFile("booking.txt");
    if (!inFile.is_open()) {
        std::cerr << "Unable to open file: booking.txt" << std::endl;
        return;
    }

    std::cout << "Vaccination Requests:" << std::endl;
    int requestNumber = 1;
    std::map<int, std::string> requestDetails; 
    std::string line;
    bool bookingsAvailable = false;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string hospitalID;
        iss >> hospitalID;
        if (hospitalID == loggedInUsername) {
            std::cout << requestNumber << ". " << line << std::endl;
            requestDetails[requestNumber] = line;
            requestNumber++;
            bookingsAvailable = true;
        }
    }
    inFile.close();

    if (!bookingsAvailable) {
        std::cout << "No vaccination bookings available." << std::endl;
        return;
    }

    std::cout << "Enter the numbers of requests to approve (comma-separated), '0' to approve all, 'D' to deny all, or 'E' to exit: ";
    std::string approvalInput;
    std::getline(std::cin >> std::ws, approvalInput);

    if (approvalInput == "D" || approvalInput == "d") {
        std::cout << "All requests denied." << std::endl;
        return;
    } else if (approvalInput == "E" || approvalInput == "e") {
        std::cout << "Exiting process." << std::endl;
        return;
    }

    std::vector<int> approvalNumbers;
    if (approvalInput == "0") {
        for (int i = 1; i < requestNumber; ++i) {
            approvalNumbers.push_back(i);
        }
    } else {
        std::istringstream approvalStream(approvalInput);
        std::string approvalNumberStr;
        while (std::getline(approvalStream, approvalNumberStr, ',')) {
            approvalNumbers.push_back(std::stoi(approvalNumberStr));
        }
    }

    std::ofstream approvedFile("approvedvaccine.txt", std::ios::app);
    if (!approvedFile.is_open()) {
        std::cerr << "Unable to open file: approvedvaccine.txt" << std::endl;
        return;
    }

    for (int i : approvalNumbers) {
        if (i > 0 && i <= requestDetails.size()) {
            auto it = requestDetails.find(i);
            if (it != requestDetails.end()) {
                approvedFile << it->second << std::endl;
            }
        }
    }
    approvedFile.close();

    std::ofstream tempFile("temp.txt");
    if (!tempFile.is_open()) {
        std::cerr << "Unable to open file: temp.txt" << std::endl;
        return;
    }

    std::ifstream inFileAgain("booking.txt");
    if (!inFileAgain.is_open()) {
        std::cerr << "Unable to open file: booking.txt" << std::endl;
        return;
    }
    while (std::getline(inFileAgain, line)) {
        std::istringstream iss(line);
        std::string hospitalID;
        iss >> hospitalID;
        if (hospitalID != loggedInUsername) {
            tempFile << line << std::endl;
        }
    }
    inFileAgain.close();
    tempFile.close();

    if (std::remove("booking.txt") != 0) {
        std::cerr << "Error deleting file: booking.txt" << std::endl;
        return;
    }
    if (std::rename("temp.txt", "booking.txt") != 0) {
        std::cerr << "Error renaming file: temp.txt to booking.txt" << std::endl;
        return;
    }
}



void Hospital::requestVaccineStockFromAdmin() {
    std::ifstream vaccineFile("vaccines.txt");
    if (!vaccineFile.is_open()) {
        std::cerr << "Unable to open file: vaccines.txt" << std::endl;
        return;
    }

    std::cout << "Available vaccines and their quantities:" << std::endl;
    std::cout << "NO.\tVaccine Name\t\tAvailable stock" << std::endl;
    std::string vaccineName;
    int stock, count = 1;
    while (vaccineFile >> vaccineName >> stock) {
        std::cout << count << "\t" << vaccineName << "\t\t\t" << stock << std::endl;
        count++;
    }
    vaccineFile.close();
    std::cout << std::endl;

    std::string requestedVaccine;
    int numVaccines;

    while (true) {
      
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Enter the name of the vaccine you need to request (or type 'exit' to stop): ";
        std::getline(std::cin, requestedVaccine);

        if (requestedVaccine == "exit") {
            break; 
        }

        if (requestedVaccine.empty()) {
            std::cout << "Invalid vaccine name. Please enter a valid name." << std::endl;
            continue;
        }

        std::ifstream vaccineFile("vaccines.txt");
        if (!vaccineFile.is_open()) {
            std::cerr << "Unable to open file: vaccines.txt" << std::endl;
            continue;
        }

        bool validVaccine = false;
        while (vaccineFile >> vaccineName >> stock) {
            if (vaccineName == requestedVaccine) {
                validVaccine = true;
                break;
            }
        }
        vaccineFile.close();

        if (!validVaccine) {
            std::cout << "Invalid vaccine name. Please choose a name from the above list" << std::endl;
            continue;
        }

        std::cout << "Enter the number of vaccines required for " << requestedVaccine << ": ";
        std::cin >> numVaccines;

        if (numVaccines <= 0 || numVaccines > stock) {
            std::cout << "Invalid number of vaccines. Please enter a positive number less than or equal to the available stock." << std::endl;
            continue; 
        }

        std::ofstream outFile("vaccinerequest.txt", std::ios::app);
        if (outFile.is_open()) {
            outFile << loggedInUsername << " " << requestedVaccine << " " << numVaccines << std::endl;
            std::cout << "Request for " << numVaccines << " doses of " << requestedVaccine << " sent to admin." << std::endl;
            outFile.close();
        } else {
            std::cerr << "Unable to open file: vaccinerequest.txt" << std::endl;
        }
    }
}
void Hospital::viewCurrentVaccinationsDoneDetails() {

        std::ifstream inFile("approvedvaccine.txt");
        if (!inFile.is_open()) {
            std::cerr << "Unable to open file: approvedvaccine.txt" << std::endl;
            return;
        }

        std::cout << "Current Vaccinations Done Details:" << std::endl;
        std::string line;
        int patientCount = 1; 
        bool dataFound = false; 
        while (std::getline(inFile, line)) {
            std::istringstream iss(line);
            std::string hospitalID, patientName, phoneNumber, vaccineName, doseNumber, dateOfAdministration;
            iss >> hospitalID >> patientName >> phoneNumber >> vaccineName >> doseNumber >> dateOfAdministration;

           
            if (hospitalID == loggedInUsername) {
                std::cout << "Patient " << patientCount << ":" << std::endl;
                std::cout << "Hospital Code: " << hospitalID << std::endl;
                std::cout << "Patient Name: " << patientName << std::endl;
                std::cout << "Phone Number: " << phoneNumber << std::endl;
                std::cout << "Vaccine Administered: " << vaccineName << std::endl;
                std::cout << "Dose: " << doseNumber << std::endl;
                std::cout << "Date of Administration: " << dateOfAdministration << std::endl;
                std::cout << std::endl; 
                patientCount++; 
                dataFound = true;
            }
        }
        inFile.close();

        if (!dataFound) {
            std::cout << "No vaccination details found for the logged-in hospital." << std::endl;
        }
}



void VaccinationManagementSystem::run() {
    Hospital hospital;
    bool exitHospitalMenu = false; 

    while (!exitHospitalMenu) { 
        std::string choice;
        std::cout << "Register or login? (R/L): ";
        std::cin >> choice;

        std::regex choiceRegex("^[RrLl]\\s*$");

        if (!std::regex_match(choice, choiceRegex)) {
            std::cout << "Invalid choice. Please enter 'R' or 'L'." << std::endl;
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            continue; 
        }

        char choiceChar = std::tolower(choice[0]);

        if (choiceChar == 'r') {
            std::string newUsername, newPassword;
            std::cout << "Enter new hospital ID: ";
            std::cin >> newUsername;
            std::cout << "Enter new password: ";
            std::cin >> newPassword;
            hospital.registerHospital(newUsername, newPassword);
        } else if (choiceChar == 'l') {
            std::string username, password;
            std::cout << "Enter hospital ID: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;
            if (hospital.login(username, password)) {
                std::cout << "Welcome to the vaccination management system!" << std::endl;

                while (true) {
                    std::cout << "1. View current stock of vaccines\n";
                    std::cout << "2. View current user vaccination bookings\n";
                    std::cout << "3. Process vaccination requests\n";
                    std::cout << "4. Request vaccines stock from Admin\n";
                    std::cout << "5. View current vaccinations done details\n";
                    std::cout << "6. Exit\n";
                    std::cout << "Enter your choice: ";
                    std::string input;
                    std::cin >> input;

                    std::regex inputRegex("^[1-6]$");

                    if (!std::regex_match(input, inputRegex)) {
                        std::cout << "Invalid input. Please enter a single digit between 1 and 6." << std::endl;
                        std::cin.clear(); 
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue; 
                    }

                    int option = std::stoi(input);

                    switch (option) {
                        case 1:
                            hospital.viewCurrentStock();
                            break;
                        case 2:
                            hospital.viewUserVaccinationBookings();
                            break;
                        case 3:
                            hospital.processVaccinationRequests();
                            break;
                        case 4:
                            hospital.requestVaccineStockFromAdmin();
                            break;
                        case 5:
                            hospital.viewCurrentVaccinationsDoneDetails();
                            break;
                        case 6:
                            exitHospitalMenu = true; 
                            break;
                        default:
                            std::cout << "Invalid option. Please try again." << std::endl;
                            break;
                    }

                    if (exitHospitalMenu) {
                        break; 
                    }
                }
            }
        }
    }
}

// int main() {
//     VaccinationManagementSystem system;
//     system.run();
//     return 0;
// }