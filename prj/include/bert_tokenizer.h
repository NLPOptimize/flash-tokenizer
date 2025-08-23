//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this
//  license. If you do not agree to this license, do not download, install, copy
//  or use the software.
//
//
//                               License Agreement
//                               FlashBertTokenizer
//
//               Copyright (C) 2025, Kim Bomm, all rights reserved.
//
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright
//   notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote
//   products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is"
// and any express or implied warranties, including, but not limited to, the
// implied warranties of merchantability and fitness for a particular purpose
// are disclaimed. loss of use, data, or profits; or business interruption)
// however caused and on any theory of liability, whether in contract, strict
// liability, or tort (including negligence or otherwise) arising in any way out
// of the use of this software, even if advised of the possibility of such
// damage.
//
#pragma once
#ifndef MPNVTDXZKUBGNRMLPKGYLBWAARLLHQUSXVSQJDZTBGHQEJXGPMTNHHDFCVLMMU
#define MPNVTDXZKUBGNRMLPKGYLBWAARLLHQUSXVSQJDZTBGHQEJXGPMTNHHDFCVLMMU

#include "env.h"
#include "functions.h"
#include "robin_hood.h"
#include "version.h"
#include "vocab.h"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "basic_tokenizer.h"
#include "charmap.h"
#include "wordpiece_tokenizer.h"
#include "wordpiecebackward_tokenizer.h"

class FlashBertTokenizer
{
protected:
    const std::string UNK = "[UNK]";
    const std::string CLS = "[CLS]";
    const std::string SEP = "[SEP]";
    int CLS_NUM{};
    int SEP_NUM{};
    int UNK_NUM{};
    const int model_max_length;

    const Vocab vocab;
    const BasicTokenizer basic;
    const WordpieceTokenizer wordpiece;
    std::string _version_ = "Unknown";

public:
    struct Environment
    {
        std::string OS;
        std::string COMPILER_NAME;
        std::string PARALLEL_LIB;
    } env{};

    explicit FlashBertTokenizer(const std::string &vocab_file,
                                const bool do_lower_case = true,
                                const int model_max_length = -1,
                                const bool tokenize_chinese_chars = true)
        : vocab(vocab_file), basic(do_lower_case, tokenize_chinese_chars),
          model_max_length(model_max_length), wordpiece(vocab, this->UNK)
    {
        this->CLS_NUM = vocab.get(this->CLS);
        this->SEP_NUM = vocab.get(this->SEP);
        this->UNK_NUM = vocab.get(this->UNK);
        this->_version_ = cpp_env(VERSION_INFO_STR(VERSION_INFO));
        this->env.OS = GetOS();
        this->env.COMPILER_NAME = GetCompiler();
        this->env.PARALLEL_LIB = GetParallelImpl();
        if (accent_mapping.empty())
        {
            accent_mapping = initializeCharMap();
        }
    }

    virtual ~FlashBertTokenizer() = default;

    virtual std::string version() { return this->_version_; }

    [[nodiscard]] virtual std::vector<int> tokenizer_ids(const std::string &text,
                                                         const int max_length,
                                                         const std::string &padding) const
    {
        // Handle the -1 case by using model_max_length
        int effective_max_length = max_length;
        if (max_length == -1)
        {
            effective_max_length = this->model_max_length;
            // If model_max_length is also -1, use unlimited (but practical limit)
            if (effective_max_length == -1)
            {
                effective_max_length = std::numeric_limits<int>::max();
            }
        }

        const size_t allowed_length = static_cast<size_t>(effective_max_length - 1);
        thread_local std::vector<int> input_ids(1024);
        input_ids.clear();

        input_ids.emplace_back(this->CLS_NUM);
        basic.tokenize_early_stop(text, wordpiece, effective_max_length, input_ids, allowed_length);
        input_ids.emplace_back(this->SEP_NUM);

        if (padding == "max_length" && effective_max_length != std::numeric_limits<int>::max())
        {
            input_ids.resize(effective_max_length, 0);
        }

        return input_ids;
    }

    virtual std::vector<std::string> tokenize(const std::string &text)
    {
        constexpr auto max_length = std::numeric_limits<int>::max();
        const auto input_ids = this->tokenizer_ids(text, max_length, "longest");
        std::vector<std::string> tokens;
        tokens.reserve(input_ids.size() - 2);
        for (size_t i = 1; i < input_ids.size() - 1; i++)
        {
            tokens.emplace_back(this->vocab.get(input_ids[i]));
        }
        return tokens;
    }

    [[nodiscard]] virtual std::vector<int> encode(const std::string &text,
                                                  const std::string &padding = "max_length",
                                                  int max_length = -1) const
    {
        if (max_length == -1)
        {
            max_length = this->model_max_length;
        }
        return this->tokenizer_ids(text, max_length, padding);
    }

