#include "bert_tokenizer.h"
#include "debugging.h"
#include "env.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <sstream>
#include <string>
#include <vector>
#include<Python.h>
using namespace std;
#ifdef _MSC_VER
static const std::string base_dir = "../";
#else
static const std::string base_dir = "";
#endif
class Data {
public:
    struct Config {
        const std::string bert_base_cased = "bert-base-cased";
        const std::string bert_base_uncased = "bert-base-uncased";
        const std::string bert_base_multilingual_cased = "bert-base-multilingual-cased";
        const std::string bert_base_multilingual_uncased = "bert-base-multilingual-uncased";
        const std::string bert_base_chinese = "bert-base-chinese";
        const std::string kcbert_base = "kcbert-base";
        const std::string KR_BERT = "KR-BERT";
        const std::string llmlingua_2_bert_base_multilingual_cased_meetingbank = "llmlingua-2-bert-base-multilingual-cased-meetingbank";
    } config;

    struct Dataset {
        const std::string texts_en = "texts_en.all.parquet";
        const std::string texts_ko = "texts_ko.all.parquet";
        const std::string texts_cn = "texts_cn.all.parquet";
        const std::string texts_multilingual = "texts_multilingual.all.parquet";
    } dataset;
};

class TestData {
public:
    std::string vocab_file;
    std::string tokenizer_config_file;
    bool do_lower_case{};
    int model_max_length{};
    std::vector<std::string> texts{};
    std::vector<std::vector<int> > gts{};
    FlashBertTokenizer *tokenizer = nullptr;

    ~TestData() {
        if (this->tokenizer != nullptr) {
            delete tokenizer;
        }
    }

    explicit TestData(const std::string &config, const std::string &dataset) {
        std::ostringstream dirname, data_path;

        dirname << base_dir << "../dataset/config/" << config << "/";
        data_path << base_dir << "../dataset/data/" << dataset;

        this->vocab_file = dirname.str() + "vocab.txt";
        this->tokenizer_config_file = dirname.str() + "tokenizer_config.json";

        pycall(data_path.str(), config);
        this->load_tokenizer_config(this->tokenizer_config_file);
        if (config == "KR-BERT") {
            tokenizer = new FlashBertTokenizerBidirectional(this->vocab_file,
                                                            this->do_lower_case);
        } else {
            tokenizer = new FlashBertTokenizer(this->vocab_file, this->do_lower_case);
        }
    }


    void pycall(const std::string &parquet_path, const std::string &config) {
        Py_Initialize();
        PyRun_SimpleString("import sys; sys.path.append('.')");
        PyObject *pName = PyUnicode_DecodeFSDefault("data_loader");
        PyObject *pModule = PyImport_Import(pName);
        Py_DECREF(pName);
        if (!pModule) {
            PyErr_Print();
            std::cerr << "Cannot load module." << std::endl;
            exit(1);
        }
        PyObject *pFunc = PyObject_GetAttrString(pModule, "load_parquet");
        if (!pFunc || !PyCallable_Check(pFunc)) {
            PyErr_Print();
            std::cerr << "Cannot find function." << std::endl;
            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
            Py_Finalize();
            exit(1);
        }
        PyObject *pArgs = PyTuple_Pack(2,
                                       PyUnicode_FromString(parquet_path.c_str()),
                                       PyUnicode_FromString(config.c_str()));

        PyObject *pReturn = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);

        if (!pReturn) {
            PyErr_Print();
            std::cerr << "Function call failed." << std::endl;
            Py_DECREF(pFunc);
            Py_DECREF(pModule);
            Py_Finalize();
            exit(1);
        }

        PyObject *pyTexts = PyTuple_GetItem(pReturn, 0); // texts list
        PyObject *pyGts = PyTuple_GetItem(pReturn, 1); // gts list

