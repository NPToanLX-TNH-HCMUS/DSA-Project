#include "header.h"
#include "normalize.h"

struct WordData {
  string definition;
  string definition_vi;
  bool in_roadmap;
  vector<string> roadmap;
  string term;
  string classify;
};

class SEARCH_DICTIONARY {
private:
  // Using unordered_map to optimize RAM.
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
  Node *newNode() {
    return new Node(); // Dynamic mem allocation
  }
  void dfs(Node *p, string &cur, vector<WordData> &keyword) {
    if (p->exist) {
      keyword.push_back(dict[cur]);
    }
    for (auto const &[key, childNode] : p->child) {
      cur.push_back(key);
      dfs(childNode, cur, keyword);
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
      // Load data to dict[]:
      // ----------------- Maybe use later: ------------------
      // if (in_rmap && item["roadmap"].is_array()) {
      //   vector<string> rmap = item["roadmap"].get<vector<string>>();
      //   dict[id].roadmap = rmap;
      // }
      // ----------------- Maybe use later: ------------------
      dict[id].definition = def;
      dict[id].definition_vi = def_vie;
      dict[id].in_roadmap = in_rmap;
      dict[id].term = term_val;
      add(id);
    }
  }

  vector<WordData> Search(string prefix) {
    vector<WordData> keyword;
    prefix = slugify(prefix);
    Node *p = root;
    for (char c : prefix) {
      if (p->child.find(c) == p->child.end())
        return keyword;
      p = p->child[c];
    }
    dfs(p, prefix, keyword);
    return keyword;
  }
  SEARCH_DICTIONARY() { root = newNode(); }
};