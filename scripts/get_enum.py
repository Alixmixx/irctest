import re

text = open("scripts/full_rfcs.txt").read()
with open("scripts/gen_enum.txt", "w") as f:
    print("typedef enum ReplyCode {", file=f)
    for code in sorted(set(re.findall(r"(?:RPL|ERR)_[A-Z]+", text))):
        print(f"\t{code},", file=f)
    print("} ReplyCode;", file=f)