        Py_ssize_t num_texts = PyList_Size(pyTexts);
        for (Py_ssize_t i = 0; i < num_texts; ++i) {
            PyObject *pyItem = PyList_GetItem(pyTexts, i);
            const char *text = PyUnicode_AsUTF8(pyItem);
            this->texts.emplace_back(text);
        }

        Py_ssize_t num_gts = PyList_Size(pyGts);
        for (Py_ssize_t i = 0; i < num_gts; ++i) {
            PyObject *pyInnerList = PyList_GetItem(pyGts, i);
            Py_ssize_t inner_size = PyList_Size(pyInnerList);
            std::vector<int> inner_vec;
            for (Py_ssize_t j = 0; j < inner_size; ++j) {
                PyObject *pyInt = PyList_GetItem(pyInnerList, j);
                int val = static_cast<int>(PyLong_AsLong(pyInt));
                inner_vec.push_back(val);
            }
            this->gts.push_back(inner_vec);
        }
        Py_DECREF(pReturn);
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        Py_Finalize();
    }

    void load_tokenizer_config(const std::string &file_path) {
        std::ifstream file(file_path);
        const nlohmann::json data = nlohmann::json::parse(file);

        this->do_lower_case = data.value("do_lower_case", false);
        this->model_max_length = data.value("model_max_length", 512);
    }
};


void perf_test(bool parallel = false) {
    const auto CONFIG = Data().config.bert_base_chinese;
    const auto DATASET = Data().dataset.texts_multilingual;
    cout << "Load dataset..." << endl;
    cout << "\t" << CONFIG << endl;
    cout << "\t" << DATASET << endl;
    const TestData td(CONFIG, DATASET);
    cout << "Start performance test..." << endl;
    std::string rt = parallel ? "MultiThread" : "SingleThread";
    cout << "\t" << rt << endl;
    cout << "\t" << td.tokenizer->env.OS << endl;
    cout << "\t" << td.tokenizer->env.COMPILER_NAME << endl;
    cout << "\t" << td.tokenizer->env.PARALLEL_LIB << endl;
    cout << "\t" << VERSION_INFO_STR(VERSION_INFO) << endl;

    std::chrono::duration<double> diff{};
    size_t correct = 0;
    const auto t_beg = std::chrono::system_clock::now();
    vector<vector<int> > ids_list;
    if (parallel) {
        ids_list = td.tokenizer->batch_encode(td.texts, "longest", td.model_max_length, parallel);
    } else {
        ids_list.assign(td.texts.size(), std::vector<int>());
        for (size_t i = 0; i < td.texts.size(); i++) {
            auto ids = td.tokenizer->encode(td.texts[i], "longest", td.model_max_length);
            ids_list[i] = ids;
        }
    }
    const auto t_end = std::chrono::system_clock::now();
    diff = t_end - t_beg;
    const auto elapsed_time = diff.count();

    for (size_t i = 0; i < td.texts.size(); i++) {
        correct += ids_list[i] == td.gts[i];
    }

    std::cout << rt << " | " << elapsed_time << " seconds" << "  |  ";
    std::cout << td.texts.size() << "  |  ";
    const double accuracy = static_cast<double>(correct) * 100.0 /
                            static_cast<double>(td.texts.size());
    std::cout << accuracy << " % Accuracy" << std::endl;

    ofstream fout;
    fout.open(base_dir + "../log.csv", std::ios::app);
    fout << CONFIG << "," << DATASET << "," << rt << "," << td.tokenizer->env.OS << "," << td.tokenizer->env.COMPILER_NAME << "," << td.tokenizer->env.PARALLEL_LIB
            << "," << VERSION_INFO_STR(VERSION_INFO) << "," << elapsed_time << "," << td.texts.size() << "," << accuracy << std::endl;
    fout.close();
}


int main() {
    std::ios::sync_with_stdio(false);

    cout << cpp_env() << endl;
    perf_test(false);

    //BPE


    //cout << g_1 << "\t" << g_2 << "\t" << g_3 << "\t" << g_4 << endl;
    return 0;
}
