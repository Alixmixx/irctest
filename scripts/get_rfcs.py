import requests
import time

RFCS = [1459, 2810, 2811, 2812, 2813, 7194]
full = ""
for rfc in RFCS:
    print(f"Downloading rfc {rfc}")
    url = f"https://www.rfc-editor.org/rfc/rfc{rfc}.txt"
    res = requests.get(url).text
    full += res
    time.sleep(1)
open("scripts/files/full_rfcs.txt", "w").write(full)