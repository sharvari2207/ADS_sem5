#include "BloodBankSystem.h"
#include <iostream>
#include <string>
#include <limits>

void displayWelcomeBanner() {
    std::cout << "\n=========================================================================================\n";
    std::cout << "                 CENTRAL BLOOD BANK MANAGEMENT & LIFESAVING SYSTEM                       \n";
    std::cout << "             [Advanced Data Structures Project - Mapped to ER Diagram]                   \n";
    std::cout << "=========================================================================================\n";
}

void printTopLevelMenu() {
    std::cout << "\n============================== SELECT YOUR ROLE / OPTION ===============================\n";
    std::cout << "  [1] DONOR PORTAL    : Person who wants to DONATE blood\n";
    std::cout << "  [2] RECEIVER PORTAL : Person who wants to RECEIVE blood units (Request & Urgency)\n";
    std::cout << "  [3] HOSPITAL PORTAL : Midcentral Operations Hub (RESTRICTED - PASSWORD PROTECTED)\n";
    std::cout << "  [4] EXIT            : Exit Application\n";
    std::cout << "=========================================================================================\n";
    std::cout << "  Enter your choice [1-4]: ";
}

void handleHospitalAuthentication(BloodBankSystem& system) {
    std::cout << "\n-----------------------------------------------------------------------------------------\n";
    std::cout << "  [SECURITY CHECK] HOSPITAL CENTRAL WORKSTATION - RESTRICTED ACCESS AREA                 \n";
    std::cout << "  This section is strictly restricted to hospital staff and authorized personnel only.   \n";
    std::cout << "-----------------------------------------------------------------------------------------\n";

    std::string enteredPassword;
    std::cout << "  Enter Hospital Security Password: ";
    std::cin >> enteredPassword;

    if (system.verifyHospitalPassword(enteredPassword)) {
        std::cout << "\n>>> [ACCESS GRANTED] Identity Verified! Welcome to Hospital Central Workstation. <<<\n";
        system.runHospitalDashboard();
    } else {
        std::cout << "\n>>> [ACCESS DENIED] Incorrect Password! Unauthorized access blocked. <<<\n";
        std::cout << "  Returning to the main portal for security purposes.\n";
    }
}

int main() {
    BloodBankSystem system;
    displayWelcomeBanner();

    int choice = -1;
    while (choice != 4) {
        printTopLevelMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                // 1st: Person who wants to donate blood
                system.donorPortalWorkflow();
                break;
            case 2:
                // 2nd: Person who wants to receive blood units
                system.receiverPortalWorkflow();
                break;
            case 3:
                // 3rd: Hospital (Midcentral between donor & receiver - Password Restricted: Blood2026)
                handleHospitalAuthentication(system);
                break;
            case 4:
                // 4th: Exit
                std::cout << "\nThank you for using the Central Blood Bank Management System. Goodbye!\n\n";
                break;
            default:
                std::cout << "Invalid choice! Please choose an option between 1 and 4.\n";
        }
    }

    return 0;
}
