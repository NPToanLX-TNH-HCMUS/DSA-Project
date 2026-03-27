#include "header.h"
#include "normalize.h"

struct WordData {
  string definition;
  string definition_vi;
  bool in_roadmap;
  vector<string> roadmap;
  string term;
};

class SEARCH_DICTIONARY {
private:
  vector<string> word;
  struct Node {
    unordered_map<char, Node *> child;
    int cnt;
    int exist;
    Node() {
      cnt = 0;
      exist = 0;
    }
  };

  Node *root;

  Node *newNode() { return new Node(); }

  void dfs(Node *p, string &cur) {
    if (p->exist) {
      word.push_back(cur);
    }
    for (auto const &[key, childNode] : p->child) {
      cur.push_back(key);
      dfs(childNode, cur);
      cur.pop_back();
    }
  }

  void add(string s) {
    Node *p = root;
    for (char c : s) {
      if (p->child.find(c) == p->child.end())
        p->child[c] = newNode();
      p = p->child[c];
      p->cnt++;
    }
    p->exist++;
  }

public:
  unordered_map<string, WordData> dict;
  void load_dictionary(json &dt, string filename) {
    ifstream f(filename);
    if (!f.is_open()) {
      cerr << "Cannot open the dictionary file" << "\n";
      abort();
    }
    f >> dt;
    for (auto &item : dt) {
      string id = lower(item["id"].get<string>());
      string def = item["definition"].get<string>();
      string def_vie = item["definition_vi"].get<string>();
      bool in_rmap = item["in_roadmap"].get<bool>();
      string term_val = item["term"].get<string>();

      if (in_rmap && item["roadmap"].is_array()) {
        vector<string> rmap = item["roadmap"].get<vector<string>>();
        dict[id].roadmap = rmap;
      }
      dict[id].definition = def;
      dict[id].definition_vi = def_vie;
      dict[id].in_roadmap = in_rmap;
      dict[id].term = term_val;
      add(id);
    }
  }

  void Search(string prefix) {
    word.clear();
    prefix = slugify(prefix);
    Node *p = root;
    for (char c : prefix) {
      if (p->child.find(c) == p->child.end())
        return;
      p = p->child[c];
    }
    // ... (Giữ nguyên logic in kết quả bên dưới của bạn) ...
    if (p->exist) {
      string W = prefix;
      string def = dict[W].definition;

      if (def.rfind("http", 0) == 0) {
        cout << def << '\n';
      } else {
        cout << "=====DEFINITION_ENG=====\n";
        cout << def << '\n';
        cout << "=====DEFINITION_VIE=====\n";
        cout << dict[W].definition_vi << '\n';
      }
      dfs(p, prefix);
      if (word.size() == 1)
        return;
      cout << "=====OTHER WORDS=====\n";
      for (string s : word)
        if (s != W)
          cout << dict[slugify(s)].term << '\n';
      return;
    }
    dfs(p, prefix);
    if (word.size() == 0)
      cout << "NOT FOUND!!!";
    else {
      for (string s : word)
        cout << dict[slugify(s)].term << '\n';
    }
    if (word.size() == 1) {
      string W = word[0];
      string def = dict[W].definition;

      if (def.rfind("http", 0) == 0) {
        cout << def << '\n';
      } else {
        cout << "=====DEFINITION_ENG=====\n";
        cout << def << '\n';
        cout << "=====DEFINITION_VIE=====\n";
        cout << dict[W].definition_vi << '\n';
      }
    }
  }

  SEARCH_DICTIONARY() { root = newNode(); }
};