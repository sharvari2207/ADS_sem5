#ifndef DIRECTORY_TRIE_H
#define DIRECTORY_TRIE_H

#include "ERModels.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>

// Prefix tree (Trie) for autocomplete search across people, hospitals, and blood banks
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

// Directory entry card
struct DirectoryRecord {
    DirectoryRole role;
    int entityId;
    std::string name;
    std::string contactPhone;
    std::string email;
    BloodGroup bloodGroup; // for donors & requesters
    std::string address;
    std::string extraInfo; // e.g. "Active Donor", "Sahyadri Hospital"
};

// Node in the prefix trie
struct TrieNode {
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    bool isEndOfWord;
    std::vector<DirectoryRecord> records; // Stores matching profiles for identical prefixes

    TrieNode() : isEndOfWord(false) {}
};

class DirectoryTrie {
private:
    std::shared_ptr<TrieNode> root;
    size_t totalRecords;

    // Helper routines
    std::string normalize(const std::string& str) const;
    void collectAllWords(std::shared_ptr<TrieNode> node, 
                         std::vector<DirectoryRecord>& results, 
                         int maxLimit) const;

public:
    DirectoryTrie();

    // Insert new profile
    void insert(const DirectoryRecord& record);

    // Search operations
    std::vector<DirectoryRecord> searchExact(const std::string& name) const;
    std::vector<DirectoryRecord> autocomplete(const std::string& prefix, int maxLimit = 10) const;

    // Summary and display
    size_t size() const { return totalRecords; }
    void printAutocompleteResults(const std::string& prefix, int maxLimit = 10) const;
    static void printRecordCard(const DirectoryRecord& rec);
};

#endif // DIRECTORY_TRIE_H
