/**
 * =========================================================================================
 * FEATURE 2: Max Life Saving Blood Allocation using Edmonds-Karp Max-Flow Algorithm
 * Advanced Data Structures (ADS) Subject Project
 * Mapped to ER Diagram Entities: `blood_units` (blood_bank_id) & `blood_requests`
 * =========================================================================================
 * 
 * Key Concepts:
 * 1. Bipartite Compatibility Flow Network:
 *      Source (S) -> Blood Types in Blood Bank Inventory (8 nodes: O-, O+, A-, A+, B-, B+, AB-, AB+)
 *      Blood Types -> Compatible Patient Requests (based on ABO/Rh medical compatibility)
 *      Patient Requests -> Sink (T) (capacity = quantity demanded)
 * 2. Edmonds-Karp Algorithm:
 *      - Implements Ford-Fulkerson using Breadth-First Search (BFS) to find augmenting paths.
 *      - Time Complexity: O(V * E^2).
 * 3. Life-Saving Guarantee:
 *      - Naive greedy allocation wastes universal donor blood (O-) on recipients who could
 *        accept other types (e.g., A+ or AB+), leading to death for rare O- patients.
 *      - Edmonds-Karp pushes flow along residual augmenting paths, automatically re-routing
 *        allocations to maximize the total number of patients saved!
 * =========================================================================================
 */

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <iomanip>
#include <algorithm>

const int INF = 1e9;

enum class BloodGroup {
    O_NEG, O_POS, A_NEG, A_POS, B_NEG, B_POS, AB_NEG, AB_POS
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
    }
    return "Unknown";
}

// Medical ABO/Rh Blood Transfusion Compatibility
inline bool isCompatible(BloodGroup donor, BloodGroup recipient) {
    if (donor == BloodGroup::O_NEG) return true; // Universal Donor
    if (recipient == BloodGroup::AB_POS) return true; // Universal Recipient
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

struct PatientDemand {
    int requestId;
    std::string patientName;
    BloodGroup requiredGroup;
    int unitsNeeded;
};

class EdmondsKarpBloodAllocator {
private:
    int totalNodes;
    int source;
    int sink;
    std::vector<std::vector<int>> capacity;
    std::vector<std::vector<int>> flow;
    std::vector<std::vector<int>> adj;

    std::vector<BloodGroup> allGroups = {
        BloodGroup::O_NEG, BloodGroup::O_POS,
        BloodGroup::A_NEG, BloodGroup::A_POS,
        BloodGroup::B_NEG, BloodGroup::B_POS,
        BloodGroup::AB_NEG, BloodGroup::AB_POS
    };

    std::map<BloodGroup, int> groupToNode;
    std::map<int, BloodGroup> nodeToGroup;
    std::map<int, int> reqToNode;
    std::map<int, PatientDemand> nodeToReq;

    // BFS search for shortest augmenting path
    bool bfs(std::vector<int>& parent, int& bottleneck) {
        std::fill(parent.begin(), parent.end(), -1);
        parent[source] = -2;

        std::queue<std::pair<int, int>> q;
        q.push({source, INF});

        while (!q.empty()) {
            int cur = q.front().first;
            int curFlow = q.front().second;
            q.pop();

            for (int nxt : adj[cur]) {
                int residual = capacity[cur][nxt] - flow[cur][nxt];
                if (parent[nxt] == -1 && residual > 0) {
                    parent[nxt] = cur;
                    int newFlow = std::min(curFlow, residual);
                    if (nxt == sink) {
                        bottleneck = newFlow;
                        return true;
                    }
                    q.push({nxt, newFlow});
                }
            }
        }
        return false;
    }

public:
    void solve(const std::map<BloodGroup, int>& inventory, 
               const std::vector<PatientDemand>& patients) {
        source = 0;
        int curr = 1;
        for (auto bg : allGroups) {
            groupToNode[bg] = curr;
            nodeToGroup[curr] = bg;
            curr++;
        }
        for (const auto& p : patients) {
            reqToNode[p.requestId] = curr;
            nodeToReq[curr] = p;
            curr++;
        }
        sink = curr;
        totalNodes = sink + 1;

        capacity.assign(totalNodes, std::vector<int>(totalNodes, 0));
        flow.assign(totalNodes, std::vector<int>(totalNodes, 0));
        adj.assign(totalNodes, std::vector<int>());

        // Source -> Blood Groups
        for (auto bg : allGroups) {
            int u = groupToNode[bg];
            int supply = inventory.count(bg) ? inventory.at(bg) : 0;
            capacity[source][u] = supply;
            adj[source].push_back(u);
            adj[u].push_back(source);
        }

        // Blood Groups -> Patients (Compatibility)
        for (auto bg : allGroups) {
            int u = groupToNode[bg];
            for (const auto& p : patients) {
                if (isCompatible(bg, p.requiredGroup)) {
                    int v = reqToNode[p.requestId];
                    capacity[u][v] = p.unitsNeeded;
                    adj[u].push_back(v);
                    adj[v].push_back(u);
                }
            }
        }

        // Patients -> Sink
        for (const auto& p : patients) {
            int v = reqToNode[p.requestId];
            capacity[v][sink] = p.unitsNeeded;
            adj[v].push_back(sink);
            adj[sink].push_back(v);
        }

        std::cout << "\n=========================================================================================\n";
        std::cout << "                 RUNNING EDMONDS-KARP MAX FLOW (BFS AUGMENTING PATHS)                    \n";
        std::cout << "=========================================================================================\n";

        std::vector<int> parent(totalNodes);
        int bottleneck = 0;
        int maxFlow = 0;
        int iter = 0;

        while (bfs(parent, bottleneck)) {
            iter++;
            maxFlow += bottleneck;

            std::cout << " Iteration #" << iter << " -> Augmenting Flow: +" << bottleneck << " unit(s) along BFS Path: ";
            std::vector<int> path;
            for (int v = sink; v != source; v = parent[v]) path.push_back(v);
            path.push_back(source);
            std::reverse(path.begin(), path.end());

            for (size_t i = 0; i < path.size(); ++i) {
                int node = path[i];
                if (node == source) std::cout << "Source";
                else if (node == sink) std::cout << " -> Sink";
                else if (nodeToGroup.count(node)) std::cout << " -> [" << bloodGroupToString(nodeToGroup[node]) << "]";
                else if (nodeToReq.count(node)) std::cout << " -> " << nodeToReq[node].patientName;
            }
            std::cout << "\n";

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                flow[u][v] += bottleneck;
                flow[v][u] -= bottleneck;
            }
        }

        std::cout << "\n>>> Total Maximum Life Saving Flow Achieved: " << maxFlow << " units allocated.\n";

        // Display Detailed Allocation Report
        std::cout << "\n=========================================================================================\n";
        std::cout << "                    OPTIMAL LIFE-SAVING PATIENT ALLOCATION REPORT                        \n";
        std::cout << "=========================================================================================\n";
        std::cout << std::left 
                  << std::setw(8)  << "ReqID"
                  << std::setw(25) << "Patient Name"
                  << std::setw(12) << "Required"
                  << std::setw(8)  << "Demand"
                  << std::setw(10) << "Allocated"
                  << std::setw(14) << "Status"
                  << "Donor Unit Breakdown\n";
        std::cout << "-----------------------------------------------------------------------------------------\n";

        for (const auto& p : patients) {
            int v = reqToNode[p.requestId];
            int allocated = 0;
            std::string breakdown = "";

            for (auto bg : allGroups) {
                int u = groupToNode[bg];
                if (flow[u][v] > 0) {
                    if (!breakdown.empty()) breakdown += ", ";
                    breakdown += bloodGroupToString(bg) + " (" + std::to_string(flow[u][v]) + ")";
                    allocated += flow[u][v];
                }
            }
            if (breakdown.empty()) breakdown = "None";

            std::string status = (allocated >= p.unitsNeeded) ? "FULFILLED" : (allocated > 0 ? "PARTIAL" : "UNMET");

            std::cout << std::left 
                      << std::setw(8)  << p.requestId
                      << std::setw(25) << p.patientName
                      << std::setw(12) << bloodGroupToString(p.requiredGroup)
                      << std::setw(8)  << p.unitsNeeded
                      << std::setw(10) << allocated
                      << std::setw(14) << status
                      << breakdown << "\n";
        }
        std::cout << "=========================================================================================\n";
    }
};

