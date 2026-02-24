#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>
#include "json.hpp" 

// --- CẤU HÌNH WINDOWS ĐỂ HIỂN THỊ TIẾNG VIỆT ---
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using json = nlohmann::json;

// --- MÀU SẮC (ANSI CODES) ---
const string RESET = "\033[0m";
const string GREEN = "\033[1;32m";  // Màu xanh lá (Node đang chọn)
const string CYAN = "\033[1;36m";   // Màu xanh dương (Tiêu đề)
const string YELLOW = "\033[1;33m"; // Màu vàng (Mũi tên)
const string GRAY = "\033[1;30m";   // Màu xám (Các node chưa tới hoặc đã qua)

// --- 1. CẤU TRÚC DỮ LIỆU ---

struct Node {
    string id;
    string term;
    string definition;
    vector<string> prerequisites; // ID các môn cha (Cần học trước)
};

// --- 2. CẤU TRÚC TRIE (AUTO-COMPLETE) ---
struct TrieNode {
    map<char, TrieNode*> children;
    bool isEndOfWord;
    Node* data;

    TrieNode() { isEndOfWord = false; data = nullptr; }
};

class Trie {
private:
    TrieNode* root;

    void dfsCollect(TrieNode* node, vector<Node*>& results) {
        if (node->isEndOfWord) results.push_back(node->data);
        for (auto const& [key, child] : node->children) {
            dfsCollect(child, results);
        }
    }

public:
    Trie() { root = new TrieNode(); }

    void insert(string word, Node* nodeData) {
        TrieNode* curr = root;
        string temp = word;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        for (char c : temp) {
            if (curr->children.find(c) == curr->children.end())
                curr->children[c] = new TrieNode();
            curr = curr->children[c];
        }
        curr->isEndOfWord = true;
        curr->data = nodeData;
    }

    vector<Node*> searchPrefix(string prefix) {
        TrieNode* curr = root;
        string temp = prefix;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        for (char c : temp) {
            if (curr->children.find(c) == curr->children.end()) return {};
            curr = curr->children[c];
        }
        vector<Node*> results;
        dfsCollect(curr, results);
        return results;
    }
};

// --- 3. CẤU TRÚC GRAPH (ROADMAP LOGIC NÂNG CAO) ---
class RoadmapGraph {
private:
    map<string, Node> nodes;            // Lưu trữ thông tin Node
    map<string, vector<string>> adjForward; // Danh sách kề xuôi (Cha -> Con) để tìm tương lai
    
    set<string> visited;
    vector<Node*> tempPath; // Biến tạm dùng cho đệ quy

    // DFS ngược: Tìm về tổ tiên (Internet -> HTML -> ...)
    void dfsFindAncestors(string currentID) {
        if (visited.count(currentID)) return;
        visited.insert(currentID);

        Node& currentNode = nodes[currentID];
        // Đệ quy các node cha trước
        for (string preID : currentNode.prerequisites) {
            dfsFindAncestors(preID);
        }
        // Thêm node hiện tại vào path
        tempPath.push_back(&nodes[currentID]);
    }

    // DFS xuôi: Tìm hậu duệ (Sau node này học gì tiếp? ... -> React -> Fullstack)
    void dfsFindDescendants(string currentID) {
        // Chỉ lấy nhánh đầu tiên tìm thấy để vẽ thành 1 đường thẳng (Linear Roadmap)
        // Trong thực tế có thể rẽ nhánh, nhưng demo console nên vẽ thẳng.
        if (adjForward[currentID].empty()) return;

        // Lấy node con đầu tiên
        string nextID = adjForward[currentID][0]; 
        tempPath.push_back(&nodes[nextID]);
        
        // Tiếp tục đi sâu xuống
        dfsFindDescendants(nextID);
    }

public:
    void addNode(Node n) {
        nodes[n.id] = n;
        
        // Xây dựng đồ thị xuôi (Forward Graph) ngay khi nạp dữ liệu
        // Nếu A cần B (B là prereq của A), nghĩa là từ B sẽ đi tới A (B -> A)
        for (string parentID : n.prerequisites) {
            adjForward[parentID].push_back(n.id);
        }
    }

