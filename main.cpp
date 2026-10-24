#include "BloodBankSystem.h"
#include <iostream>
#include <string>
#include <limits>

// Welcome banner
void displayWelcomeBanner() {
    std::cout << "\n=========================================================================================\n";
    std::cout << "                 CENTRAL BLOOD BANK MANAGEMENT & LIFESAVING SYSTEM                       \n";
    std::cout << "             Emergency Transfusion, Triage & Automated Lifesaving Platform               \n";
    std::cout << "=========================================================================================\n";
}

// Main navigation menu
void printTopLevelMenu() {
    std::cout << "\n============================== SELECT YOUR ROLE / OPTION ===============================\n";
    std::cout << "  [1] DONOR PORTAL    : Register and donate blood units\n";
    std::cout << "  [2] RECEIVER PORTAL : Request blood units for patients (Clinical Urgency & Triage)\n";
    std::cout << "  [3] HOSPITAL PORTAL : Hospital Authority Central Workstation (Password: Blood2026)\n";
    std::cout << "  [4] EXIT            : Exit Application\n";
    std::cout << "=========================================================================================\n";
    std::cout << "  Enter your choice [1-4]: ";
}

// Authenticate hospital staff and authority
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
            if (std::cin.eof()) break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                // Donor registration and blood donation
                system.donorPortalWorkflow();
                break;
            case 2:
                // Receiver request submission
                system.receiverPortalWorkflow();
                break;
            case 3:
                // Hospital authority management dashboard
                handleHospitalAuthentication(system);
                break;
            case 4:
                // Exit system
                std::cout << "\nThank you for using the Central Blood Bank Management System. Goodbye!\n\n";
                break;
            default:
                std::cout << "Invalid choice! Please choose an option between 1 and 4.\n";
        }
    }

    return 0;
}
