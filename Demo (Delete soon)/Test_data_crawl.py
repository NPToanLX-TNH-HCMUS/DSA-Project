import sys
sys.stdout.reconfigure(encoding='utf-8')
import requests
import json

def fetch_frontend_roadmap():
    # URL tải trực tiếp file JSON định nghĩa cấu trúc của Frontend Roadmap
    # (Đường dẫn này dựa trên cấu trúc thư mục phổ biến của repo developer-roadmap)
    raw_json_url = "https://raw.githubusercontent.com/kamranahmedse/developer-roadmap/master/src/data/roadmaps/frontend/frontend.json"
    
    print("Đang kết nối đến GitHub để tải cấu trúc Frontend...")
    response = requests.get(raw_json_url)
    
    if response.status_code == 200:
        try:
            roadmap_data = response.json()
            
            # Bạn có thể tiền xử lý dữ liệu ở đây trước khi đưa vào dataset
            # Ví dụ: Lọc bỏ các trường UI/UX không cần thiết (tọa độ x, y) để data gọn hơn
            
            with open("frontend_dataset.json", "w", encoding="utf-8") as f:
                json.dump(roadmap_data, f, ensure_ascii=False, indent=4)
                
            print("Đã tải và lưu thành công vào file 'frontend_dataset.json'!")
            
        except json.JSONDecodeError:
            print("Lỗi: Dữ liệu trả về không phải định dạng JSON hợp lệ.")
    else:
        print(f"Không thể tải file (Mã lỗi: {response.status_code}). Có thể tác giả đã đổi tên hoặc thay đổi cấu trúc thư mục.")

if __name__ == "__main__":
    fetch_frontend_roadmap()