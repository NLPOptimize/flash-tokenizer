//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                               License Agreement
//                                 BasicTokenizer
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
#ifndef A509LU8VZQRP3PNW02Q5UH286VBZ13PY7GBV9F767ALLP3P9P50WVH7NZL7B3YS6
#define A509LU8VZQRP3PNW02Q5UH286VBZ13PY7GBV9F767ALLP3P9P50WVH7NZL7B3YS6
#include "functions.h"
#include "wordpiece_tokenizer.h"

using SplitFunction = void (*)(std::string_view, std::vector<std::string> &);
using CleanFunction = std::pmr::string (*)(const std::string &);

class BasicTokenizer {
public:
    const bool do_lower_case;
    const bool tokenize_chinese_chars;
    const SplitFunction split_function[2] = {run_split_on_punc, run_split_on_punc_do_lower};
    const CleanFunction clean_function[2] = {clean_text, clean_and_tokenize};

    explicit BasicTokenizer(const bool lower_case = true, const bool tokenize_chinese_chars = true)
        : do_lower_case(lower_case), tokenize_chinese_chars(tokenize_chinese_chars) {
    }

    [[nodiscard]] std::vector<std::string> tokenize(const std::string &text) const {
        const SplitFunction split_func = this->split_function[this->do_lower_case];
        const std::pmr::string &tokenized = this->clean_function[this->tokenize_chinese_chars](text);
        const std::pmr::vector<std::pmr::string> &orig_tokens = whitespace_tokenize(tokenized);

        thread_local std::vector<std::string> output_tokens(1024);
        output_tokens.clear();
        for (const auto &token: orig_tokens) {
            split_func(token, output_tokens);
        }
        return output_tokens;
    }

    void tokenize_early_stop(const std::string &text, const WordpieceTokenizer &wordpiece, const int max_length, std::vector<int> &input_ids, const size_t allowed_length) const {
        const SplitFunction split_func = this->split_function[this->do_lower_case];
        const std::pmr::string &tokenized = this->clean_function[this->tokenize_chinese_chars](text);
        const std::pmr::vector<std::pmr::string> &orig_tokens = whitespace_tokenize(tokenized);

        thread_local std::vector<std::string> output_tokens(1024);
        for (const auto &token: orig_tokens) {
            output_tokens.clear();
            split_func(token, output_tokens);
            for (const auto &_token: output_tokens) {
                if (wordpiece.tokenizer_ids(_token, max_length - 1, input_ids) == allowed_length) {
                    break;
                }
            }
        }
    }
};


#endif
