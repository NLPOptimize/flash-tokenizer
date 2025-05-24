//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                               License Agreement
//                         Aho-Corasick Trie(LinMaxMatching)
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

#pragma once
#ifndef REMOVEEXISTINGLEGACYCODEANDUSEONLYAHOCORASICKTRIE
#define REMOVEEXISTINGLEGACYCODEANDUSEONLYAHOCORASICKTRIE


#include <vector>
#include <array>
#include <string>
#include <queue>

class ACTrie;

class TrieNode {
    std::array<int, 256> children;
    std::array<bool, 256> explicitChild;
    int vocab_index;
    int word_len;
    int fail;

public:
    TrieNode() : children(), explicitChild(), vocab_index(-1), word_len(0), fail(0) {
        children.fill(-1);
        explicitChild.fill(false);
    }

    friend class ACTrie;
};

class ACTrie {
    std::vector<TrieNode> pool;
    std::vector<int> dfa; // deterministic next state: state*256 + c
    std::vector<bool> isExplicit; // whether the transition was explicit

public:
    ACTrie() {
        pool.emplace_back();
    }

    void insert_bytes(const std::vector<unsigned char> &seq, int index) {
        int node = 0;
        for (unsigned char b: seq) {
            if (pool[node].children[b] == -1) {
                pool[node].children[b] = (int) pool.size();
                pool[node].explicitChild[b] = true;
                pool.emplace_back();
            }
            node = pool[node].children[b];
        }
        pool[node].vocab_index = index;
        pool[node].word_len = (int) seq.size();
    }

    // Insert a word, storing its vocab index and length
    void insert(const std::string &word, int index) {
        int node = 0;
        for (unsigned char c: word) {
            if (pool[node].children[c] == -1) {
                pool[node].children[c] = pool.size();
                pool[node].explicitChild[c] = true;
                pool.emplace_back();
            }
            node = pool[node].children[c];
        }
        pool[node].vocab_index = index;
        pool[node].word_len = word.size();
    }

    // Build failure links and DFA table
    void build() {
        std::queue<int> q;
        // Initialize root
        for (int c = 0; c < 256; ++c) {
            int child = pool[0].children[c];
            if (child != -1) {
                pool[child].fail = 0;
                q.push(child);
            } else {
                pool[0].children[c] = 0;
            }
        }
        // BFS for failure links
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            int f = pool[cur].fail;
            for (int c = 0; c < 256; ++c) {
                int nxt = pool[cur].children[c];
                if (nxt != -1 && pool[cur].explicitChild[c]) {
                    pool[nxt].fail = pool[f].children[c];
                    q.push(nxt);
                } else {
                    pool[cur].children[c] = pool[f].children[c];
                }
            }
        }
        // Build DFA and explicit flags
        int states = pool.size();
        dfa.resize(states * 256);
        isExplicit.resize(states * 256);
        for (int s = 0; s < states; ++s) {
            for (int c = 0; c < 256; ++c) {
                int idx = s * 256 + c;
                dfa[idx] = pool[s].children[c];
                isExplicit[idx] = pool[s].explicitChild[c];
            }
        }
    }

    [[nodiscard]] std::pair<int, int> search(const std::string &token, const int start) const {
        int current = 0;
        int best_length = 0;
        int best_index = -1;
        const int size = static_cast<int>(token.size());
        for (int pos = start; pos < size; ++pos) {
            const unsigned char c = token[pos];
            const int index = current * 256 + c;
            const int next = dfa[index];
            if (!isExplicit[index])
                break;
            current = next;
            if (pool[current].vocab_index != -1) {
                best_length = pos - start + 1;
                best_index = pool[current].vocab_index;
            }
        }
        return {best_length, best_index};
    }

    // Advance state by character
    int step(int state, unsigned char c) const {
        return dfa[state * 256 + c];
    }

    // Get failure link of a state
    int fail_link(int state) const {
        return pool[state].fail;
    }

    // Get vocab index stored at this node (or -1)
};

using Trie = ACTrie;
#endif