    [[nodiscard]] virtual std::vector<std::vector<int>>
    batch_encode(const std::vector<std::string> &texts,
                 const std::string &padding, int max_length,
                 const bool parallel) const
    {
        if (max_length == -1)
        {
            max_length = this->model_max_length;
        }
        if (parallel)
        {
#ifdef _OPENMP
            std::vector<std::vector<int>> input_ids(texts.size());

#pragma omp parallel for
            for (int i = 0; i < texts.size(); ++i)
            {
                input_ids[i] = this->tokenizer_ids(texts[i], max_length, padding);
            }
            return input_ids;
#else
            std::vector<std::vector<int>> input_ids;
            input_ids.reserve(texts.size());
            for (auto &text : texts)
            {
                input_ids.push_back(this->encode(text, padding, max_length));
            }

            return input_ids;

#endif
        }
        else
        {
            std::vector<std::vector<int>> input_ids;
            input_ids.reserve(texts.size());
            for (auto &text : texts)
            {
                input_ids.push_back(this->encode(text, padding, max_length));
            }

            return input_ids;
        }
    }
};

class FlashBertTokenizerBidirectional : public FlashBertTokenizer
{
protected:
    WordpieceBackwardTokenizer wordpiece_backward;

public:
    explicit FlashBertTokenizerBidirectional(
        const std::string &vocab_file, const bool do_lower_case = true,
        const int model_max_length = -1, const bool tokenize_chinese_chars = true)
        : FlashBertTokenizer(vocab_file, do_lower_case, model_max_length,
                             tokenize_chinese_chars),
          wordpiece_backward(vocab, this->UNK)
    {
    }

    [[nodiscard]] std::vector<int> tokenizer_ids(const std::string &text, const int max_length,
                                                 const std::string &padding) const override
    {
        // Handle the -1 case by using model_max_length
        int effective_max_length = max_length;
        if (max_length == -1)
        {
            effective_max_length = this->model_max_length;
            // If model_max_length is also -1, use unlimited (but practical limit)
            if (effective_max_length == -1)
            {
                effective_max_length = std::numeric_limits<int>::max();
            }
        }

        const size_t allowed_length = static_cast<size_t>(effective_max_length - 1);
        std::vector<int> input_ids;
        input_ids.reserve(1024);

        std::vector<int> i0, i1;
        std::vector<int> filtered_i0, filtered_i1;

        i0.reserve(effective_max_length);
        i1.reserve(effective_max_length);
        filtered_i0.reserve(effective_max_length);
        filtered_i1.reserve(effective_max_length);

        input_ids.emplace_back(this->CLS_NUM);
        auto basic_tokens = basic.tokenize(text);
        for (const auto &token : basic_tokens)
        {
            i0.clear();
            i1.clear();
            filtered_i0.clear();
            filtered_i1.clear();
            wordpiece.tokenizer_ids(token, effective_max_length - 1, i0);
            wordpiece_backward.tokenizer_ids(token, effective_max_length - 1, i1);
            if (i0 == i1)
            {
                std::move(i0.begin(), i0.end(), std::back_inserter(input_ids));
            }
            else
            {
                std::copy_if(i0.begin(), i0.end(), std::back_inserter(filtered_i0),
                             [](const int i)
                             { return i > 4; });
                std::copy_if(i1.begin(), i1.end(), std::back_inserter(filtered_i1),
                             [](const int i)
                             { return i > 4; });
                std::vector<int> &target =
                    compare_ids(filtered_i0, filtered_i1) ? i0 : i1;
                std::move(target.begin(), target.end(), std::back_inserter(input_ids));
            }
            if (input_ids.size() > allowed_length)
            {
                input_ids.resize(allowed_length);
                break;
            }
        }
        input_ids.emplace_back(this->SEP_NUM);
        if (padding == "max_length" && effective_max_length != std::numeric_limits<int>::max())
        {
            input_ids.resize(effective_max_length, 0);
        }
        return input_ids;
    }

    [[nodiscard]] std::vector<int> encode(const std::string &text,
                                          const std::string &padding,
                                          int max_length) const override
    {
        if (max_length == -1)
        {
            max_length = this->model_max_length;
        }
        return this->tokenizer_ids(text, max_length, padding);
    }

    [[nodiscard]] std::vector<std::vector<int>>
    batch_encode(const std::vector<std::string> &texts,
                 const std::string &padding, int max_length,
                 const bool parallel) const override
    {
        if (max_length == -1)
        {
            max_length = this->model_max_length;
        }
        if (parallel)
        {
#ifdef _OPENMP
            std::vector<std::vector<int>> input_ids(texts.size());

#pragma omp parallel for
            for (int i = 0; i < texts.size(); ++i)
            {
                input_ids[i] = this->tokenizer_ids(texts[i], max_length, padding);
            }

            return input_ids;
#else
            std::vector<std::vector<int>> input_ids;
            input_ids.reserve(texts.size());
            for (auto &text : texts)
            {
                input_ids.push_back(this->encode(text, padding, max_length));
            }

            return input_ids;
#endif
        }
        else
        {
            std::vector<std::vector<int>> input_ids;
            input_ids.reserve(texts.size());
            for (auto &text : texts)
            {
                input_ids.push_back(this->encode(text, padding, max_length));
            }

            return input_ids;
        }
    }
};

#endif
