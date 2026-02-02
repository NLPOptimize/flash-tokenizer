//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                               License Agreement
//                               Tokenizer Functions
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
#ifndef LVCZSHPWCVMPWCPYHRKYHEJBSZRTVAXCMPUQFCFWFSTTETZCMVYCJYDVVSSNZS
#define LVCZSHPWCVMPWCPYHRKYHEJBSZRTVAXCMPUQFCFWFSTTETZCMVYCJYDVVSSNZS
#include "defines.h"
#include <codecvt>
#include <array>
#include <memory_resource>
#include"charmap.h"


constexpr std::array<bool, 65520> create_punctuation_array() {
    std::array<bool, 65520> arr{};

    for (int i = 33; i <= 47; i++) arr[i] = true;
    for (int i = 58; i <= 64; i++) arr[i] = true;
    for (int i = 91; i <= 96; i++) arr[i] = true;
    for (int i = 123; i <= 126; i++) arr[i] = true;

    for (int i = 0x2000; i <= 0x206F; i++) arr[i] = true;
    for (int i = 0x3000; i <= 0x303F; i++) arr[i] = true;
    for (int i = 0xFF00; i <= 0xFFEF; i++) arr[i] = true;
    for (int i = 0xFE30; i <= 0xFE4F; i++) arr[i] = true;

    constexpr int special_punct[] = {
        0x201C, 0x201D, 0x2018, 0x2019, 0x300C, 0x300D,
        0x300E, 0x300F, 0xFF5F, 0xFF60, 0x2E80, 0x2E99,
        0x2E9B, 0x2EF3, 0x2028, 0x2029, 0x30FB, 183
    };
    for (int cp: special_punct) arr[cp] = true;

    return arr;
}

constexpr std::array<bool, 8293> create_control_array() {
    std::array<bool, 8293> arr{};

    for (int i = 0; i <= 0x1F; i++) {
        if (i != 0x09 && i != 0x0A && i != 0x0D) {
            arr[i] = true;
        }
    }

    for (int i = 0x7F; i <= 0x9F; i++) arr[i] = true;

    for (int i = 0x200B; i <= 0x200F; i++) arr[i] = true;
    for (int i = 0x202A; i <= 0x202E; i++) arr[i] = true;
    for (int i = 0x2060; i <= 0x2064; i++) arr[i] = true;

    return arr;
}

constexpr std::array<bool, 12289> create_whitespace_array() {
    std::array<bool, 12289> arr{};

    arr[0x20] = true;
    arr[0x09] = true;
    arr[0x0A] = true;
    arr[0x0D] = true;

    constexpr int unicode_spaces[] = {
        0xA0, 0x1680, 0x2000, 0x2001, 0x2002, 0x2003,
        0x2004, 0x2005, 0x2006, 0x2007, 0x2008, 0x2009,
        0x200A, 0x202F, 0x205F, 0x3000
    };
    for (int cp: unicode_spaces) arr[cp] = true;

    return arr;
}

constexpr auto PUNCTUATION_ARRAY = create_punctuation_array();
constexpr auto CONTROL_ARRAY = create_control_array();
constexpr auto WHITESPACE_ARRAY = create_whitespace_array();
constexpr std::array<size_t, 256> UTF8_CHAR_LEN_ARRAY = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1
};


inline bool is_punctuation_cp(const int cp) {
    return cp < PUNCTUATION_ARRAY.size() && PUNCTUATION_ARRAY[cp];
}

inline bool is_control_cp(const int cp) {
    return cp < CONTROL_ARRAY.size() && CONTROL_ARRAY[cp];
}

inline bool is_whitespace_cp(const int cp) {
    return cp < WHITESPACE_ARRAY.size() && WHITESPACE_ARRAY[cp];
}

inline size_t utf8_char_length(const unsigned char c) {
    return UTF8_CHAR_LEN_ARRAY[c];
}

