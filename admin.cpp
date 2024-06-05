#include "admin.h"

Admin::Admin() : username("admin"), password("password") {}

bool Admin::login(const std::string &uname, const std::string &pwd)
{
    if (uname != "admin" || pwd != "password")
    {
        std::cout << "Invalid username or password. Please try again.\n";
        return false;
    }
    else
    {
        return true;
    }
}

bool isValidUsername(const std::string &uname)
{
    std::regex unameRegex("^[a-zA-Z]+$");
    return std::regex_match(uname, unameRegex);
}

bool isValidPassword(const std::string &pwd)
{
    std::regex passwordRegex("^[a-zA-Z]+$");
    return std::regex_match(pwd, passwordRegex);
}

void Admin::loginPage()
{
    std::string uname, pwd;
    std::cout << "Admin Login\n";

    while (true)
    {
        std::cout << "UserName: ";
        std::getline(std::cin, uname);
        if (!isValidUsername(uname))
        {
            std::cout << "Invalid username! username should contain only letters.\n";
        }
        else
        {
            username = uname;

            break;
        }
    }

    while (true)
    {
        std::cout << "Password: ";
        std::getline(std::cin, pwd);
        if (!isValidPassword(pwd))
        {
            std::cout << "Invalid password! password should contain only letters.\n";
        }
        else
        {
            password = pwd;
            break;
        }
    }

    if (login(username, password))
    {
        std::cout << "Login successful!\n";
    }
    else
    {
        username.clear();
        password.clear();
        loginPage();
    }
}

bool Admin::displayMenu()
{
    char choice;
    std::string input;
    do
    {
        std::cout << "\nAdmin Menu:\n";
        std::cout << "1. Add Hospital\n";
        std::cout << "2. Add Vaccine\n";
        std::cout << "3. Approve Vaccine Request\n";
        std::cout << "4. Exit\n";
        std::cout << "Your choice: ";

        std::getline(std::cin, input);

        if (input.length() == 1 && isdigit(input[0]))
        {
            choice = input[0];
            switch (choice)
            {
            case '1':
                addHospital();
                break;
            case '2':
                addVaccine();
                break;
            case '3':
                approveVaccineRequest();
            case '4':
                std::cout << "Exiting Admin Page..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
        else
        {
            std::cout << "Invalid input. Please enter a number between 1 and 4." << std::endl;
        }
    } while (choice != '4');

    return 0;
}

bool isValidName(const std::string &name)
{
    std::regex nameRegex("^[a-zA-Z]+$");
    return std::regex_match(name, nameRegex);
}

bool isValidHospitalID(const std::string &id)
{
    std::regex idRegex("^H#[0-9]{4}$");
    return std::regex_match(id, idRegex);
}

bool Admin::isHospitalIDExists(const std::string &id)
{
    std::ifstream file("hospitals.txt");
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open hospitals file for reading." << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string hospitalID;
        if (std::getline(iss, hospitalID, ','))
        {
            if (std::getline(iss, hospitalID, ','))
            {
                if (hospitalID == id)
                {
                    file.close();
                    return true;
                }
            }
        }
    }
    file.close();
    return false;
}

bool isValidAddress(const std::string &address)
{
    std::regex addressRegex("^[a-zA-Z]+$");
    return std::regex_match(address, addressRegex);
}

bool isStockAvailable(const std::string &vaccineName, int requestedStock)
{
    std::ifstream inFile("vaccines.txt");
    if (!inFile)
    {
        std::cerr << "Error: Unable to open vaccines file for reading." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(inFile, line))
    {
        std::istringstream iss(line);
        std::string name;
        int stock;
        if (iss >> name >> stock)
        {
            if (name == vaccineName)
            {
                if (stock >= requestedStock)
                {
                    inFile.close();
                    return true;
                }
                else
                {
                    std::cout << "Insufficient stock for " << vaccineName << ". Available stock: " << stock << std::endl;
                    inFile.close();
                    return false;
                }
            }
        }
    }

    std::cout << "Vaccine " << vaccineName << " not found." << std::endl;
    inFile.close();
    return false;
}

void Admin::displayVaccineData()
{
    std::ifstream infile("vaccines.txt");
    std::string line;
    std::cout << "Available vaccines in the stock:\n";
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::string vaccineName;
        int stock;
        if (!(iss >> vaccineName >> stock))
        {
            break;
        }
        std::cout << "Vaccine: " << vaccineName << ", Stock: " << stock << "\n";
    }
}

