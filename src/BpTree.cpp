#include "BpTree.hpp"

#include <stdexcept>
#include <cstring>
#include "Type.hpp"

BpTree::BpTree(const std::string& dirPath) : bpt (dirPath.c_str(), false) {
    // Initialize the B+ tree with sync options
    // Note: bpt::bplus_tree might need to be modified to support syncWrites
}

void BpTree::Put(const std::vector<byte>& key, const LogRecordPos& pos) {
    char value[sizeof(LogRecordPos)];
    std::memcpy(value, &pos, sizeof(LogRecordPos));
    tree_.insert(key.data(), value);
}

std::unique_ptr<LogRecordPos> BpTree::Get(const std::vector<char>& key) {
    char value[sizeof(LogRecordPos)];
    bpt.search(key.data(), value);
    LogRecordPos pos;
    std::memcpy(&pos, value, sizeof(LogRecordPos));
    return std::make_unique<LogRecordPos>(pos);
}

bool BpTree::Delete(const std::vector<byte>& key) {
    return bpt.remove(reinterpret_cast<const char*>(key.data())) == -1 ? 0 : 1;
}

int BpTree::Size() {
    // Implement size calculation
    // This may require modifying the zcbenz/BPlusTree implementation to expose size info
    return 0; // Placeholder
}