FORCE_INLINE int utf8_to_codepoint(const std::string_view str, const size_t pos) {
    const size_t len = str.size();
    if (pos >= len)
        return 0;
    const auto *bytes = reinterpret_cast<const unsigned char *>(str.data() + pos);
    const unsigned char b0 = bytes[0];
    if (b0 < 128)
        return b0;
    if ((b0 & 0xE0) == 0xC0) {
        if (pos + 1 >= len)
            return 0;
        const unsigned char b1 = bytes[1];
        if ((b1 & 0xC0) != 0x80)
            return 0;
        const int cp = ((b0 & 0x1F) << 6) | (b1 & 0x3F);
        return (cp < 0x80) ? 0 : cp;
    }
    if ((b0 & 0xF0) == 0xE0) {
        if (pos + 2 >= len)
            return 0;
        const unsigned char b1 = bytes[1];
        const unsigned char b2 = bytes[2];
        if (((b1 & 0xC0) != 0x80) || ((b2 & 0xC0) != 0x80))
            return 0;
        const int cp = ((b0 & 0x0F) << 12) | ((b1 & 0x3F) << 6) | (b2 & 0x3F);
        return (cp < 0x800 || (cp >= 0xD800 && cp <= 0xDFFF)) ? 0 : cp;
    }
    if ((b0 & 0xF8) == 0xF0) {
        if (pos + 3 >= len)
            return 0;
        const unsigned char b1 = bytes[1];
        const unsigned char b2 = bytes[2];
        const unsigned char b3 = bytes[3];
        if (((b1 & 0xC0) != 0x80) || ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80))
            return 0;
        const int cp = ((b0 & 0x07) << 18) | ((b1 & 0x3F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);
        return (cp < 0x10000 || cp > 0x10FFFF) ? 0 : cp;
    }
    return 0;
}

FORCE_INLINE std::pmr::string to_lower_case_and_strip_accents(const std::string_view &text) {
    static constexpr std::array<char, 128> lower_map = [] {
        std::array<char, 128> map{};
        for (size_t i = 0; i < 128; ++i)
            map[i] = (i >= 'A' && i <= 'Z') ? static_cast<char>(i + 32) : static_cast<char>(i);
        return map;
    }();
    thread_local char buffer[2048];
    thread_local std::pmr::monotonic_buffer_resource pool(buffer, sizeof(buffer));
    pool.release();
    std::pmr::string result{&pool};
    result.reserve(1024);

    for (size_t i = 0; i < text.size();) {
        const int cp = utf8_to_codepoint(text, i);
        const size_t len = utf8_char_length(text[i]);

        if (cp < 128) {
            result.push_back(lower_map[static_cast<unsigned char>(text[i])]);
        } else {
            if (auto it = accent_mapping.find(cp); it != accent_mapping.end()) {
                result.append(it->second);
            } else {
                result.append(text.data() + i, len);
            }
        }
        i += len;
    }

    return result;
}

FORCE_INLINE bool is_chinese_char(const int &cp) {
    if (cp >= 0x4E00 && cp <= 0x9FFF) return true;
    if (cp >= 0x3400 && cp <= 0x4DBF) return true;
    if (cp >= 0xF900 && cp <= 0xFAFF) return true;
    if (cp >= 0x20000 && cp <= 0x2A6DF) return true;
    if (cp >= 0x2A700 && cp <= 0x2B73F) return true;
    if (cp >= 0x2B740 && cp <= 0x2B81F) return true;
    if (cp >= 0x2B820 && cp <= 0x2CEAF) return true;
    if (cp >= 0x2F800 && cp <= 0x2FA1F) return true;
    return false;
}

[[nodiscard]] FORCE_INLINE std::pmr::string clean_text(const std::string &text) {
    //thread_local char buffer[2048];
    //thread_local std::pmr::monotonic_buffer_resource pool(buffer, sizeof(buffer));
    //thread_local std::string space = " ";
    //pool.release();
    //std::pmr::string output{&pool};
    thread_local std::pmr::string output(2048, ' ');
    //output.reserve(1024);

    std::fill(output.begin(), output.end(), ' ');
    size_t output_index = 0;
    for (size_t i = 0; i < text.size();) {
        const int cp = utf8_to_codepoint(text, i);
        const size_t char_len = utf8_char_length(text[i]);
        if (cp == 0 || cp == 0xfffd || cp == 0x2028 || cp == 0x2029 || is_control_cp(cp)) {
            i += char_len;
            continue;
        }
        if (!is_whitespace_cp(cp)) {
            std::copy_n(text.begin() + i, char_len, output.begin() + output_index);
            output_index += char_len;
            //output.append(text.substr(i, char_len));
        } else {
            //output.append(space);
            output_index++;
        }
        i += char_len;
    }
    output.resize(output_index);
    return output;
}

