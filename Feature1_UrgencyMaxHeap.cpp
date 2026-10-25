

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

enum class BloodGroup {
    O_NEG, O_POS, A_NEG, A_POS, B_NEG, B_POS, AB_NEG, AB_POS, UNKNOWN
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

enum class UrgencyLevel {
    ROUTINE = 1,
    MODERATE = 2,
    URGENT = 3,
    CRITICAL = 4
};

inline std::string urgencyLevelToString(UrgencyLevel ul) {
    switch (ul) {
        case UrgencyLevel::CRITICAL: return "CRITICAL (Emergency)";
        case UrgencyLevel::URGENT:   return "URGENT (High)";
        case UrgencyLevel::MODERATE: return "MODERATE (Medium)";
        case UrgencyLevel::ROUTINE:  return "ROUTINE (Low)";
        default: return "Unknown";
    }
}

// Patient blood request model
struct BloodRequest {
    int requestId;
    int requesterId;
    std::string patientName;
    BloodGroup bloodGroup;
    int quantityRequested;
    UrgencyLevel baseUrgency;
    std::string status;
    int patientAge;
    bool isICUorTrauma;
    int minutesWaited;
    double dynamicScore;

    BloodRequest(int reqId, int reqrId, const std::string& pName, BloodGroup bg, 
                 int qty, UrgencyLevel urg, int age = 30, bool trauma = false, int waitMins = 0)
        : requestId(reqId), requesterId(reqrId), patientName(pName), bloodGroup(bg),
          quantityRequested(qty), baseUrgency(urg), status("Pending"),
          patientAge(age), isICUorTrauma(trauma), minutesWaited(waitMins), dynamicScore(0.0) {
        calculateDynamicPriority();
    }

    double calculateDynamicPriority() {
        double score = 0.0;
        switch (baseUrgency) {
            case UrgencyLevel::CRITICAL: score += 1000.0; break;
            case UrgencyLevel::URGENT:   score += 700.0; break;
            case UrgencyLevel::MODERATE: score += 400.0; break;
            case UrgencyLevel::ROUTINE:  score += 100.0; break;
        }
        score += (minutesWaited * 5.0); // Starvation prevention
        if (isICUorTrauma) score += 200.0;
        if (patientAge < 10 || patientAge > 65) score += 150.0;
        if (bloodGroup == BloodGroup::O_NEG || bloodGroup == BloodGroup::AB_NEG) score += 50.0;

        dynamicScore = score;
        return dynamicScore;
    }
};

class BloodRequestMaxHeap {
private:
    std::vector<BloodRequest> heap;

    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }

    void heapifyUp(int index) {
        while (index > 0 && heap[index].dynamicScore > heap[parent(index)].dynamicScore) {
            std::swap(heap[index], heap[parent(index)]);
            index = parent(index);
        }
    }

    void heapifyDown(int index) {
        int maxIndex = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left < static_cast<int>(heap.size()) && heap[left].dynamicScore > heap[maxIndex].dynamicScore) {
            maxIndex = left;
        }
        if (right < static_cast<int>(heap.size()) && heap[right].dynamicScore > heap[maxIndex].dynamicScore) {
            maxIndex = right;
        }
        if (index != maxIndex) {
            std::swap(heap[index], heap[maxIndex]);
            heapifyDown(maxIndex);
        }
    }

