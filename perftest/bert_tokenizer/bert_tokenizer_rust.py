# from rust_tokenizers import PyBertTokenizer
# from .config import get_config
#
#
# class RustBertTokenizer:
#     def __init__(self, config_dir: str):
#         config = get_config(config_dir)
#         self.tokenizer = PyBertTokenizer(config.vocab_file, do_lower_case=config.do_lower_case, strip_accents=False)
#         self.model_max_length = config.model_max_length
#
#     def __call__(self, text, max_length=-1, padding="max_length"):
#         if max_length == -1:
#             max_length = self.model_max_length
#         tokens = self.tokenizer.encode(text, max_len=max_length, truncation_strategy='longest_first', stride=0)
#         return tokens.token_ids
#
#     @property
#     def name(self):
#         return "rust_tokenizers(guillaume-be)"
