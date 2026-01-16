//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                               License Agreement
//                                     Vocab
//
//               Copyright (C) 2025, Kim Bomm, all rights reserved.
//
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
#ifndef WPSE67NL85E1DDCFWXUHATELTDAUWCQX2J728EEQNZ1PUTKXMPX6E8FVXLUS0PET
#define WPSE67NL85E1DDCFWXUHATELTDAUWCQX2J728EEQNZ1PUTKXMPX6E8FVXLUS0PET

#include <fstream>
#include <iostream>
#include "defines.h"
//#include <robin_hood.h>
#include <unordered_dense.h>

struct VTHasher {
    size_t operator()(const std::string &key) const {
        auto data = reinterpret_cast<const unsigned char *>(key.data());
        const size_t size = key.size();
        uint64_t hash = 0xcbf29ce484222325ull;
        const unsigned char *end = data + size;
        while (data < end) {
            hash = (*data++ ^ hash) * 0x100000001b3ull;
        }
        return static_cast<size_t>(hash);
    }
};

class Vocab {
public:
    std::vector<std::string> tokens{};
    //robin_hood::unordered_flat_map<std::string, int, VTHasher> token_to_index{};
    //robin_hood::unordered_flat_map<int, std::string> index_to_token{};
    ankerl::unordered_dense::segmented_map<std::string,int,VTHasher> token_to_index{};
    ankerl::unordered_dense::segmented_map<int,std::string> index_to_token{};

    explicit Vocab(const std::string &filename) {
        std::ifstream ifs(filename);
        if (!ifs) {
            std::cerr << "File not found" << filename << "\n";
            std::exit(1);
        }
        std::string line;
        while (std::getline(ifs, line)) {
            line.erase(line.find_last_not_of(" \n\r\t") + 1);
            if (line.empty()) continue;
            const auto idx = static_cast<int>(tokens.size());
            this->tokens.push_back(line);
            this->token_to_index[line] = idx;
            this->index_to_token[idx] = line;
        }
        ifs.close();
    }

    [[nodiscard]] int get(const std::string &token, const int default_value = 0) const {
        if (const auto it = this->token_to_index.find(token); it != this->token_to_index.end())
            return it->second;
        return default_value;
    }
    [[nodiscard]] std::string get(const int id) const {
        return this->index_to_token.find(id)->second;
    }
};

FORCE_INLINE std::vector<int>
convert_by_vocab(const Vocab &vocab, const std::vector<std::string> &items, const int max_length = -1) {
    const size_t size = items.size();
    const size_t final_size = (max_length != -1 && static_cast<size_t>(max_length) > size)
                                  ? static_cast<size_t>(max_length)
                                  : size;
    std::vector<int> ids(final_size, 0);

    const auto &token_to_index = vocab.token_to_index;
    size_t index = 0;

    for (const auto &item: items) {
        auto it = token_to_index.find(item);
        ids[index] = (it != token_to_index.end() ? it->second : -1);
        index++;
    }
    return ids;
}
#endif
