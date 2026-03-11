import requests
from bs4 import BeautifulSoup
import json
import os
import re
import unicodedata


def slugify(text):
    text = text.lower().strip()
    text = unicodedata.normalize("NFKD", text).encode("ascii", "ignore").decode("utf-8")
    pattern1 = r"[^\w\s-]"
    pattern2 = r"[-\s]+"
    text = re.sub(pattern1, "", text)
    text = re.sub(pattern2, "", text)
    return text


if __name__ == "__main__":
    url = "https://www.geeksforgeeks.org/computer-networks/technical-terminology-of-computer/"
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36"
    }
    r = requests.get(url, headers=headers)
    if r.status_code != 200:
        print("Error, status code: {r.status_code}")
    else:
        existing_data = []
        existing_ids = set()
        with open(
            "Main/DSA-Project/Dataset/Dataset_JSONfiles/dictionary.json",
            "r",
            encoding="utf-8",
        ) as f:
            existing_data = json.load(f)
            existing_ids = {item["id"] for item in existing_data}
        # with open("testdata.txt", "w", encoding = 'utf-8') as f:
        #   f.write(r.text)
        with open(
            "Main/DSA-Project/Dataset/Dataset_Pythonfiles/raw_data_GfG.txt",
            "r",
            encoding="utf-8",
        ) as f:
            html_content = f.read()
            soup = BeautifulSoup(html_content, "lxml")
            data = []
            for li_tag in soup.find_all("li"):
                strong_tag = li_tag.find("strong")
                if strong_tag:
                    term = strong_tag.get_text().strip().replace(":", "")
                    full_text = li_tag.get_text(separator=" ", strip=True)
                    definition = (
                        full_text.replace(term, "", 1).strip().lstrip(":").strip()
                    )
                    definition = definition.replace("\xa0", " ")
                    if term and len(definition) > 10:
                        id_value = slugify(term)
                        if id_value not in existing_ids:
                            data.append(
                                {
                                    "id": id_value,
                                    "term": term,
                                    "definition": definition,
                                    "in_roadmap": False,
                                    "related_terms": [],
                                }
                            )
                            existing_ids.add(id_value)
        with open("output.json", "w", encoding="utf-8") as json_file:
            json.dump(data, json_file, ensure_ascii=False, indent=4)


# After creating output.json, we also manually checked, fixed and cleaned dataset so that our dataset would have few "trash" data
