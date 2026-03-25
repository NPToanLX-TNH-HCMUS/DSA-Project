#include "Search.cpp" // Class SEARCH_DICTIONARY của bạn nằm ở đây
#include "header.h"

// Khởi tạo các biến toàn cục cần thiết
json dt_Search;
SEARCH_DICTIONARY search_dictionary;

int main(int argc, char *argv[]) {

  // 2. Kiểm tra tham số đầu vào
  // Nếu gọi: ./engine.exe "trie", thì argc = 2 và argv[1] = "trie"
  if (argc < 2) {
    // Nếu không truyền từ khóa, không in gì cả hoặc in thông báo lỗi ngắn
    return 0;
  }

  // 3. Load dữ liệu từ file JSON
  // Lưu ý: Đường dẫn này phải tính từ vị trí file server.js đang đứng
  // Ở đây mình giả sử file JSON nằm cùng thư mục hoặc đúng theo cấu trúc bạn
  // gửi
  string path_to_json = "../C++/dictionary.json";

  try {
    search_dictionary.load_dictionary(dt_Search, path_to_json);
  } catch (...) {
    cout << "ERROR: Could not load dictionary file at " << path_to_json;
    return 1;
  }

  // 4. Lấy từ khóa từ tham số dòng lệnh
  string query = argv[1];

  // 5. Thực hiện tìm kiếm
  // Hàm Search của bạn sẽ tự động cout ra Definition và Other words
  search_dictionary.Search(query);

  return 0;
}