#include "BloodRequestHeap.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

BloodRequestHeap::BloodRequestHeap() {}

// Bubble up to restore max-heap property
void BloodRequestHeap::heapifyUp(int index) {
    while (index > 0 && heap[index].dynamicScore > heap[parent(index)].dynamicScore) {
        std::swap(heap[index], heap[parent(index)]);
        index = parent(index);
    }
}

// Push down to restore max-heap property
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

// Enqueue new patient blood request
void BloodRequestHeap::insert(BloodRequest request, bool silent) {
    request.calculateDynamicPriority();
    heap.push_back(request);
    heapifyUp(static_cast<int>(heap.size()) - 1);
    if (!silent) {
        std::cout << "[Priority Queue] Enqueued Request #" << request.requestId 
                  << " (" << request.patientName << ", " << bloodGroupToString(request.bloodGroup)
                  << ") with Priority Score: " << std::fixed << std::setprecision(1) << request.dynamicScore << "\n";
    }
}

// Remove and return highest priority request
BloodRequest BloodRequestHeap::extractMax() {
    if (heap.empty()) {
        throw std::runtime_error("Priority Queue is empty!");
    }

    BloodRequest top = heap[0];
    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty()) {
        heapifyDown(0);
    }

    std::cout << "[Priority Queue] Dispatched Top Request #" << top.requestId 
              << " (" << top.patientName << ", Score: " << std::fixed << std::setprecision(1) << top.dynamicScore 
              << ", Urgency: " << urgencyLevelToString(top.baseUrgency) << ")\n";

    return top;
}

// View top priority request without removing
BloodRequest BloodRequestHeap::peekMax() const {
    if (heap.empty()) {
        throw std::runtime_error("Priority Queue is empty!");
    }
    return heap[0];
}

bool BloodRequestHeap::isEmpty() const {
    return heap.empty();
}

size_t BloodRequestHeap::size() const {
    return heap.size();
}

// Find position of request in heap vector
int BloodRequestHeap::findIndexById(int requestId) const {
    for (size_t i = 0; i < heap.size(); ++i) {
        if (heap[i].requestId == requestId) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Remove request by its unique ID (e.g. after fulfillment)
bool BloodRequestHeap::removeById(int requestId) {
    int idx = findIndexById(requestId);
    if (idx == -1) return false;

    // Replace with last element and restore heap
    heap[idx] = heap.back();
    heap.pop_back();

    if (idx < static_cast<int>(heap.size())) {
        heapifyDown(idx);
        heapifyUp(idx);
    }
    return true;
}

// Escalate priorities as wait time increases
void BloodRequestHeap::simulateTimeTick(int minutesPassed) {
    std::cout << "\n>>> [Triage Aging] " << minutesPassed << " minutes elapsed in Emergency Ward...\n";
    for (auto& req : heap) {
        req.minutesWaited += minutesPassed;
        double oldScore = req.dynamicScore;
        req.calculateDynamicPriority();
        std::cout << "    Request #" << req.requestId << " (" << req.patientName 
                  << "): Priority updated from " << oldScore << " -> " << req.dynamicScore << "\n";
    }
    rebuildHeap();
}

// Rebuild heap using Floyd's bottom-up method
void BloodRequestHeap::rebuildHeap() {
    int n = static_cast<int>(heap.size());
    for (int i = (n / 2) - 1; i >= 0; --i) {
        heapifyDown(i);
    }
}

// Update urgency level if patient condition changes
bool BloodRequestHeap::updatePatientUrgency(int requestId, UrgencyLevel newLevel, bool isICU) {
    int idx = findIndexById(requestId);
    if (idx == -1) return false;

    heap[idx].baseUrgency = newLevel;
    if (isICU) heap[idx].isICUorTrauma = true;
    double oldScore = heap[idx].dynamicScore;
    heap[idx].calculateDynamicPriority();

    std::cout << "[Priority Queue] Updated Condition for Request #" << requestId 
              << " (" << heap[idx].patientName << "): Score " << oldScore << " -> " << heap[idx].dynamicScore << "\n";

    if (heap[idx].dynamicScore > oldScore) {
        heapifyUp(idx);
    } else {
        heapifyDown(idx);
    }
    return true;
}

// Add wait minutes to a specific patient
bool BloodRequestHeap::addWaitTime(int requestId, int additionalMinutes) {
    int idx = findIndexById(requestId);
    if (idx == -1) return false;

    heap[idx].minutesWaited += additionalMinutes;
    double oldScore = heap[idx].dynamicScore;
    heap[idx].calculateDynamicPriority();

    std::cout << "[Priority Queue] Increased Wait Time for Request #" << requestId 
              << " by " << additionalMinutes << " mins. Score: " << oldScore << " -> " << heap[idx].dynamicScore << "\n";

    if (heap[idx].dynamicScore > oldScore) {
        heapifyUp(idx);
    } else {
        heapifyDown(idx);
    }
    return true;
}

// Display current queue table
void BloodRequestHeap::printHeapTable() const {
    std::cout << "\n=========================================================================================\n";
    std::cout << "                          CURRENT URGENCY PRIORITY QUEUE                                 \n";
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
              << std::setw(12) << "Priority" << "\n";
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

// Print tree representation of the binary max-heap
void BloodRequestHeap::printTreeStructure(int index, int depth) const {
    if (index >= static_cast<int>(heap.size())) return;

    printTreeStructure(rightChild(index), depth + 1);

    for (int i = 0; i < depth; ++i) std::cout << "       ";
    std::cout << "|--[#" << heap[index].requestId << ":" << heap[index].patientName 
              << " (" << std::fixed << std::setprecision(0) << heap[index].dynamicScore << ")]\n";

    printTreeStructure(leftChild(index), depth + 1);
}
