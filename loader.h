//
// Created by spring on 2025. 5. 11..
//

#ifndef LOADER_H
#define LOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

static std::vector<std::string> load_text(const std::string &filename) {
    std::vector<std::string> texts;
    std::ifstream fin(filename);
    if (!fin) {
        throw std::runtime_error("Could not open file " + filename);
    }
    std::string line;
    while (std::getline(fin, line)) {
        texts.push_back(line);
    }
    return texts;
}

static std::vector<std::vector<int> > load_gt(const std::string &filename) {
    std::vector<std::vector<int> > gts;
    std::ifstream fin(filename);
    if (!fin) {
        throw std::runtime_error("Could not open file " + filename);
    }
    std::string line;
    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        int x;
        while (iss >> x) row.push_back(x);
        gts.push_back(row);
    }
    return gts;
}


#endif //LOADER_H
