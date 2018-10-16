//
//  benchmark.hpp
//  db_benchmarks
//
//  Created by 戴硕 on 2018/10/16.
//

#ifndef benchmark_h
#define benchmark_h

#include <set>
#include <vector>
#include <random>
#include "data.hpp"
#include "timer.hpp"

template <typename RecordType, int S>
class benchmark {
    enum { init_size = S };
    
public:
    benchmark(std::shared_ptr<database<RecordType>> db) : _db(db) {
        
    }
    
    void run() {
        ns_timer t;
        _db->open_or_create();
        
        std::cout << "add initial dataset: " << init_size << " records...\n";
        RecordType r;
        std::set<typename RecordType::key_type> k;
        while (k.size() < init_size) {
            r.randomize();
            _db->put(r);
            k.insert(r.key());
        }
        std::vector<typename RecordType::key_type> keys(k.begin(), k.end());
        std::size_t try_count = 100;
        std::vector<std::size_t> positions;
        
        std::set<RecordType> to_inserts;
        while (to_inserts.size() < try_count) {
            r.randomize();
            if (k.find(r.key()) == k.end()) {
                to_inserts.insert(r);
            }
        }
        k.clear();
        
        std::cout << "add " << try_count << " records...\n";
        t.reset(); t.start();
        for (const RecordType& rec: to_inserts) {
            _db->put(rec);
        }
        t.stop();
        std::cout << "avg = " << t.duration() / try_count << " ns per record\n";
        
        
        for (const RecordType& rec: to_inserts) {
            keys.push_back(rec.key());
        }
        
        get_random_positions(keys.size(), try_count, positions);
        std::cout << "query " << try_count << " records...\n";
        t.reset(); t.start();
        for (std::size_t p: positions) {
            _db->get(keys[p]);
        }
        t.stop();
        std::cout << "avg = " << t.duration() / try_count << " ns per record\n";
        
        get_random_positions(keys.size(), try_count, positions);
        std::cout << "update " << try_count << " records...\n";
        t.reset(); t.start();
        for (std::size_t p: positions) {
            _db->modify(keys[p], [](RecordType& rec) {
                rec.randomize_data();
            });
        }
        t.stop();
        std::cout << "avg = " << t.duration() / try_count << " ns per record\n";
        
        get_random_positions(keys.size(), try_count, positions);
        std::cout << "delete " << try_count << " records...\n";
        t.reset(); t.start();
        for (std::size_t p: positions) {
            _db->del(keys[p]);
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
    std::shared_ptr<database<RecordType>> _db;
};


#endif /* benchmark_h */
