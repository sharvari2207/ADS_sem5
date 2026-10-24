#ifndef ER_MODELS_H
#define ER_MODELS_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

// Standard blood groups
enum class BloodGroup {
    O_NEG, O_POS,
    A_NEG, A_POS,
    B_NEG, B_POS,
    AB_NEG, AB_POS,
    UNKNOWN
};

inline std::string bloodGroupToString(BloodGroup bg) {
    switch (bg) {
        case BloodGroup::O_NEG:  return "O-";
        case BloodGroup::O_POS:  return "O+";
        case BloodGroup::A_NEG:  return "A-";
        case BloodGroup::A_POS:  return "A+";
        case BloodGroup::B_NEG:  return "B-";
        case BloodGroup::B_POS:  return "B+";
        case BloodGroup::AB_NEG: return "AB-";
        case BloodGroup::AB_POS: return "AB+";
        default: return "Unknown";
    }
}

inline BloodGroup stringToBloodGroup(const std::string& str) {
    if (str == "O-" || str == "O_NEG" || str == "o-") return BloodGroup::O_NEG;
    if (str == "O+" || str == "O_POS" || str == "o+") return BloodGroup::O_POS;
    if (str == "A-" || str == "A_NEG" || str == "a-") return BloodGroup::A_NEG;
    if (str == "A+" || str == "A_POS" || str == "a+") return BloodGroup::A_POS;
    if (str == "B-" || str == "B_NEG" || str == "b-") return BloodGroup::B_NEG;
    if (str == "B+" || str == "B_POS" || str == "b+") return BloodGroup::B_POS;
    if (str == "AB-" || str == "AB_NEG" || str == "ab-") return BloodGroup::AB_NEG;
    if (str == "AB+" || str == "AB_POS" || str == "ab+") return BloodGroup::AB_POS;
    return BloodGroup::UNKNOWN;
}

// Checks if donor blood can be given to recipient
inline bool isBloodCompatible(BloodGroup donor, BloodGroup recipient) {
    if (donor == BloodGroup::O_NEG) return true; // Universal donor
    if (recipient == BloodGroup::AB_POS) return true; // Universal recipient
    if (donor == BloodGroup::O_POS) {
        return (recipient == BloodGroup::O_POS || recipient == BloodGroup::A_POS ||
                recipient == BloodGroup::B_POS || recipient == BloodGroup::AB_POS);
    }
    if (donor == BloodGroup::A_NEG) {
        return (recipient == BloodGroup::A_NEG || recipient == BloodGroup::A_POS ||
                recipient == BloodGroup::AB_NEG || recipient == BloodGroup::AB_POS);
    }
    if (donor == BloodGroup::A_POS) {
        return (recipient == BloodGroup::A_POS || recipient == BloodGroup::AB_POS);
    }
    if (donor == BloodGroup::B_NEG) {
        return (recipient == BloodGroup::B_NEG || recipient == BloodGroup::B_POS ||
                recipient == BloodGroup::AB_NEG || recipient == BloodGroup::AB_POS);
    }
    if (donor == BloodGroup::B_POS) {
        return (recipient == BloodGroup::B_POS || recipient == BloodGroup::AB_POS);
    }
    if (donor == BloodGroup::AB_NEG) {
        return (recipient == BloodGroup::AB_NEG || recipient == BloodGroup::AB_POS);
    }
    return (donor == recipient);
}

// Base user account
struct User {
    int userId;
    std::string name;
    std::string password;
    std::string email;
    std::vector<std::string> phones;
    std::string role;
    std::string address;
    std::string dob;
    std::string gender;
    int age;
};

// Registered blood donor
struct Donor {
    int donorId;
    int userId;
    std::string name;
    std::string status;
    std::string resignationDate;
};

// Requester placing orders on behalf of patients
struct Requester {
    int requesterId;
    int userId;
    std::string name;
    std::string hospitalName;
};

// Medical fitness details for a donor
struct DonorHealth {
    int healthId;
    int donorId;
    double weight;
    double height;
    BloodGroup bloodGroup;
    std::string medications;
    std::vector<std::string> previousIllness;
    bool recentSurgery;
    std::string lastDonationDate;
    std::string addiction;

    bool isFitToDonate() const {
        return (weight >= 45.0 && !recentSurgery && medications == "None");
    }
};

// Hospital facility
struct Hospital {
    int hospitalId;
    std::string name;
    std::string email;
    std::string phone;
    std::string address;
};

