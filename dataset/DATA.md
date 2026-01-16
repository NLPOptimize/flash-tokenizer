# flash-tokenizer/dataset

## config

Each configuration folder contains `vocab.txt` and `tokenizer_config.json`, and the config directory also contains the
blingfire model file.

## data

Running `download.bat` or `download.sh` will download 4 parquet files.

Each parquet stores the text and the ids of the config for the text as an implementation
of [BertTokenizer(Google)](https://github.com/google-research/bert/blob/master/tokenization.py).

* `text`
* `bert-base-cased`
* `bert-base-chinese`
* `bert-base-multilingual-cased`
* `bert-base-multilingual-uncased`
* `bert-base-uncased`
* `kcbert-base`
* `llmlingua-2-bert-base-multilingual-cased-meetingbank`