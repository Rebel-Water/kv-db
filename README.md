# The KV Storage Engine Based on Bitcask - C++ Implementation

A key-value storage engine built on the Bitcask model and implemented in modern C++. This database is compatible with Redis data structures and provides an HTTP interaction interface. It boasts high performance, characterized by high write throughput and low read amplification. Compared with Redis, which is memory-based, this database implements persistent storage, significantly reducing memory usage while offering various indexing structures for efficient data retrieval.

## Library Used

```shell
apt install zlib1g-dev
https://github.com/nlohmann/json
https://github.com/Kronuz/cpp-btree
https://github.com/google/benchmark
https://github.com/greensky00/skiplist
https://github.com/google/googletest
https://github.com/yhirose/cpp-httplib
```

## Examples

### Common Operations

```cpp
	Options option;
	DB db(option);
	db.Put("key1", "value1");
	auto res1 = db.Get("key1");
	auto res2 = db.Delete("key1");
```

### HTTP Interface

```cpp
    auto db = std::make_unique<DB>(opt);
    httplib::Server server;
    server.Post("/bitcask/put", handlePut);
    server.Get("/bitcask/get", handleGet);
    server.Delete("/bitcask/delete", handleDelete);
    server.Get("/bitcask/list", handleList);
    std::cout << "Server listening on port 8080..." << std::endl;
    server.listen("0.0.0.0", 8080);		
```

### Batch

```cpp
    // use a global mutex to ensure serialization
	Options option;
	DB  db(option);
	WriteBatchOptions wbOption;
	auto wb = db.NewWriteBatch(wbOption);
	wb->Put("first", "1");
    wb->Put("second", "2");
    wb->Delete("first");
    wb->Commit();
```

### Redis-Structure

```cpp
    Options opt;
    opt.DirPath = "/home/ace/kv/redis";
    auto redis = RedisDataStructure(opt);
    redis.Set("tiktok", std::chrono::seconds(0), "china");
    redis.Del("tiktok");

    auto flag = redis.HSet("key1", "field1", "value1");
    auto val = redis.HGet("key1", "field1");
    flag = redis.HDel("key1", "field1");
    val = redis.HGet("key1", "field1");

    auto res = redis.LPush("key1", "val1");
    res = redis.LPush("key1", "val1");
    val = redis.RPop("key1");
    val = redis.LPop("key1");

    auto ok = redis.ZAdd("key1", 113, "value1");
    auto score = redis.ZScore("key1", "value2");

    ok = redis.SAdd("key1", "value1");
    ok = redis.SIsMember("key2", "value1");
    ok = redis.SRem("key2", "value1");
```

