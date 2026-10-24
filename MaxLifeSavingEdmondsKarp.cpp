#include "MaxLifeSavingEdmondsKarp.h"
#include <queue>
#include <algorithm>
#include <iomanip>

const int INF = 1e9;

MaxLifeSavingEdmondsKarp::MaxLifeSavingEdmondsKarp() 
    : totalNodes(0), sourceNode(0), sinkNode(0) {
    bloodGroupTypes = {
        BloodGroup::O_NEG, BloodGroup::O_POS,
        BloodGroup::A_NEG, BloodGroup::A_POS,
        BloodGroup::B_NEG, BloodGroup::B_POS,
        BloodGroup::AB_NEG, BloodGroup::AB_POS
    };
}

void MaxLifeSavingEdmondsKarp::buildNetwork(const std::map<BloodGroup, int>& inventory, 
                                          const std::vector<BloodRequest>& activeRequests) {
    groupToNode.clear();
    nodeToGroup.clear();
    requestToNode.clear();
    nodeToRequest.clear();

    // Source = node 0
    sourceNode = 0;

    // 8 Supply Nodes: node 1 to 8 (one for each blood group)
    int currentNode = 1;
    for (auto bg : bloodGroupTypes) {
        groupToNode[bg] = currentNode;
        nodeToGroup[currentNode] = bg;
        currentNode++;
    }

    // Demand Nodes: one for each patient request
    for (const auto& req : activeRequests) {
        requestToNode[req.requestId] = currentNode;
        nodeToRequest[currentNode] = req;
        currentNode++;
    }

    // Sink Node
    sinkNode = currentNode;
    totalNodes = sinkNode + 1;

    // Initialize capacity and flow matrices
    capacity.assign(totalNodes, std::vector<int>(totalNodes, 0));
    flow.assign(totalNodes, std::vector<int>(totalNodes, 0));
    adj.assign(totalNodes, std::vector<int>());

    // 1. Edges from Source -> Blood Group Supply nodes
    for (auto bg : bloodGroupTypes) {
        int u = groupToNode[bg];
        int availableUnits = 0;
        auto it = inventory.find(bg);
        if (it != inventory.end()) {
            availableUnits = it->second;
        }

        capacity[sourceNode][u] = availableUnits;
        adj[sourceNode].push_back(u);
        adj[u].push_back(sourceNode); // reverse edge for residual graph
    }

    // 2. Edges from Blood Group Supply nodes -> Compatible Patient Request nodes
    for (auto donorBg : bloodGroupTypes) {
        int u = groupToNode[donorBg];
        for (const auto& req : activeRequests) {
            if (isBloodCompatible(donorBg, req.bloodGroup)) {
                int v = requestToNode[req.requestId];
                // Capacity is bounded by requested quantity
                capacity[u][v] = req.quantityRequested;
                adj[u].push_back(v);
                adj[v].push_back(u); // reverse edge
            }
        }
    }

    // 3. Edges from Patient Request nodes -> Sink
    for (const auto& req : activeRequests) {
        int v = requestToNode[req.requestId];
        capacity[v][sinkNode] = req.quantityRequested;
        adj[v].push_back(sinkNode);
        adj[sinkNode].push_back(v); // reverse edge
    }
}

// BFS augmenting path search
bool MaxLifeSavingEdmondsKarp::bfsAugmentingPath(std::vector<int>& parentPath, int& bottleneckCapacity) {
    std::fill(parentPath.begin(), parentPath.end(), -1);
    parentPath[sourceNode] = -2;

    std::queue<std::pair<int, int>> q; // {currentNode, currentFlowBottleNeck}
    q.push({sourceNode, INF});

    while (!q.empty()) {
        int cur = q.front().first;
        int curFlow = q.front().second;
        q.pop();

        for (int next : adj[cur]) {
            int residual = capacity[cur][next] - flow[cur][next];
            if (parentPath[next] == -1 && residual > 0) {
                parentPath[next] = cur;
                int newFlow = std::min(curFlow, residual);
                if (next == sinkNode) {
                    bottleneckCapacity = newFlow;
                    return true;
                }
                q.push({next, newFlow});
            }
        }
    }

    return false;
}

