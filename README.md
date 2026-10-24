# Advanced Data Structures (ADS) Project: Central Blood Bank Management System
**Mapped directly with the Blood Bank ER Diagram & Core ADS Algorithms**

---

## 📌 Project Architecture & User Flow

The system architecture organizes all interactions into **4 distinct top-level options**:

```
                              [MAIN PORTAL]
                                    |
     +-----------------+------------+------------+-----------------+
     |                 |                         |                 |
[1] DONOR         [2] RECEIVER              [3] HOSPITAL       [4] EXIT
   PORTAL            PORTAL                    PORTAL
 (Register &       (Place Urgent            (Midcentral Hub)
  Screening)        Blood Request)           [RESTRICTED]
                                          Password: "Blood2026"
                                                 |
                   +-----------------------------+-----------------------------+
                   |                             |                             |
          [FEATURE 1: ADS]              [FEATURE 2: ADS]              [FEATURE 3: ADS]
        Dynamic Max-Heap Urgency        Max Life-Saving Flow          Trie Autocomplete
        (Starvation Prevention)        (Edmonds-Karp with BFS)       (Directory Search)
                   |                             |                             |
                   +-----------------------------+-----------------------------+
                                                 |
                                    [CENTRAL ER DATABASE RECORDS]
                                    - Donors & Health Evaluations
                                    - Receiver Demands & Urgency
                                    - Blood Units & Expiry Batches
                                    - Appointments, Tests & Donations
```

---

### Option 1: Donor Portal (Person who wants to DONATE blood)
- Follows the complete donor onboarding pipeline from the ER diagram:
  - User profile creation (`user`)
  - Donor registration (`donor`)
  - Mandatory health evaluation (`donor_health`): weight ($\ge 45\text{ kg}$), height, blood group, recent surgery check, prescription medications, chronic illnesses, addictions.
  - If eligible, automatically schedules hospital appointment (`appointments`), performs medical screening test (`medical_tests`), records donation (`donations`), and creates a fresh tested blood unit (`blood_units`) added directly to the Blood Bank inventory!
  - Indexes the donor into the **Trie directory** for instant autocomplete search.

### Option 2: Receiver Portal (Person who wants to RECEIVE blood units)
- Allows patients/doctors to request blood units:
  - Patient name, age, admitted hospital, contact phone
  - Blood group required and units needed
  - Urgency level:
    - **Critical**: Trauma, emergency surgery, life-threatening hemorrhage
    - **Urgent**: ICU admission, acute bleeding
    - **Moderate**: Subacute medical condition
    - **Routine**: Planned elective surgery
  - Dynamic Priority Calculation:
    $$\text{Dynamic Score} = \text{Base Urgency} + (\text{Minutes Waited} \times 5) + \text{ICU/Trauma Bonus} (+200) + \text{Age Vulnerability} (+150) + \text{Rare Blood Bonus} (+50)$$
  - Directly enqueues the request into the **Dynamic Priority Max-Heap** (`requestMaxHeap.insert(req)`).
  - Indexes the patient into the **Trie directory**.

### Option 3: Hospital Portal (Midcentral between Receiver & Donor)
- **Security**: Strictly restricted to hospital workers and authorized personnel only.
- **Password**: `Blood2026`
- **Features & Controls inside Hospital Workstation**:
  1. **Blood Request Urgency (Max-Heap)**:
     - View dynamic priority queue rankings
     - Visualize the binary Max-Heap tree
     - Dispatch/Triage highest priority patient (`extractMax`)
     - Simulate time aging (escalates priorities to prevent starvation using Floyd's $O(N)$ algorithm)
  2. **Max Life-Saving Blood Allocation (Edmonds-Karp Max-Flow)**:
     - Formulates bipartite network: Source $\to$ Blood Units $\to$ Compatible Patient Demands $\to$ Sink
     - Uses **BFS** to find shortest augmenting paths
     - Re-routes allocations dynamically through reverse residual edges to **maximize lives saved**
  3. **Name Search (Trie Autocomplete)**:
     - Instant prefix search across all Donors, Receivers, Hospitals, and Blood Banks
  4. **Central ER Database Records**:
     - View all Donors & Medical Health Screenings
     - View all Patient Demands & Urgency Statuses
     - View Blood Units Inventory, Batches, & Expiry Dates
     - View Hospital Appointments, Tests, and Donation Logs
     - Run Complete Automated ADS Verification Test Suite

### Option 4: Exit
- Safely terminates the application.

---

## 💻 Commands to Run the Project in Terminal

### Compile and Run the Integrated 4-Option System:
```powershell
g++ -std=c++14 main.cpp BloodRequestHeap.cpp MaxLifeSavingEdmondsKarp.cpp DirectoryTrie.cpp BloodBankSystem.cpp -o blood_bank_system.exe; .\blood_bank_system.exe
```

### Run Standalone Feature Files Independently:
```powershell
# Feature 1: Dynamic Max-Heap
g++ -std=c++14 Feature1_UrgencyMaxHeap.cpp -o f1.exe; .\f1.exe

# Feature 2: Edmonds-Karp Max Life Saving Allocation
g++ -std=c++14 Feature2_MaxLifeSavingEdmondsKarp.cpp -o f2.exe; .\f2.exe

# Feature 3: Trie Directory Autocomplete Search
g++ -std=c++14 Feature3_DirectoryTrie.cpp -o f3.exe; .\f3.exe
```
