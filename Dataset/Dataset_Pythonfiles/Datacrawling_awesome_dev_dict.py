import json
import re
import unicodedata


def slugify(text):
    text = text.lower().strip()
    text = unicodedata.normalize("NFKD", text).encode("ascii", "ignore").decode("utf-8")
    text = re.sub(r"[^\w\s-]", "", text)
    text = re.sub(r"[-\s]+", "-", text)
    return text


if __name__ == "__main__":
    existing_data = []
    existing_ids = set()
    with open(
        "Main/DSA-Project/Dataset/Dataset_JSONfiles/dictionary.json",
        "r",
        encoding="utf-8",
    ) as f:
        existing_data = json.load(f)
        existing_ids = {item["id"] for item in existing_data}
    with open(
        "Main/DSA-Project/Dataset/Dataset_Pythonfiles/raw_data_awesome_dev_dict.txt",
        "r",
        encoding="utf-8",
    ) as f:
        data = []
        markdown_content = f.read()
        pattern = re.compile(r"^-\s\*\*(.*?)\*\*:\s*(.*)$", re.MULTILINE)
        for match in pattern.finditer(markdown_content):
            term = match.group(1).strip()
            definition = match.group(2).strip()
            id_value = slugify(term)
            if id_value and id_value not in existing_ids:
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
