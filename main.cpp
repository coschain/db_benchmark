#include <iostream>
#include "multi_index.hpp"
#include "level_db.hpp"
#include "benchmark.hpp"


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

    return 0;
}
