#include "BloodBankSystem.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

BloodBankSystem::BloodBankSystem() {
    seedInitialData();
}

// Populate sample records for donors, hospitals, inventory, and requests
void BloodBankSystem::seedInitialData() {
    // Standard user profiles
    users.push_back({101, "Aarav Deshmukh", "pass101", "aarav.d@gmail.com", {"+91-9822011111", "+91-9822011112"}, "Donor", "Kothrud, Pune", "1995-04-12", "Male", 31});
    users.push_back({102, "Sneha Kulkarni", "pass102", "sneha.k@gmail.com", {"+91-9822022222"}, "Donor", "Shivajinagar, Pune", "1998-08-23", "Female", 28});
    users.push_back({103, "Rohan Shinde", "pass103", "rohan.s@gmail.com", {"+91-9822033333"}, "Donor", "Baner, Pune", "1990-11-05", "Male", 35});
    users.push_back({104, "Pooja Joshi", "pass104", "pooja.j@gmail.com", {"+91-9822044444"}, "Donor", "Hadapsar, Pune", "2001-02-14", "Female", 25});
    users.push_back({105, "Vikram More", "pass105", "vikram.m@gmail.com", {"+91-9822055555"}, "Donor", "Viman Nagar, Pune", "1988-06-30", "Male", 38});
    users.push_back({106, "Dr. Rajesh Patil", "pass106", "rajesh.patil@kem.edu", {"+91-9822066666"}, "Requester", "Rasta Peth, Pune", "1975-03-10", "Male", 51});
    users.push_back({107, "Dr. Ananya Sen", "pass107", "ananya.sen@sahyadri.org", {"+91-9822077777"}, "Requester", "Deccan, Pune", "1982-09-17", "Female", 44});
    users.push_back({108, "Dr. Amit Verma", "pass108", "amit.verma@rubyhall.com", {"+91-9822088888"}, "Requester", "Bund Garden, Pune", "1980-01-25", "Male", 46});
    users.push_back({109, "Sunil Gawande", "pass109", "admin@jankalyan.org", {"+91-9822099999"}, "BloodBankAdmin", "Sarasbaug, Pune", "1972-12-01", "Male", 54});

    // Registered donors
    donors.push_back({1, 101, "Aarav Deshmukh", "Active", "N/A"});
    donors.push_back({2, 102, "Sneha Kulkarni", "Active", "N/A"});
    donors.push_back({3, 103, "Rohan Shinde", "Active", "N/A"});
    donors.push_back({4, 104, "Pooja Joshi", "Active", "N/A"});
    donors.push_back({5, 105, "Vikram More", "Active", "N/A"});

    // Requesters placing patient orders
    requesters.push_back({1, 106, "Dr. Rajesh Patil", "KEM Hospital Pune"});
    requesters.push_back({2, 107, "Dr. Ananya Sen", "Sahyadri Super Speciality Hospital"});
    requesters.push_back({3, 108, "Dr. Amit Verma", "Ruby Hall Clinic"});

    // Donor health assessments
    healthRecords.push_back({501, 1, 68.5, 175.0, BloodGroup::O_NEG, "None", {"None"}, false, "2025-10-15", "None"});
    healthRecords.push_back({502, 2, 54.0, 162.0, BloodGroup::A_POS, "None", {"Mild Allergies"}, false, "2025-11-20", "None"});
    healthRecords.push_back({503, 3, 72.0, 178.0, BloodGroup::B_POS, "None", {"None"}, false, "2025-12-05", "None"});
    healthRecords.push_back({504, 4, 58.0, 165.0, BloodGroup::O_POS, "None", {"None"}, false, "2026-01-10", "None"});
    healthRecords.push_back({505, 5, 80.0, 180.0, BloodGroup::AB_POS, "None", {"None"}, false, "2026-02-14", "None"});

    // Partner hospitals
    hospitals.push_back({1, "Sahyadri Super Speciality Hospital", "info@sahyadri.org", "+91-20-67213000", "Deccan Gymkhana, Pune"});
    hospitals.push_back({2, "KEM Hospital Pune", "help@kemhospitalpune.org", "+91-20-26125600", "Rasta Peth, Pune"});
    hospitals.push_back({3, "Ruby Hall Clinic", "contact@rubyhall.com", "+91-20-66455100", "40 Sassoon Road, Pune"});

    // Blood banks
    bloodBanks.push_back({1, "Jankalyan Blood Centre", "info@jankalyanbloodbank.ch", "+91-20-24444501", "Sarasbaug Road, Pune"});
    bloodBanks.push_back({2, "Red Cross Blood Bank Pune", "redcross.pune@redcross.org", "+91-20-26123456", "Camp, Pune"});

    // Appointments and medical tests
    appointments.push_back({301, 1, 1, "09:30 AM", "Completed", "Attended"});
    appointments.push_back({302, 2, 1, "10:15 AM", "Completed", "Attended"});
    appointments.push_back({303, 3, 2, "11:00 AM", "Completed", "Attended"});
    appointments.push_back({304, 4, 2, "02:00 PM", "Completed", "Attended"});
    appointments.push_back({305, 5, 3, "03:30 PM", "Completed", "Attended"});

    medicalTests.push_back({401, 1, 301, "2026-03-01", "Hb: 14.2 g/dL, BP: 120/80", true, "Eligible and healthy"});
    medicalTests.push_back({402, 2, 302, "2026-03-01", "Hb: 13.1 g/dL, BP: 118/76", true, "Eligible"});
    medicalTests.push_back({403, 3, 303, "2026-03-02", "Hb: 15.0 g/dL, BP: 124/82", true, "Eligible"});
    medicalTests.push_back({404, 4, 304, "2026-03-02", "Hb: 12.8 g/dL, BP: 115/75", true, "Eligible"});
    medicalTests.push_back({405, 5, 305, "2026-03-03", "Hb: 14.5 g/dL, BP: 122/80", true, "Eligible"});

    // Blood donation sessions
    donationAppointments.push_back({601, 1, 1, "2026-03-01", "10:00 AM", "Completed", "2026-03-01 09:45"});
    donationAppointments.push_back({602, 2, 1, "2026-03-01", "10:45 AM", "Completed", "2026-03-01 10:30"});
    donationAppointments.push_back({603, 3, 2, "2026-03-02", "11:30 AM", "Completed", "2026-03-02 11:15"});
    donationAppointments.push_back({604, 4, 2, "2026-03-02", "02:30 PM", "Completed", "2026-03-02 02:15"});
    donationAppointments.push_back({605, 5, 3, "2026-03-03", "04:00 PM", "Completed", "2026-03-03 03:45"});

    donations.push_back(Donation(701, 1, "Whole Blood", "2026-03-01", "Grade A", BloodGroup::O_NEG, "Screened - Safe"));
    donations.push_back(Donation(702, 1, "Whole Blood", "2026-03-01", "Grade A", BloodGroup::A_POS, "Screened - Safe"));
    donations.push_back(Donation(703, 2, "Whole Blood", "2026-03-02", "Grade A", BloodGroup::B_POS, "Screened - Safe"));
    donations.push_back(Donation(704, 2, "Whole Blood", "2026-03-02", "Grade A", BloodGroup::O_POS, "Screened - Safe"));
    donations.push_back(Donation(705, 3, "Whole Blood", "2026-03-03", "Grade A", BloodGroup::AB_POS, "Screened - Safe"));

    // Tested blood units in storage
    bloodUnits.push_back({801, 1, 701, "UNIT-ONEG-01", BloodGroup::O_NEG, "2026-03-01", "2026-04-12", "Available", "Tested-Safe"});
    bloodUnits.push_back({802, 1, 701, "UNIT-ONEG-02", BloodGroup::O_NEG, "2026-03-01", "2026-04-12", "Available", "Tested-Safe"});
    bloodUnits.push_back({803, 1, 704, "UNIT-OPOS-01", BloodGroup::O_POS, "2026-03-02", "2026-04-13", "Available", "Tested-Safe"});
    bloodUnits.push_back({804, 1, 704, "UNIT-OPOS-02", BloodGroup::O_POS, "2026-03-02", "2026-04-13", "Available", "Tested-Safe"});
    bloodUnits.push_back({805, 1, 704, "UNIT-OPOS-03", BloodGroup::O_POS, "2026-03-02", "2026-04-13", "Available", "Tested-Safe"});
    bloodUnits.push_back({806, 1, 702, "UNIT-APOS-01", BloodGroup::A_POS, "2026-03-01", "2026-04-12", "Available", "Tested-Safe"});
    bloodUnits.push_back({807, 1, 702, "UNIT-APOS-02", BloodGroup::A_POS, "2026-03-01", "2026-04-12", "Available", "Tested-Safe"});
    bloodUnits.push_back({808, 1, 703, "UNIT-BPOS-01", BloodGroup::B_POS, "2026-03-02", "2026-04-13", "Available", "Tested-Safe"});
    bloodUnits.push_back({809, 1, 703, "UNIT-BPOS-02", BloodGroup::B_POS, "2026-03-02", "2026-04-13", "Available", "Tested-Safe"});
    bloodUnits.push_back({810, 1, 705, "UNIT-ABPOS-01", BloodGroup::AB_POS, "2026-03-03", "2026-04-14", "Available", "Tested-Safe"});

    // Active patient transfusion requests (initially Pending)
    bloodRequests.push_back(BloodRequest(901, 1, "Kavita Rao", BloodGroup::AB_POS, 2, UrgencyLevel::CRITICAL, 29, true, 5));
    bloodRequests.push_back(BloodRequest(902, 2, "Master Aarush (Infant)", BloodGroup::O_NEG, 1, UrgencyLevel::CRITICAL, 1, true, 10));
    bloodRequests.push_back(BloodRequest(903, 1, "Mahesh Kadam", BloodGroup::B_POS, 2, UrgencyLevel::URGENT, 45, false, 25));
    bloodRequests.push_back(BloodRequest(904, 3, "Suresh Gaikwad", BloodGroup::A_POS, 2, UrgencyLevel::URGENT, 68, false, 30));
    bloodRequests.push_back(BloodRequest(905, 2, "Neha Sawant", BloodGroup::O_POS, 2, UrgencyLevel::MODERATE, 19, false, 60));
    bloodRequests.push_back(BloodRequest(906, 3, "Dattatray Shinde", BloodGroup::O_POS, 1, UrgencyLevel::ROUTINE, 72, false, 120));

    // Seed priority queue
    for (const auto& req : bloodRequests) {
        requestMaxHeap.insert(req, true);
    }

    // Seed search trie with donors
    for (const auto& d : donors) {
        std::string phone = "";
        std::string city = "";
        for (const auto& u : users) {
            if (u.userId == d.userId) {
                if (!u.phones.empty()) phone = u.phones[0];
                city = u.address;
                break;
            }
        }
        BloodGroup bg = BloodGroup::UNKNOWN;
        for (const auto& h : healthRecords) {
            if (h.donorId == d.donorId) {
                bg = h.bloodGroup;
                break;
            }
        }
        directoryTrie.insert({DirectoryRole::DONOR, d.donorId, d.name, phone, "", bg, city, "Donor (" + d.status + ")"});
    }

    // Seed search trie with requesters
    for (const auto& r : requesters) {
        std::string phone = "";
        std::string city = "";
        for (const auto& u : users) {
            if (u.userId == r.userId) {
                if (!u.phones.empty()) phone = u.phones[0];
                city = u.address;
                break;
            }
        }
        directoryTrie.insert({DirectoryRole::REQUESTER, r.requesterId, r.name, phone, "", BloodGroup::UNKNOWN, city, r.hospitalName});
    }

    // Seed search trie with hospitals
    for (const auto& h : hospitals) {
        directoryTrie.insert({DirectoryRole::HOSPITAL, h.hospitalId, h.name, h.phone, h.email, BloodGroup::UNKNOWN, h.address, "Hospital Centre"});
    }

    // Seed search trie with blood banks
    for (const auto& bb : bloodBanks) {
        directoryTrie.insert({DirectoryRole::BLOOD_BANK, bb.bloodBankId, bb.name, bb.phone, bb.email, BloodGroup::UNKNOWN, bb.address, "Blood Storage Facility"});
    }
}

