#ifndef BLOOD_REQUEST_HEAP_H
#define BLOOD_REQUEST_HEAP_H

#include "ERModels.h"
#include <vector>
#include <string>

// Priority queue using Max-Heap for patient blood requests
class BloodRequestHeap {
private:
    std::vector<BloodRequest> heap;

    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }

    // Heapify helpers to restore order
    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    BloodRequestHeap();

    // Core queue operations
    void insert(BloodRequest request, bool silent = false);
    BloodRequest extractMax();
    BloodRequest peekMax() const;
    bool isEmpty() const;
    size_t size() const;

    // Priority updates and removals
    void simulateTimeTick(int minutesPassed);
    bool removeById(int requestId);
    bool updatePatientUrgency(int requestId, UrgencyLevel newLevel, bool isICU);
    bool addWaitTime(int requestId, int additionalMinutes);
    void rebuildHeap();

    // Lookups and display
    int findIndexById(int requestId) const;
    const std::vector<BloodRequest>& getAllRequests() const { return heap; }
    void printHeapTable() const;
    void printTreeStructure(int index = 0, int depth = 0) const;
};

#endif // BLOOD_REQUEST_HEAP_H
