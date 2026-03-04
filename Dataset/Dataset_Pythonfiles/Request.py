import requests
url = ""

"""
url = [
    https://raw.githubusercontent.com/ephraimduncan/awesome-developer-dictionary/master/README.md
    https://www.geeksforgeeks.org/computer-networks/technical-terminology-of-computer/
    https://wiki.vnoi.info/
    ]
"""

r = requests.get(url)

"""
open = [
    raw_data_awesome_dev_dict.txt
    raw_data_GfG.txt
    raw_data_VNOI.txt
    ]
"""
if r.status_code != 200:
    print("Error: Status code {r.status_code}")
else:
    with open("raw_data_awesome_dev_dict.txt", "w", encoding = "utf-8") as f:
        f.write(r.text)