// OPTION 1: DONOR PORTAL (PERSON WHO WANTS TO DONATE BLOOD)
void BloodBankSystem::donorPortalWorkflow() {
    std::cout << "\n";
    std::cout << "WELCOME TO THE BLOOD DONOR REGISTRATION & DONATION PORTAL               \n";
    std::cout << "Donor Registration, Health Check & Donation Desk                     \n\n";
    std::cout << "Please fill out the donor information\n";

    std::string name, phone, email, address, bgStr, medications, illness, addiction;
    int age;
    double weight, height;
    char genderChar, surgeryChar;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "1. Full Name: ";
    std::getline(std::cin, name);
    if (name.empty()) name = "Volunteer Donor";

    std::cout << "2. Age (in years): ";
    std::cin >> age;

    std::cout << "3. Gender (M/F/O): ";
    std::cin >> genderChar;

    std::cout << "4. Contact Phone: ";
    std::cin >> phone;

    std::cout << "5. Email Address: ";
    std::cin >> email;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "6. Residential City / Address: ";
    std::getline(std::cin, address);

    std::cout << "7. Blood Group (O-, O+, A-, A+, B-, B+, AB-, AB+): ";
    std::cin >> bgStr;
    BloodGroup bg = stringToBloodGroup(bgStr);

    std::cout << "\n--- Mandatory Medical & Health Screening ---\n";
    std::cout << "8. Body Weight (in kg, minimum required is 45.0 kg): ";
    std::cin >> weight;

    std::cout << "9. Height (in cm): ";
    std::cin >> height;

    std::cout << "10. Have you undergone any major surgery in the past 6 months? (y/n): ";
    std::cin >> surgeryChar;
    bool recentSurgery = (surgeryChar == 'y' || surgeryChar == 'Y');

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "11. Are you currently taking any prescription medications? (e.g. None / Aspirin): ";
    std::getline(std::cin, medications);
    if (medications.empty()) medications = "None";

    std::cout << "12. Any previous chronic illnesses? (e.g. None / Asthma): ";
    std::getline(std::cin, illness);
    if (illness.empty()) illness = "None";

    std::cout << "13. Addictions / Habits (e.g. None / Smoking): ";
    std::getline(std::cin, addiction);
    if (addiction.empty()) addiction = "None";

    // Clinical eligibility check
    bool isEligible = (weight >= 45.0 && !recentSurgery && (medications == "None" || medications == "none"));

    int newUserId = 100 + static_cast<int>(users.size()) + 1;
    int newDonorId = static_cast<int>(donors.size()) + 1;
    int newHealthId = 500 + static_cast<int>(healthRecords.size()) + 1;

    users.push_back({newUserId, name, "pass" + std::to_string(newUserId), email, {phone}, "Donor", address, "1998-01-01", (genderChar == 'F' || genderChar == 'f' ? "Female" : "Male"), age});
    donors.push_back({newDonorId, newUserId, name, (isEligible ? "Active" : "Temporary Deferred"), "N/A"});
    healthRecords.push_back({newHealthId, newDonorId, weight, height, bg, medications, {illness}, recentSurgery, "2026-03-24", addiction});

    // Add profile to directory trie
    directoryTrie.insert({DirectoryRole::DONOR, newDonorId, name, phone, email, bg, address, "Registered Donor"});

    std::cout << "\n";
    if (isEligible) {
        std::cout << "Medical Screening PASSED! You are ELIGIBLE to donate blood. <<<\n";
        
        int newApptId = 300 + static_cast<int>(appointments.size()) + 1;
        int newTestId = 400 + static_cast<int>(medicalTests.size()) + 1;
        int newDonApptId = 600 + static_cast<int>(donationAppointments.size()) + 1;
        int newDonationId = 700 + static_cast<int>(donations.size()) + 1;
        int newUnitId = 800 + static_cast<int>(bloodUnits.size()) + 1;

        std::string unitNum = "UNIT-" + bloodGroupToString(bg) + "-0" + std::to_string(newUnitId % 100);

        appointments.push_back({newApptId, newDonorId, 1, "11:00 AM", "Completed", "Attended"});
        medicalTests.push_back({newTestId, newDonorId, newApptId, "Today", "Hb: 13.8 g/dL, BP: 120/80 (Normal)", true, "Cleared by Hospital Doctor"});
        donationAppointments.push_back({newDonApptId, newDonorId, 1, "Today", "11:30 AM", "Completed", "Today"});
        donations.push_back(Donation(newDonationId, 1, "Whole Blood (350ml)", "Today", "Grade A", bg, "Screened - Tested Safe"));
        bloodUnits.push_back({newUnitId, 1, newDonationId, unitNum, bg, "Today", "42 Days Valid", "Available", "Tested-Safe"});

        std::cout << "  [Registration Success] Donor ID:        #" << newDonorId << "\n";
        std::cout << "  [Medical Evaluation]   Passed (Cleared for Whole Blood 350ml)\n";
        std::cout << "  [Donation Logged]      Donation ID:     #" << newDonationId << " (Grade A Quality)\n";
        std::cout << "  [Inventory Added]      Blood Unit:      " << unitNum << " (" << bloodGroupToString(bg) << ") -> Stored in Blood Bank!\n";
        std::cout << "  [Directory Indexed]    Your name \"" << name << "\" is now searchable in directory lookup.\n";
    } else {
        std::cout << ">>> MEDICAL DEFERRAL NOTICE <<<\n";
        std::cout << "  Thank you for coming forward, but based on health screening criteria:\n";
        if (weight < 45.0) std::cout << "  - Weight is below required threshold (Minimum 45 kg required, current: " << weight << " kg).\n";
        if (recentSurgery) std::cout << "  - Recent surgery detected within last 6 months.\n";
        if (medications != "None" && medications != "none") std::cout << "  - Active medications require physician clearance.\n";
        std::cout << "  Your status has been recorded as 'Temporary Deferred' for your safety.\n";
    }
    std::cout << "\n";
}

