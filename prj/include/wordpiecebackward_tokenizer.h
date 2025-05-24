//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                               License Agreement
//                            WordPieceBackwardTokenizer
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

#ifndef WNSE25Q0AU9UPQM0LRTS50YFDZCG0F7ZEXKTE878EJD1LYEWU42GZUGL
#define WNSE25Q0AU9UPQM0LRTS50YFDZCG0F7ZEXKTE878EJD1LYEWU42GZUGL


#include "defines.h"
#include "vocab.h"
#include "wordpiece_tokenizer.h"
#include "trie.h"

class WordpieceBackwardTokenizer final : public WordpieceTokenizer {
public:
    explicit WordpieceBackwardTokenizer(const Vocab &vocab_, std::string unk = "[UNK]")
        : WordpieceTokenizer(vocab_, unk) {
    }

    int tokenizer_ids(const std::string &token, const int max_length, std::vector<int> &input_ids) const  {
        const size_t token_size = token.size();
        if (token_size > static_cast<size_t>(max_length) && input_ids.size() < max_length) {
            input_ids.emplace_back(this->UNK_NUM);
            return input_ids.size();
        }
        const size_t original_size = input_ids.size();
        size_t pos = token_size;
        std::vector<int> temp_ids;
        std::string candidate;

        while (pos > 0) {
            bool found = false;
            size_t new_pos = pos;
            for (size_t i = 0; i < pos; ++i) {
                size_t len = pos - i;
                if (i == 0) {
                    candidate = token.substr(0, pos);
                    auto [m_len, m_idx] = initialTrie.search(candidate, 0);
                    if (m_idx != -1 && m_len == candidate.size()) {
                        found = true;
                        temp_ids.emplace_back(m_idx);
                        new_pos = i;
                        break;
                    }
                } else {
                    candidate = token.substr(i, len);
                    auto [m_len, m_idx] = suffixTrie.search(candidate, 0);
                    if (m_idx != -1 && m_len == candidate.size()) {
                        found = true;
                        temp_ids.emplace_back(m_idx);
                        new_pos = i;
                        break;
                    }
                }
            }
            if (!found) {
                input_ids.resize(original_size);
                if (input_ids.size() < max_length)
                    input_ids.emplace_back(vocab.get(UNK));

                return input_ids.size();
            }
            pos = new_pos;
        }
        const size_t current_size = input_ids.size();
        size_t space_left = 0;

        if (current_size < max_length) {
            space_left = max_length - current_size;
        }
        if (const size_t tokens_to_add = std::min(temp_ids.size(), space_left); tokens_to_add > 0) {
            input_ids.insert(input_ids.end(), temp_ids.rbegin(), temp_ids.rbegin() + tokens_to_add);
        }
        return input_ids.size();
    }
};


#endif
