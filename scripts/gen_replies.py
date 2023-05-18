from pprint import pprint
import re
import yaml
from yaml.loader import SafeLoader

with open("scripts/reply_codes.yaml") as f:
    data = yaml.load(f, Loader=SafeLoader)

TOP = """void Client::reply(ReplyCode replyCode, std::string arg1, std::string arg2, std::string arg3, std::string arg4) const
{
	switch (replyCode)
		{"""

BOTTOM = """		default:
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


with open("scripts/reply_codes.out", "w") as f:
    print(TOP, file=f)
    for code, response in sorted(data.items()):
        print(f"\t\tcase {response['name']}:", file=f)
        print(
            f"\t\t\treturn addInfo({code}, {transform(response['reply'])});",
            file=f,
        )
    print(BOTTOM, file=f)
