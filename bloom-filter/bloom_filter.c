#include "bloom_filter.h"
#include <stdlib.h>

uint64_t calc_hash(const char* str, uint64_t modulus, uint64_t seed) {
    uint64_t h = 0;
    uint64_t m = 1;
    const uint64_t k = 28;
    for (uint64_t i = 0; str[i] != '\0'; ++i) {
        uint64_t x = (uint64_t) (str[i] - 'a' + 1);
        h = (h + m * x) % modulus;
        m = (m * k) % modulus;
    }
    return h + seed;
}

void bloom_init(struct BloomFilter* bloom_filter, uint64_t set_size, hash_fn_t hash_fn, uint64_t hash_fn_count) {
    bloom_filter->set = NULL;
    bloom_filter->set_size = set_size;
    bloom_filter->hash_fn = hash_fn;
    bloom_filter->hash_fn_count = hash_fn_count;
}

uint64_t idx = 0;

void bloom_destroy(struct BloomFilter* bloom_filter) {
    if (bloom_filter->set != NULL) {
        free(bloom_filter->set);
    }
}

void bloom_insert(struct BloomFilter* bloom_filter, Key key) {
    if (bloom_filter->set == NULL) {
        bloom_filter->set = (uint64_t *) malloc(bloom_filter->set_size * sizeof(uint64_t));
    }
    bloom_filter->set[idx] = calc_hash(key, 1488, bloom_filter->hash_fn_count);
    ++idx;
}

bool bloom_check(struct BloomFilter* bloom_filter, Key key) {
    uint64_t check = calc_hash(key, 1488, bloom_filter->hash_fn_count);
    if (bloom_filter->set != NULL) {
        for (uint64_t i = 0; i < bloom_filter->set_size; ++i) {
            if (check == bloom_filter->set[i]) {
                return true;
            }
        }
    }
    return false;
}
