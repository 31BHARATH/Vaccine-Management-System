#include <iostream>
#include <string>
#include "admin.h"
#include "hospital.h"
#include "user.h"

using namespace std;

int main()
{
    char choice;
    string input;
    Admin admin;
    Hospital hospital;
    User user;

    do
    {
        cout << "Welcome to VaccineManagementSystem!! \n";
        cout << "Please select an option:" << endl;
        cout << "1. Admin" << endl;
        cout << "2. User" << endl;
        cout << "3. Hospital" << endl;
        cout << "4. Exit" << endl;
        cout << "Your choice: ";

        getline(cin, input);

        if (input.length() == 1 && isdigit(input[0]))
        {
            choice = input[0];
            switch (choice)
            {
            case '1':
                admin.loginPage();
                admin.displayMenu();
                break;
            case '2':
                user.displayMenu();
                break;
            case '3':
                VaccinationManagementSystem vaccine;
                vaccine.run();
            case '4':
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
        else
        {
            cout << "Invalid input. Please enter a number between 1 and 4." << endl;
            // Clear input buffer
            // cin.clear();
            // cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (choice != '4');

    return 0;
}
