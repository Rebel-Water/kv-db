#include <chrono>
#include <iostream>
#include "DB.hpp"
#include "Options.hpp"
#include "Type.hpp"
#include "Util.hpp"
#include "lib/random.hpp"
#include <vector>
#include <unordered_map>
#include <filesystem>

int main() {
    std::filesystem::remove_all("/home/ace/kv/data");
    Options opt;
    opt.Type = IndexType::SkipList_Index;
    DB db(opt);
    // Measure Put operation
    auto before = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < N; i++)
        db.Put(std::to_string(i), Random::generate_random_string(1024));
    auto after = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
    std::cout << "Put: " << duration.count() << " ms" << std::endl;
    // Measure Get operation
    before = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < N; i++)
        db.Get(Random::random_int());
    after = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
    std::cout << "Get: " << duration.count() << " ms" << std::endl;

    // Measure Delete operation
    before = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < N; i++)
        db.Delete(Random::random_int());
    after = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
    std::cout << "Del: " << duration.count() << " ms" << std::endl;

    return 0;
}
