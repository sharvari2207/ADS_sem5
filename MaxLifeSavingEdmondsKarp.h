#ifndef MAX_LIFE_SAVING_EDMONDS_KARP_H
#define MAX_LIFE_SAVING_EDMONDS_KARP_H

#include "ERModels.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>

// Optimal life-saving blood allocation using Edmonds-Karp Max-Flow algorithm
struct AllocationResult {
    int totalUnitsAllocated;
    int fullySatisfiedPatients;
    int partiallySatisfiedPatients;
    int unsatisfiedPatients;
    
    // Request ID -> vector of allocated pairs (BloodGroup, quantity)
    std::map<int, std::vector<std::pair<BloodGroup, int>>> patientAllocations;
    // Remaining units in inventory
    std::map<BloodGroup, int> remainingInventory;
};

class MaxLifeSavingEdmondsKarp {
private:
    int totalNodes;
    int sourceNode;
    int sinkNode;
    
    // Residual network capacity and flow matrices
    std::vector<std::vector<int>> capacity;
    std::vector<std::vector<int>> flow;
    std::vector<std::vector<int>> adj;

    // Node mappings
    std::vector<BloodGroup> bloodGroupTypes;
    std::map<BloodGroup, int> groupToNode;
    std::map<int, BloodGroup> nodeToGroup;
    std::map<int, int> requestToNode;
    std::map<int, BloodRequest> nodeToRequest;

    // BFS to discover shortest augmenting path
    bool bfsAugmentingPath(std::vector<int>& parentPath, int& bottleneckCapacity);

public:
    MaxLifeSavingEdmondsKarp();

    // Construct flow network using current inventory and active blood requests
    void buildNetwork(const std::map<BloodGroup, int>& inventory, 
                      const std::vector<BloodRequest>& activeRequests);

    // Compute maximum units that can be allocated
    AllocationResult computeMaxLifeSavingAllocation(bool verbose = true);

    // Network visualization and reports
    void printNetworkStructure() const;
    static void printAllocationSummary(const AllocationResult& result, 
                                       const std::vector<BloodRequest>& requests);
};

#endif // MAX_LIFE_SAVING_EDMONDS_KARP_H
