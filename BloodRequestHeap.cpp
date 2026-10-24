#include "BloodRequestHeap.h"
#include <iomanip>
#include <algorithm>

BloodRequestHeap::BloodRequestHeap() {}

void BloodRequestHeap::heapifyUp(int index) {
    while (index > 0 && heap[index].dynamicScore > heap[parent(index)].dynamicScore) {
        std::swap(heap[index], heap[parent(index)]);
        index = parent(index);
    }
}

void BloodRequestHeap::heapifyDown(int index) {
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

void BloodRequestHeap::insert(BloodRequest request) {
    request.calculateDynamicPriority();
    heap.push_back(request);
    heapifyUp(static_cast<int>(heap.size()) - 1);
    std::cout << "[MaxHeap] Enqueued Request #" << request.requestId 
              << " (" << request.patientName << ", " << bloodGroupToString(request.bloodGroup)
              << ") with Dynamic Priority: " << std::fixed << std::setprecision(1) << request.dynamicScore << "\n";
}

BloodRequest BloodRequestHeap::extractMax() {
    if (heap.empty()) {
        throw std::runtime_error("Cannot extractMax: Dynamic Max-Heap is empty!");
    }

    BloodRequest top = heap[0];
    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty()) {
        heapifyDown(0);
    }

    std::cout << "[MaxHeap] Extracted Top Priority Request #" << top.requestId 
              << " (" << top.patientName << ", Score: " << std::fixed << std::setprecision(1) << top.dynamicScore 
              << ", Urgency: " << urgencyLevelToString(top.baseUrgency) << ")\n";

    return top;
}

BloodRequest BloodRequestHeap::peekMax() const {
    if (heap.empty()) {
        throw std::runtime_error("Cannot peekMax: Dynamic Max-Heap is empty!");
    }
    return heap[0];
}

bool BloodRequestHeap::isEmpty() const {
    return heap.empty();
}

size_t BloodRequestHeap::size() const {
    return heap.size();
}

int BloodRequestHeap::findIndexById(int requestId) const {
    for (size_t i = 0; i < heap.size(); ++i) {
        if (heap[i].requestId == requestId) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Simulates real-time aging: as minutes pass, waiting requests escalate dynamically!
void BloodRequestHeap::simulateTimeTick(int minutesPassed) {
    std::cout << "\n>>> [MaxHeap Simulation] " << minutesPassed << " minutes passed in the Emergency Ward...\n";
    for (auto& req : heap) {
        req.minutesWaited += minutesPassed;
        double oldScore = req.dynamicScore;
        req.calculateDynamicPriority();
        std::cout << "    Request #" << req.requestId << " (" << req.patientName 
                  << "): Dynamic Score changed from " << oldScore << " -> " << req.dynamicScore << "\n";
    }
    rebuildHeap();
}

void BloodRequestHeap::rebuildHeap() {
    // Floyd's O(N) heap construction algorithm
    int n = static_cast<int>(heap.size());
    for (int i = (n / 2) - 1; i >= 0; --i) {
        heapifyDown(i);
    }
    std::cout << "[MaxHeap] Floyd's algorithm re-balanced heap dynamically. New Top: Request #" 
              << (heap.empty() ? 0 : heap[0].requestId) << "\n";
}

bool BloodRequestHeap::updatePatientUrgency(int requestId, UrgencyLevel newLevel, bool isICU) {
    int idx = findIndexById(requestId);
    if (idx == -1) return false;

    heap[idx].baseUrgency = newLevel;
    if (isICU) heap[idx].isICUorTrauma = true;
    double oldScore = heap[idx].dynamicScore;
    heap[idx].calculateDynamicPriority();

    std::cout << "[MaxHeap] Updated Condition for Request #" << requestId 
              << " (" << heap[idx].patientName << "): Score " << oldScore << " -> " << heap[idx].dynamicScore << "\n";

    if (heap[idx].dynamicScore > oldScore) {
        heapifyUp(idx);
    } else {
        heapifyDown(idx);
    }
    return true;
}

bool BloodRequestHeap::addWaitTime(int requestId, int additionalMinutes) {
    int idx = findIndexById(requestId);
    if (idx == -1) return false;

    heap[idx].minutesWaited += additionalMinutes;
    double oldScore = heap[idx].dynamicScore;
    heap[idx].calculateDynamicPriority();

    std::cout << "[MaxHeap] Increased Wait Time for Request #" << requestId 
              << " by " << additionalMinutes << " mins. Score: " << oldScore << " -> " << heap[idx].dynamicScore << "\n";

    if (heap[idx].dynamicScore > oldScore) {
        heapifyUp(idx);
    } else {
        heapifyDown(idx);
    }
    return true;
}

void BloodRequestHeap::printHeapTable() const {
    std::cout << "\n=========================================================================================\n";
    std::cout << "                      CURRENT DYNAMIC URGENCY MAX-HEAP QUEUE                             \n";
    std::cout << "=========================================================================================\n";
    std::cout << std::left 
              << std::setw(5)  << "Pos"
              << std::setw(8)  << "ReqID"
              << std::setw(18) << "Patient Name"
              << std::setw(8)  << "Blood"
              << std::setw(6)  << "Units"
              << std::setw(20) << "Base Urgency"
              << std::setw(10) << "Wait(min)"
              << std::setw(8)  << "ICU/Age"
              << std::setw(12) << "Dyn.Score" << "\n";
    std::cout << "-----------------------------------------------------------------------------------------\n";

    for (size_t i = 0; i < heap.size(); ++i) {
        const auto& r = heap[i];
        std::string extraInfo = (r.isICUorTrauma ? "ICU/" : "") + std::to_string(r.patientAge) + "y";
        std::cout << std::left 
                  << std::setw(5)  << i
                  << std::setw(8)  << r.requestId
                  << std::setw(18) << r.patientName
                  << std::setw(8)  << bloodGroupToString(r.bloodGroup)
                  << std::setw(6)  << r.quantityRequested
                  << std::setw(20) << urgencyLevelToString(r.baseUrgency)
                  << std::setw(10) << r.minutesWaited
                  << std::setw(8)  << extraInfo
                  << std::setw(12) << std::fixed << std::setprecision(1) << r.dynamicScore << "\n";
    }
    std::cout << "=========================================================================================\n";
}

void BloodRequestHeap::printTreeStructure(int index, int depth) const {
    if (index >= static_cast<int>(heap.size())) return;

    printTreeStructure(rightChild(index), depth + 1);

    for (int i = 0; i < depth; ++i) std::cout << "       ";
    std::cout << "|--[#" << heap[index].requestId << ":" << heap[index].patientName 
              << " (" << std::fixed << std::setprecision(0) << heap[index].dynamicScore << ")]\n";

    printTreeStructure(leftChild(index), depth + 1);
}

