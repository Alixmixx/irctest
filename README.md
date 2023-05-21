# Proxy usage

1. 1st terminal : `python3 scripts/proxy.py 127.0.0.1:5555 188.240.145.40:6667`
2. 2nd terminal : `irssi -c localhost -p 5555 -n gertrude1`
3. Un peu de patience (genre 10 secondes)
4. 3rd terminal : `irssi -c localhost -p 5555 -n gertrude2`
5. 2nd terminal : `join miao`
6. 3rd terminal : `join miao`
7. 3rd terminal : `salut sa va`
8. 2nd terminal : `/names`
9. 2nd terminal : `/whois gertrude2`
10. 2nd terminal : `/kick gertrude2 Elle sait pas ecrire.`
