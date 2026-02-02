//
// Created by spring on 1/20/2026.
//

#ifndef FLASH_TOKENIZER_KV_H
#define FLASH_TOKENIZER_KV_H

#include"defines.h"

#if defined(USE_ROBIN_HOOD_LIB)
#include <robin_hood.h>
template<typename K, typename V, typename H = robin_hood::hash<K> >
using KVMAP = robin_hood::unordered_flat_map<K, V, H>;

template<typename K, typename H = robin_hood::hash<K> >
using KVSET = robin_hood::unordered_flat_set<K, H>;

SELECT_ANY const char *KVLIB = "robin_hood::unordered_flat_map";

#elif defined(USE_ANKERL_LIB)
#include <unordered_dense.h>


template<typename K, typename V, typename H=ankerl::unordered_dense::hash<K> >
using KVMAP = ankerl::unordered_dense::segmented_map<K, V, H>;

template<typename K, typename H=ankerl::unordered_dense::hash<K> >
using KVSET = ankerl::unordered_dense::segmented_set<K, H>;

SELECT_ANY const char *KVLIB = "ankerl::unordered_dense::segmented_map";
#elif defined(USE_ABSEIL_LIB)
#include <absl/container/flat_hash_set.h>
#include <absl/container/flat_hash_map.h>
#include <absl/container/hash_container_defaults.h>

template<typename K, typename V, typename H=absl::container_internal::hash_default_hash<K> >
using KVMAP = absl::flat_hash_map<K, V, H>;

template<typename K, typename H=absl::container_internal::hash_default_hash<K> >
using KVSET = absl::flat_hash_set<K, H>;

SELECT_ANY const char *KVLIB = "absl::flat_hash_map";
#endif


#endif //FLASH_TOKENIZER_KV_H