// Blood bank facility
struct BloodBank {
    int bloodBankId;
    std::string name;
    std::string email;
    std::string phone;
    std::string address;
};

// Donor visit appointment
struct Appointment {
    int applicationId;
    int donorId;
    int hospitalId;
    std::string timeSlot;
    std::string appointmentStatus;
    std::string status;
};

// Pre-donation screening test
struct MedicalTest {
    int testId;
    int donorId;
    int appointmentId;
    std::string testDate;
    std::string result;
    bool eligibility;
    std::string remarks;
};

// Scheduled blood draw appointment
struct DonationAppointment {
    int donationApptId;
    int donorId;
    int hospitalId;
    std::string appointmentDate;
    std::string timeSlot;
    std::string status;
    std::string createdAt;
};

// Record of completed donation
struct Donation {
    int donationId;
    int hospitalId;
    std::string donationType;
    std::string donationDate;
    std::string quality;
    BloodGroup bloodGroup;
    std::string screeningStatus;

    Donation(int dId, int hId, const std::string& type, const std::string& date,
             const std::string& qual, BloodGroup bg, const std::string& screen)
        : donationId(dId), hospitalId(hId), donationType(type), donationDate(date),
          quality(qual), bloodGroup(bg), screeningStatus(screen) {}
};

// Individual blood unit packet stored in blood bank
struct BloodUnit {
    int unitId;
    int bloodBankId;
    int donationId;
    std::string unitNumber;
    BloodGroup bloodGroup;
    std::string collectionDate;
    std::string expiryDate;
    std::string status;     // "Available", "Allocated", "Expired"
    std::string testStatus; // "Tested-Safe", "Pending"
};

// Priority categories for patient urgency
enum class UrgencyLevel {
    ROUTINE = 1,
    MODERATE = 2,
    URGENT = 3,
    CRITICAL = 4
};

inline std::string urgencyLevelToString(UrgencyLevel ul) {
    switch (ul) {
        case UrgencyLevel::CRITICAL: return "CRITICAL";
        case UrgencyLevel::URGENT:   return "URGENT";
        case UrgencyLevel::MODERATE: return "MODERATE";
        case UrgencyLevel::ROUTINE:  return "ROUTINE";
        default: return "Normal";
    }
}

// Patient blood request placed by hospital or receiver
struct BloodRequest {
    int requestId;
    int requesterId;
    std::string patientName;
    BloodGroup bloodGroup;
    int quantityRequested;
    UrgencyLevel baseUrgency;
    std::string status; // "Pending", "DONE", "Partially Fulfilled"
    int patientAge;
    bool isICUorTrauma;
    int minutesWaited;
    double dynamicScore;

    BloodRequest() 
        : requestId(0), requesterId(0), patientName(""), bloodGroup(BloodGroup::UNKNOWN),
          quantityRequested(1), baseUrgency(UrgencyLevel::ROUTINE), status("Pending"),
          patientAge(30), isICUorTrauma(false), minutesWaited(0), dynamicScore(0.0) {}

    BloodRequest(int reqId, int reqrId, const std::string& pName, BloodGroup bg, 
                 int qty, UrgencyLevel urg, int age = 30, bool trauma = false, int waitMins = 0)
        : requestId(reqId), requesterId(reqrId), patientName(pName), bloodGroup(bg),
          quantityRequested(qty), baseUrgency(urg), status("Pending"),
          patientAge(age), isICUorTrauma(trauma), minutesWaited(waitMins), dynamicScore(0.0) {
        calculateDynamicPriority();
    }

    // Calculates priority score considering urgency and wait time
    double calculateDynamicPriority() {
        double score = 0.0;
        switch (baseUrgency) {
            case UrgencyLevel::CRITICAL: score += 1000.0; break;
            case UrgencyLevel::URGENT:   score += 700.0; break;
            case UrgencyLevel::MODERATE: score += 400.0; break;
            case UrgencyLevel::ROUTINE:  score += 100.0; break;
        }

        // Add points for time waited to avoid starvation
        score += (minutesWaited * 5.0);

        if (isICUorTrauma) score += 200.0;
        if (patientAge < 10 || patientAge > 65) score += 150.0;
        if (bloodGroup == BloodGroup::O_NEG || bloodGroup == BloodGroup::AB_NEG) {
            score += 50.0;
        }

        dynamicScore = score;
        return dynamicScore;
    }
};

#endif // ER_MODELS_H
