//
//  multi_index.hpp
//  db_benchmarks
//
//  Created by 戴硕 on 2018/10/15.
//

#ifndef multi_index_h
#define multi_index_h

#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <cstdio>
#include "data.hpp"

namespace bip = boost::interprocess;
namespace bmi = boost::multi_index;

template <typename RecordType>
class multi_index_database : public database<RecordType> {
    
    using container_type = boost::multi_index::multi_index_container<
        RecordType,
        bmi::indexed_by<
            bmi::ordered_unique<bmi::const_mem_fun<RecordType, const typename RecordType::key_type&, &RecordType::key>>
        >,
        bip::allocator<RecordType, bip::managed_mapped_file::segment_manager>
    >;
    using container_alloc_type = typename container_type::allocator_type;
    
public:
    multi_index_database(const std::string& db_filename, std::size_t file_size = 10 * 1024 * 1048576ULL)
        : database<RecordType>(db_filename), _file_size(file_size) {

    }
    
    void open_or_create() override {
        _seg = std::make_shared<bip::managed_mapped_file>(bip::open_or_create, this->_db_filename.c_str(), this->_file_size);
        _container = _seg->find_or_construct<container_type>("data")( container_alloc_type(_seg->get_segment_manager()) );
    }
    
    void close() override {
        _container = nullptr;
        _seg.reset();
    }
    
    const RecordType get(const typename RecordType::key_type& key) override {
        return *(_container->template get<0>().find(key));
    }
    
    void put(const RecordType& r) override {
        auto& idx = _container->template get<0>();
        auto ins_result = idx.insert(r);
        if (!ins_result.second) {
            idx.replace(ins_result.first, r);
        }
    }
    
    void del(const typename RecordType::key_type& key) override {
        _container->template get<0>().erase(key);
    }
    
    void modify(const typename RecordType::key_type& key, const typename database<RecordType>::record_modifier& m) override {
        auto& idx = _container->template get<0>();
        idx.modify(idx.find(key), m);
    }
    
private:
    std::shared_ptr<bip::managed_mapped_file> _seg;
    container_type *_container;
    std::size_t _file_size;
};

using multi_index_database_with_i64_key = multi_index_database<record_with_i64_key>;
using multi_index_database_with_s16_key = multi_index_database<record_with_s16_key>;

#endif /* multi_index_h */
