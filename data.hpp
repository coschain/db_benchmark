//
//  data.h
//  db_benchmarks
//
//  Created by 戴硕 on 2018/10/15.
//

#ifndef data_h
#define data_h

#include <cstddef>
#include <random>
#include <algorithm>
#include <iostream>
#include <cstdio>

class string16 {
public:
    string16() {
        memset(_data, 0, sizeof(_data));
    }
    string16(const char *s, std::size_t size) {
        memset(_data, 0, sizeof(_data));
        memcpy(_data, s, size > 16? 16 : size);
    }
    string16(const std::string& s) : string16(s.data(), s.size()) {
        
    }
    string16(const string16& s) : string16(s.data(), 16) {
        
    }
    const char *data() const {
        return _data;
    }
    bool operator < (const string16& s) const {
        return memcmp(data(), s.data(), 16) < 0;
    }
    bool operator > (const string16& s) const {
        return memcmp(data(), s.data(), 16) > 0;
    }
    bool operator == (const string16& s) const {
        return memcmp(data(), s.data(), 16) == 0;
    }
    bool operator != (const string16& s) const {
        return memcmp(data(), s.data(), 16) != 0;
    }
private:
    char _data[16];
};

template <typename KeyType>
class record {
public:
    using key_type = KeyType;
    
    record() {
        randomize_key();
        randomize_data();
    }
    
    record(const KeyType& k) {
        memcpy(&_key, &k, sizeof(KeyType));
        randomize_data();
    }
    
    record(const record<KeyType>& r) {
        memcpy(this, &r, sizeof(r));
    }
    
    const KeyType& key() const {
        return _key;
    }
    
    const char* data() const {
        return _data;
    }
    
    const std::size_t data_size() const {
        return sizeof(_data);
    }
    
    void randomize_key() {
        randomize_bytes(&_key, sizeof(_key));
    }
    
    void randomize_data() {
        randomize_bytes(&_data, sizeof(_data));
    }
    
    void randomize() {
        randomize_key();
        randomize_data();
    }
    
    bool operator < (const record<KeyType>& r) const {
        return key() < r.key();
    }
    
    bool operator > (const record<KeyType>& r) const {
        return key() > r.key();
    }
    
    bool operator == (const record<KeyType>& r) const {
        return key() == r.key();
    }
    
    bool operator != (const record<KeyType>& r) const {
        return key() != r.key();
    }
    
    void operator = (const record<KeyType>& r) {
        memcpy(this, &r, sizeof(r));
    }
    
    static std::string to_string(const record<KeyType>& r) {
        char buf[sizeof(r._key) + sizeof(r._data)];
        memcpy(buf, &r._key, sizeof(r._key));
        memcpy(buf + sizeof(r._key), &r._data, sizeof(r._data));
        return std::string(buf, sizeof(buf));
    }
    
    static record<KeyType> from_string(const std::string& s) {
        record<KeyType> r;
        const char *buf = s.data();
        memcpy(&r._key, buf, sizeof(r._key));
        memcpy(&r._data, buf + sizeof(r._key) , sizeof(r._data));
        return r;
    }
    
private:
    static void randomize_bytes(void *buffer, std::size_t size) {
        std::random_device rd;
        std::independent_bits_engine<std::default_random_engine, CHAR_BIT, unsigned long> rbe(rd());
        unsigned char *begin = static_cast<unsigned char*>(buffer);
        unsigned char *end = begin + size;
        while (begin < end) {
            *begin++ = rbe();
        }
    }

private:
    KeyType _key;
    char _data[128];
};

using record_with_i64_key = record<int64_t>;
using record_with_s16_key = record<string16>;


template <typename RecordType>
class database {
public:
    using record_type = RecordType;
    using record_modifier = std::function<void(RecordType&)>;
    
    database(const std::string& db_filename) : _db_filename(db_filename) {
        
    }
    virtual ~database() {}
    
    virtual void open_or_create() = 0;
    virtual void close() = 0;
    virtual void remove() {
        std::remove(_db_filename.c_str());
    }
    virtual void close_and_remove() {
        close();
        remove();
    }
    
    virtual const RecordType get(const typename RecordType::key_type& key) = 0;
    virtual void put(const RecordType& r) = 0;
    virtual void del(const typename RecordType::key_type& key) = 0;
    
    virtual void modify(const typename RecordType::key_type& key, const record_modifier& m) {
        const RecordType& old_record = get(key);
        RecordType new_record = old_record;
        m(new_record);
        if (new_record != old_record) {
            del(old_record.key());
        }
        put(new_record);
    }
protected:
    std::string _db_filename;
};

using database_with_i64_key = database<record_with_i64_key>;
using database_with_s16_key = database<record_with_s16_key>;

#endif /* data_h */
