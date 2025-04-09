from transformers import GPT2TokenizerFast

tokenizer = GPT2TokenizerFast(vocab_file='../dataset/BPE/vocab.json'
                              , merges_file="../dataset/BPE/merges.txt"
                              )

a = tokenizer.encode("Hello, world!")
print(a)
