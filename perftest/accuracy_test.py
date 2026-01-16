from performance_test import Config, Data, load_parquet
from bert_tokenizer import FlashBertTokenizer
from tqdm import tqdm
import os
from metrics import jaccard_sim
from pathlib import Path

config_path = "../dataset/config/" + Config.bert_base_multilingual_uncased
dataset_path = "../dataset/data/" + Data.texts_ko_all

texts, gts = load_parquet(Path(dataset_path), Path(os.path.basename(config_path)))

tokenizer = FlashBertTokenizer(config_path)
correct = 0
jaccard_sims = []
wrong_answers = []
with tqdm(total=len(texts)) as pbar:
    for text, gt in zip(texts, gts):
        ids = tokenizer(text, padding="longest", max_length=tokenizer.model_max_length)
        correct += ids == gt
        if ids != gt:
            jsim = jaccard_sim(ids, gt)
            jaccard_sims.append(jsim)
            wrong_answers.append(text)
        pbar.update(1)
with open("error_notebook.txt", "wt", encoding="utf-8") as f:
    f.write("\n".join(wrong_answers))
print(f'Accuracy: {correct * 100.0 / len(texts):.4f}%')

print(f'Max: {max(jaccard_sims)}')
print(f'Min: {min(jaccard_sims)}')
print(f'Avg: {sum(jaccard_sims) / len(jaccard_sims)}')
