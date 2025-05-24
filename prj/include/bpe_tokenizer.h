// fAmalgamate
#ifndef HVYZM564TDUNM45YCK8AYQ497GQ2HFSHJV3XYXTE0MG07X0XH5V07ESAFLPZG6
#define HVYZM564TDUNM45YCK8AYQ497GQ2HFSHJV3XYXTE0MG07X0XH5V07ESAFLPZG6
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <robin_hood.h>
#include <algorithm>
#include "json.hpp"
#include <codecvt>
#include <locale>

using json = nlohmann::json;
using Pair = std::pair<std::string, std::string>;

struct pair_hash {
    std::size_t operator()(Pair const &p) const noexcept {
        return std::hash<std::string>()(p.first) ^ (std::hash<std::string>()(p.second) << 1);
    }
};

class FlashBPETokenizer {
public:
    FlashBPETokenizer(const std::string &vocab_path, const std::string &merges_path) {
        std::ifstream vf(vocab_path);
        json j;
        vf >> j;
        for (auto &[k, v]: j.items()) {
            vocab_[k] = v.get<int>();
        }
        bpe_ranks_ = loadMerges(merges_path);
    }

    std::vector<int> operator()(const std::string &text) const {
        std::vector<int> tokens;
        std::istringstream iss(text);
        std::string word;
        while (iss >> word) {
            for (auto &sub: encodeWord(word)) {
                auto it = vocab_.find(sub);
                if (it != vocab_.end()) {
                    tokens.push_back(it->second);
                }
            }
        }
        return tokens;
    }

private:
    robin_hood::unordered_flat_map<std::string, int> vocab_;
    robin_hood::unordered_flat_map<Pair, int, pair_hash> bpe_ranks_;

    static robin_hood::unordered_flat_map<Pair, int, pair_hash>
    loadMerges(const std::string &merges_path) {
        robin_hood::unordered_flat_map<Pair, int, pair_hash> ranks;
        std::ifstream mf(merges_path);
        std::string line;
        int rank = 0;
        while (std::getline(mf, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::istringstream ls(line);
            std::string a, b;
            if (ls >> a >> b) {
                ranks[{a, b}] = rank++;
            }
        }
        return ranks;
    }

    static robin_hood::unordered_flat_set<Pair, pair_hash>
    getPairs(const std::vector<std::string> &word) {
        robin_hood::unordered_flat_set<Pair, pair_hash> pairs;
        for (size_t i = 0; i + 1 < word.size(); ++i) {
            pairs.insert({word[i], word[i + 1]});
        }
        return pairs;
    }

    std::vector<std::string> encodeWord(const std::string &token) const {
        // 1) UTF-8 → UTF-32
        using converter = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>;
        static converter conv;
        std::u32string u32 = conv.from_bytes(token);

        // 2) 각 코드포인트를 UTF-8 문자열로 다시 변환
        std::vector<std::string> word;
        word.reserve(u32.size() + 1);
        for (char32_t cp: u32) {
            word.push_back(conv.to_bytes(cp));
        }
        word.emplace_back("</w>");

        // — 이하 기존 BPE 합병 로직 그대로 —
        while (true) {
            auto pairs = getPairs(word);
            Pair best{"", ""};
            int best_rank = std::numeric_limits<int>::max();
            for (auto &p: pairs) {
                auto it = bpe_ranks_.find(p);
                if (it != bpe_ranks_.end() && it->second < best_rank) {
                    best = p;
                    best_rank = it->second;
                }
            }
            if (best_rank == std::numeric_limits<int>::max()) break;

            std::vector<std::string> next;
            size_t i = 0;
            while (i < word.size()) {
                auto it = std::find(word.begin() + i, word.end(), best.first);
                if (it == word.end()) {
                    next.insert(next.end(), word.begin() + i, word.end());
                    break;
                }
                size_t j = std::distance(word.begin(), it);
                next.insert(next.end(), word.begin() + i, word.begin() + j);
                if (j + 1 < word.size() && word[j + 1] == best.second) {
                    next.push_back(best.first + best.second);
                    i = j + 2;
                } else {
                    next.push_back(word[j]);
                    i = j + 1;
                }
            }
            word.swap(next);
        }

        // "</w>" 처리
        if (!word.empty()) {
            auto &back = word.back();
            if (back == "</w>") {
                word.pop_back();
            } else if (back.size() > 4 && back.substr(back.size() - 4) == "</w>") {
                back.erase(back.size() - 4);
            }
        }
        return word;
    }
};

#endif
