from transformers import BertTokenizerFast
from .config import get_config


class HuggingFaceBertTokenizerFast:
    def __init__(self, config_dir: str):
        config = get_config(config_dir)
        self.tokenizer = BertTokenizerFast(vocab_file=config.vocab_file, do_lower_case=config.do_lower_case, tokenize_chinese_chars=config.tokenize_chinese_chars)
        self.model_max_length = config.model_max_length

    def __call__(self, text, max_length=-1, padding="max_length"):
        if max_length == -1:
            max_length = self.model_max_length
        tokens = self.tokenizer(text, max_length=max_length, padding=padding, return_tensors="np", return_token_type_ids=False, return_attention_mask=False)
        return tokens.input_ids[0].tolist()

    @property
    def name(self):
        return "BertTokenizerFast(Huggingface)"