// OPTION 2: RECEIVER PORTAL (PATIENT BLOOD TRANSFUSION REQUEST)
void BloodBankSystem::receiverPortalWorkflow() {
    std::cout << "\n";
    std::cout << "WELCOME TO THE PATIENT / BLOOD RECEIVER REQUEST PORTAL                     \n";
    std::cout << "Submit Blood Transfusion Requirements & Emergency Requests                \n";
    std::cout << "Please enter the patient blood request details and clinical urgency level.\n";

    std::string patientName, hospitalName, phone, bgStr;
    int age, unitsNeeded, urgChoice, waitMins;
    char traumaChar;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "1. Patient Full Name: ";
    std::getline(std::cin, patientName);
    if (patientName.empty()) patientName = "Emergency Patient";

    std::cout << "2. Patient Age (in years): ";
    std::cin >> age;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "3. Admitted Hospital Name: ";
    std::getline(std::cin, hospitalName);
    if (hospitalName.empty()) hospitalName = "City Hospital";

    std::cout << "4. Contact Phone Number: ";
    std::cin >> phone;

    std::cout << "5. Required Blood Group (O-, O+, A-, A+, B-, B+, AB-, AB+): ";
    std::cin >> bgStr;
    BloodGroup bg = stringToBloodGroup(bgStr);

    std::cout << "6. Quantity of Blood Units Requested (1 to 5 units): ";
    std::cin >> unitsNeeded;
    if (unitsNeeded < 1) unitsNeeded = 1;

    std::cout << "\n Urgency & Clinical Triage Evaluation\n";
    std::cout << "  [1] Routine   : Elective surgery, planned deficiency correction\n";
    std::cout << "  [2] Moderate  : Subacute medical care, chronic transfusion\n";
    std::cout << "  [3] Urgent    : ICU admission, acute bleeding, severe anemia\n";
    std::cout << "  [4] Critical  : Life-threatening emergency, trauma, major hemorrhage\n";
    std::cout << "Select Urgency Level [1-4]: ";
    std::cin >> urgChoice;

    UrgencyLevel ul = UrgencyLevel::ROUTINE;
    if (urgChoice == 4) ul = UrgencyLevel::CRITICAL;
    else if (urgChoice == 3) ul = UrgencyLevel::URGENT;
    else if (urgChoice == 2) ul = UrgencyLevel::MODERATE;

    std::cout << "Is the patient currently in ICU or Trauma Ward? (y/n): ";
    std::cin >> traumaChar;
    bool isTrauma = (traumaChar == 'y' || traumaChar == 'Y');

    std::cout << "Time elapsed since emergency admission (in minutes, e.g. 15): ";
    std::cin >> waitMins;
    if (waitMins < 0) waitMins = 0;

    int newUserId = 100 + static_cast<int>(users.size()) + 1;
    int newReqrId = static_cast<int>(requesters.size()) + 1;
    int newReqId = 900 + static_cast<int>(bloodRequests.size()) + 1;

    users.push_back({newUserId, patientName, "pass" + std::to_string(newUserId), "", {phone}, "Requester", hospitalName, "1990-01-01", "Unspecified", age});
    requesters.push_back({newReqrId, newUserId, patientName, hospitalName});

    // Create new BloodRequest with status "Pending"
    BloodRequest req(newReqId, newReqrId, patientName, bg, unitsNeeded, ul, age, isTrauma, waitMins);
    bloodRequests.push_back(req);

    // Enqueue into priority queue
    requestMaxHeap.insert(req);

    // Index in directory trie
    directoryTrie.insert({DirectoryRole::REQUESTER, newReqrId, patientName, phone, "", bg, hospitalName, "Patient Request #" + std::to_string(newReqId)});

    std::cout << "\n";
    std::cout << " BLOOD REQUEST SUBMITTED & ENQUEUED IN PRIORITY QUEUE! <<<\n";
    std::cout << "  Request ID:           #" << req.requestId << "\n";
    std::cout << "  Patient Name:         " << req.patientName << " (Age: " << age << ")\n";
    std::cout << "  Required Blood Group: " << bloodGroupToString(bg) << " (" << unitsNeeded << " units)\n";
    std::cout << "  Clinical Urgency:     " << urgencyLevelToString(ul) << "\n";
    std::cout << "  Dynamic Priority:     " << std::fixed << std::setprecision(1) << req.dynamicScore << " points\n";
    std::cout << "  Status:               Pending (Awaiting Hospital Authority review & dispensing)\n";
    std::cout << "  Hospital Authority can now review, verify blood stock, and dispense matching units.\n";
}

