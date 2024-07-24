#include "BTree.hpp"
#include "LogRecord.hpp"
#include "Code.hpp"
#include "DB.hpp"
#include <vector>
int main()
{
    try {
    Options option("/home/ace/kv/", 128, true, IndexType::BTree_Index);
    DB db(option);
    const char *k1 = "key_a";
    const char *k2 = "key_b";
    const char *k3 = "key_c";
    const char *v1 = "value_a";
    const char *v2 = "value_b";
    const char *v3 = "value_c";
    std::vector<byte> key1(k1, k1 + strlen(k1)), value1(v1, v1 + strlen(v1));
    std::vector<byte> key2(k2, k2 + strlen(k2)), value2(v2, v2 + strlen(v2));
    std::vector<byte> key3(k3, k3 + strlen(k3)), value3(v3, v3 + strlen(v3));
    db.Put(key1, value1);
    auto res1 = db.Get(key1);
    db.Put(key2, value2);
    auto res2 = db.Get(key2);
    db.Put(key1, value2);
    auto res3 = db.Get(key1);

    // db.Delete(key1);
    // db.Delete(key2);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
