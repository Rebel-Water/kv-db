#include "Iterator.hpp"
#include <cstring>

void Iterator::Rewind() {
    index->Rewind();
    SkipToNext();
}

void Iterator::Seek(const std::vector<uint8_t>& key) {
    index->Seek(key);
    SkipToNext();
}

void Iterator::Next() {
    index->Next();
    SkipToNext();
}

bool Iterator::Valid() const {
    return index->Valid();
}

std::vector<uint8_t> Iterator::Key() const {
    return index->Key();
}

std::vector<byte> Iterator::Value() {
    auto logRecordPos = index->Value();
    return db->getValueByPosition(logRecordPos);
}

void Iterator::Close() {
    index->Close();
}

void Iterator::SkipToNext() {
    int prefixLen = option.Prefix.size();
    if (prefixLen == 0)
        return;

    while (index->Valid()) {
        auto key = index->Key();
        if (prefixLen <= key.size() && std::memcmp(option.Prefix.data(), key.data(), prefixLen) == 0)
            break;
        index->Next();
    }
}