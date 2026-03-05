import json
if __name__ == "__main__":
    with open("Main/DSA-Project/Dataset/Dataset_JSONfiles/dictionary_End_Vie.json", "r", encoding="utf-8") as f:
        data = json.load(f)
    data.sort(key=lambda x: x["id"])
    with open("data_sorted.json", "w", encoding="utf-8") as f:
        json.dump(data, f, indent=4, ensure_ascii=False)