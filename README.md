# db_benchmark

A program to benchmark database solutions such as boost::multi_index_container, leveldb.

## Build

```shell
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
```

## Results

A series of tests were performed on different data scales. Results are measured in ***nano-seconds per record***.

Tests run on a macbook pro with 16GB DDR3 RAM, Intel Core i5 2.9GHz and SSD disk.

*Note: an UPDATE operation queries the target first, and then changes its content.*

### initial dataset = 100,000

|             | multi_index/key: int64 | multi_index/key: string16 | leveldb/key: int64 | leveldb/key: string16 |
| ----------- | ---------------------- | ------------------------- | ------------------ | --------------------- |
| INSERT      | 3079                   | 3261                      | 5236               | 5166                  |
| QUERY       | 1571                   | 1696                      | 27302              | 25099                 |
| UPDATE      | 6970                   | 7003                      | 31145              | 33361                 |
| DELETE      | 1759                   | 1940                      | 4384               | 4476                  |

### initial dataset = 1,000,000

|             | multi_index/key: int64 | multi_index/key: string16 | leveldb/key: int64 | leveldb/key: string16 |
| ----------- | ---------------------- | ------------------------- | ------------------ | --------------------- |
| INSERT      | 3701                   | 4133                      | 5282               | 5728                  |
| QUERY       | 3435                   | 2799                      | 30092              | 28922                 |
| UPDATE      | 8883                   | 7681                      | 33950              | 34580                 |
| DELETE      | 3736                   | 2952                      | 3927               | 5034                  |

### initial dataset = 10,000,000

|             | multi_index/key: int64 | multi_index/key: string16 | leveldb/key: int64 | leveldb/key: string16 |
| ----------- | ---------------------- | ------------------------- | ------------------ | --------------------- |
| INSERT      | 5459                   | 5419                      | 5721               | 6031                  |
| QUERY       | 3582                   | 4502                      | 42831              | 76946                 |
| UPDATE      | 11076                  | 12748                     | 43868              | 58680                 |
| DELETE      | 3631                   | 4810                      | 4032               | 4205                  |

### special case with dataset = 150,000,000 with int64 keys

150 million records and their indices are larger than physical RAM of testing machine, so that multi_index's mapping file has a much higher chance of page faults.

|             | multi_index/key: int64 | leveldb/key: int64 |
| ----------- | ---------------------- | -------------------|
| INSERT      | 5562617                | 142615             |
| QUERY       | 3049534                | 333895             |
| UPDATE      | 4016757                | 627281             |
| DELETE      | 10836929               | 5781               |

### Conclusion

- leveldb is good at writing.
- multi_index has a 15x faster query speed and a close writing speed in comparsion with leveldb.
- multi_index will be hundreds of times slower when dataset is larger than physical RAM.
