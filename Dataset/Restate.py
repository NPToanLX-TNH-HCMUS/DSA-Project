import json
import re
import unicodedata
def slugify(text):
    text = text.lower().strip()
    text = unicodedata.normalize("NFKD", text).encode("ascii", "ignore").decode("utf-8")
    text = re.sub(r"[^\w\s-]", "", text)
    text = re.sub(r"[-\s]+", "", text)
    return text
if __name__ == "__main__":
    with open(
            "Main/DSA-Project/Dataset/Dataset_JSONfiles/dictionary.json",
            "r",
            encoding="utf-8",
        ) as f:
        data=json.load(f)
    for item in data:
        if "id" in item:
            if "term" in item:
                item["id"] = slugify(item["term"])
    with open(
            "Main/DSA-Project/Dataset/Dataset_JSONfiles/dictionary.json",
            "w",
            encoding="utf-8",
        ) as f:
        json.dump(data, f, ensure_ascii=False, indent=4)

