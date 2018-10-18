//
//  benchmark_huge.hpp
//  db_benchmarks
//
//  Created by 戴硕 on 2018/10/17.
//

#ifndef benchmark_huge_h
#define benchmark_huge_h

#include <set>
#include <vector>
#include <random>
#include "data.hpp"
#include "timer.hpp"

class benchmark_huge {
    
public:
    benchmark_huge(std::shared_ptr<database_with_i64_key> db, int64_t init_size) : _db(db), _init_size(init_size) {
        
    }
    
    void run() {
        ns_timer t;
        _db->open_or_create();
        
        std::cout << "add initial dataset: " << _init_size << " records...\n";
        record_with_i64_key r;
        int64_t max_key = _init_size;
        for (int64_t i = 0; i < _init_size; i++) {
            r.set_key(i);
            r.randomize_data();
            _db->put(r);
        }
        
        std::size_t try_count = 100;
        std::vector<std::size_t> positions;
        
        std::cout << "add " << try_count << " records...\n";
        t.reset(); t.start();
        for (int64_t i = 0; i < try_count ; i++) {
            r.set_key(i + max_key);
            _db->put(r);
        }
        t.stop();
        max_key += try_count;
        std::cout << "avg = " << t.duration() / try_count << " ns per record\n";
        
        get_random_positions(max_key, try_count, positions);
        std::cout << "query " << try_count << " records...\n";
        t.reset(); t.start();
        for (std::size_t p: positions) {
            _db->get((int64_t)p);
        }
        t.stop();
        std::cout << "avg = " << t.duration() / try_count << " ns per record\n";
        
        get_random_positions(max_key, try_count, positions);
        std::cout << "update " << try_count << " records...\n";
        t.reset(); t.start();
        for (std::size_t p: positions) {
            _db->modify((int64_t)p, [](record_with_i64_key& rec) {
                rec.randomize_data();
            });
        }
        t.stop();
        std::cout << "avg = " << t.duration() / try_count << " ns per record\n";
        
        get_random_positions(max_key, try_count, positions);
        std::cout << "delete " << try_count << " records...\n";
        t.reset(); t.start();
        for (std::size_t p: positions) {
            _db->del((int64_t)p);
        }
        t.stop();
        std::cout << "avg = " << t.duration() / try_count << " ns per record\n";
        
        _db->close_and_remove();
    }
    
private:
    void get_random_positions(std::size_t size, std::size_t count, std::vector<std::size_t>& holder) {
        holder.clear();
        holder.shrink_to_fit();
        holder.reserve(count);
        if (size > 0) {
            auto rand_eng = std::mt19937_64(std::random_device()());
            std::uniform_int_distribution<> distr(0, size - 1);
            for (std::size_t i = 0; i < count; i++) {
                holder.push_back(distr(rand_eng));
            }
        }
    }
    
private:
    std::shared_ptr<database_with_i64_key> _db;
    int64_t _init_size;
};

#endif /* benchmark_huge_h */