void Admin::addHospital()
{
    std::string name, id, address, pincode, choice1, choice2, vaccineName, stock;
    std::ofstream outFile("hospitals.txt", std::fstream::app);
    if (outFile.fail() == true)
    {
        std::cout << "File Failed to Open\n";
        return;
    }
    std::cout << "Enter Hospital Details: \n";

    while (true)
    {
        std::cout << "Name: ";
        std::getline(std::cin, name);
        if (!isValidName(name))
        {
            std::cout << "Invalid name! Name should contain only letters.\n";
        }
        else
        {
            break;
        }
    }

    while (true)
    {
        std::cout << "Enter unique hospital ID starting with 'H#', followed by 4 digits: ";
        std::getline(std::cin, id);
        if (!isValidHospitalID(id))
        {
            std::cerr << "Error: Invalid hospital ID format! Hospital ID must start with 'H#' followed by 4 digits." << std::endl;
        }
        else if (isHospitalIDExists(id))
        {
            std::cerr << "Error: Hospital ID already exists!" << std::endl;
        }
        else
        {
            break;
        }
    }

    while (true)
    {
        std::cout << "Address: ";
        std::getline(std::cin, address);
        if (!isValidAddress(address))
        {
            std::cout << "Invalid address! Address should contain only letters.\n";
        }
        else
        {
            break;
        }
    }

    bool validPincode = false;
    while (!validPincode)
    {
        std::cout << "Enter 6-digit PIN code: ";
        std::getline(std::cin, pincode);
        if (pincode.length() == 6 && std::all_of(pincode.begin(), pincode.end(), ::isdigit))
        {
            validPincode = true;
        }
        else
        {
            std::cout << "Invalid PIN code! Please enter 6 digits.\n";
        }
    }
    outFile << name << "," << id << "," << address << "," << pincode;

    displayVaccineData();

    std::cout << "\nDo you want to provide Vaccine to this hospital(y/n): ";
    getline(std::cin, choice1);
    while (1)
    {
        if (choice1 != "y" && choice1 != "n")
        {
            std::cout << "\nInvalid input\nDo you want to provide Vaccine to this hospital(y/n): ";
            getline(std::cin, choice1);
        }
        else
        {
            if (choice1 == "n")
            {
                outFile << std::endl;
                std::cout << "Hospital " << name << " data stored\n";
                return;
            }
            break;
        }
    }
    if (choice1 == "y")
    {
        while (1)
        {
            std::cout << "\nEnter the Vaccine Name: ";
            std::getline(std::cin, vaccineName);

            std::cout << "Enter the stock for " << vaccineName << ": ";
            std::cin >> stock;
            std::cin.ignore();

            if (!isStockAvailable(vaccineName, std::stoi(stock)))
            {
                continue;
            }
            outFile << "," << vaccineName << "," << stock;

            updateVaccineStock(vaccineName, std::stoi(stock));

            std::cout << "Do you want to add more vaccine(y/n): ";
            getline(std::cin, choice2);
            while (1)
            {
                if (choice2 != "y" && choice2 != "n")
                {
                    std::cout << "\nInvalid input\nDo you want to add more vaccine(y/n): ";
                    getline(std::cin, choice2);
                }
                else
                {
                    if (choice2 == "n")
                    {
                        outFile << "\n";
                        std::cout << "Hospital " << name << " data stored\n";
                        return;
                    }
                    break;
                }
            }
        }
    }
}

