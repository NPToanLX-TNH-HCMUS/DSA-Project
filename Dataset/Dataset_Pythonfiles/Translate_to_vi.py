import json
import time
from deep_translator import GoogleTranslator
import re

def translate_dataset(input_file, output_file):
    translator = GoogleTranslator(source='en', target='vi')
    with open(input_file, 'r', encoding='utf-8') as f:
        data = json.load(f)
    for index, item in enumerate(data):
        if 'definition_vi' not in item or not item['definition_vi']:
            try:
                english_text = item['definition']
                if re.match(r'^https?://', english_text):
                    item['definition_vi'] = ""  
                else:
                    vietnamese_text = translator.translate(english_text)
                    item['definition_vi'] = vietnamese_text
                time.sleep(1) 
            except Exception as e:
                print(f"Error at '{item['term']}': {e}")
                item['definition_vi'] = "" 
                time.sleep(2)
        if (index + 1) % 10 == 0:
            with open(output_file, 'w', encoding='utf-8') as f:
                json.dump(data, f, ensure_ascii=False, indent=4)
            print(">>> Saving ....")
    with open(output_file, 'w', encoding='utf-8') as f:
        json.dump(data, f, ensure_ascii=False, indent=4)

if __name__ == "__main__":
    translate_dataset("Main/DSA-Project/Dataset/Dataset_JSONfiles/dictionary.json", 'Main/DSA-Project/Dataset/Dataset_JSONfiles/dictionary_End_Vie.json')