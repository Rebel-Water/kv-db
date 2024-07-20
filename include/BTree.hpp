#pragma once

#include "lib/btree.hpp"
#include "lib/set.hpp"

#include <shared_mutex>
#include "Indexer.hpp"

class BTree : public Indexer {
    // BTree();
    public:
    virtual bool Put(const std::vector<byte>& key, std::unique_ptr<LogRecordPos> data);
    virtual std::unique_ptr<LogRecordPos> Get(const std::vector<byte>& key);
    virtual bool Delete(const std::vector<byte>& key);
    btree::set<Item> btree;
    std::shared_mutex RWMutex;
};