#pragma once
#include "lib/bpt.hpp"
#include "Indexer.hpp"

class BpTree : public Indexer{
    public:
    BpTree(const std::string& path);
    virtual int Size();
    virtual std::unique_ptr<Iterator_Interface> Iter(bool reverse);
    virtual bool Put(const std::vector<byte> &key, const LogRecordPos &data);
    virtual std::unique_ptr<LogRecordPos> Get(const std::vector<byte> &key);
    virtual bool Delete(const std::vector<byte> &key);

    bpt::bplus_tree bpt;
};