import re
import yaml
from yaml.loader import SafeLoader

with open("scripts/files/reply_codes.yaml") as f:
    data = yaml.load(f, Loader=SafeLoader)

TOP = """void Client::reply(ReplyCode replyCode, std::string arg1, std::string arg2, std::string arg3, std::string arg4) const
{
	switch (replyCode)
	{"""

BOTTOM = """	default:
		exit(OUTSTANDING_ERROR);
	}
}"""


def transform(reply):
    i = 0

    def increment():
        nonlocal i
        i += 1
        return str(i)

    def replace(m):
        if m[0] == "<":
            return "arg" + increment()
        else:
            return '"' + m + '"'

    reply = " ".join(reply.split())
    reply = [replace(x) for x in re.findall(r"<(?:[a-z ']+?)>|[^<]+", reply)]
    return " + ".join(reply)


fr = open("scripts/files/reply_codes.out", "w")
fe = open("scripts/files/enum_yaml.out", "w")
print(TOP, file=fr)
print("typedef enum ReplyCode {", file=fe)
for code, response in sorted(data.items()):
    print(f"\tcase {response['name']}:", file=fr)
    print(
        f"\t\treturn reply(replyCode, {transform(response['reply'])});",
        file=fr,
    )
    print(f"\t{response['name']} = {code},", file=fe)
print(BOTTOM, file=fr)
print("} ReplyCode;", file=fe)

fr.close()
fe.close()
