#ifndef BLOOD_BANK_SYSTEM_H
#define BLOOD_BANK_SYSTEM_H

#include "ERModels.h"
#include "BloodRequestHeap.h"
#include "MaxLifeSavingEdmondsKarp.h"
#include "DirectoryTrie.h"
#include <vector>
#include <map>
#include <string>

class BloodBankSystem {
private:
    // ER Diagram Tables
    std::vector<User> users;
    std::vector<Donor> donors;
    std::vector<Requester> requesters;
    std::vector<DonorHealth> healthRecords;
    std::vector<Hospital> hospitals;
    std::vector<BloodBank> bloodBanks;
    std::vector<Appointment> appointments;
    std::vector<MedicalTest> medicalTests;
    std::vector<DonationAppointment> donationAppointments;
    std::vector<Donation> donations;
    std::vector<BloodUnit> bloodUnits;
    std::vector<BloodRequest> bloodRequests;

    // Advanced Data Structures
    BloodRequestHeap requestMaxHeap;
    MaxLifeSavingEdmondsKarp maxFlowSolver;
    DirectoryTrie directoryTrie;

    // Helper to seed realistic data based on ER diagram
    void seedERData();

public:
    BloodBankSystem();

    // ADS Feature Accessors
    BloodRequestHeap& getHeap() { return requestMaxHeap; }
    MaxLifeSavingEdmondsKarp& getMaxFlowSolver() { return maxFlowSolver; }
    DirectoryTrie& getDirectoryTrie() { return directoryTrie; }

    // Option 1: Donor Workflow (Donate Blood)
    void donorPortalWorkflow();

    // Option 2: Receiver Workflow (Request Blood Units with Urgency)
    void receiverPortalWorkflow();

    // Option 3: Hospital Portal (Restricted with Password "Blood2026")
    bool verifyHospitalPassword(const std::string& password) const;
    void runHospitalDashboard();

    // Hospital sub-features & ADS implementations:
    // Feature 1: Urgency MaxHeap Management
    void runHeapInteractiveDemo();
    void addNewBloodRequest(const BloodRequest& req);
    void processNextUrgentRequest();
    void simulateTimePassage(int minutes);

    // Feature 2: Max Life Saving Allocation using Edmonds-Karp
    void runMaxLifeSavingEdmondsKarpDemo();

    // Feature 3: Trie Directory Autocomplete Search
    void runTrieAutocompleteDemo();
    void searchDirectoryInteractive();

    // Comprehensive ER Database Records
    void displayERDatabaseStats() const;
    void displayAllDonorsWithHealth() const;
    void displayAllReceiversAndRequests() const;
    void displayAllBloodUnits() const;
    void displayAllDonations() const;
    void displayAllAppointmentsAndTests() const;
    
    // Automated end-to-end ADS test suite
    void runAllADSTests();
};

#endif // BLOOD_BANK_SYSTEM_H
