#ifndef DIRECTORY_TRIE_H
#define DIRECTORY_TRIE_H

#include "ERModels.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>

/**
 * FEATURE 3: Name Directory Search & Autocomplete using Trie
 * 
 * Context in Advance Data Structures:
 * - A Trie (Prefix Tree) is used for fast autocomplete and prefix-based directory lookup.
 * - Time Complexity:
 *   - Insertion: O(L) where L = length of name
 *   - Prefix Search: O(P + K) where P = prefix length, K = number of matching descendants
 * - Stores entities from the ER Diagram:
 *   - Donors (with Donor ID, Blood Group, Eligibility, Phone)
 *   - Requesters / Patients (with Requester ID, Hospital)
 *   - Hospitals (with Hospital ID, Phone, City)
 *   - Blood Banks (with Blood Bank ID, Contact)
 */

enum class DirectoryRole {
    DONOR,
    REQUESTER,
    HOSPITAL,
    BLOOD_BANK
};

inline std::string roleToString(DirectoryRole r) {
    switch (r) {
        case DirectoryRole::DONOR:      return "Donor";
        case DirectoryRole::REQUESTER:  return "Requester/Patient";
        case DirectoryRole::HOSPITAL:   return "Hospital";
        case DirectoryRole::BLOOD_BANK: return "Blood Bank";
        default: return "User";
    }
}

struct DirectoryRecord {
    DirectoryRole role;
    int entityId;
    std::string name;
    std::string contactPhone;
    std::string email;
    BloodGroup bloodGroup; // for donors & requesters
    std::string address;
    std::string extraInfo; // e.g. "Active Donor", "Apollo Hospital", etc.
};

struct TrieNode {
    // Hash map or array for children characters
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    bool isEndOfWord;
    std::vector<DirectoryRecord> records; // Can have multiple entities with same name!

    TrieNode() : isEndOfWord(false) {}
};

class DirectoryTrie {
private:
    std::shared_ptr<TrieNode> root;
    size_t totalRecords;

    // Helper functions
    std::string normalize(const std::string& str) const;
    void collectAllWords(std::shared_ptr<TrieNode> node, 
                         std::vector<DirectoryRecord>& results, 
                         int maxLimit) const;

public:
    DirectoryTrie();

    // Insertion
    void insert(const DirectoryRecord& record);

    // Search operations
    std::vector<DirectoryRecord> searchExact(const std::string& name) const;
    std::vector<DirectoryRecord> autocomplete(const std::string& prefix, int maxLimit = 10) const;

    // Utility & Displays
    size_t size() const { return totalRecords; }
    void printAutocompleteResults(const std::string& prefix, int maxLimit = 10) const;
    static void printRecordCard(const DirectoryRecord& rec);
};

#endif // DIRECTORY_TRIE_H