void Admin::updateVaccineStock(const std::string &vaccineName, int reduction)
{
    std::ifstream inFile("vaccines.txt");
    if (!inFile)
    {
        std::cerr << "Error: Unable to open hospital data file for reading." << std::endl;
        return;
    }

    std::ofstream outFile("temp.txt");
    if (!outFile)
    {
        std::cerr << "Error: Unable to open temporary file for writing." << std::endl;
        inFile.close();
        return;
    }

    std::string vaccine;
    int stock;

    while (inFile >> vaccine >> stock)
    {
        if (vaccine == vaccineName)
        {
            stock -= reduction;
            if (stock < 0)
            {
                inFile.close();
                outFile.close();
                return;
            }
        }

        outFile << vaccine << " " << stock << "\n";
    }
    inFile.close();
    outFile.close();

    if (remove("vaccines.txt") != 0)
    {
        std::cerr << "Error: Unable to delete original hospital data file." << std::endl;
        return;
    }
    if (rename("temp.txt", "vaccines.txt") != 0)
    {
        std::cerr << "Error: Unable to rename temporary file." << std::endl;
        return;
    }
}

bool hasOnlyAlphabets(const std::string &str)
{
    std::regex alphabetsRegex("^[a-z]+$");
    return std::regex_match(str, alphabetsRegex);
}

bool hasOnlyNumbers(const std::string &str)
{
    std::regex numbersRegex("^[0-9]+$");
    return std::regex_match(str, numbersRegex);
}

void Admin::addVaccine()
{
    std::string name, stock;
    std::ifstream inFile("vaccines.txt");
    std::ofstream outFile("temp.txt");

    if (!inFile)
    {
        std::cerr << "Error: Unable to open vaccine file for reading." << std::endl;
        return;
    }
    if (!outFile)
    {
        std::cerr << "Error: Unable to open temporary file for writing." << std::endl;
        return;
    }

    std::cout << "\nEnter details of the vaccine:\n";
    std::cout << "Enter the name of Vaccine (cowin & covishield etc): ";
    getline(std::cin, name);

    if (!hasOnlyAlphabets(name))
    {
        std::cout << "Invalid vaccine name! Please enter characters only." << std::endl;
        return;
    }

    std::cout << "Enter the stock for " << name << ": ";
    getline(std::cin, stock);

    if (!hasOnlyNumbers(stock))
    {
        std::cout << "Invalid stock number! Please enter integers only." << std::endl;
        return;
    }

    std::string line;
    bool found = false;

    while (getline(inFile, line))
    {
        std::stringstream ss(line);
        std::string existingName, existingStock;
        ss >> existingName >> existingStock;
        if (existingName == name)
        {
            found = true;
            existingStock = std::to_string(std::stoi(existingStock) + std::stoi(stock));
            outFile << existingName << " " << existingStock << std::endl;
        }
        else
        {
            outFile << line << std::endl;
        }
    }

    if (!found)
    {
        outFile << name << " " << stock << std::endl;
    }

    inFile.close();
    outFile.close();
    remove("vaccines.txt");
    rename("temp.txt", "vaccines.txt");

    std::cout << "\nVaccine Added" << std::endl;
}

int Admin::idCheckInApproveRequest(const std::string &id)
{
    std::string line;
    std::ifstream inFile("vaccinerequest.txt");
    if (inFile.fail() == true)
    {
        std::cout << "File Failed to Open\n";
        return 0;
    }
    while (getline(inFile, line))
    {
        if (line.substr(0, line.find(' ')) == id)
        {
            return 1;
        }
    }
    return 0;
}

