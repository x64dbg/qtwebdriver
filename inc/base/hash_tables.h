// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Modernized: replaced deprecated hash_map/hash_set with
// std::unordered_map/std::unordered_set from C++11.

#ifndef BASE_HASH_TABLES_H_
#define BASE_HASH_TABLES_H_

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <functional>

#include "base/string16.h"

namespace std {

template<>
struct hash<string16> {
    std::size_t operator()(const string16& s) const {
        std::size_t result = 0;
        for (auto c : s)
            result = (result * 131) + c;
        return result;
    }
};

}  // namespace std

namespace base {
template <typename Key, typename Value, typename Hash = std::hash<Key>,
          typename Pred = std::equal_to<Key>>
using hash_map = std::unordered_map<Key, Value, Hash, Pred>;

template <typename Key, typename Hash = std::hash<Key>,
          typename Pred = std::equal_to<Key>>
using hash_set = std::unordered_set<Key, Hash, Pred>;
}  // namespace base

#endif  // BASE_HASH_TABLES_H_
