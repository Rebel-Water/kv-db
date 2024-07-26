#pragma once
#include "Type.hpp"
#include <vector>
#include <memory>
#include "Options.hpp"
#include "LogRecord.hpp"
#include "DB.hpp"

class DB;
class Iterator_Interface {
    public:
    virtual void Rewind() = 0;
    virtual void Seek(const std::vector<byte>& key) = 0;
    virtual void Next() = 0;
    virtual bool Valid() = 0;
    virtual std::vector<byte> Key() = 0;
    virtual LogRecordPos Value() = 0;
    virtual void Close() = 0;
};

class Iterator {
    public:
    void Rewind();
    void Seek(const std::vector<byte>& key);
    void Next();
    bool Valid() const;
    std::vector<byte> Key() const;
    std::vector<byte> Value();
    void Close();

    void SkipToNext();
    Iterator(DB* db, std::unique_ptr<Iterator_Interface> ptr, IteratorOptions option) : db(db), index(std::move(ptr)), option(option)
    {}
    std::unique_ptr<Iterator_Interface> index;
	IteratorOptions option;
    DB* db;
};