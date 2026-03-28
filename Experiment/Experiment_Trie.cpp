#include "json.hpp"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>
#include <psapi.h>

#define maxn 100000

using namespace std;
using namespace std::chrono;
using json = nlohmann::json;

// COLORS:
const string RESET = "\033[0m";
const string GREEN = "\033[1;32m";
const string RED = "\033[1;31m";
const string PINK = "\033[1;35m";
const string YELLOW = "\033[0;33m";
const string BLUE = "\033[0;34m";
const string MAGENTA = "\033[0;35m";
const string CYAN = "\033[0;36m";

struct WordData
{
  string definition;
  string definition_vi;
  bool in_roadmap;
  vector<string> roadmap;
  string term;
  string classify;
};

string lower(string s)
{
  transform(s.begin(), s.end(), s.begin(), ::tolower);
  return s;
}

// Evaluate Space Usage:
double getMemoryUsage()
{
  PROCESS_MEMORY_COUNTERS_EX pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc,
                       sizeof(pmc));
  return (double)pmc.WorkingSetSize / (1024 * 1024);
}

// ==========================================
// Version 1: Trie using pointer (ARRAY)
// ==========================================
struct NodeArray
{
  NodeArray *child[256];
  int cnt;
  int exist;
  NodeArray()
  {
    for (int i = 0; i < 256; i++)
      child[i] = nullptr;
    cnt = 0;
    exist = 0;
  }
};
NodeArray node[maxn]; // maxn = 100000
NodeArray *root;
int cur = 0;
NodeArray *newNode() { return &node[cur++]; }
class TrieArrayVersion
{
  NodeArray *root;
  void clearMemory(NodeArray *node)
  {
    if (!node)
      return;
    for (int i = 0; i < 256; i++)
    {
      if (node->child[i])
        clearMemory(node->child[i]);
    }
    delete node;
  }

public:
  TrieArrayVersion() { root = new NodeArray(); }
  ~TrieArrayVersion() { clearMemory(root); } // Destructor
  void insert(const string &s)
  {
    NodeArray *p = root;
    for (char c : s)
    {
      int index = (unsigned char)c;
      if (p->child[index] == nullptr)
        p->child[index] = new NodeArray();
      p = p->child[index];
    }
    p->exist++;
  }
};

// ==========================================
// Version 2: Trie using Map
// ==========================================
struct NodeMap
{
  unordered_map<char, NodeMap *> child;
  int exist;
  NodeMap() { exist = 0; }
};
class TrieMapVersion
{
  NodeMap *root;
  void clearMemory(NodeMap *node)
  {
    if (!node)
      return;
    for (auto const &[key, childNode] : node->child)
    {
      clearMemory(childNode);
    }
    delete node;
  }

public:
  TrieMapVersion() { root = new NodeMap(); }
  ~TrieMapVersion() { clearMemory(root); } // Destructor
  void insert(const string &s)
  {
    NodeMap *p = root;
    for (char c : s)
    {
      if (p->child.find(c) == p->child.end())
        p->child[c] = new NodeMap();
      p = p->child[c];
    }
    p->exist++;
  }
};

int main()
{
  string path_to_json = "../Experiment/dictionary.json";
  ifstream f(path_to_json);
  if (!f.is_open())
  {
    cerr << "Error: Can't open file dictionary.json" << endl;
    return 1;
  }
  json dt;
  f >> dt;
  unordered_map<string, WordData> global_dict;
  vector<string> keys;
  for (auto &item : dt)
  {
    string id = lower(item["id"].get<string>());
    WordData wd;
    wd.definition = item.value("definition", "");
    wd.definition_vi = item.value("definition_vi", "");
    wd.in_roadmap = item.value("in_roadmap", false);
    wd.term = item.value("term", "");
    global_dict[id] = wd;
    keys.push_back(id);
  }
  f.close();

  // --------------------------------------------------
  // Test Version 1:
  // --------------------------------------------------
  cout << "[Old Version] TRIE - STATIC ARRAY OF POINTERS (Node* child[256])"
       << endl;
  double memBeforeArray = getMemoryUsage();
  auto startArray = high_resolution_clock::now();

  TrieArrayVersion *trie1 = new TrieArrayVersion();
  for (const string &word : keys)
  {
    trie1->insert(word);
  }

  auto stopArray = high_resolution_clock::now();
  double memAfterArray = getMemoryUsage();

  auto durationArray = duration_cast<milliseconds>(stopArray - startArray);
  cout << "--> Time: " << CYAN << durationArray.count() << " ms" << RESET
       << endl;
  cout << "--> RAM: " << RED << (memAfterArray - memBeforeArray) << " MB"
       << RESET << endl;
  cout << "--------------------------------------------------" << endl;

  // --------------------------------------------------
  // Test Version 2:
  // --------------------------------------------------
  cout << "[New Version] TRIE - HASH MAP (unordered_map)" << endl;
  double memBeforeMap = getMemoryUsage();
  auto startMap = high_resolution_clock::now();

  TrieMapVersion *trie2 = new TrieMapVersion();
  for (const string &word : keys)
  {
    trie2->insert(word);
  }

  auto stopMap = high_resolution_clock::now();
  double memAfterMap = getMemoryUsage();

  auto durationMap = duration_cast<milliseconds>(stopMap - startMap);
  cout << "--> Time: " << CYAN << durationMap.count() << " ms" << RESET << endl;
  cout << "-> RAM: " << RED << (memAfterMap - memBeforeMap) << " MB" << RESET
       << endl;
  cout << "--------------------------------------------------" << endl;

  return 0;
}