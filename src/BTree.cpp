#include "BTree.hpp"
#include <mutex>


// BTree::BTree() {
// }    


bool BTree::Put(const std::vector<byte>& key, std::unique_ptr<LogRecordPos> pos) { // in cpp set, there is no replaceOrInsert
    Item item(key, std::move(pos));
    Item key_item(key); // maybe i shouldn't use unique...
    Item key_item_erase(key);
    if(btree.find(std::move(key_item)) != btree.end())
        btree.erase(std::move(key_item_erase));
    btree.insert(std::move(item));
    return true;
}

std::unique_ptr<LogRecordPos> BTree::Get(const std::vector<byte>& key) {
   // std::shared_lock<std::shared_mutex> lock(this->RWMutex); don't need this read lock in DB::Get has read-lock
   // we just wana different procs' DB::Get maintain read-parel
    Item item(key);
    auto iter = btree.find(item);
    if(iter == btree.end())
        return nullptr;
    return std::move(iter->Pos);
}

bool BTree::Delete(const std::vector<byte>& key) {
    // std::unique_lock<std::shared_mutex> lock(this->RWMutex);
    Item item(key);
    return btree.erase(std::move(item));
}