    // Hàm mới: Lấy lộ trình TOÀN CẢNH (Quá khứ + Tương lai)
    vector<Node*> getFullRoadmap(string targetID) {
        tempPath.clear();
        visited.clear();

        // Bước 1: Tìm quá khứ (Ancestors + Chính nó)
        dfsFindAncestors(targetID);

        // Bước 2: Tìm tương lai (Descendants)
        // Bắt đầu tìm từ chính node targetID
        dfsFindDescendants(targetID);

        return tempPath;
    }
};

// --- HÀM LOAD DATA ---
void loadData(Trie& t, RoadmapGraph& g) {
    ifstream f("web_dev_roadmap.json");
    if (!f.is_open()) {
        cerr << "LOI: Khong tim thay file JSON!\n";
        return;
    }

    json jsonData;
    try { f >> jsonData; } 
    catch (json::parse_error& e) { return; }

    for (const auto& item : jsonData) {
        Node newNode;
        newNode.id = item["id"].get<string>();
        newNode.term = item["term"].get<string>();
        newNode.definition = item["definition"].get<string>();
        
        if (item.contains("prerequisites")) {
            for (const auto& pre : item["prerequisites"]) {
                newNode.prerequisites.push_back(pre.get<string>());
            }
        }
        g.addNode(newNode);
        t.insert(newNode.term, new Node(newNode)); // Lưu ý: Cần delete node khi hủy Trie (demo bỏ qua)
    }
}

// --- MAIN ---
int main() {
    // [FIX TIẾNG VIỆT] Thiết lập mã trang (Code Page) sang UTF-8 cho Console Windows
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif

    Trie myTrie;
    RoadmapGraph myGraph;
    
    loadData(myTrie, myGraph);

    cout << "========================================\n";
    cout << "   IT ROADMAP TRA CUU (Full Context)\n";
    cout << "========================================\n";

    string input;
    while (true) {
        cout << "\n" << CYAN << ">>> Nhap tu khoa (go 'x' de thoat): " << RESET;
        getline(cin, input);

        if (input == "x") break;
        if (input.empty()) continue;

        // BƯỚC 1: Tìm kiếm
        vector<Node*> suggestions = myTrie.searchPrefix(input);

        if (suggestions.empty()) {
            cout << "(!) Khong tim thay.\n";
            continue;
        }

        Node* selectedNode = nullptr;
        if (suggestions.size() == 1) {
            selectedNode = suggestions[0];
        } else {
            cout << "Goi y:\n";
            for (size_t i = 0; i < suggestions.size(); ++i) 
                cout << i + 1 << ". " << suggestions[i]->term << "\n";
            
            cout << "Chon so: ";
            int choice;
            if (!(cin >> choice)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
            cin.ignore();
            if (choice > 0 && choice <= suggestions.size()) selectedNode = suggestions[choice - 1];
        }

        // BƯỚC 2: Hiển thị Full Roadmap
        if (selectedNode) {
            cout << "\n" << string(40, '-') << "\n";
            cout << "DINH NGHIA: " << GREEN << selectedNode->term << RESET << "\n";
            cout << "--> " << selectedNode->definition << "\n";
            cout << string(40, '-') << "\n";
            
            cout << "LỘ TRÌNH:\n\n";
            
            // Lấy toàn bộ lộ trình (Trước -> Đang chọn -> Sau)
            vector<Node*> path = myGraph.getFullRoadmap(selectedNode->id);

            // In Roadmap đẹp
            for (size_t i = 0; i < path.size(); ++i) {
                if (path[i]->id == selectedNode->id) {
                    // Node ĐANG TRA CỨU: Highlight XANH LÁ to rõ
                    cout << GREEN << " [ ★ " << path[i]->term << " ] " << RESET;
                } else {
                    // Node khác: Màu xám nhạt hoặc trắng
                    cout << path[i]->term;
                }

                if (i < path.size() - 1) {
                    cout << YELLOW << " -> " << RESET;
                }
            }
            cout << "\n\n";
        }
    }
    return 0;
}