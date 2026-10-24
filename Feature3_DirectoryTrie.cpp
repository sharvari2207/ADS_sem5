/**
 * =========================================================================================
 * FEATURE 3: Directory Name Search & Autocomplete using Trie (Prefix Tree)
 * Advanced Data Structures (ADS) Subject Project
 * Mapped to ER Diagram Entities: `user` (donor/requester), `hospital`, `blood_bank`
 * =========================================================================================
 * 
 * Key Concepts:
 * 1. Trie Data Structure:
 *      - Tree where each node corresponds to a character of a name.
 *      - Shared prefixes save space and enable instant autocomplete suggestions.
 * 2. Time Complexity:
 *      - Insert a Name: O(L) where L is name length.
 *      - Prefix Search: O(P + K) where P is prefix length and K is number of matched entities.
 *      - Significant speedup over O(N * L) linear scan on large directories!
 * 3. Case-Insensitive Prefix Autocomplete:
 *      - Searching "aa" suggests "Aarav Deshmukh (Donor)".
 *      - Searching "dr" suggests "Dr. Rajesh Patil", "Dr. Ananya Sen", "Dr. Amit Verma".
 *      - Searching "sa" suggests "Sahyadri Hospital".
 * =========================================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iomanip>
#include <cctype>

struct DirectoryRecord {
    std::string role;       // "Donor", "Requester", "Hospital", "Blood Bank"
    int entityId;
    std::string name;
    std::string phone;
    std::string bloodGroup;
    std::string location;
    std::string statusOrDetails;
};

struct TrieNode {
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    bool isEndOfWord;
    std::vector<DirectoryRecord> records;

    TrieNode() : isEndOfWord(false) {}
};

class BloodBankDirectoryTrie {
private:
    std::shared_ptr<TrieNode> root;
    size_t count;

    std::string normalize(const std::string& str) const {
        std::string res = "";
        for (char ch : str) {
            if (std::isalnum(static_cast<unsigned char>(ch)) || ch == ' ' || ch == '-') {
                res += std::tolower(static_cast<unsigned char>(ch));
            }
        }
        return res;
    }

    void dfsCollect(std::shared_ptr<TrieNode> node, 
                    std::vector<DirectoryRecord>& list, 
                    int maxLimit) const {
        if (!node || static_cast<int>(list.size()) >= maxLimit) return;
        if (node->isEndOfWord) {
            for (const auto& rec : node->records) {
                list.push_back(rec);
                if (static_cast<int>(list.size()) >= maxLimit) return;
            }
        }
        for (auto& pair : node->children) {
            dfsCollect(pair.second, list, maxLimit);
            if (static_cast<int>(list.size()) >= maxLimit) return;
        }
    }

public:
    BloodBankDirectoryTrie() : count(0) {
        root = std::make_shared<TrieNode>();
    }

    void insert(const DirectoryRecord& rec) {
        std::string key = normalize(rec.name);
        if (key.empty()) return;

        auto curr = root;
        for (char ch : key) {
            if (curr->children.find(ch) == curr->children.end()) {
                curr->children[ch] = std::make_shared<TrieNode>();
            }
            curr = curr->children[ch];
        }
        curr->isEndOfWord = true;
        curr->records.push_back(rec);
        count++;
    }

    std::vector<DirectoryRecord> autocomplete(const std::string& prefix, int maxLimit = 10) const {
        std::string key = normalize(prefix);
        std::vector<DirectoryRecord> results;

        auto curr = root;
        for (char ch : key) {
            if (curr->children.find(ch) == curr->children.end()) {
                return results; // No prefix match
            }
            curr = curr->children[ch];
        }

        dfsCollect(curr, results, maxLimit);
        return results;
    }

    void printAutocomplete(const std::string& prefix) const {
        std::cout << "\n>>> Autocomplete Search for Prefix: \"" << prefix << "\" <<<\n";
        auto results = autocomplete(prefix);
        if (results.empty()) {
            std::cout << "    [No matches found in directory]\n";
            return;
        }

        std::cout << "    Found " << results.size() << " match(es):\n";
        for (size_t i = 0; i < results.size(); ++i) {
            const auto& r = results[i];
            std::cout << "    " << (i + 1) << ". [" << std::setw(11) << std::left << r.role << " #" << r.entityId << "] "
                      << std::setw(24) << std::left << r.name;
            if (!r.bloodGroup.empty()) {
                std::cout << " | Blood: " << std::setw(4) << r.bloodGroup;
            }
            std::cout << " | Phone: " << std::setw(16) << r.phone
                      << " | Location: " << r.location;
            if (!r.statusOrDetails.empty()) {
                std::cout << " (" << r.statusOrDetails << ")";
            }
            std::cout << "\n";
        }
    }

    size_t size() const { return count; }
};

int main() {
    std::cout << "=======================================================================\n";
    std::cout << "   FEATURE 3 DEMO: TRIE DIRECTORY AUTOCOMPLETE & PREFIX SEARCH         \n";
    std::cout << "=======================================================================\n";

    BloodBankDirectoryTrie directory;

    // Populating with ER diagram entities
    // 1. Donors (user + donor + donor_health)
    directory.insert({"Donor", 1, "Aarav Deshmukh", "+91-9822011111", "O-",  "Kothrud, Pune", "Active"});
    directory.insert({"Donor", 2, "Sneha Kulkarni", "+91-9822022222", "A+",  "Shivajinagar, Pune", "Active"});
    directory.insert({"Donor", 3, "Rohan Shinde",   "+91-9822033333", "B+",  "Baner, Pune", "Active"});
    directory.insert({"Donor", 4, "Pooja Joshi",    "+91-9822044444", "O+",  "Hadapsar, Pune", "Active"});
    directory.insert({"Donor", 5, "Vikram More",    "+91-9822055555", "AB+", "Viman Nagar, Pune", "Active"});

    // 2. Requesters (user + requester)
    directory.insert({"Requester", 1, "Dr. Rajesh Patil", "+91-9822066666", "", "Rasta Peth, Pune", "KEM Hospital"});
    directory.insert({"Requester", 2, "Dr. Ananya Sen",   "+91-9822077777", "", "Deccan, Pune", "Sahyadri Hospital"});
    directory.insert({"Requester", 3, "Dr. Amit Verma",   "+91-9822088888", "", "Bund Garden, Pune", "Ruby Hall Clinic"});

    // 3. Hospitals
    directory.insert({"Hospital", 1, "Sahyadri Super Speciality Hospital", "+91-20-67213000", "", "Deccan, Pune", "Tertiary Care"});
    directory.insert({"Hospital", 2, "KEM Hospital Pune",                 "+91-20-26125600", "", "Rasta Peth, Pune", "Medical College"});
    directory.insert({"Hospital", 3, "Ruby Hall Clinic",                  "+91-20-66455100", "", "Sassoon Road, Pune", "Multi-speciality"});

    // 4. Blood Banks
    directory.insert({"Blood Bank", 1, "Jankalyan Blood Centre", "+91-20-24444501", "", "Sarasbaug, Pune", "Regional Centre"});
    directory.insert({"Blood Bank", 2, "Red Cross Blood Bank",   "+91-20-26123456", "", "Camp, Pune", "Main Depot"});

    std::cout << "\nTotal entities indexed in Trie: " << directory.size() << "\n";

    // Testing various prefix autocomplete queries
    directory.printAutocomplete("aa");
    directory.printAutocomplete("dr");
    directory.printAutocomplete("dr. a");
    directory.printAutocomplete("sa");
    directory.printAutocomplete("poo");
    directory.printAutocomplete("red");
    directory.printAutocomplete("xyz"); // not found test

    std::cout << "\n>>> Feature 3 Demo Complete! <<<\n";
    return 0;
}

