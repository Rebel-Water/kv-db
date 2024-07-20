#pragma once
#include "Type.hpp"
#include "LogRecord.hpp"
#include <vector>
#include <memory>


class Indexer {
    public:
    virtual bool Put(const std::vector<byte>& key, std::unique_ptr<LogRecordPos> data) = 0;
    virtual std::unique_ptr<LogRecordPos> Get(const std::vector<byte>& key) = 0;
    virtual bool Delete(const std::vector<byte>& key) = 0;
};

class Item {
    public:
    bool operator <(const Item& other) const {
        return Key < other.Key;
    }
    Item() {
    }
    Item(const std::vector<byte>& key) : Key(key) {
    }
    Item(const std::vector<byte>& key, std::unique_ptr<LogRecordPos> pos) : Key(key), Pos(std::move(pos)) {
    }
    std::vector<byte> Key;
    std::unique_ptr<LogRecordPos> Pos; 
};