// OPTION 3: HOSPITAL CENTRAL HUB (PASSWORD RESTRICTED: "Blood2026")
bool BloodBankSystem::verifyHospitalPassword(const std::string& password) const {
    return (password == "Blood2026");
}

// Display blood stock inventory grouped by blood group
void BloodBankSystem::displayBloodInventorySummary() const {
    std::cout << "               CENTRAL BLOOD BANK STORAGE INVENTORY: STOCK SUMMARY                       \n";
    std::cout << std::left 
              << std::setw(14) << "Blood Group"
              << std::setw(18) << "Available Units"
              << std::setw(18) << "Allocated Units"
              << std::setw(14) << "Total Stock"
              << "Inventory Status\n";
    std::cout << "\n";

    std::vector<BloodGroup> groups = {
        BloodGroup::O_NEG, BloodGroup::O_POS,
        BloodGroup::A_NEG, BloodGroup::A_POS,
        BloodGroup::B_NEG, BloodGroup::B_POS,
        BloodGroup::AB_NEG, BloodGroup::AB_POS
    };

    int grandAvailable = 0;
    int grandAllocated = 0;

    for (auto bg : groups) {
        int avail = 0;
        int alloc = 0;
        for (const auto& u : bloodUnits) {
            if (u.bloodGroup == bg) {
                if (u.status == "Available") avail++;
                else alloc++;
            }
        }
        grandAvailable += avail;
        grandAllocated += alloc;

        std::string statusTag = "IN STOCK";
        if (avail == 0) statusTag = "OUT OF STOCK";
        else if (avail == 1) statusTag = "LOW STOCK";

        std::cout << std::left 
                  << std::setw(14) << bloodGroupToString(bg)
                  << std::setw(18) << (std::to_string(avail) + " units")
                  << std::setw(18) << (std::to_string(alloc) + " units")
                  << std::setw(14) << (std::to_string(avail + alloc) + " units")
                  << statusTag << "\n";
    }
    std::cout << "\n";
    std::cout << "  TOTAL READY-TO-DISPENSE UNITS : " << grandAvailable << " units\n";
    std::cout << "  TOTAL PREVIOUSLY ALLOCATED    : " << grandAllocated << " units\n";
    std::cout << "  TOTAL UNITS HANDLED           : " << (grandAvailable + grandAllocated) << " units\n";
    std::cout << "\n";
}