[[nodiscard]] FORCE_INLINE std::pmr::string clean_and_tokenize(const std::string &text) {
    thread_local char buffer[2048];
    thread_local std::pmr::monotonic_buffer_resource pool(buffer, sizeof(buffer));
    pool.release();
    std::pmr::string output{&pool};
    output.reserve(1024);
    static const std::string space = " ";
    for (size_t i = 0; i < text.size();) {
        int cp = utf8_to_codepoint(text, i);
        const size_t char_len = utf8_char_length(text[i]);
        if (cp == 0 || cp == 0xfffd || cp == 0x2028 || cp == 0x2029 || is_control_cp(cp)) {
            i += char_len;
            continue;
        }
        if (is_whitespace_cp(cp)) {
            output.append(space);
        } else if (is_chinese_char(cp)) {
            output.append(space);
            output.append(text, i, char_len);
            output.append(space);
        } else {
            output.append(text, i, char_len);
        }
        i += char_len;
    }
    return output;
}

FORCE_INLINE void run_split_on_punc(const std::string_view text, std::vector<std::string> &output) {
    const size_t sz = text.size();
    size_t i = 0, word_start = 0;
    bool in_word = false, has_punc = false;

    while (i < sz) {
        const int cp = utf8_to_codepoint(text, i);
        const size_t len = utf8_char_length(text[i]);
        if (is_punctuation_cp(cp)) {
            has_punc = true;
            if (in_word) {
                output.emplace_back(text.substr(word_start, i - word_start));
                in_word = false;
            }
            output.emplace_back(text.substr(i, len));
        } else if (!in_word) {
            word_start = i;
            in_word = true;
        }
        i += len;
    }

    if (!has_punc) {
        output.emplace_back(text);
        return;
    }
    if (in_word) {
        output.emplace_back(text.substr(word_start));
    }
}


FORCE_INLINE void run_split_on_punc_do_lower(const std::string_view text, std::vector<std::string> &output) {
    const std::pmr::string &&processed_text = to_lower_case_and_strip_accents(text);
    run_split_on_punc(processed_text, output);
}

static std::pmr::vector<std::pmr::string> whitespace_tokenize(const std::pmr::string &text) {
    if (text.empty()) {
        return {};
    }
    thread_local char buffer[8192];
    thread_local std::pmr::monotonic_buffer_resource pool(buffer, sizeof(buffer));
    pool.release();
    std::pmr::vector<std::pmr::string> tokens{&pool};
    tokens.reserve(std::count(text.begin(), text.end(), ' ') + 1);
    const char *start = text.data();
    const char *end = start + text.size();
    const char *token_start = nullptr;
    for (const char *p = start; p != end; ++p) {
        if (!is_whitespace_cp(*p)) {
            if (!token_start) {
                token_start = p;
            }
        } else if (token_start) {
            tokens.emplace_back(token_start, p - token_start);
            token_start = nullptr;
        }
    }
    if (token_start) {
        tokens.emplace_back(token_start, end - token_start);
    }
    return tokens;
}

static void insertion_sort(std::vector<int> &vec) {
    if (vec.size() <= 1) return;
    const int n = static_cast<int>(vec.size());
    int *arr = vec.data();
    int validEnd = 0;
    for (int i = 0; i < n; ++i) {
        if (const int key = arr[i]; key >= 4) {
            int j = validEnd - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                --j;
            }
            arr[j + 1] = key;
            ++validEnd;
        }
    }
    vec.resize(validEnd);
}

FORCE_INLINE bool compare_ids(std::vector<int> &a, std::vector<int> &b) {
    if (a.empty()) return true;
    if (b.empty()) return false;
    const int min1 = *std::min_element(a.begin(), a.end());
    const int min2 = *std::min_element(b.begin(), b.end());
    if (min1 < min2)return true;
    insertion_sort(a);
    insertion_sort(b);
    return a < b;
}

#endif
