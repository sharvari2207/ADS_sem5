#ifndef BLOOD_REQUEST_HEAP_H
#define BLOOD_REQUEST_HEAP_H

#include "ERModels.h"
#include <vector>
#include <string>

// Max-Heap implementation to prioritize blood requests dynamically
class BloodRequestHeap {
private:
    std::vector<BloodRequest> heap;

    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    BloodRequestHeap();

    // Standard heap operations
    void insert(BloodRequest request, bool silent = false);
    BloodRequest extractMax();
    BloodRequest peekMax() const;
    bool isEmpty() const;
    size_t size() const;

    // Manage dynamic priorities and status
    void simulateTimeTick(int minutesPassed);
    bool removeById(int requestId);
    void rebuildHeap();

    // Queries and display
    int findIndexById(int requestId) const;
    const std::vector<BloodRequest>& getAllRequests() const { return heap; }
    void printHeapTable() const;
    void printTreeStructure(int index = 0, int depth = 0) const;
};

#endif // BLOOD_REQUEST_HEAP_H