AllocationResult MaxLifeSavingEdmondsKarp::computeMaxLifeSavingAllocation(bool verbose) {
    if (verbose) {
        std::cout << "\n=========================================================================================\n";
        std::cout << "          EXECUTING EDMONDS-KARP MAX FLOW ALGORITHM (BFS AUGMENTING PATHS)               \n";
        std::cout << "=========================================================================================\n";
    }

    std::vector<int> parent(totalNodes);
    int maxFlow = 0;
    int iteration = 0;
    int bottleneck = 0;

    // Edmonds-Karp Loop: BFS for shortest augmenting path
    while (bfsAugmentingPath(parent, bottleneck)) {
        iteration++;
        maxFlow += bottleneck;

        if (verbose) {
            std::cout << "  Iteration #" << iteration << ": Found BFS Augmenting Path with Bottleneck = " 
                      << bottleneck << " units\n    Path: ";
            std::vector<int> pathTrace;
            for (int v = sinkNode; v != sourceNode; v = parent[v]) {
                pathTrace.push_back(v);
            }
            pathTrace.push_back(sourceNode);
            std::reverse(pathTrace.begin(), pathTrace.end());

            for (size_t i = 0; i < pathTrace.size(); ++i) {
                int node = pathTrace[i];
                if (node == sourceNode) std::cout << "Source(S)";
                else if (node == sinkNode) std::cout << " -> Sink(T)";
                else if (nodeToGroup.find(node) != nodeToGroup.end()) {
                    std::cout << " -> Blood[" << bloodGroupToString(nodeToGroup[node]) << "]";
                } else if (nodeToRequest.find(node) != nodeToRequest.end()) {
                    std::cout << " -> Req#" << nodeToRequest[node].requestId << " (" 
                              << nodeToRequest[node].patientName << ")";
                }
            }
            std::cout << "\n";
        }

        // Augment flow and adjust residual capacities
        for (int v = sinkNode; v != sourceNode; v = parent[v]) {
            int u = parent[v];
            flow[u][v] += bottleneck;
            flow[v][u] -= bottleneck;
        }
    }

    if (verbose) {
        std::cout << "\n>>> [Edmonds-Karp Complete] No more augmenting paths in residual graph.\n";
        std::cout << ">>> Total Maximum Flow (Total Units Allocated): " << maxFlow << "\n";
    }

    // Compile Allocation Results
    AllocationResult result;
    result.totalUnitsAllocated = maxFlow;
    result.fullySatisfiedPatients = 0;
    result.partiallySatisfiedPatients = 0;
    result.unsatisfiedPatients = 0;

    // Check request allocations
    for (const auto& pair : requestToNode) {
        int reqId = pair.first;
        int v = pair.second;
        int reqDemanded = nodeToRequest[v].quantityRequested;
        int received = 0;

        for (auto bg : bloodGroupTypes) {
            int u = groupToNode[bg];
            int allocatedUnits = flow[u][v];
            if (allocatedUnits > 0) {
                result.patientAllocations[reqId].push_back({bg, allocatedUnits});
                received += allocatedUnits;
            }
        }

        if (received >= reqDemanded) {
            result.fullySatisfiedPatients++;
        } else if (received > 0) {
            result.partiallySatisfiedPatients++;
        } else {
            result.unsatisfiedPatients++;
        }
    }

    // Calculate remaining inventory
    for (auto bg : bloodGroupTypes) {
        int u = groupToNode[bg];
        int initialSupply = capacity[sourceNode][u];
        int used = flow[sourceNode][u];
        result.remainingInventory[bg] = initialSupply - used;
    }

    return result;
}

void MaxLifeSavingEdmondsKarp::printNetworkStructure() const {
    std::cout << "\n--- Bipartite Compatibility Flow Network Structure ---\n";
    std::cout << "Source (Node " << sourceNode << ") connects to Blood Group Supply nodes.\n";
    for (auto bg : bloodGroupTypes) {
        auto it = groupToNode.find(bg);
        if (it != groupToNode.end()) {
            int u = it->second;
            std::cout << "  - Supply Node " << u << " [" << bloodGroupToString(bg) 
                      << "] Initial Capacity: " << capacity[sourceNode][u] << " units\n";
        }
    }
    std::cout << "Patient Request Nodes connect to Sink (Node " << sinkNode << "):\n";
    for (const auto& p : nodeToRequest) {
        int v = p.first;
        const auto& r = p.second;
        std::cout << "  - Request Node " << v << " [Req #" << r.requestId << ": " << r.patientName 
                  << " (" << bloodGroupToString(r.bloodGroup) << ")] Demand: " 
                  << r.quantityRequested << " units\n";
    }
}

void MaxLifeSavingEdmondsKarp::printAllocationSummary(const AllocationResult& result, 
                                                    const std::vector<BloodRequest>& requests) {
    std::cout << "\n=========================================================================================\n";
    std::cout << "                    MAX LIFE SAVING BLOOD ALLOCATION SUMMARY                             \n";
    std::cout << "=========================================================================================\n";
    std::cout << "Total Units Transfused / Allocated: " << result.totalUnitsAllocated << "\n";
    std::cout << "Fully Satisfied Patients:            " << result.fullySatisfiedPatients << "\n";
    std::cout << "Partially Satisfied Patients:        " << result.partiallySatisfiedPatients << "\n";
    std::cout << "Unsatisfied Patients:                " << result.unsatisfiedPatients << "\n";
    std::cout << "-----------------------------------------------------------------------------------------\n";
    std::cout << std::left 
              << std::setw(8)  << "ReqID"
              << std::setw(18) << "Patient Name"
              << std::setw(10) << "Required"
              << std::setw(8)  << "Need"
              << std::setw(10) << "Received"
              << std::setw(14) << "Status"
              << "Allocated From (Donor Group -> Units)\n";
    std::cout << "-----------------------------------------------------------------------------------------\n";

    for (const auto& req : requests) {
        int reqId = req.requestId;
        auto it = result.patientAllocations.find(reqId);
        int totalReceived = 0;
        std::string allocDetails = "";

        if (it != result.patientAllocations.end()) {
            for (const auto& alloc : it->second) {
                totalReceived += alloc.second;
                if (!allocDetails.empty()) allocDetails += ", ";
                allocDetails += bloodGroupToString(alloc.first) + " x" + std::to_string(alloc.second);
            }
        }
        if (allocDetails.empty()) allocDetails = "None (Shortage/No compatible stock)";

        std::string statusStr = "UNSATISFIED";
        if (totalReceived >= req.quantityRequested) statusStr = "FULFILLED";
        else if (totalReceived > 0) statusStr = "PARTIAL";

        std::cout << std::left 
                  << std::setw(8)  << reqId
                  << std::setw(18) << req.patientName
                  << std::setw(10) << bloodGroupToString(req.bloodGroup)
                  << std::setw(8)  << req.quantityRequested
                  << std::setw(10) << totalReceived
                  << std::setw(14) << statusStr
                  << allocDetails << "\n";
    }

    std::cout << "\n---------------- Remaining Blood Bank Inventory ----------------\n";
    for (const auto& item : result.remainingInventory) {
        std::cout << "  " << std::setw(6) << bloodGroupToString(item.first) 
                  << ": " << item.second << " units remaining\n";
    }
    std::cout << "=========================================================================================\n";
}

