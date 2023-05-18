def get_codes(filename):
	lines = open(filename).readlines()[1:-1]
	return {line.strip("\n\t ,=0123456789") for line in lines}

def display(title, diff):
	RESET = "\u001b[0m"
	BOLDBLUE = "\033[1m\u001b[34m"
	print(f"{BOLDBLUE}--- {title} ---{RESET}")
	print(*sorted(diff), sep="\n")

manual = get_codes("scripts/enum_yaml.out")
generated = get_codes("scripts/enum_rfcs.out")
display("NOT IN GENERATED", manual - generated)
display("NOT IN MANUAL", generated - manual)
