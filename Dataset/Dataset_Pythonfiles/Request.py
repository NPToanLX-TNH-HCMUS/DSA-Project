import requests
url = ""

"""
url = [
    https://wiki.vnoi.info/
    
    ]
"""

r = requests.get(url)

"""
open = [
    raw_data_VNOI.txt

    ]
"""

with open("", "w", encoding = "utf-8") as f:
    f.write(r.text)