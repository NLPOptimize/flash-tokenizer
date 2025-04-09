<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="https://github.com/NLPOptimize/flash-tokenizer/blob/main/assets/FlashTokenizer_main_dark.png?raw=true">
    <img alt="FlashTokenizer" src="https://github.com/NLPOptimize/flash-tokenizer/blob/main/assets/FlashTokenizer_main_light.png?raw=true" width=60%>
  </picture>
</p>
<h1 align="center">
The world's fastest CPU tokenizer library!
</h1>

### 🇨🇳 [简体中文](./README.zh-CN.md) 🇰🇷[한국어](./README.ko-KR.md) 🇯🇵[日本語](./README.ja-JP.md)


## EFFICIENT AND OPTIMIZED TOKENIZER ENGINE FOR LLM INFERENCE SERVING

[FlashTokenizer](https://pypi.org/project/flash-tokenizer/) is a **high-performance tokenizer implementation in C++ of the BertTokenizer used for LLM inference**. It has the highest speed and accuracy of any tokenizer, such as [FlashAttention](https://github.com/Dao-AILab/flash-attention) and [FlashInfer](https://github.com/flashinfer-ai/flashinfer), and is **10 times faster** than `BertTokenizerFast` in transformers.


## Performance Benchmark Demo Video
[![Video Label](http://img.youtube.com/vi/a_sTiAXeSE0/0.jpg)](https://youtu.be/a_sTiAXeSE0)





> [!NOTE]  
>
> ### Why?
> - We need a tokenizer that is faster, more accurate, and easier to use than [Huggingface's BertTokenizerFast](https://github.com/huggingface/transformers/blob/main/src/transformers/models/bert/tokenization_bert_fast.py). ([link1](https://stackoverflow.com/questions/75595699/huggingfaces-berttokenizerfast-is-between-39000-and-258300-times-slower-than-ex), [link2](https://github.com/PaddlePaddle/PaddleNLP/issues/8565), [link3](https://blog.csdn.net/xhw205/article/details/129578988))
> - [PaddleNLP's BertTokenizerFast](https://paddlenlp.readthedocs.io/en/stable/_modules/paddlenlp/experimental/faster_tokenizer.html) achieves a 1.2x performance improvement by implementing [Huggingface's Rust version](https://github.com/huggingface/tokenizers) in `C++`.  However, using it requires installing both the massive [PaddlePaddle](https://github.com/PaddlePaddle/Paddle) and [PaddleNLP](https://github.com/PaddlePaddle/PaddleNLP) packages.
> - [Tensorflow-text's FastBertTokenizer](https://www.tensorflow.org/text/api_docs/python/text/FastBertTokenizer) actually demonstrates slower performance in comparison.
> - [Microsoft's Blingfire](https://github.com/microsoft/BlingFire) **takes over 8 hours** to train on custom data and shows relatively lower accuracy.
> - [Rapid's cuDF](https://github.com/rapidsai/cudf) provides a GPU-based BertTokenizer, but it suffers from accuracy issues.
> - Unfortunately, [FastBertTokenizer](https://github.com/georg-jung/FastBertTokenizer) and [BertTokenizers](https://github.com/NMZivkovic/BertTokenizers) developed in `C#` and cannot be used in `Python`.
>
> - This is why we developed `FlashTokenizer`. It can be easily installed via `pip` and is **developed in C++ for straightforward maintenance**. Plus, it guarantees extremely fast speeds. We've created an implementation that's faster than Blingfire and easier to use. FlashTokenizer is implemented using the **LinMax Tokenizer** proposed in [Fast WordPiece Tokenization](https://arxiv.org/abs/2012.15524), enabling tokenization in linear time. Finally It supports **parallel processing at the C++ level for batch encoding**, delivering outstanding speed.
>




<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="https://github.com/NLPOptimize/flash-tokenizer/blob/main/assets/Banner_dark.png?raw=true">
    <img alt="Banner" src="https://github.com/NLPOptimize/flash-tokenizer/blob/main/assets/Banner_light.png?raw=true" width=100%>
  </picture>
</p>


<p>
<img align="left" src="https://img.shields.io/badge/success-0B86F1?style=flat&logo=python&logoColor=white&label=MacOS_build">
<img align="left" src="https://img.shields.io/badge/success-0B86F1?style=flat&logo=python&logoColor=white&label=Windows_build">
<img align="left" src="https://img.shields.io/badge/success-0B86F1?style=flat&logo=python&logoColor=white&label=Linux_build">
</p><br>

* * *

### FlashTokenizer includes the following core features

> [!TIP]
> 
>  * Implemented in C++17.
>     * **MacOS**: `clang++`.
>     * **Windows**: `Visual Studio 2022`.
>     * **Ubuntu**: `g++`. 
>
> * Equally fast in Python via pybind11.
> * Support for parallel processing at the C++ level using OPENMP.
>     



## News

> [!IMPORTANT]  
> **[Apr 02 2025]**
> - Add performance benchmarking code
> - Performance benchmarking is conducted using Python, and required packages can be installed via [setup.sh](./perftest/setup.sh).  
> - A minor performance improvement has been achieved by adding the `tokenize_early_stop` feature to `BasicTokenizer`.  
> - [OpenMP](https://www.openmp.org/) demonstrated better performance than `std::thread` across Windows, Linux, and macOS, so we've switched exclusively to OpenMP.
> 
> **[Mar 31 2025]**
> - Modified to provide pre-built whl files for each OS.
>
> **[Mar 22 2025]**
>
> - Added [DFA](https://blog.cloudflare.com/pt-br/making-waf-ai-models-go-brr/#:~:text=We%20can%20also%20tune%20Aho,settings%20based%20on%20this%20recommendation) to AC Trie.
>
> **[Mar 21 2025]**
> - Improving Tokenizer Accuracy
>
> **[Mar 19 2025]** 
> - Memory reduction and slight performance improvement by applying LinMaxMatching from [Aho–Corasick](https://en.wikipedia.org/wiki/Aho%E2%80%93Corasick_algorithm) algorithm.
> - Improved branch pipelining of all functions and force-inline applied.
> - Removed unnecessary operations of `WordpieceTokenizer(Backward)`.
> - Optimizing all functions to operate except for [Bloom filter](https://en.wikipedia.org/wiki/Bloom_filter) is faster than caching.
> - `punctuation`,` control`, and `whitespace` are defined as constexprs in advance and used as Bloom filters.
> - Reduce unnecessary memory allocation with statistical memory profiling.
> -  In ✨FlashTokenizer✨, `bert-base-uncased` can process **35K** texts per second on a single core, with an approximate processing time of **28ns** per text.
>
> **[Mar 18 2025]** 
> - Improvements to the accuracy of the BasicTokenizer have improved the overall accuracy and, in particular, produce more accurate results for Unicode input.
>
> **[Mar 14 2025]** 
> - The performance of the `WordPieceTokenizer` and `WordPieceBackwordTokenizer` has been improved using [Trie](https://en.wikipedia.org/wiki/Trie), which was introduced in [Fast WordPiece Tokenization](https://arxiv.org/abs/2012.15524).
> - Using `FastPoolAllocator` in `std::list` improves performance in SingleEncoding, but it is not thread-safe, so `std::list<std::string>` is used as is in BatchEncoding. In BatchEncoding, `OPENMP` is completely removed and only `std::thread` is used.
>
> **[Mar 10 2025]** 
> - Performance improvements through faster token mapping with robin_hood and memory copy minimization with **std::list**.
>
>
> #### Token Ids Map Table Performance Test.
>
> Token and Ids Map used the fastest `robin_hood::unordered_flat_map<std::string, int>`.
>
> **[Mar 09 2025]** Completed development of flash-tokenizer for BertTokenizer.



## 1. Installation

### Requirements
 * `Windows(AMD64)`, `MacOS(ARM64)`, `Ubuntu(x86-64)` .
 * `g++` / `clang++` / `MSVC`.
 * `python 3.8 ~ 3.13`.

### Install from [PIP](https://pypi.org/project/flash-tokenizer/)



On Windows, you need to install [vc_redist.x64.exe](https://github.com/NLPOptimize/flash-tokenizer/releases/download/Packages/VC_redist.x64.exe).
```bash
# Windows
pip install -U flash-tokenizer
```
```bash
# Linux
pip install -U flash-tokenizer
```
```bash
# MacOS
pip install -U flash-tokenizer
```

### Install from Source
```bash
git clone https://github.com/NLPOptimize/flash-tokenizer
cd flash-tokenizer/prj
pip install .
```


## 2. Sample

```python
from flash_tokenizer import BertTokenizerFlash
from transformers import BertTokenizer

titles = [
    '绝不能放弃，世界上没有失败，只有放弃。',
    'is there any doubt about it "None whatsoever"',
    "세상 어떤 짐승이 이를 드러내고 사냥을 해? 약한 짐승이나 몸을 부풀리지, 진짜 짐승은 누구보다 침착하지.",
    'そのように二番目に死を偽装して生き残るようになったイタドリがどうして初めて見る自分をこんなに気遣ってくれるのかと尋ねると「私が大切にする人たちがあなたを大切にするから」と答えては'
]

tokenizer1 = BertTokenizerFlash.from_pretrained('bert-base-multilingual-cased')
tokenizer2 = BertTokenizer.from_pretrained('bert-base-multilingual-cased')

correct = 0
for title in titles:
    print(title)
    tokens1 = tokenizer1.tokenize(title)
    tokens2 = tokenizer2.tokenize(title)
    ids1 = tokenizer1(title, max_length=512, padding="longest").input_ids[0]
    ids2 = tokenizer2(title, max_length=512, padding="longest", return_tensors="np").input_ids[0].tolist()
    if tokens1 == tokens2 and ids1 == ids2:
        correct += 1
        print("Accept!")
    else:
        print("Wrong Answer")
    print(ids1)
    print(ids2)
    print()

print(f'Accuracy: {correct * 100.0 / len(titles):.2f}%')
```

```
绝不能放弃，世界上没有失败，只有放弃。
Accept!
[101, 6346, 2080, 6546, 4284, 3704, 10064, 2087, 5621, 2078, 4917, 4461, 3204, 7480, 10064, 2751, 4461, 4284, 3704, 1882, 102]
[101, 6346, 2080, 6546, 4284, 3704, 10064, 2087, 5621, 2078, 4917, 4461, 3204, 7480, 10064, 2751, 4461, 4284, 3704, 1882, 102]

is there any doubt about it "None whatsoever"
Accept!
[101, 10124, 11155, 11178, 86697, 10978, 10271, 107, 86481, 12976, 11669, 23433, 107, 102]
[101, 10124, 11155, 11178, 86697, 10978, 10271, 107, 86481, 12976, 11669, 23433, 107, 102]

세상 어떤 짐승이 이를 드러내고 사냥을 해? 약한 짐승이나 몸을 부풀리지, 진짜 짐승은 누구보다 침착하지.
Accept!
[101, 9435, 14871, 55910, 9710, 48210, 10739, 35756, 9113, 30873, 31605, 11664, 9405, 118729, 10622, 9960, 136, 9539, 11102, 9710, 48210, 43739, 9288, 10622, 9365, 119407, 12692, 12508, 117, 9708, 119235, 9710, 48210, 10892, 9032, 17196, 80001, 9783, 119248, 23665, 119, 102]
[101, 9435, 14871, 55910, 9710, 48210, 10739, 35756, 9113, 30873, 31605, 11664, 9405, 118729, 10622, 9960, 136, 9539, 11102, 9710, 48210, 43739, 9288, 10622, 9365, 119407, 12692, 12508, 117, 9708, 119235, 9710, 48210, 10892, 9032, 17196, 80001, 9783, 119248, 23665, 119, 102]

そのように二番目に死を偽装して生き残るようになったイタドリがどうして初めて見る自分をこんなに気遣ってくれるのかと尋ねると「私が大切にする人たちがあなたを大切にするから」と答えては
Accept!
[101, 11332, 24273, 2150, 5632, 5755, 1943, 4805, 1980, 2371, 7104, 11592, 5600, 1913, 4814, 1975, 27969, 15970, 21462, 15713, 21612, 10898, 56910, 22526, 22267, 2547, 19945, 7143, 1975, 6621, 2534, 1980, 28442, 60907, 11312, 4854, 7770, 14813, 18825, 58174, 75191, 11662, 3456, 1945, 100812, 1890, 5949, 1912, 3197, 2535, 84543, 2179, 78776, 111787, 22946, 20058, 11377, 3197, 2535, 84543, 16867, 1891, 1940, 6076, 27144, 11588, 102]
[101, 11332, 24273, 2150, 5632, 5755, 1943, 4805, 1980, 2371, 7104, 11592, 5600, 1913, 4814, 1975, 27969, 15970, 21462, 15713, 21612, 10898, 56910, 22526, 22267, 2547, 19945, 7143, 1975, 6621, 2534, 1980, 28442, 60907, 11312, 4854, 7770, 14813, 18825, 58174, 75191, 11662, 3456, 1945, 100812, 1890, 5949, 1912, 3197, 2535, 84543, 2179, 78776, 111787, 22946, 20058, 11377, 3197, 2535, 84543, 16867, 1891, 1940, 6076, 27144, 11588, 102]

Accuracy: 100.00%
```

## 3. Other Implementations


<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="./assets/logos_dark.png">
    <img alt="Banner" src="./assets/logos_light.png" width=100%>
  </picture>
</p>


Most [BERT](https://arxiv.org/abs/1810.04805)-based models use the [WordPiece Tokenizer](https://static.googleusercontent.com/media/research.google.com/ja//pubs/archive/37842.pdf), whose code can be found [here](https://github.com/google-research/bert/blob/master/tokenization.py).
(A simple implementation of Huggingface can be found [here](https://github.com/huggingface/transformers/blob/main/src/transformers/models/bert/tokenization_bert.py)).

Since the BertTokenizer is a CPU intensive algorithm, inference can be a bottleneck, and unoptimized tokenizers can be severely slow. A good example is the [BidirectionalWordpieceTokenizer](https://github.com/snunlp/KR-BERT/blob/master/krbert_tensorflow/tokenization_ranked.py) introduced in [KR-BERT](https://arxiv.org/abs/2008.03979). Most of the code is the same, but the algorithm traverses the sub token backwards and writes a larger value compared to the forward traversal. The paper claims accuracy improvements, but it's hard to find other quantitative metrics, and the accuracy improvements aren't significant, and the tokenizer is seriously slowed down.

* transformers (Rust Impl, PyO3)
* paddlenlp (C++ Impl, pybind)
* tensorflow-text (C++ Impl, pybind)
* blingfire (C++ Impl, Native binary call)

Most developers will either use `transformers.BertTokenizer` or `transformers.AutoTokenizer`, but using `AutoTokenizer` will return `transformers.BertTokenizerFast`.

Naturally, it's faster than BertTokenizer, but the results aren't exactly the same, which means you're already giving up 100% accuracy starting with the tokenizer.

BertTokenizer is not only provided by transformers. [PaddleNLP](https://github.com/PaddlePaddle/PaddleNLP) and [tensorflow-text](https://www.tensorflow.org/text) also provide BertTokenizer.

Then there's [Blingfire](https://github.com/microsoft/BlingFire), which is developed by Microsoft and is being abandoned.

PaddleNLP requires PaddlePaddle and provides tokenizer functionality starting with version 3.0rc. You can install it as follows

```bash
##### Install PaddlePaddle, PaddleNLP
python -m pip install paddlepaddle==3.0.0b1 -i https://www.paddlepaddle.org.cn/packages/stable/cpu/
pip install --upgrade paddlenlp==3.0.0b3
##### Install transformers
pip install transformers==4.47.1
##### Install tf-text
pip install tensorflow-text==2.18.1
##### Install blingfire
pip install blingfire
```


With the exception of blingfire, vocab.txt is all you need to run the tokenizer right away. 
(blingfire also requires only vocab.txt and can be used after 8 hours of learning).

The implementations we'll look at in detail are `PaddleNLP's BertTokenizerFast` and `blingfire`.

* `blingfire`: Uses a [Deterministic Finite State Machine (DFSM)](https://github.com/microsoft/BlingFire/blob/master/doc/Bling_Fire_Tokenizer_Algorithms.pdf) to eliminate one linear scan and unnecessary comparisons, resulting in a time of O(n), which is impressive.
  * **Advantages**: **5-10x faster than other implementations**.
  * **Disadvantages**: Long training time (8 hours) and lower accuracy than other implementations. (+Difficult to get help due to de facto development hiatus).
* `PaddleNLP`: As shown in the experiments below, PaddleNLP is always faster than BertTokenizerFast (HF) to the same number of decimal places, and is always faster on any OS, whether X86 or Arm.
  * **Advantages**:  **Internal implementation is in C++** Compared to `transformers.BertTokenizerFast` implemented in Rust, it is 1.2x faster while outputting exactly the same values.
    * You can't specify `pt(pytorch tensor)` in `return_tensors`, but this is not a problem.
  * **Disadvantages**: none, other than the need to install PaddlePaddle and PaddleNLP.

## 4. Performance test


### 4.1 Performance test (Single text encoding)

Accuracy is the result of measuring [google's BertTokenizerFast](https://github.com/google-research/bert/blob/master/tokenization.py) as a baseline. If even one of the `input_ids` is incorrect, the answer is considered incorrect.


<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="./assets/comp_speed_dark.png">
    <img alt="FlashTokenizer" src="./assets/comp_speed_light.png" width=100%>
  </picture>
</p>

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="./assets/comp_accuracy_dark.png">
    <img alt="FlashTokenizer" src="./assets/comp_accuracy_light.png" width=100%>
  </picture>
</p>


### Tokenizer Performance Comparison 

#### [google-bert/bert-base-cased](https://huggingface.co/google-bert/bert-base-cased)

| Tokenizer                      | Elapsed Time | texts     | Accuracy |
|--------------------------------|----------------:|-----------:|------------:|
| BertTokenizerFast(Huggingface) | 84.3700s     | 1,000,000 | 99.9226% |
| BertTokenizerFast(PaddleNLP)   | 75.6551s     | 1,000,000 | 99.9226% |
| FastBertTokenizer(Tensorflow)  | 219.1259s    | 1,000,000 | 99.9160% |
| Blingfire                      | 13.6183s     | 1,000,000 | 99.8991% |
| **FlashBertTokenizer**             | 8.1968s      | 1,000,000 | 99.8216% |

#### [google-bert/bert-base-uncased](https://huggingface.co/google-bert/bert-base-uncased)

| Tokenizer                      |   Elapsed Time |     texts |   Accuracy |
|--------------------------------|----------------:|-----------:|------------:|
| BertTokenizerFast(Huggingface) |       91.7882s | 1,000,000 |   99.9326% |
| BertTokenizerFast(PaddleNLP)   |       83.6839s | 1,000,000 |   99.9326% |
| FastBertTokenizer(Tensorflow)  |      204.2240s | 1,000,000 |   99.1379% |
| Blingfire                      |       13.2374s | 1,000,000 |   99.8588% |
| **FlashBertTokenizer**             |        7.6313s | 1,000,000 |   99.6884% |

#### [google-bert/bert-base-multilingual-cased](https://huggingface.co/google-bert/bert-base-multilingual-cased)



| Tokenizer                      | Elapsed Time | texts     | Accuracy |
|--------------------------------|----------------:|-----------:|------------:|
| BertTokenizerFast(Huggingface) | 212.1570s    | 2,000,000 | 99.7964% |
| BertTokenizerFast(PaddleNLP)   | 193.9921s    | 2,000,000 | 99.7964% |
| FastBertTokenizer(Tensorflow)  | 394.1574s    | 2,000,000 | 99.7892% |
| Blingfire                      | 38.9013s     | 2,000,000 | 99.9780% |
| **FlashBertTokenizer**             | 20.4570s     | 2,000,000 | 99.8970% |


#### [beomi/kcbert-base](https://github.com/Beomi/KcBERT)

| Tokenizer                      |   Elapsed Time |     texts |   Accuracy |
|--------------------------------|----------------:|-----------:|------------:|
| BertTokenizerFast(Huggingface) |       52.5744s | 1,000,000 |   99.6754% |
| BertTokenizerFast(PaddleNLP)   |       44.8943s | 1,000,000 |   99.6754% |
| FastBertTokenizer(Tensorflow)  |      198.0270s | 1,000,000 |   99.6639% |
| Blingfire                      |       13.0701s | 1,000,000 |   99.9434% |
| **FlashBertTokenizer**             |        5.2601s | 1,000,000 |   99.9484% |


| Tokenizer                      |   Elapsed Time |     texts |   Accuracy |
|--------------------------------|----------------|-----------|------------|
| **FlashBertTokenizer**             |        5.1875s | 1,000,001 |   99.9484% |
| Blingfire                      |       13.2783s | 1,000,001 |   99.9435% |
| rust_tokenizers(guillaume-be)  |       16.6308s | 1,000,001 |   99.9829% |
| BertTokenizerFast(PaddleNLP)   |       44.5476s | 1,000,001 |   99.6754% |
| BertTokenizerFast(Huggingface) |       53.2525s | 1,000,001 |   99.6754% |
| FastBertTokenizer(Tensorflow)  |      202.1633s | 1,000,001 |   99.6639% |

#### [microsoft/llmlingua-2-bert-base-multilingual-cased-meetingbank](https://huggingface.co/microsoft/llmlingua-2-bert-base-multilingual-cased-meetingbank)

| Tokenizer                      |   Elapsed Time |     texts |   Accuracy |
|--------------------------------|----------------:|-----------:|------------:|
| BertTokenizerFast(Huggingface) |      208.8858s | 2,000,000 |   99.7964% |
| BertTokenizerFast(PaddleNLP)   |      192.6593s | 2,000,000 |   99.7964% |
| FastBertTokenizer(Tensorflow)  |      413.2010s | 2,000,000 |   99.7892% |
| Blingfire                      |       39.3765s | 2,000,000 |   99.9780% |
| **FlashBertTokenizer**             |       22.8820s | 2,000,000 |   99.8970% |

| Tokenizer                      |   Elapsed Time |     texts |   Accuracy |
|--------------------------------|----------------|-----------|------------|
| **FlashBertTokenizer**             |       22.0901s | 2,000,001 |   99.8971% |
| Blingfire                      |       37.9836s | 2,000,001 |   99.9780% |
| rust_tokenizers(guillaume-be)  |       98.0366s | 2,000,001 |   99.9976% |
| BertTokenizerFast(PaddleNLP)   |      208.6889s | 2,000,001 |   99.7964% |
| BertTokenizerFast(Huggingface) |      219.2644s | 2,000,001 |   99.7964% |
| FastBertTokenizer(Tensorflow)  |      413.9725s | 2,000,001 |   99.7892% |

#### [KR-BERT](https://github.com/snunlp/KR-BERT)


| Tokenizer                                    |   Elapsed Time |     texts |   Accuracy |
|--------------------------------|----------------:|-----------:|------------:|
| BertTokenizerBidirectional(KR-BERT Original) |      128.3320s | 1,000,000 |  100.0000% |
| **FlashBertTokenizer(Bidirectional)**                           |       10.4492s | 1,000,000 |   99.9631% |



```mermaid
%%{ init: { "er" : { "layoutDirection" : "LR" } } }%%
erDiagram
    Text ||--o{ Preprocess : tokenize
    Preprocess o{--|| Inference : memcpy_h2d
    Inference o{--|| Postprocess : memcpy_d2h
```





## 6. Compatibility

FlashBertTokenizer can be used with any framework.  CUDA version compatibility for each framework is also important for fast inference of LLMs.

 * [PyTorch](https://pytorch.org/) no longer supports installation using conda.
 * [ONNXRUNTIME](https://onnxruntime.ai/docs/execution-providers/CUDA-ExecutionProvider.html#cuda-12x) is separated by CUDA version.
 * PyTorch is also looking to ditch CUDA 12.x in favor of the newer CUDA 12.8. However, the trend is to keep CUDA 11.8 in all frameworks.
   * CUDA 12.x was made for the newest GPUs, Hopper and Blackwell, and on GPUs like Volta, CUDA 11.8 is faster than CUDA 12.x.



| DL Framework | Version | OS   | CPU  | CUDA 11.8 | CUDA 12.3 | CUDA 12.4 | CUDA 12.6 | CUDA 12.8 |
| ------------ | ----|---- | ---- | --------- | ----|----- | --------- | --------- |
| PyTorch | 2.6| Linux, Windows | ⚪|⚪|❌|⚪| ⚪ |    ❌      |
| PyTorch | 2.7|Linux, Windows|⚪|⚪|❌|❌|⚪|⚪|
| ONNXRUNTIME(11) | 1.20.x| Linux, Windows|⚪|⚪|❌|❌|❌|❌|
| ONNXRUNTIME(12) | 1.20.x| Linux, Windows|⚪|❌|⚪|⚪|⚪|⚪|
| PaddlePaddle | 3.0-beta | Linux, Windows|⚪|⚪|❌|❌|❌|❌|


## 7. GPU Tokenizer

Here is an example of installing and running cuDF in [Run State of the Art NLP Workloads at Scale with RAPIDS, HuggingFace, and Dask](https://developer.nvidia.com/blog/run-state-of-the-art-nlp-workloads-at-scale-with-rapids-huggingface-and-dask/#:~:text=,and%20then%20used%20in%20subsequent).
*(It's incredibly fast)*

You can run WordPiece Tokenizer on GPUs on [rapids(cudf)](https://docs.rapids.ai/).
 * [Implemention](https://github.com/rapidsai/cudf/blob/0e99ec3ec15b8b0ebe68bd884c7d22d600e9259e/python/cudf/cudf/core/wordpiece_tokenize.py#L10)
 * [Example](https://github.com/rapidsai/cudf/blob/0e99ec3ec15b8b0ebe68bd884c7d22d600e9259e/python/cudf/cudf/tests/text/test_subword_tokenizer.py#L244)

As you can see in [how to install rapids](https://docs.rapids.ai/install/), it only supports Linux and the CUDA version is not the same as other frameworks, so [docker](https://hub.docker.com/r/rapidsai/base) is the best choice, which is faster than CPU for batch processing but slower than CPU for streaming processing.

There are good example codes and explanations in the[ blog](https://developer.nvidia.com/blog/run-state-of-the-art-nlp-workloads-at-scale-with-rapids-huggingface-and-dask/#:~:text=,and then used in subsequent). To use cuDF, you must first convert vocab.txt to [hash_vocab](https://github.com/rapidsai/cudf/blob/branch-25.06/python/cudf/cudf/utils/hash_vocab_utils.py) as shown below. The problem is that the hash_vocab function cannot convert multilingual. Therefore, the WordpieceTokenizer of cuDF cannot be used if there are any characters other than English/Chinese in the vocab.

```python
import cudf
from cudf.utils.hash_vocab_utils import hash_vocab
hash_vocab('bert-base-cased-vocab.txt', 'voc_hash.txt')
```





## TODO

- [x] [BidirectionalWordPieceTokenizer](https://github.com/snunlp/KR-BERT/blob/master/krbert_tensorflow/tokenization_ranked.py)
- [x] BatchEncoder with Multithreading. 
- [x] Replace `std::list` to `boost::intrusive::list`.
- [x] ~~[MaxMatch-Dropout: Subword Regularization for WordPiece](https://arxiv.org/abs/2209.04126) Option.~~
- [x] Use stack memory for reduce memory allocation. (C-Style, [alloca](https://man7.org/linux/man-pages/man3/alloca.3.html), [_alloca](https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/reference/alloca?view=msvc-170))
- [x] ~~Support for parallel processing option for single encode.~~
- [ ] `circle.ai`
  - [ ] Implement distribution of compiled wheel packages for installation.
- [ ] SIMD
- [ ] ~~CUDA Version.~~



## Acknowledgement

FlashTokenizer is inspired by [FlashAttention](https://github.com/Dao-AILab/flash-attention), [FlashInfer](https://github.com/flashinfer-ai/flashinfer), [FastBertTokenizer](https://github.com/georg-jung/FastBertTokenizer) and [tokenizers-cpp](https://github.com/mlc-ai/tokenizers-cpp) projects.



## Performance comparison

* **WordPiece**
  * 📒  [huggingface/tokenizers (Rust)](https://github.com/huggingface/tokenizers)
    * Rust implementation of transformers.BertTokenizerFast, provided as a Python package.
    * 🔵 **Provided as a Python package.**
  * 🔥 [FastBertTokenizer (C#)](https://fastberttokenizer.gjung.com)
    * It demonstrates incredibly fast performance, but accuracy significantly decreases for non-English queries.
  * ❌ [BertTokenizers (C#)](https://github.com/NMZivkovic/BertTokenizers)
    * It can be confirmed from [FastBertTokenizer (C#) VS BertTokenizers (C#)](https://github.com/georg-jung/FastBertTokenizer/tree/master?tab=readme-ov-file#comparison-to-berttokenizers) that `FastBertTokenizer(C#)` is faster.
  * 🔥 [rust-tokenizers (Rust)](https://github.com/guillaume-be/rust-tokenizers)
    * Slower than BertTokenizerFlash and Blingfire but faster and more accurate than other implementations.
    * 🔵 **Provided as a Python package.**
  * ❌ [tokenizers-cpp (C++)](https://github.com/mlc-ai/tokenizers-cpp)
    * `tokenizer-cpp` is a wrapper around SentencePiece and HuggingFace's Rust implementation, so performance benchmarking is meaningless.
  * ❌ [bertTokenizer (Java)](https://github.com/ankiteciitkgp/bertTokenizer)
    * Java is not covered.
  * ✅ [ZhuoruLin/fast-wordpiece (Rust)](https://github.com/ZhuoruLin/fast-wordpiece)
    * A Rust implementation using LinMaxMatching, runnable only in Rust, and expected to be no faster than the C++ implementation.
  * ❌ [huggingface_tokenizer_cpp (C++)](https://github.com/Sorrow321/huggingface_tokenizer_cpp)
    * Very slow due to naive C++ implementation.
  * ❌ [SeanLee97/BertWordPieceTokenizer.jl (Julia)](https://github.com/SeanLee97/BertWordPieceTokenizer.jl)
    * Julia is not covered.
* **BPE**
  * https://github.com/openai/tiktoken
* **SentencePiece**
  * [google/sentencepiece (C++)](https://github.com/google/sentencepiece)



## ⭐ History

<a href="https://www.star-history.com/#NLPOptimize/flash-tokenizer&Date">

 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=NLPOptimize/flash-tokenizer&type=Date&theme=dark" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=NLPOptimize/flash-tokenizer&type=Date" />
   <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=NLPOptimize/flash-tokenizer&type=Date" />
 </picture>
</a>


## References

* https://medium.com/@techhara/which-bert-tokenizer-is-faster-b832aa978b46
* https://medium.com/@atharv6f_47401/wordpiece-tokenization-a-bpe-variant-73cc48865cbf
* https://www.restack.io/p/transformer-models-bert-answer-fast-berttokenizerfast-cat-ai
* https://medium.com/@anmolkohli/my-notes-on-bert-tokenizer-and-model-98dc22d0b64
* https://nocomplexity.com/documents/fossml/nlpframeworks.html
* https://github.com/martinus/robin-hood-hashing
* https://arxiv.org/abs/2012.15524
* https://github.com/google/highway
