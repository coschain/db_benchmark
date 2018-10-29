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

A series of tests were performed on different data scales. Results are measured in ***transactions per second***.

Tests run on a macbook pro with 16GB DDR3 RAM, Intel Core i5 2.9GHz and SSD disk.

*Note: an UPDATE operation queries the target first, and then changes its content.*

### initial dataset = 100,000

|             | multi_index/key: int64 | multi_index/key: string16 | leveldb/key: int64 | leveldb/key: string16 |
| ----------- | ---------------------- | ------------------------- | ------------------ | --------------------- |
| INSERT      | 324,780                | 306,654                   | 190,985            | 193,573               |
| QUERY       | 636,537                | 589,622                   | 36,627             | 39,842                |
| UPDATE      | 143,472                | 142,795                   | 32,107             | 29,975                |
| DELETE      | 568,504                | 515,463                   | 228,10             | 223,41                |

### initial dataset = 1,000,000

|             | multi_index/key: int64 | multi_index/key: string16 | leveldb/key: int64 | leveldb/key: string16 |
| ----------- | ---------------------- | ------------------------- | ------------------ | --------------------- |
| INSERT      | 270,197                | 241,954                   | 189,322            | 174,581               |
| QUERY       | 291,120                | 357,270                   | 33,231             | 34,575                |
| UPDATE      | 112,574                | 130,191                   | 29,455             | 28,918                |
| DELETE      | 267,665                | 338,753                   | 254,647            | 198,649               |

### initial dataset = 10,000,000

|             | multi_index/key: int64 | multi_index/key: string16 | leveldb/key: int64 | leveldb/key: string16 |
| ----------- | ---------------------- | ------------------------- | ------------------ | --------------------- |
| INSERT      | 183,183                | 184,535                   | 174,794            | 165,809               |
| QUERY       | 279,173                | 222,123                   | 23,347             | 12,996                |
| UPDATE      | 90,285                 | 78,443                    | 22,795             | 17,041                |
| DELETE      | 275,406                | 207,900                   | 248,015            | 237,812               |

### special case with dataset = 150,000,000 with int64 keys

150 million records and their indices are larger than physical RAM of testing machine, so that multi_index's mapping file has a much higher chance of page faults.

|             | multi_index/key: int64 | leveldb/key: int64 |
| ----------- | ---------------------- | -------------------|
| INSERT      | 179                    | 7,011              |
| QUERY       | 327                    | 2,994              |
| UPDATE      | 248                    | 1,594              |
| DELETE      | 92                     | 172,980            |

### Conclusion

- leveldb is good at writing.
- multi_index has a 15x faster query speed and a close writing speed in comparsion with leveldb.
- multi_index will be hundreds of times slower when dataset is larger than physical RAM.