void Admin::updateStockHospital(const std::string &id)
{
    std::ifstream inFile("hospitals.txt");
    std::ofstream outHospital("temp1.txt");
    std::ifstream inFileRequest("vaccinerequest.txt");
    std::ofstream outRequest("temp2.txt");

    if (inFile.fail() || outHospital.fail() || inFileRequest.fail() || outRequest.fail())
    {
        std::cerr << "Error: Unable to open files." << std::endl;
        return;
    }

    std::string line, hospital_name, hosp_id, req_vacc, req_stock, line1, req_id;

    while (getline(inFileRequest, line1))
    {
        if (line1.substr(0, line1.find(' ')) == id)
        {
            std::istringstream is(line1);
            getline(is, req_id, ' ');
            getline(is, req_vacc, ' ');
            getline(is, req_stock, ' ');
            continue;
        }
        outRequest << line1 << std::endl;
    }

    bool stockAvailable = isStockAvailable(req_vacc, std::stoi(req_stock));

    if (!stockAvailable)
    {
        // std::cout << "This vaccine does not exist in the database or the requested stock is not available." << std::endl;
        return;
    }

    updateVaccineStock(req_vacc, std::stoi(req_stock));

    while (getline(inFile, line))
    {
        std::istringstream iss(line);
        std::string updatedLine, currentVaccine;

        getline(iss, updatedLine, ',');
        getline(iss, hosp_id, ',');
        updatedLine += "," + hosp_id + ",";

        while (getline(iss, currentVaccine, ','))
        {
            std::string currentStock;
            getline(iss, currentStock, ','); 

            if (currentVaccine == req_vacc && hosp_id == id)
            {
                int newStock = std::stoi(currentStock) + std::stoi(req_stock);
                updatedLine += req_vacc + "," + std::to_string(newStock) + ",";
            }
            else
            {
                updatedLine +=  currentVaccine + "," + currentStock + ","; 
            }
        }

        if (hosp_id == id && updatedLine.find(req_vacc) == std::string::npos)
        {
            updatedLine += req_vacc + "," + req_stock ;
        }
        
        outHospital << updatedLine << std::endl;
    }

    std::cout << "\nRequest from " + hosp_id + " approved" << std::endl;

    inFile.close();
    outHospital.close();
    inFileRequest.close();
    outRequest.close();

    remove("hospitals.txt");
    remove("vaccinerequest.txt");
    if (rename("temp1.txt", "hospitals.txt") != 0 || rename("temp2.txt", "vaccinerequest.txt") != 0)
    {
        std::cerr << "Error renaming files." << std::endl;
    }
}

void Admin::rejectAll()
{
    std::ofstream vaccineRequestFile("vaccinerequest.txt");
    if (!vaccineRequestFile)
    {
        std::cerr << "Error: Unable to open vaccine request file for writing." << std::endl;
        return;
    }
    vaccineRequestFile.clear();
    vaccineRequestFile.close();
    std::cout << "Request Rejected \n";
}

void Admin::displayVaccineRequestData()
{
    std::ifstream infile("vaccinerequest.txt");
    std::string line;
    std::cout << "\n----------Pending Request---------\n";
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::string hospid, vaccineName;
        int stock;
        if (!(iss >> hospid >> vaccineName >> stock))
        {
            break;
        }
        std::cout << "HospitalID: " << hospid << ", RequestedVaccine: " << vaccineName << ", RequestedStock: " << stock << "\n";
    }
}

void Admin::approveVaccineRequest()
{
    std::string choice, reqid;
    std::ifstream inFile("vaccinerequest.txt");
    if (inFile.fail() == true)
    {
        std::cout << "File Failed to Open\n";
        return;
    }

    while (1)
    {
        displayVaccineRequestData();
        std::cout << "1.Accept Request\n2.Reject Request\n3.Back\n";
        std::cout << "Enter choice: ";
        getline(std::cin, choice);
        if (choice == "1")
        {
            std::cout << "Enter the Hospital ID to be Approved: ";
            getline(std::cin, reqid);
            if (idCheckInApproveRequest(reqid) == 0)
            {
                std::cout << "No such ID in request list. Try again" << std::endl;
                continue;
            }
            updateStockHospital(reqid);
        }
        else if (choice == "2")
        {
            rejectAll();
        }
        else
        {
            return;
        }
    }
}

// int main()
// {
//     Admin admin;
//     admin.loginPage();
//     admin.displayMenu();
// }