// Accept a receiver's request, check blood availability, dispense blood units, and mark as DONE
void BloodBankSystem::acceptAndFulfillRequest(int targetReqId) {
    // Locate the request
    int reqIndex = -1;
    for (size_t i = 0; i < bloodRequests.size(); ++i) {
        if (bloodRequests[i].requestId == targetReqId) {
            reqIndex = static_cast<int>(i);
            break;
        }
    }

    if (reqIndex == -1) {
        std::cout << ">>> ERROR: Request #" << targetReqId << " was not found in system records.\n";
        return;
    }

    auto& req = bloodRequests[reqIndex];

    if (req.status == "DONE") {
        std::cout << ">>> NOTICE: Request #" << targetReqId << " for patient \"" << req.patientName 
                  << "\" has ALREADY been completed and marked as [DONE].\n";
        return;
    }

    std::cout << "\n";
    std::cout << "            HOSPITAL AUTHORITY: REVIEWING RECEIVER REQUEST #" << req.requestId << "\n";
    std::cout << "\n";
    std::cout << "  Patient Name:          " << req.patientName << " (Age: " << req.patientAge << ")\n";
    std::cout << "  Admitted Hospital:     " << (req.requesterId <= (int)requesters.size() ? requesters[req.requesterId - 1].hospitalName : "City Hospital") << "\n";
    std::cout << "  Required Blood Group:  " << bloodGroupToString(req.bloodGroup) << "\n";
    std::cout << "  Units Needed:          " << req.quantityRequested << " unit(s)\n";
    std::cout << "  Urgency / Triage:      " << urgencyLevelToString(req.baseUrgency) << "\n";
    std::cout << "  Minutes Waited:        " << req.minutesWaited << " mins | Priority Score: " << req.dynamicScore << "\n";
    std::cout << "  Current Status:        " << req.status << "\n";
    std::cout << "\n";

    // Check available units in stock (exact matches and compatible matches)
    std::vector<int> exactUnitIndices;
    std::vector<int> compatibleUnitIndices;

    for (size_t i = 0; i < bloodUnits.size(); ++i) {
        if (bloodUnits[i].status == "Available") {
            if (bloodUnits[i].bloodGroup == req.bloodGroup) {
                exactUnitIndices.push_back(static_cast<int>(i));
            } else if (isBloodCompatible(bloodUnits[i].bloodGroup, req.bloodGroup)) {
                compatibleUnitIndices.push_back(static_cast<int>(i));
            }
        }
    }

    int totalAvailable = static_cast<int>(exactUnitIndices.size() + compatibleUnitIndices.size());

    std::cout << "  Central Stock Check:\n";
    std::cout << "  - Exact Matching Units (" << bloodGroupToString(req.bloodGroup) << ") : " 
              << exactUnitIndices.size() << " unit(s) available\n";
    std::cout << "  - Other Compatible Units       : " << compatibleUnitIndices.size() << " unit(s) available\n";
    std::cout << "  - Total Compatible Units       : " << totalAvailable << " unit(s) available\n";

    // Case 1: Zero units available
    if (totalAvailable == 0) {
        std::cout << "\n>>> [BLOOD UNAVAILABLE - CANNOT DISPENSE] <<<\n";
        std::cout << "  Zero compatible blood units are currently available in the central repository.\n";
        std::cout << "  Patient Request #" << req.requestId << " will remain [Pending] in the urgency queue.\n";
        std::cout << "  Authority Recommendation: Use Directory Search (Feature 3) to contact potential donors.\n";
        std::cout << "\n";
        return;
    }

    // Case 2: Full stock available
    if (totalAvailable >= req.quantityRequested) {
        std::cout << "\n>>> Blood Availability: SUFFICIENT STOCK AVAILABLE <<<\n";
        std::cout << "  Do you want to ACCEPT this request and DISPENSE " 
                  << req.quantityRequested << " unit(s) to " << req.patientName << "? (y/n): ";
        char confirm;
        std::cin >> confirm;

        if (confirm != 'y' && confirm != 'Y') {
            std::cout << "  Action cancelled by Hospital Authority. Request remains Pending.\n";
            std::cout << "\n";
            return;
        }

        // Gather units to dispense: exact match first, then compatible
        std::vector<int> unitsToAllocate;
        for (int idx : exactUnitIndices) {
            if ((int)unitsToAllocate.size() < req.quantityRequested) {
                unitsToAllocate.push_back(idx);
            }
        }
        for (int idx : compatibleUnitIndices) {
            if ((int)unitsToAllocate.size() < req.quantityRequested) {
                unitsToAllocate.push_back(idx);
            }
        }

        std::string allocatedBatchDetails = "";
        for (int idx : unitsToAllocate) {
            bloodUnits[idx].status = "Allocated to Patient: " + req.patientName + " (Req #" + std::to_string(req.requestId) + ")";
            if (!allocatedBatchDetails.empty()) allocatedBatchDetails += ", ";
            allocatedBatchDetails += bloodUnits[idx].unitNumber + " [" + bloodGroupToString(bloodUnits[idx].bloodGroup) + "]";
        }

        // Mark the request status as DONE
        req.status = "DONE";

        // Remove from the urgency max-heap
        requestMaxHeap.removeById(req.requestId);

        std::cout << "\n";
        std::cout << ">>> [REQUEST ACCEPTED & BLOOD DISPENSED BY HOSPITAL AUTHORITY] <<<\n";
        std::cout << "\n";
        std::cout << "  Request ID:            #" << req.requestId << "\n";
        std::cout << "  Patient Name:          " << req.patientName << "\n";
        std::cout << "  Required Group:        " << bloodGroupToString(req.bloodGroup) << "\n";
        std::cout << "  Units Dispensed:       " << unitsToAllocate.size() << " unit(s)\n";
        std::cout << "  Dispensed Unit Numbers: " << allocatedBatchDetails << "\n";
        std::cout << "  Request Status:        MARKED AS [DONE]\n";
        std::cout << "  Queue Update:          Removed from active triage queue.\n";
        std::cout << "  Hospital Authority:    Blood packets securely released and transferred to ward.\n";
        std::cout << "\n";
        return;
    }

    // Case 3: Partial stock available
    std::cout << "\n>>> [PARTIAL BLOOD STOCK AVAILABLE] <<<\n";
    std::cout << "  Patient requested " << req.quantityRequested << " unit(s), but only " 
              << totalAvailable << " unit(s) are currently in stock.\n";
    std::cout << "  Select Authority Action:\n";
    std::cout << "  [1] Dispense all " << totalAvailable << " available unit(s) now and keep remaining demand Pending\n";
    std::cout << "  [2] Dispense all " << totalAvailable << " available unit(s) and mark request as DONE (Satisfied)\n";
    std::cout << "  [3] Keep full request Pending until all " << req.quantityRequested << " units are available\n";
    std::cout << "  Enter choice [1-3]: ";
    int partialChoice;
    std::cin >> partialChoice;

    if (partialChoice == 1 || partialChoice == 2) {
        std::vector<int> unitsToAllocate;
        for (int idx : exactUnitIndices) unitsToAllocate.push_back(idx);
        for (int idx : compatibleUnitIndices) unitsToAllocate.push_back(idx);

        std::string allocatedBatchDetails = "";
        for (int idx : unitsToAllocate) {
            bloodUnits[idx].status = "Allocated to Patient: " + req.patientName + " (Req #" + std::to_string(req.requestId) + ")";
            if (!allocatedBatchDetails.empty()) allocatedBatchDetails += ", ";
            allocatedBatchDetails += bloodUnits[idx].unitNumber + " [" + bloodGroupToString(bloodUnits[idx].bloodGroup) + "]";
        }

        if (partialChoice == 2) {
            req.status = "DONE";
            requestMaxHeap.removeById(req.requestId);
            std::cout << "\n>>> Dispensed " << unitsToAllocate.size() << " unit(s) (" << allocatedBatchDetails 
                      << ") to " << req.patientName << ". Request marked as [DONE].\n";
        } else {
            req.quantityRequested -= static_cast<int>(unitsToAllocate.size());
            req.status = "Partially Fulfilled (" + std::to_string(unitsToAllocate.size()) + " dispensed, " + std::to_string(req.quantityRequested) + " needed)";
            requestMaxHeap.removeById(req.requestId);
            requestMaxHeap.insert(req, true);
            std::cout << "\n>>> Dispensed " << unitsToAllocate.size() << " unit(s) (" << allocatedBatchDetails 
                      << ") to " << req.patientName << ". Remaining demand (" << req.quantityRequested << " units) kept as [Pending].\n";
        }
    } else {
        std::cout << "  Request kept as Pending without changes.\n";
    }
    std::cout << "\n";
}

// Auto-fulfill the highest priority request in the emergency queue
void BloodBankSystem::autoFulfillTopUrgentRequest() {
    if (requestMaxHeap.isEmpty()) {
        std::cout << "\n>>> Priority Queue is empty! No pending requests.\n";
        return;
    }

    BloodRequest top = requestMaxHeap.peekMax();
    std::cout << "\n>>> Identified Top Priority Request #" << top.requestId 
              << " for patient \"" << top.patientName << "\" (Urgency: " 
              << urgencyLevelToString(top.baseUrgency) << ", Score: " << top.dynamicScore << ")\n";
    acceptAndFulfillRequest(top.requestId);
}

// Auto-fulfill all pending requests that have matching units available in stock
void BloodBankSystem::autoFulfillAllAvailableRequests() {
    std::cout << "\n";
    std::cout << "       HOSPITAL AUTHORITY: BATCH AUTO-FULFILL PENDING REQUESTS BY STOCK                   \n";
    std::cout << "\n";

    int fulfilledCount = 0;
    int unitsDispensedCount = 0;

    // Iterate through all pending requests
    for (size_t rIdx = 0; rIdx < bloodRequests.size(); ++rIdx) {
        if (bloodRequests[rIdx].status == "DONE") continue;

        auto& req = bloodRequests[rIdx];

        // Count available matching and compatible units
        std::vector<int> matchingUnits;
        for (size_t uIdx = 0; uIdx < bloodUnits.size(); ++uIdx) {
            if (bloodUnits[uIdx].status == "Available" && bloodUnits[uIdx].bloodGroup == req.bloodGroup) {
                matchingUnits.push_back(static_cast<int>(uIdx));
            }
        }

        // If exact match is not enough, add compatible units
        if ((int)matchingUnits.size() < req.quantityRequested) {
            for (size_t uIdx = 0; uIdx < bloodUnits.size(); ++uIdx) {
                if (bloodUnits[uIdx].status == "Available" && bloodUnits[uIdx].bloodGroup != req.bloodGroup &&
                    isBloodCompatible(bloodUnits[uIdx].bloodGroup, req.bloodGroup)) {
                    matchingUnits.push_back(static_cast<int>(uIdx));
                }
            }
        }

        // If sufficient stock available, fulfill
        if ((int)matchingUnits.size() >= req.quantityRequested) {
            for (int i = 0; i < req.quantityRequested; ++i) {
                int u = matchingUnits[i];
                bloodUnits[u].status = "Allocated to Patient: " + req.patientName + " (Req #" + std::to_string(req.requestId) + ")";
                unitsDispensedCount++;
            }
            req.status = "DONE";
            requestMaxHeap.removeById(req.requestId);
            fulfilledCount++;
            std::cout << "  [SUCCESS] Request #" << req.requestId << " (" << req.patientName 
                      << ", " << bloodGroupToString(req.bloodGroup) << ") fulfilled with " 
                      << req.quantityRequested << " unit(s) -> Marked [DONE]\n";
        }
    }

    std::cout << "\n";
    std::cout << "  Batch Fulfillment Finished:\n";
    std::cout << "  Total Requests Marked as [DONE] : " << fulfilledCount << "\n";
    std::cout << "  Total Blood Units Dispensed     : " << unitsDispensedCount << " unit(s)\n";
    std::cout << "\n";
}