public:
    void insert(BloodRequest request) {
        request.calculateDynamicPriority();
        heap.push_back(request);
        heapifyUp(static_cast<int>(heap.size()) - 1);
        std::cout << " [MaxHeap] Enqueued Req #" << request.requestId 
                  << " (" << request.patientName << ", " << bloodGroupToString(request.bloodGroup)
                  << ") -> Dynamic Urgency Score: " << std::fixed << std::setprecision(1) << request.dynamicScore << "\n";
    }

    BloodRequest extractMax() {
        if (heap.empty()) throw std::runtime_error("Heap is empty!");
        BloodRequest top = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
        return top;
    }

    bool isEmpty() const { return heap.empty(); }
    size_t size() const { return heap.size(); }

    void simulateTimeTick(int minutes) {
        std::cout << "\n>>> [Time Simulation] " << minutes << " minutes elapsed in Emergency Ward...\n";
        for (auto& req : heap) {
            req.minutesWaited += minutes;
            double old = req.dynamicScore;
            req.calculateDynamicPriority();
            std::cout << "    Req #" << req.requestId << " (" << req.patientName << "): Score " 
                      << old << " -> " << req.dynamicScore << "\n";
        }
        // Re-heapify in O(N) using Floyd's algorithm
        int n = static_cast<int>(heap.size());
        for (int i = (n / 2) - 1; i >= 0; --i) {
            heapifyDown(i);
        }
        std::cout << ">>> Re-heapified dynamically. Top request is now: #" << heap[0].requestId << " (" << heap[0].patientName << ")\n";
    }

    void printTable() const {
        std::cout << "\n";
        std::cout << "                   DYNAMIC PRIORITY MAX-HEAP: ACTIVE BLOOD REQUESTS                      \n";
        std::cout << "\n";
        std::cout << std::left 
                  << std::setw(5)  << "Pos"
                  << std::setw(8)  << "ReqID"
                  << std::setw(22) << "Patient Name"
                  << std::setw(8)  << "Blood"
                  << std::setw(7)  << "Units"
                  << std::setw(22) << "Base Urgency"
                  << std::setw(10) << "Wait(m)"
                  << std::setw(12) << "Dyn.Score" << "\n";
        std::cout << "\n";
        for (size_t i = 0; i < heap.size(); ++i) {
            const auto& r = heap[i];
            std::cout << std::left 
                      << std::setw(5)  << i
                      << std::setw(8)  << r.requestId
                      << std::setw(22) << r.patientName
                      << std::setw(8)  << bloodGroupToString(r.bloodGroup)
                      << std::setw(7)  << r.quantityRequested
                      << std::setw(22) << urgencyLevelToString(r.baseUrgency)
                      << std::setw(10) << r.minutesWaited
                      << std::setw(12) << std::fixed << std::setprecision(1) << r.dynamicScore << "\n";
        }
        std::cout << "\n";
    }

    void printTree(int idx = 0, int depth = 0) const {
        if (idx >= static_cast<int>(heap.size())) return;
        printTree(rightChild(idx), depth + 1);
        for (int i = 0; i < depth; ++i) std::cout << "       ";
        std::cout << "|--[#" << heap[idx].requestId << ":" << heap[idx].patientName 
                  << " (" << std::fixed << std::setprecision(0) << heap[idx].dynamicScore << ")]\n";
        printTree(leftChild(idx), depth + 1);
    }
};

int main() {
   std::cout << "   FEATURE 1 DEMO: DYNAMIC PRIORITY MAX-HEAP FOR BLOOD REQUEST URGENCY  \n";

    BloodRequestMaxHeap heap;

    std::cout << "1. Inserting initial patient blood requests into Max-Heap:\n";
    heap.insert(BloodRequest(901, 1, "Kavita Rao", BloodGroup::AB_POS, 2, UrgencyLevel::CRITICAL, 29, true, 5));
    heap.insert(BloodRequest(902, 2, "Master Aarush (Infant)", BloodGroup::O_NEG, 1, UrgencyLevel::CRITICAL, 1, true, 10));
    heap.insert(BloodRequest(903, 1, "Mahesh Kadam", BloodGroup::B_POS, 2, UrgencyLevel::URGENT, 45, false, 20));
    heap.insert(BloodRequest(904, 3, "Suresh Gaikwad", BloodGroup::A_POS, 2, UrgencyLevel::URGENT, 68, false, 30));
    heap.insert(BloodRequest(905, 2, "Neha Sawant", BloodGroup::O_POS, 2, UrgencyLevel::MODERATE, 19, false, 60));
    heap.insert(BloodRequest(906, 3, "Dattatray Shinde", BloodGroup::O_POS, 1, UrgencyLevel::ROUTINE, 72, false, 120));

    heap.printTable();

    std::cout << "\n2. Visualizing Binary Max-Heap Tree Structure:\n";
    heap.printTree();

    std::cout << "\n3. Simulating passage of 45 minutes (Dynamic priority aging):\n";
    heap.simulateTimeTick(45);
    heap.printTable();

    std::cout << "\n4. Extracting top urgent requests (ExtractMax triage dispatch):\n";
    while (!heap.isEmpty()) {
        BloodRequest top = heap.extractMax();
        std::cout << "   [DISPATCHED] Patient: " << std::setw(22) << std::left << top.patientName 
                  << " | Urgency: " << urgencyLevelToString(top.baseUrgency) 
                  << " | Dynamic Score: " << top.dynamicScore << "\n";
    }

    std::cout << "\n Feature 1 Demo Complete!\n";
    return 0;
}

