#include "SkipList.hpp"
#include <optional>

SkipList::SkipListIterator::SkipListIterator(SkipList &sk, bool reverse) : curIndex(0), reverse(reverse)
{
    values.reserve(sk.skiplist.size());
    if (reverse)
        for (auto it = sk.skiplist.rbegin(); it != sk.skiplist.rend(); ++it)
            values.push_back(*it);
    else
        for (auto it = sk.skiplist.begin(); it != sk.skiplist.end(); ++it)
            values.push_back(*it);
}

void SkipList::SkipListIterator::Rewind()
{
    curIndex = 0;
}

void SkipList::SkipListIterator::Next()
{
    curIndex++;
}

void SkipList::SkipListIterator::Seek(const std::vector<byte> &key)
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

bool SkipList::SkipListIterator::Valid()
{
    return curIndex < values.size();
}

std::vector<byte> SkipList::SkipListIterator::Key() {
    return values[curIndex].Key;
}

LogRecordPos SkipList::SkipListIterator::Value() {
    return values[curIndex].Pos;
}

void SkipList::SkipListIterator::Close() {
    values.clear();
}

int SkipList::Size()
{
    return skiplist.size();
}

std::unique_ptr<Iterator_Interface> SkipList::Iter(bool reverse)
{
    return std::make_unique<SkipListIterator>(*this, reverse);
}

std::optional<LogRecordPos> SkipList::Put(const std::vector<byte> &key, const LogRecordPos &pos)
{ // in this libary set, there is no insert_or_assign
    Item item(key, pos);
    auto oldItem = skiplist.find(item);
    std::optional<LogRecordPos> ret(std::nullopt);
    if (oldItem != skiplist.end())
    {
        ret = (*oldItem).Pos;
        skiplist.erase(item);
    }
    skiplist.insert(item);
    return std::move(ret);
}

std::optional<LogRecordPos> SkipList::Get(const std::vector<byte> &key)
{
    Item item(key);
    auto iter = skiplist.find(item);
    if (iter == skiplist.end())
        return std::nullopt;
    return std::make_optional((*iter).Pos);
}

std::optional<LogRecordPos> SkipList::Delete(const std::vector<byte> &key)
{
    Item item(key);
    auto oldItem = skiplist.find(item);
    std::optional<LogRecordPos> ret(std::nullopt);
    if (oldItem != skiplist.end())
    {
        ret = (*oldItem).Pos;
        skiplist.erase(std::move(item));
    }
    return std::move(ret);
}
