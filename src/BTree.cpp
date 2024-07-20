#include "BTree.hpp"
#include <mutex>


// BTree::BTree() {
// }    


bool BTree::Put(const std::vector<byte>& key, std::unique_ptr<LogRecordPos> pos) {
    std::unique_lock<std::shared_mutex> lock(this->RWMutex);
    Item item(key, std::move(pos));
    btree.insert(std::move(item));
    return true;
}

std::unique_ptr<LogRecordPos> BTree::Get(const std::vector<byte>& key) {
    std::shared_lock<std::shared_mutex> lock(this->RWMutex);
    Item item(key);
    auto iter = btree.find(item);
    if(iter == btree.end())
        return nullptr;
    return std::move(iter->Pos);
}

bool BTree::Delete(const std::vector<byte>& key) {
    std::unique_lock<std::shared_mutex> lock(this->RWMutex);
    Item item(key);
    return btree.erase(std::move(item));
}