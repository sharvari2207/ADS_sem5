#ifndef MAX_LIFE_SAVING_EDMONDS_KARP_H
#define MAX_LIFE_SAVING_EDMONDS_KARP_H

#include "ERModels.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>

/**
 * FEATURE 2: Max Life Saving Blood Allocation using Edmonds-Karp Algorithm
 * 
 * Context in Advance Data Structures:
 * - Solves the Maximum Flow problem on a bipartite compatibility network.
 * - Uses BFS (Breadth-First Search) to find augmenting paths, ensuring O(V * E^2) worst-case time complexity.
 * - Overcomes greedy allocation pitfalls:
 *   E.g., if rare O- is greedily handed to an AB+ patient, a critical O- patient will die.
 *   Edmonds-Karp finds the globally optimal flow that maximizes the total units/lives saved!
 */

struct AllocationResult {
    int totalUnitsAllocated;
    int fullySatisfiedPatients;
    int partiallySatisfiedPatients;
    int unsatisfiedPatients;
    
    // Mapping: Request ID -> vector of pair<BloodGroup (allocated type), count>
    std::map<int, std::vector<std::pair<BloodGroup, int>>> patientAllocations;
    // Remaining units in Blood Bank inventory
    std::map<BloodGroup, int> remainingInventory;
};

class MaxLifeSavingEdmondsKarp {
private:
    int totalNodes;
    int sourceNode;
    int sinkNode;
    
    // Residual graph capacity matrix
    std::vector<std::vector<int>> capacity;
    // Actual flow matrix
    std::vector<std::vector<int>> flow;
    // Adjacency list for fast BFS traversal
    std::vector<std::vector<int>> adj;

    // Helper mappings
    std::vector<BloodGroup> bloodGroupTypes;
    std::map<BloodGroup, int> groupToNode;
    std::map<int, BloodGroup> nodeToGroup;
    std::map<int, int> requestToNode;
    std::map<int, BloodRequest> nodeToRequest;

    // BFS to find the shortest augmenting path in the residual graph
    bool bfsAugmentingPath(std::vector<int>& parentPath, int& bottleneckCapacity);

public:
    MaxLifeSavingEdmondsKarp();

    // Builds the flow network from blood bank inventory and active blood requests
    void buildNetwork(const std::map<BloodGroup, int>& inventory, 
                      const std::vector<BloodRequest>& activeRequests);

    // Runs Edmonds-Karp Max Flow algorithm using BFS
    AllocationResult computeMaxLifeSavingAllocation(bool verbose = true);

    // Display tools
    void printNetworkStructure() const;
    static void printAllocationSummary(const AllocationResult& result, 
                                      const std::vector<BloodRequest>& requests);
};

#endif // MAX_LIFE_SAVING_EDMONDS_KARP_H

