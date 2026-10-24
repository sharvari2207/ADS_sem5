#include "DirectoryTrie.h"
#include <cctype>
#include <iomanip>

DirectoryTrie::DirectoryTrie() : totalRecords(0) {
    root = std::make_shared<TrieNode>();
}

// Convert input string to lowercase and strip special characters
std::string DirectoryTrie::normalize(const std::string& str) const {
    std::string clean = "";
    for (char ch : str) {
        if (std::isalnum(static_cast<unsigned char>(ch)) || ch == ' ' || ch == '-') {
            clean += std::tolower(static_cast<unsigned char>(ch));
        }
    }
    return clean;
}

// Add a directory profile into the trie
void DirectoryTrie::insert(const DirectoryRecord& record) {
    std::string key = normalize(record.name);
    if (key.empty()) return;

    auto current = root;
    for (char ch : key) {
        if (current->children.find(ch) == current->children.end()) {
            current->children[ch] = std::make_shared<TrieNode>();
        }
        current = current->children[ch];
    }

    current->isEndOfWord = true;
    current->records.push_back(record);
    totalRecords++;
}

// Search for exact match
std::vector<DirectoryRecord> DirectoryTrie::searchExact(const std::string& name) const {
    std::string key = normalize(name);
    auto current = root;

    for (char ch : key) {
        if (current->children.find(ch) == current->children.end()) {
            return {};
        }
        current = current->children[ch];
    }

    if (current->isEndOfWord) {
        return current->records;
    }
    return {};
}

// Collect all matching profiles under current prefix node
void DirectoryTrie::collectAllWords(std::shared_ptr<TrieNode> node, 
                                   std::vector<DirectoryRecord>& results, 
                                   int maxLimit) const {
    if (!node || static_cast<int>(results.size()) >= maxLimit) return;

    if (node->isEndOfWord) {
        for (const auto& rec : node->records) {
            results.push_back(rec);
            if (static_cast<int>(results.size()) >= maxLimit) return;
        }
    }

    for (auto& pair : node->children) {
        collectAllWords(pair.second, results, maxLimit);
        if (static_cast<int>(results.size()) >= maxLimit) return;
    }
}

// Autocomplete prefix search
std::vector<DirectoryRecord> DirectoryTrie::autocomplete(const std::string& prefix, int maxLimit) const {
    std::string key = normalize(prefix);
    std::vector<DirectoryRecord> results;

    auto current = root;
    for (char ch : key) {
        if (current->children.find(ch) == current->children.end()) {
            return results;
        }
        current = current->children[ch];
    }

    collectAllWords(current, results, maxLimit);
    return results;
}

// Display single directory record
void DirectoryTrie::printRecordCard(const DirectoryRecord& rec) {
    std::cout << "  [" << roleToString(rec.role) << " #" << rec.entityId << "] " 
              << rec.name;
    if (rec.bloodGroup != BloodGroup::UNKNOWN) {
        std::cout << " | Blood Group: " << bloodGroupToString(rec.bloodGroup);
    }
    std::cout << " | Phone: " << rec.contactPhone 
              << " | City: " << rec.address;
    if (!rec.extraInfo.empty()) {
        std::cout << " (" << rec.extraInfo << ")";
    }
    std::cout << "\n";
}

// Print autocomplete search results to console
void DirectoryTrie::printAutocompleteResults(const std::string& prefix, int maxLimit) const {
    std::cout << "\n--- Autocomplete Search for Prefix: \"" << prefix << "\" ---\n";
    auto results = autocomplete(prefix, maxLimit);

    if (results.empty()) {
        std::cout << "  No directory entries found starting with \"" << prefix << "\".\n";
        return;
    }

    std::cout << "  Found " << results.size() << " match(es):\n";
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "  " << (i + 1) << ". ";
        printRecordCard(results[i]);
    }
}
