#include "redis/types.hpp"
#include "Options.hpp"
#include "Code.hpp"
#include "Batch.hpp"
#include <stdexcept>
#include "DB.hpp"
#include "Options.hpp"
#include "Type.hpp"

RedisDataStructure::RedisDataStructure(const Options &opt)
{
    db = std::make_unique<DB>(opt);
}

void RedisDataStructure::Set(const std::vector<byte> &key,
                             std::chrono::nanoseconds ttl,
                             const std::vector<byte> &value)
{
    if (value.empty())
        throw std::runtime_error("RedisDataStructure::Set value is empty");
    if (key.empty())
        throw std::runtime_error("RedisDataStructure::Set key is empty");

    std::vector<byte> buf(MaxVarintLen64 + 1);         // Placeholder size for MaxVarintLen64 + 1
    buf[0] = static_cast<byte>(RedisDataType::String); // String type identifier (assumed)

    size_t index = 1;
    int64_t expire = 0;

    if (ttl.count() != 0)
    {
        auto now = std::chrono::system_clock::now();
        expire = std::chrono::duration_cast<std::chrono::seconds>(
                     now.time_since_epoch() + ttl)
                     .count();
    }

    index += Code::PutVarint(buf, index, expire);

    std::copy(value.begin(), value.end(), buf.begin() + index);
    buf.resize(value.size() + index);

    db->Put(key, buf);
}

bool RedisDataStructure::HSet(const std::vector<byte> &key, const std::vector<byte> &field, const std::vector<byte> &value)
{
    auto meta = findMetaData(key, RedisDataType::Hash);
    auto hk = hashInternalKey(key, meta.version, field);
    auto encKey = hk.encode();
    int exist = true;
    try
    {
        db->Get(encKey);
    }
    catch (const std::exception& e)
    {
        exist = false;
    }
    WriteBatchOptions opt;
    auto wb = db->NewWriteBatch(opt);
    if (!exist)
    {
        meta.size++;
        wb->Put(key, meta.encode());
    }
    try
    {
        wb->Put(encKey, value);
        wb->Commit();
    }
    catch (const std::exception& e)
    {
        return false;
    }
    return !exist;
}

std::vector<byte>
RedisDataStructure::HGet(const std::vector<byte> &key, const std::vector<byte> &field)
{
    auto meta = findMetaData(key, RedisDataType::Hash);
    if (meta.size == 0)
        return {};

    auto hk = hashInternalKey(key, meta.version, field);

    return db->Get(hk.encode());
}

bool RedisDataStructure::HDel(const std::vector<byte> &key, const std::vector<byte> &field)
{
    auto meta = findMetaData(key, RedisDataType::Hash);
    if (meta.size == 0)
        return false;

    auto hk = hashInternalKey(key, meta.version, field);
    auto encKey = hk.encode();
    int exist = true;
    try
    {
        db->Get(encKey);
    }
    catch (const std::exception& e)
    {
        exist = false;
    }

    if (exist)
    {
        WriteBatchOptions opt;
        auto wb = db->NewWriteBatch(opt);
        meta.size--;
        wb->Put(key, meta.encode());
        wb->Delete(encKey);
        try
        {
            wb->Commit();
        }
        catch (const std::exception& e)
        {
            return false;
        }
    }

    return exist;
}

std::vector<byte> RedisDataStructure::Get(const std::vector<byte> &key)
{
    std::vector<byte> encValue;
    try
    {
        encValue = db->Get(key);
    }
    catch (const std::exception& e)
    {
        std::cerr << "RedisDataStructure::Get Key is Not Found. Message: " << e.what() << std::endl;
        return {};
    }
    if (encValue.empty())
        return {};

    // Decode dataType from encValue
    uint8_t dataType = encValue[0];
    if (dataType != static_cast<uint8_t>(RedisDataType::String))
    { // Assuming 'S' is the String type
        std::cerr << "Error: Wrong type operation" << std::endl;
        return {};
    }

    int index = 1;
    int64_t expire = 0;

    // Decode expiration time (equivalent to binary.Varint)
    int varintSize = 0;
    expire = Code::GetVarint(encValue, index, &varintSize);
    index += varintSize;

    // Check if expired
    auto now = std::chrono::system_clock::now().time_since_epoch();
    int64_t currentNanoTime = std::chrono::duration_cast<std::chrono::seconds>(now).count();
    if (expire > 0 && expire <= currentNanoTime)
        return {};

    // Return the actual value (rest of encValue starting at index)
    std::vector<uint8_t> result(encValue.begin() + index, encValue.end());
    return std::move(result);
}

void RedisDataStructure::Del(const std::vector<byte> &key)
{
    db->Delete(key);
}

RedisDataType RedisDataStructure::Type(const std::vector<byte> &key)
{
    try
    {
        auto encValue = db->Get(key);
        return static_cast<RedisDataType>(encValue[0]);
    }
    catch (const std::exception &e)
    {
        return RedisDataType::NIL;
    }
}

metadata RedisDataStructure::findMetaData(const std::vector<byte> &key, RedisDataType datatype)
{
    int exist = true;
    std::vector<byte> metabuf;
    metadata meta;
    try
    {
        metabuf = db->Get(key);
        meta = metadata::decode(metabuf);
        if (meta.dataType != datatype)
            throw std::runtime_error("RedisDataStructure::findMetaData Data Type is Wrong");
        auto now = std::chrono::system_clock::now().time_since_epoch();
        int64_t currentNanoTime = std::chrono::duration_cast<std::chrono::seconds>(now).count();
        if (meta.expire != 0 && meta.expire <= currentNanoTime)
            exist = false;
    }
    catch (const std::exception& e)
    {
        exist = false;
    }
    if (!exist)
    {
        uint64 head = 0, tail = 0;
        if (datatype == RedisDataType::List)
            head = tail = metadata::initialListMark;
        return {datatype, 0, std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count(), 0, head, tail};
    }

    return meta;
}
