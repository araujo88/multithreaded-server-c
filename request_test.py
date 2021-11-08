import requests

r = requests.get(url="http://127.0.0.1:9002")
print(f"\n{r.text}\n")