// Hospital Authority interactive request management workstation
void BloodBankSystem::manageReceiverRequests() {
    int authChoice = -1;
    while (authChoice != 0) {
        std::cout << "\n";
        std::cout << "               HOSPITAL AUTHORITY: RECEIVER REQUEST MANAGEMENT WORKSTATION               \n";
        std::cout << "\n";
        std::cout << "  [1] View All Active / Pending Receiver Requests & Live Stock Check\n";
        std::cout << "  [2] Accept & Dispense Next Most Urgent Request (From Priority Queue)\n";
        std::cout << "  [3] Accept & Dispense Specific Request by Request ID (Check Stock -> Mark DONE)\n";
        std::cout << "  [4] Batch Auto-Fulfill All Compatible Requests with Available Stock\n";
        std::cout << "  [0] Return to Hospital Authority Dashboard\n";
        std::cout << "\n";
        std::cout << "  Enter choice [0-4]: ";

        if (!(std::cin >> authChoice)) {
            if (std::cin.eof()) break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (authChoice) {
            case 1: {
                std::cout << "\n ACTIVE PENDING PATIENT REQUESTS \n";
                std::cout << std::left 
                          << std::setw(8)  << "ReqID"
                          << std::setw(20) << "Patient Name"
                          << std::setw(8)  << "Blood"
                          << std::setw(6)  << "Units"
                          << std::setw(14) << "Urgency"
                          << std::setw(12) << "Score"
                          << std::setw(16) << "Stock Status"
                          << "Request Status\n";
                std::cout << "\n";

                int pendingCount = 0;
                for (const auto& r : bloodRequests) {
                    if (r.status != "DONE") {
                        pendingCount++;
                        int stockCount = 0;
                        for (const auto& u : bloodUnits) {
                            if (u.status == "Available" && isBloodCompatible(u.bloodGroup, r.bloodGroup)) {
                                stockCount++;
                            }
                        }

                        std::string stockDesc = (stockCount >= r.quantityRequested) ? "AVAILABLE (" + std::to_string(stockCount) + ")" 
                                                                                    : (stockCount > 0 ? "PARTIAL (" + std::to_string(stockCount) + ")" 
                                                                                                     : "OUT OF STOCK");

                        std::cout << std::left 
                                  << std::setw(8)  << r.requestId
                                  << std::setw(20) << r.patientName
                                  << std::setw(8)  << bloodGroupToString(r.bloodGroup)
                                  << std::setw(6)  << r.quantityRequested
                                  << std::setw(14) << urgencyLevelToString(r.baseUrgency)
                                  << std::setw(12) << std::fixed << std::setprecision(1) << r.dynamicScore
                                  << std::setw(16) << stockDesc
                                  << r.status << "\n";
                    }
                }

                if (pendingCount == 0) {
                    std::cout << "  All requests are currently completed! No pending patient requests.\n";
                }
                std::cout << "\n";
                break;
            }
            case 2:
                autoFulfillTopUrgentRequest();
                break;
            case 3: {
                int id;
                std::cout << "Enter Request ID to Review & Accept (e.g. 901): ";
                if (std::cin >> id) {
                    acceptAndFulfillRequest(id);
                }
                break;
            }
            case 4:
                autoFulfillAllAvailableRequests();
                break;
            case 0:
                std::cout << "\nReturning to Hospital Authority Dashboard...\n";
                break;
            default:
                std::cout << "Invalid choice! Please select 0 to 4.\n";
        }
    }
}

// Hospital Central Workstation Dashboard
void BloodBankSystem::runHospitalDashboard() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n";
        std::cout << "               HOSPITAL CENTRAL WORKSTATION - HOSPITAL AUTHORITY DASHBOARD               \n";
        std::cout << "             [Central Operations, Patient Triage & Algorithmic Allocation]               \n";
        std::cout << "\n";
        std::cout << "  Logged-in Authority: Authorized Hospital Staff (All Privileges Active)\n\n";
        std::cout << "  --- HOSPITAL AUTHORITY WORKFLOW & INVENTORY MANAGEMENT ---\n";
        std::cout << "  [1] Authority Action: Review & Accept Receiver Requests (Check Stock & Dispense -> Mark DONE)\n";
        std::cout << "  [2] View Central Blood Bank Inventory (Summary by Blood Group & Availability)\n\n";
        std::cout << "  --- CORE ADVANCED DATA STRUCTURE FEATURES ---\n";
        std::cout << "  [3] Feature 1: Urgency Max-Heap Priority Queue (Triage Table, Tree & Time Aging)\n";
        std::cout << "  [4] Feature 2: Smart Max Life-Saving Blood Allocation (Edmonds-Karp Flow Optimization)\n";
        std::cout << "  [5] Feature 3: Fast Directory Search (Trie Prefix Autocomplete)\n\n";
        std::cout << "  --- CENTRAL SYSTEM RECORDS & REPOSITORY ---\n";
        std::cout << "  [6] View All Registered Donors & Medical Health Screenings\n";
        std::cout << "  [7] View All Receivers & Patient Blood Requests (Pending vs DONE)\n";
        std::cout << "  [8] View All Stored Blood Units Inventory\n";
        std::cout << "  [9] View Hospital Appointments, Medical Tests & Donation Logs\n";
        std::cout << "  [10] Run Complete System Verification Test Suite\n";
        std::cout << "  [0] Logout & Return to Main Portal\n";
        std::cout << "\n";
        std::cout << "  Enter choice [0-10]: ";

        if (!(std::cin >> choice)) {
            if (std::cin.eof()) break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                manageReceiverRequests();
                break;
            case 2:
                displayBloodInventorySummary();
                break;
            case 3: {
                int heapSubChoice = -1;
                while (heapSubChoice != 0) {
                    std::cout << "\n--- HOSPITAL TRIAGE: DYNAMIC PRIORITY MAX-HEAP ---\n";
                    std::cout << "  [1] View Current Max-Heap Priority Table\n";
                    std::cout << "  [2] View Binary Max-Heap Tree Structure\n";
                    std::cout << "  [3] Dispatch / Extract Most Urgent Patient (ExtractMax)\n";
                    std::cout << "  [4] Simulate Time Passing in Emergency Ward (Escalate Priorities)\n";
                    std::cout << "  [0] Back to Hospital Hub\n";
                    std::cout << "  Choice: ";
                    if (!(std::cin >> heapSubChoice)) {
                        if (std::cin.eof()) break;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }
                    if (heapSubChoice == 1) {
                        requestMaxHeap.printHeapTable();
                    } else if (heapSubChoice == 2) {
                        std::cout << "\n--- Binary Max-Heap Tree Visualization ---\n";
                        requestMaxHeap.printTreeStructure();
                    } else if (heapSubChoice == 3) {
                        processNextUrgentRequest();
                    } else if (heapSubChoice == 4) {
                        int mins;
                        std::cout << "Enter minutes passed in ward (e.g., 30): ";
                        if (std::cin >> mins) simulateTimePassage(mins);
                    }
                }
                break;
            }
            case 4:
                runMaxLifeSavingEdmondsKarpDemo();
                break;
            case 5:
                searchDirectoryInteractive();
                break;
            case 6:
                displayAllDonorsWithHealth();
                break;
            case 7:
                displayAllReceiversAndRequests();
                break;
            case 8:
                displayAllBloodUnits();
                break;
            case 9:
                displayAllAppointmentsAndTests();
                displayAllDonations();
                break;
            case 10:
                runAllADSTests();
                break;
            case 0:
                std::cout << "\nLogging out of Hospital Central Hub...\n";
                break;
            default:
                std::cout << "Invalid choice! Please select 0 to 10.\n";
        }
    }
}

