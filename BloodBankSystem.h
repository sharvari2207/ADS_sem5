#ifndef BLOOD_BANK_SYSTEM_H
#define BLOOD_BANK_SYSTEM_H

#include "ERModels.h"
#include "BloodRequestHeap.h"
#include "MaxLifeSavingEdmondsKarp.h"
#include "DirectoryTrie.h"
#include <vector>
#include <map>
#include <string>

// Central system coordinator for donors, receivers, and hospital authority
class BloodBankSystem {
private:
    // Core database records
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

    // Advanced Data Structure engines
    BloodRequestHeap requestMaxHeap;
    MaxLifeSavingEdmondsKarp maxFlowSolver;
    DirectoryTrie directoryTrie;

    // Populate initial system data
    void seedInitialData();

public:
    BloodBankSystem();

    // Data structure accessors
    BloodRequestHeap& getHeap() { return requestMaxHeap; }
    MaxLifeSavingEdmondsKarp& getMaxFlowSolver() { return maxFlowSolver; }
    DirectoryTrie& getDirectoryTrie() { return directoryTrie; }

    // Option 1: Donor Registration & Blood Donation
    void donorPortalWorkflow();

    // Option 2: Patient Blood Request & Urgency Triage
    void receiverPortalWorkflow();

    // Option 3: Hospital Central Workstation (Password: "Blood2026")
    bool verifyHospitalPassword(const std::string& password) const;
    void runHospitalDashboard();

    // Hospital Authority Request Operations:
    // Review pending requests, verify inventory, dispense units, and mark as DONE
    void manageReceiverRequests();
    void acceptAndFulfillRequest(int requestId);
    void autoFulfillTopUrgentRequest();
    void autoFulfillAllAvailableRequests();
    void displayBloodInventorySummary() const;

    // Feature 1: Urgency Priority Queue (Max-Heap)
    void runHeapInteractiveDemo();
    void addNewBloodRequest(const BloodRequest& req);
    void processNextUrgentRequest();
    void simulateTimePassage(int minutes);

    // Feature 2: Max Life-Saving Allocation (Edmonds-Karp Max Flow)
    void runMaxLifeSavingEdmondsKarpDemo();

    // Feature 3: Fast Directory Search (Trie Prefix Autocomplete)
    void runTrieAutocompleteDemo();
    void searchDirectoryInteractive();

    // Central records and inventories
    void displayDatabaseStats() const;
    void displayERDatabaseStats() const { displayDatabaseStats(); } // Backward compatibility
    void displayAllDonorsWithHealth() const;
    void displayAllReceiversAndRequests() const;
    void displayAllBloodUnits() const;
    void displayAllDonations() const;
    void displayAllAppointmentsAndTests() const;
    
    // Automated end-to-end verification tests
    void runAllADSTests();
};

#endif // BLOOD_BANK_SYSTEM_H
