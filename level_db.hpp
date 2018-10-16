//
//  level_db.hpp
//  db_benchmarks
//
//  Created by 戴硕 on 2018/10/15.
//

#ifndef level_db_h
#define level_db_h

#include <boost/filesystem.hpp>
#include <leveldb/db.h>
#include <leveldb/options.h>
#include "data.hpp"

template <typename RecordType>
class leveldb_database : public database<RecordType> {
public:
    leveldb_database(const std::string& db_filename): database<RecordType>(db_filename) {
        
    }
    
    void open_or_create() override {
        leveldb::Options options;
        options.create_if_missing = true;
        leveldb::DB::Open(options, this->_db_filename, &_db);
    }
    
    void close() override {
        delete _db;
        _db = nullptr;
    }
    
    void remove() override {
        boost::filesystem::path p{this->_db_filename.c_str()};
        boost::filesystem::remove_all(p);
    }
    
    const RecordType get(const typename RecordType::key_type& key) override {
        std::string s;
        _db->Get(leveldb::ReadOptions(), leveldb::Slice((const char*)&key, sizeof(key)), &s);
        return RecordType::from_string(s);
    }
    
    void put(const RecordType& r) override {
        auto& key = r.key();
        _db->Put(leveldb::WriteOptions(),
                 leveldb::Slice((const char*)&key, sizeof(typename RecordType::key_type)),
                 RecordType::to_string(r));
    }
    
    void del(const typename RecordType::key_type& key) override {
        _db->Delete(leveldb::WriteOptions(), leveldb::Slice((const char*)&key, sizeof(key)));
    }
    
private:
    leveldb::DB *_db;
};

using leveldb_database_with_i64_key = leveldb_database<record_with_i64_key>;
using leveldb_database_with_s16_key = leveldb_database<record_with_s16_key>;

#endif /* level_db_h */