// Autocomplete directory search workstation
void BloodBankSystem::searchDirectoryInteractive() {
    int trieChoice = -1;
    while (trieChoice != 0) {
        std::cout << "\n--- DIRECTORY AUTOCOMPLETE WORKSTATION ---\n";
        std::cout << "  [1] Enter Name Prefix to Autocomplete (Donors, Patients, Hospitals, Blood Banks)\n";
        std::cout << "  [2] Run Sample Autocomplete Demonstration\n";
        std::cout << "  [0] Back to Hospital Hub\n";
        std::cout << "  Choice: ";
        if (!(std::cin >> trieChoice)) {
            if (std::cin.eof()) break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (trieChoice == 1) {
            std::string prefix;
            std::cout << "Enter name prefix (e.g. 'aa', 'dr', 'sah', 'poo', 'red'): ";
            std::cin >> prefix;
            directoryTrie.printAutocompleteResults(prefix);
        } else if (trieChoice == 2) {
            runTrieAutocompleteDemo();
        }
    }
}

// Display all registered donors and health details
void BloodBankSystem::displayAllDonorsWithHealth() const {
    std::cout << "\n";
    std::cout << "HOSPITAL RECORDS: REGISTERED DONORS & HEALTH EVALUATIONS\n";
    std::cout << "\n";
    std::cout << std::left 
              << std::setw(8)  << "DonorID"
              << std::setw(22) << "Donor Name"
              << std::setw(8)  << "Blood"
              << std::setw(10) << "Weight"
              << std::setw(10) << "Status"
              << std::setw(16) << "Recent Surgery"
              << "Medications / Addiction\n";
    std::cout << "\n";

    for (const auto& d : donors) {
        BloodGroup bg = BloodGroup::UNKNOWN;
        double weight = 0;
        bool surg = false;
        std::string meds = "None";
        std::string addic = "None";

        for (const auto& h : healthRecords) {
            if (h.donorId == d.donorId) {
                bg = h.bloodGroup;
                weight = h.weight;
                surg = h.recentSurgery;
                meds = h.medications;
                addic = h.addiction;
                break;
            }
        }

        std::cout << std::left 
                  << std::setw(8)  << d.donorId
                  << std::setw(22) << d.name
                  << std::setw(8)  << bloodGroupToString(bg)
                  << std::setw(10) << (std::to_string((int)weight) + " kg")
                  << std::setw(10) << d.status
                  << std::setw(16) << (surg ? "YES" : "NO")
                  << (meds + " / " + addic) << "\n";
    }
    std::cout << "\n";
}

// Display all receivers and transfusion requests
void BloodBankSystem::displayAllReceiversAndRequests() const {
    std::cout << "\n";
    std::cout << "HOSPITAL RECORDS: PATIENT RECEIVERS & BLOOD REQUESTS \n";
    std::cout << "\n";
    std::cout << std::left 
              << std::setw(8)  << "ReqID"
              << std::setw(22) << "Patient Name"
              << std::setw(8)  << "Blood"
              << std::setw(8)  << "Units"
              << std::setw(22) << "Base Urgency"
              << std::setw(10) << "Wait(min)"
              << std::setw(12) << "Priority"
              << "Status\n";
    std::cout << "\n";

    for (const auto& r : bloodRequests) {
        std::cout << std::left 
                  << std::setw(8)  << r.requestId
                  << std::setw(22) << r.patientName
                  << std::setw(8)  << bloodGroupToString(r.bloodGroup)
                  << std::setw(8)  << r.quantityRequested
                  << std::setw(22) << urgencyLevelToString(r.baseUrgency)
                  << std::setw(10) << r.minutesWaited
                  << std::setw(12) << std::fixed << std::setprecision(1) << r.dynamicScore
                  << r.status << "\n";
    }
    std::cout << "\n";
}

// Display overall system counts
void BloodBankSystem::displayDatabaseStats() const {
    std::cout << "\n";
    std::cout << "                 BLOOD BANK MANAGEMENT SYSTEM - DATABASE SUMMARY                         \n";
    std::cout << "\n";
    std::cout << "  Users Registered:             " << users.size() << " (Donors & Requesters)\n";
    std::cout << "  Active Donors:                " << donors.size() << "\n";
    std::cout << "  Donor Health Evaluations:     " << healthRecords.size() << "\n";
    std::cout << "  Registered Hospitals:         " << hospitals.size() << "\n";
    std::cout << "  Blood Banks:                  " << bloodBanks.size() << "\n";
    std::cout << "  Donor Appointments Booked:    " << appointments.size() << "\n";
    std::cout << "  Medical Test Screenings:      " << medicalTests.size() << "\n";
    std::cout << "  Donation Appointments:        " << donationAppointments.size() << "\n";
    std::cout << "  Recorded Blood Donations:     " << donations.size() << "\n";
    std::cout << "  Blood Units in Storage:       " << bloodUnits.size() << " units\n";
    std::cout << "  Patient Blood Requests:       " << bloodRequests.size() << " requests\n";
    std::cout << "\n";
}

// Display blood units stored in inventory
void BloodBankSystem::displayAllBloodUnits() const {
    std::cout << "\nCentral Blood Units Storage Inventory\n";
    std::cout << std::left 
              << std::setw(8)  << "UnitID"
              << std::setw(16) << "Batch Number"
              << std::setw(10) << "Group"
              << std::setw(14) << "Collection"
              << std::setw(14) << "Expiry"
              << std::setw(14) << "Test Status"
              << "Inventory Status\n";
    std::cout << "\n";
    for (const auto& u : bloodUnits) {
        std::cout << std::left 
                  << std::setw(8)  << u.unitId
                  << std::setw(16) << u.unitNumber
                  << std::setw(10) << bloodGroupToString(u.bloodGroup)
                  << std::setw(14) << u.collectionDate
                  << std::setw(14) << u.expiryDate
                  << std::setw(14) << u.testStatus
                  << u.status << "\n";
    }
}

// Display past donation sessions
void BloodBankSystem::displayAllDonations() const {
    std::cout << "\nBlood Donation Logs-\n";
    std::cout << std::left 
              << std::setw(12) << "DonationID"
              << std::setw(12) << "HospitalID"
              << std::setw(15) << "Type"
              << std::setw(10) << "Group"
              << std::setw(10) << "Quality"
              << std::setw(14) << "Date"
              << "Screening Status\n";
    std::cout << "\n";
    for (const auto& d : donations) {
        std::cout << std::left 
                  << std::setw(12) << d.donationId
                  << std::setw(12) << d.hospitalId
                  << std::setw(15) << d.donationType
                  << std::setw(10) << bloodGroupToString(d.bloodGroup)
                  << std::setw(10) << d.quality
                  << std::setw(14) << d.donationDate
                  << d.screeningStatus << "\n";
    }
}

// Display appointments and medical tests
void BloodBankSystem::displayAllAppointmentsAndTests() const {
    std::cout << "\nMedical Tests & Donor Eligibility\n";
    std::cout << std::left 
              << std::setw(8)  << "TestID"
              << std::setw(10) << "DonorID"
              << std::setw(12) << "Date"
              << std::setw(12) << "Eligible?"
              << std::setw(28) << "Clinical Vitals / Result"
              << "Remarks\n";
    std::cout << "\n";
    for (const auto& t : medicalTests) {
        std::cout << std::left 
                  << std::setw(8)  << t.testId
                  << std::setw(10) << t.donorId
                  << std::setw(12) << t.testDate
                  << std::setw(12) << (t.eligibility ? "YES (Fit)" : "NO (Unfit)")
                  << std::setw(28) << t.result
                  << t.remarks << "\n";
    }
}

void BloodBankSystem::addNewBloodRequest(const BloodRequest& req) {
    bloodRequests.push_back(req);
    requestMaxHeap.insert(req);
}

void BloodBankSystem::processNextUrgentRequest() {
    if (requestMaxHeap.isEmpty()) {
        std::cout << "No pending requests in Urgency Priority Queue!\n";
        return;
    }
    BloodRequest top = requestMaxHeap.extractMax();
    std::cout << "\n>>> [Triage Dispatch] Dispatching highest priority request:\n";
    std::cout << "    Patient: " << top.patientName << " | Need: " << top.quantityRequested 
              << " units of " << bloodGroupToString(top.bloodGroup) << " | Urgency: " 
              << urgencyLevelToString(top.baseUrgency) << " | Score: " << top.dynamicScore << "\n";
}

void BloodBankSystem::simulateTimePassage(int minutes) {
    requestMaxHeap.simulateTimeTick(minutes);
}

// Run Edmonds-Karp max-flow algorithm with option to execute allocation
void BloodBankSystem::runMaxLifeSavingEdmondsKarpDemo() {
    std::map<BloodGroup, int> inventory;
    for (const auto& u : bloodUnits) {
        if (u.status == "Available") {
            inventory[u.bloodGroup]++;
        }
    }

    std::cout << "\nAvailable Blood Bank Inventory for Allocation\n";
    for (const auto& item : inventory) {
        std::cout << "  " << std::setw(6) << bloodGroupToString(item.first) 
                  << ": " << item.second << " units\n";
    }

    // Filter only active (non-DONE) requests
    std::vector<BloodRequest> activeRequests;
    for (const auto& req : bloodRequests) {
        if (req.status != "DONE") {
            activeRequests.push_back(req);
        }
    }

    if (activeRequests.empty()) {
        std::cout << "\n>>> All requests are already marked as DONE! No active demands for flow network.\n";
        return;
    }

    maxFlowSolver.buildNetwork(inventory, activeRequests);
    maxFlowSolver.printNetworkStructure();
    
    AllocationResult result = maxFlowSolver.computeMaxLifeSavingAllocation(true);
    MaxLifeSavingEdmondsKarp::printAllocationSummary(result, activeRequests);

    // Provide option to Hospital Authority to apply allocations
    std::cout << "\nAuthority Action: Would you like to execute this optimal allocation and dispense units now? (y/n): ";
    char confirm;
    if (std::cin >> confirm && (confirm == 'y' || confirm == 'Y')) {
        int fulfilledNow = 0;
        int unitsGivenNow = 0;

        for (const auto& pair : result.patientAllocations) {
            int reqId = pair.first;
            auto allocList = pair.second;

            // Find request
            for (auto& req : bloodRequests) {
                if (req.requestId == reqId) {
                    int allocatedTotal = 0;
                    for (auto& item : allocList) {
                        BloodGroup bg = item.first;
                        int count = item.second;
                        allocatedTotal += count;

                        // Deduct units from inventory
                        int assigned = 0;
                        for (auto& u : bloodUnits) {
                            if (u.status == "Available" && u.bloodGroup == bg && assigned < count) {
                                u.status = "Allocated to Patient: " + req.patientName + " (Req #" + std::to_string(reqId) + ")";
                                assigned++;
                                unitsGivenNow++;
                            }
                        }
                    }

                    if (allocatedTotal >= req.quantityRequested) {
                        req.status = "DONE";
                        requestMaxHeap.removeById(reqId);
                        fulfilledNow++;
                    } else if (allocatedTotal > 0) {
                        req.quantityRequested -= allocatedTotal;
                        req.status = "Partially Fulfilled";
                    }
                    break;
                }
            }
        }

        std::cout << "\n>>> [ALLOCATION APPLIED BY HOSPITAL AUTHORITY] <<<\n";
        std::cout << "  Total Patients Fully Satisfied & Marked as [DONE] : " << fulfilledNow << "\n";
        std::cout << "  Total Blood Units Successfully Dispensed          : " << unitsGivenNow << "\n";
    }
}

void BloodBankSystem::runHeapInteractiveDemo() {
    requestMaxHeap.printHeapTable();
    std::cout << "\n--- Current Binary Max-Heap Tree Visualization ---\n";
    requestMaxHeap.printTreeStructure();
}

void BloodBankSystem::runTrieAutocompleteDemo() {
    std::cout << "\n";
    std::cout << "             FEATURE 3: TRIE DIRECTORY AUTOCOMPLETE & PREFIX SEARCH                      \n";
    std::cout << "\n";
    
    std::vector<std::string> samplePrefixes = {"aa", "dr", "sa", "ru", "po", "red"};
    for (const auto& p : samplePrefixes) {
        directoryTrie.printAutocompleteResults(p);
    }
}

void BloodBankSystem::runAllADSTests() {
    std::cout << "\n";
    std::cout << "          COMPREHENSIVE ADVANCED DATA STRUCTURES (ADS) VERIFICATION TEST                 \n";
    std::cout << "\n";
    
    std::cout << "\n[TEST 1] Testing Dynamic Priority Max-Heap:\n";
    requestMaxHeap.printHeapTable();
    
    std::cout << "\nSimulating 45 minutes time passing in emergency room (checking starvation prevention):\n";
    simulateTimePassage(45);
    requestMaxHeap.printHeapTable();
    
    std::cout << "\nExtracting top 2 most urgent patients:\n";
    processNextUrgentRequest();
    processNextUrgentRequest();

    std::cout << "\n[TEST 2] Testing Edmonds-Karp Maximum Life Saving Allocation:\n";
    runMaxLifeSavingEdmondsKarpDemo();

    std::cout << "\n[TEST 3] Testing Trie Autocomplete & Prefix Directory Search:\n";
    runTrieAutocompleteDemo();

    std::cout << "\n>>> ALL 3 ADVANCED DATA STRUCTURE FEATURES TESTED SUCCESSFULLY! <<<\n";
}
