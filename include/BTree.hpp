#pragma once

#include "lib/btree.hpp"
#include "lib/set.hpp"

#include <shared_mutex>
#include "Indexer.hpp"
#include "Iterator.hpp"

class BTree : public Indexer
{
    class BTreeIterator : public Iterator_Interface
    {
    public:
        BTreeIterator(const BTree &btree, bool reverse);
        virtual void Rewind();
        virtual void Seek(const std::vector<byte> &key);
        virtual void Next();
        virtual bool Valid();
        virtual std::vector<byte> Key();
        virtual LogRecordPos Value();
        virtual void Close();

        int curIndex;
        bool reverse;
        std::vector<Item> values;
    };
    // BTree();
public:
    virtual int Size();
    virtual std::unique_ptr<Iterator_Interface> Iter(bool reverse);
    virtual LogRecordPos Put(const std::vector<byte> &key, const LogRecordPos &data);
    virtual LogRecordPos Get(const std::vector<byte> &key);
    virtual LogRecordPos Delete(const std::vector<byte> &key);
    btree::set<Item> btree;
};