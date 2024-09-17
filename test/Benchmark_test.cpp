#include <benchmark/benchmark.h>
#include "DB.hpp"
#include "Options.hpp"
#include "Util.hpp"
#include "lib/random.hpp"
#include <vector>

static void Benchmark_DB_Put(benchmark::State &state) {
    Options opt;
    DB db(opt);
    for(const auto& _ : state) {
        for(int i = 0; i < N; i++)
            db.Put(std::to_string(i), Random::generate_random_string(4000));
    }
}
BENCHMARK(Benchmark_DB_Put);

static void Benchmark_DB_Get(benchmark::State &state) {
    Options opt;
    DB db(opt);
    for(int i = 0; i < N; i++)
        db.Put(std::to_string(i), Random::generate_random_string(4000));
    for(const auto& _ : state)
        for(int i = 0; i < N; i++)
            db.Get(Random::random_int());
}
BENCHMARK(Benchmark_DB_Get);
static void Benchmark_DB_Del(benchmark::State &state) {
    Options opt;
    DB db(opt);
    for(int i = 0; i < N; i++)
        db.Put(std::to_string(i), Random::generate_random_string(4000));
    for(const auto& _ : state)
        for(int i = 0; i < N; i++)
            db.Delete(Random::random_int());
}
BENCHMARK(Benchmark_DB_Del);
BENCHMARK_MAIN();
