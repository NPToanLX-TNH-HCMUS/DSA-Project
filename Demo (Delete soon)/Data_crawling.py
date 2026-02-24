import requests
from bs4 import BeautifulSoup
import json
import time

HEADERS = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36'
}

def crawl_roadmap_nodes(slug):
    print(f"--- Fetching nodes for: {slug} ---")
    url = f"https://roadmap.sh/{slug}"
    try:
        response = requests.get(url, headers=HEADERS)
        soup = BeautifulSoup(response.content, 'html.parser')
        nodes = []
        # Find potential skills in span, tspan, and anchor tags
        elements = soup.find_all(['span', 'tspan', 'a'])
        for el in elements:
            text = el.text.strip()
            if text and 2 < len(text) < 30:
                nodes.append({"skill": text, "type": "node"})
        
        # Remove duplicates while keeping order
        unique_nodes = list({v['skill']: v for v in nodes}.values())
        print(f"Success! Found {len(unique_nodes)} nodes for {slug}.")
        return unique_nodes
    except Exception as e:
        print(f"Error crawling {slug}: {e}")
        return []

if __name__ == "__main__":
    # Danh sách các Roadmap bạn muốn lấy dữ liệu
    # Bạn có thể thêm bất kỳ slug nào từ website roadmap.sh vào đây
    roadmap_list = ["frontend", "backend", "devops", "android", "full-stack", "data-analyst"]
    
    all_roadmaps_data = {}

    for slug in roadmap_list:
        # Cào dữ liệu cho từng slug
        data = crawl_roadmap_nodes(slug)
        all_roadmaps_data[slug] = data
        
        # Quan trọng: Nghỉ 1-2 giây để tránh bị website chặn (Rate limiting)
        time.sleep(1.5)

    # Lưu toàn bộ vào một file JSON duy nhất
    with open('mega_dataset.json', 'w', encoding='utf-8') as f:
        json.dump(all_roadmaps_data, f, ensure_ascii=False, indent=4)

    print("\n" + "="*30)
    print("ALL DONE! Check 'mega_dataset.json'")