int main() {
    std::cout << "=======================================================================\n";
    std::cout << "   FEATURE 2 DEMO: EDMONDS-KARP MAX LIFE SAVING BLOOD ALLOCATION       \n";
    std::cout << "=======================================================================\n";

    // Blood bank inventory
    std::map<BloodGroup, int> inventory = {
        {BloodGroup::O_NEG, 2},
        {BloodGroup::O_POS, 3},
        {BloodGroup::A_POS, 2},
        {BloodGroup::B_POS, 2},
        {BloodGroup::AB_POS, 1}
    };

    std::cout << "\n1. Current Available Blood Bank Stock (from ER: blood_units):\n";
    for (const auto& item : inventory) {
        std::cout << "   " << std::setw(6) << bloodGroupToString(item.first) 
                  << ": " << item.second << " units\n";
    }

    // Patient requests from ER diagram
    std::vector<PatientDemand> requests = {
        {901, "Kavita Rao (Trauma)",        BloodGroup::AB_POS, 2},
        {902, "Master Aarush (Infant/ICU)", BloodGroup::O_NEG,  1},
        {903, "Mahesh Kadam (Dengue)",      BloodGroup::B_POS,  2},
        {904, "Suresh Gaikwad (Cardiac)",   BloodGroup::A_POS,  2},
        {905, "Neha Sawant (Thalassemia)",  BloodGroup::O_POS,  2},
        {906, "Dattatray Shinde (Ortho)",   BloodGroup::O_POS,  1}
    };

    std::cout << "\n2. Active Patient Requests (from ER: blood_requests):\n";
    for (const auto& r : requests) {
        std::cout << "   Req #" << r.requestId << ": " << std::setw(28) << std::left << r.patientName 
                  << " | Needs: " << r.unitsNeeded << " units of " << bloodGroupToString(r.requiredGroup) << "\n";
    }

    EdmondsKarpBloodAllocator solver;
    solver.solve(inventory, requests);

    std::cout << "\n>>> Feature 2 Demo Complete! <<<\n";
    return 0;
}

