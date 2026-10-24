# Advanced Data Structures (ADS) Project: Central Blood Bank Management System
**Emergency Transfusion, Priority Triage & Automated Lifesaving Allocation Platform**

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
 (Register &       (Place Urgent            (Hospital Authority Hub)
  Screening)        Blood Request)           [RESTRICTED AREA]
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
                                [HOSPITAL AUTHORITY OPERATIONS]
                                - Review & Accept Receiver Requests
                                - Verify Live Blood Inventory Availability
                                - Dispense Blood Units & Mark Request as DONE
                                - Central Repository Records & Stock Tracking
```

---

### Option 1: Donor Portal (Person who wants to DONATE blood)
- Follows the complete donor onboarding workflow:
  - User profile creation
  - Donor registration
  - Mandatory health evaluation: weight ($\ge 45\text{ kg}$), height, blood group, recent surgery check, prescription medications, chronic illnesses, addictions.
  - If eligible, automatically schedules hospital appointment, performs medical screening test, records donation, and creates a fresh tested blood unit added directly to the Central Blood Bank inventory!
  - Indexes the donor into the **Trie directory** for instant autocomplete lookup.

### Option 2: Receiver Portal (Person who wants to RECEIVE blood units)
- Allows patients/doctors to request blood units:
  - Patient name, age, admitted hospital, contact phone
  - Blood group required and units needed
  - Clinical urgency level:
    - **Critical**: Trauma, emergency surgery, life-threatening hemorrhage
    - **Urgent**: ICU admission, acute bleeding
    - **Moderate**: Subacute medical condition
    - **Routine**: Planned elective surgery
  - Dynamic Priority Calculation:
    $$\text{Dynamic Score} = \text{Base Urgency} + (\text{Minutes Waited} \times 5) + \text{ICU/Trauma Bonus} (+200) + \text{Age Vulnerability} (+150) + \text{Rare Blood Bonus} (+50)$$
  - Directly enqueues the request into the **Dynamic Priority Max-Heap** (`requestMaxHeap.insert(req)`).
  - Indexes the patient into the **Trie directory**.

### Option 3: Hospital Portal (Hospital Authority Central Workstation)
- **Security**: Strictly restricted to hospital authority and authorized medical staff.
- **Password**: `Blood2026`
- **Hospital Authority Operations**:
  1. **Authority Action: Review & Accept Receiver Requests**:
     - View all pending patient transfusion requests alongside live stock availability.
     - Review requests individually or auto-fulfill the highest priority request.
     - Check blood inventory for matching and compatible units.
     - If blood is available: dispense blood units to patient, update unit inventory status to "Allocated", and mark the request status as **DONE**.
     - If blood is unavailable or partial: alerts the authority with detailed recommendations (contact donors or inter-hospital transfer).
     - Batch auto-fulfill all pending requests matching current inventory.
  2. **View Central Blood Bank Inventory**:
     - Group-by-group summary of available units, allocated units, and total stock.
  3. **Feature 1: Blood Request Urgency (Max-Heap)**:
     - View dynamic priority queue rankings.
     - Visualize the binary Max-Heap tree structure.
     - Dispatch/Triage highest priority patient (`extractMax`).
     - Simulate time aging (escalates priorities to prevent starvation using Floyd's $O(N)$ algorithm).
  4. **Feature 2: Max Life-Saving Blood Allocation (Edmonds-Karp Max-Flow)**:
     - Formulates bipartite network: Source $\to$ Blood Units $\to$ Compatible Patient Demands $\to$ Sink.
     - Uses **BFS** to discover shortest augmenting paths.
     - Re-routes allocations dynamically through reverse residual edges to **maximize lives saved**.
     - Provides authority option to apply flow allocations directly to inventory and mark satisfied requests as **DONE**.
  5. **Feature 3: Directory Name Search (Trie Autocomplete)**:
     - Instant prefix search across all Donors, Receivers, Hospitals, and Blood Banks.
  6. **Central System Records & Repository**:
     - View all Registered Donors & Medical Health Screenings.
     - View all Patient Transfusion Requests & Statuses (Pending vs DONE).
     - View Blood Units Inventory, Batches, & Expiry Dates.
     - View Hospital Appointments, Tests, and Donation Logs.
     - Run Complete Automated Verification Test Suite.

### Option 4: Exit
- Safely terminates the application.

---

## 💻 Commands to Run the Project in Terminal

### Compile and Run the Integrated System:
```bash
g++ -std=c++14 main.cpp BloodRequestHeap.cpp MaxLifeSavingEdmondsKarp.cpp DirectoryTrie.cpp BloodBankSystem.cpp -o blood_bank_system && ./blood_bank_system
```

### Windows (PowerShell / Command Prompt):
```powershell
g++ -std=c++14 main.cpp BloodRequestHeap.cpp MaxLifeSavingEdmondsKarp.cpp DirectoryTrie.cpp BloodBankSystem.cpp -o blood_bank_system.exe
.\blood_bank_system.exe
```

### Run Standalone Feature Demos Independently:
```bash
# Feature 1: Dynamic Max-Heap
g++ -std=c++14 Feature1_UrgencyMaxHeap.cpp -o f1 && ./f1

# Feature 2: Edmonds-Karp Max Life Saving Allocation
g++ -std=c++14 Feature2_MaxLifeSavingEdmondsKarp.cpp -o f2 && ./f2

# Feature 3: Trie Directory Autocomplete Search
g++ -std=c++14 Feature3_DirectoryTrie.cpp -o f3 && ./f3
```
