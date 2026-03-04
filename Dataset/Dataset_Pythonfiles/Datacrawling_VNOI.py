from bs4 import BeautifulSoup
import json
import re
import unicodedata

def slugify(text):
    text = text.lower().strip()
    text = unicodedata.normalize('NFKD', text).encode('ascii', 'ignore').decode('utf-8')
    pattern1 = r'[^\w\s-]'
    pattern2 = r'[-\s]+'
    text = re.sub(pattern1, '', text)
    text = re.sub(pattern2, '-', text)
    return text

def extract_id_from_href(href):
    if href.endswith('.md'):
        href = href[:-3]
    if '/' in href:
        return slugify(href.split('/')[-1])
    return slugify(href)

if __name__ == "__main__":
    existing_data = []
    existing_ids = set()
    with open("Main/DSA-Project/Dataset/Dataset_JSONfiles/dictionary.json", "r", encoding="utf-8") as f:
        existing_data = json.load(f)
        existing_ids = {item['id'] for item in existing_data}
    with open("Main/DSA-Project/Dataset/Dataset_Pythonfiles/raw_data_VNOI.txt", "r", encoding="utf-8") as f:
        html_content = f.read()
        soup = BeautifulSoup(html_content, "lxml")
        data = []
        for li_tag in soup.find_all("li"):
            for a_tag in li_tag.find_all("a"):
                href = a_tag.get("href")
                if href:
                    term = extract_id_from_href(href).replace('-',' ')
                    if term:  
                        term = term[0].upper() + term[1:]
                    else:
                        continue
                    id_value = extract_id_from_href(href)
                    if id_value not in existing_ids:
                        data.append({
                            "id": id_value,
                            "term": term,
                            "definition": "https://wiki.vnoi.info/" + href,
                            "in_roadmap": False,
                            "related_terms": []
                        })
                        existing_ids.add(id_value)

        with open("Main/DSA-Project/Dataset/Dataset_Pythonfiles/output.json", "w", encoding="utf-8") as json_file:
            json.dump(data, json_file, ensure_ascii=False, indent=4)


# After creating output.json, we also manually checked, fixed and cleaned dataset so that our dataset would have few "trash" data