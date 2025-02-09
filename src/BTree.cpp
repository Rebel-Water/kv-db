#include "BTree.hpp"
#include <cstring>

BTree::BTreeIterator::BTreeIterator(const BTree &btree, bool reverse)
    : curIndex(0), reverse(reverse)
{
    values.reserve(btree.btree.size());
    if (reverse)
        for (auto it = btree.btree.rbegin(); it != btree.btree.rend(); ++it)
            values.push_back(*it);
    else
        for (auto it = btree.btree.begin(); it != btree.btree.end(); ++it)
            values.push_back(*it);
}

void BTree::BTreeIterator::Seek(const std::vector<byte> &key)
{
    if (reverse)
        curIndex = std::lower_bound(values.begin(), values.end(), key, [](const Item &item, const std::vector<byte> &key)
                                    { return std::memcmp(item.Key.data(), key.data(), key.size()) <= 0; }) -
                   values.begin();
    else
        curIndex = std::lower_bound(values.begin(), values.end(), key, [](const Item &item, const std::vector<byte> &key)
                                    { return std::memcmp(item.Key.data(), key.data(), key.size()) >= 0; }) -
                   values.begin();
}

void BTree::BTreeIterator::Rewind() {
    curIndex = 0;
}

void BTree::BTreeIterator::Next() {
    curIndex++;
}

bool BTree::BTreeIterator::Valid() {
    return curIndex < values.size();
}

std::vector<byte> BTree::BTreeIterator::Key() {
    return values[curIndex].Key;
}

LogRecordPos BTree::BTreeIterator::Value() {
    return values[curIndex].Pos;
}

void BTree::BTreeIterator::Close() {
    values.clear();
}

int BTree::Size() {
    return btree.size();
}

std::unique_ptr<Iterator_Interface> BTree::Iter(bool reverse) {
    return std::make_unique<BTreeIterator>(*this, reverse);
}

std::optional<LogRecordPos> BTree::Put(const std::vector<byte> &key, const LogRecordPos &pos)
{ // in this libary set, there is no insert_or_assign
    Item item(key, pos);
    auto oldItem = btree.find(item);
    std::optional<LogRecordPos> ret(std::nullopt);
    if (oldItem != btree.end()) {
        ret = oldItem->Pos; 
        btree.erase(item);
    }
    btree.insert(std::move(item));
    return std::move(ret);
}

std::optional<LogRecordPos> BTree::Get(const std::vector<byte> &key)
{
    // std::shared_lock<std::shared_mutex> lock(this->RWMutex); don't need this read lock in DB::Get has read-lock
    // we just wana different procs' DB::Get maintain read-parel
    Item item(key);
    auto iter = btree.find(item);
    if (iter == btree.end())
        return std::nullopt; 
    return std::make_optional(iter->Pos);
}

std::optional<LogRecordPos> BTree::Delete(const std::vector<byte> &key)
{
    Item item(key);
    auto oldItem = btree.find(item);
    std::optional<LogRecordPos> ret(std::nullopt);
    if(oldItem != btree.end()) {
        ret = oldItem->Pos;
        btree.erase(std::move(item));
    }
    return std::move(ret);
}