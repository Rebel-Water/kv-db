#pragma once
#include "Type.hpp"
#include "LogRecord.hpp"
#include <vector>
#include <memory>
#include "Iterator.hpp"

class Iterator_Interface;
class Indexer {
    public:
    virtual bool Put(const std::vector<byte>& key, const LogRecordPos& data) = 0;
    virtual std::unique_ptr<LogRecordPos> Get(const std::vector<byte>& key) = 0;
    virtual bool Delete(const std::vector<byte>& key) = 0;
    virtual int Size() = 0;
    virtual std::unique_ptr<Iterator_Interface> Iter(bool reverse) = 0;
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
    Item(const std::vector<byte>& key, const LogRecordPos& pos) : Key(key), Pos(pos) {
    }
    std::vector<byte> Key;
    LogRecordPos Pos; 
};

