#pragma once
#include "lib/sl_set.hpp"
#include "Indexer.hpp"
#include "Util.hpp"
#include "Iterator.hpp"
#include <vector>

class SkipList : public Indexer {
    class SkipListIterator : public Iterator_Interface {
    public:
        SkipListIterator(SkipList &skiplist, bool reverse);
        virtual void Rewind();
        virtual void Seek(const std::vector<byte> &key);
        virtual void Next();
        virtual bool Valid();
        virtual std::vector<byte> Key();
        virtual LogRecordPos Value();
        virtual void Close();
    private:
        int curIndex;
        bool reverse;
        std::vector<Item> values;
    };
public:
    SkipList() {}
    virtual int Size();
    virtual std::unique_ptr<Iterator_Interface> Iter(bool reverse);
    virtual std::optional<LogRecordPos> Put(const std::vector<byte> &key, const LogRecordPos &data);
    virtual std::optional<LogRecordPos> Get(const std::vector<byte> &key);
    virtual std::optional<LogRecordPos>Delete(const std::vector<byte> &key);

private:
    sl_set_gc<Item> skiplist;
};