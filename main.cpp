#include <iostream>
#include "multi_index.hpp"
#include "level_db.hpp"
#include "benchmark.hpp"
#include "benchmark_huge.hpp"


template <typename RecordType, typename DatabaseType, int Size>
void run_benchmark(const std::string& db_filename) {
    benchmark<RecordType, Size> bm(std::make_shared<DatabaseType>(db_filename));
    bm.run();
}

#define BM(TYPE, SIZE, DB) do { \
    std::cout << "<<<<<<<< " << #TYPE << ", " << SIZE << ", " << #DB << " >>>>>>>>\n"; \
    run_benchmark<record_with_##TYPE##_key, DB##_database_with_##TYPE##_key, SIZE>(#DB "_" #TYPE "_" #SIZE); \
    std::cout << "\n"; \
} while(0)

#define BM2(TYPE, SIZE) do { BM(TYPE, SIZE, multi_index); BM(TYPE, SIZE, leveldb); } while(0)


int main(int argc, const char * argv[]) {
    BM2(i64, 100000);
    BM2(i64, 1000000);
    BM2(i64, 10000000);
    BM2(s16, 100000);
    BM2(s16, 1000000);
    BM2(s16, 10000000);
    
    // a test case of extremely large data scale to benchmark multi_index in case of frequent page faults.
    int64_t huge_size = 150000000;
    benchmark_huge bm_huge1(std::make_shared<multi_index_database_with_i64_key>("huge_m.db", 50 * 1024 * 1048576ULL), huge_size);
    bm_huge1.run();
    benchmark_huge bm_huge2(std::make_shared<leveldb_database_with_i64_key>("huge_l.db"), huge_size);
    bm_huge2.run();

    return 0;
}
