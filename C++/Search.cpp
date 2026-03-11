#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>
#include <unordered_map>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

#define maxn 200007
json dt;

struct WordData
{
    string definition;
    string definition_vi;
    bool in_roadmap;
};

string lower(string s)
{
    for (char &c : s)
    {
        if ('A' <= c && c <= 'Z')
            c = c - 'A' + 'a';
    }
    return s;
}

class SEARCH_DICTIONARY
{
private:
    vector<string> word;
    unordered_map<string, WordData> dict;
    unordered_map<string, string> term;
    struct Node
    {
        Node *child[256];
        int cnt;
        int exist;
        Node()
        {
            for (int i = 0; i < 256; i++)
                child[i] = nullptr;
            cnt = 0;
            exist = 0;
        }
    };
    Node node[maxn];
    Node *root;
    int cur = 0;

    Node *newNode()
    {
        return &node[cur++];
    }

    void dfs(Node *p, string &cur)
    {
        if (p->exist)
        {
            word.push_back(cur);
        }
        for (int i = 0; i < 256; i++)
        {
            if (p->child[i])
            {
                cur.push_back(char(i));
                dfs(p->child[i], cur);
                cur.pop_back();
            }
        }
    }

    void add(string s)
    {
        Node *p = root;
        for (char c : s)
        {
            if (p->child[c] == nullptr)
                p->child[c] = newNode();
            p = p->child[c];
            p->cnt++;
        }
        p->exist++;
    }

public:
    void load_dictionary(string filename)
    {
        ifstream f(filename);
        if (!f.is_open())
        {
            cerr << "Cannot open the input file" << "\n";
            return;
        }
        f >> dt;
        // input data to search dictionary:
        for (auto &item : dt)
        {
            string id = lower(item["id"].get<string>());
            string def = item["definition"].get<string>();
            string def_vie = item["definition_vi"].get<string>();
            bool in_rmap = item["in_roadmap"].get<bool>();
            string term_val = item["term"].get<string>();
            dict[id].definition = def;
            dict[id].definition_vi = def_vie;
            dict[id].in_roadmap = in_rmap;
            term[id] = term_val;
            add(id);
        }
    }

    void Search(string prefix)
    {
        word.clear();
        prefix = lower(prefix);
        Node *p = root;
        for (char c : prefix)
        {
            if (p->child[c] == nullptr)
                return;
            p = p->child[c];
        }
        if (p->exist)
        {
            string W = prefix;
            cout << "=====DEFINITION_ENG=====\n";
            cout << dict[W].definition << '\n';
            cout << "=====DEFINITION_VIE=====\n";
            cout << dict[W].definition_vi << '\n';
            dfs(p, prefix);
            if (word.size() == 1)
                return;
            cout << "=====OTHER WORDS=====\n";
            for (string s : word)
                if (s != W)
                    cout << term[s] << '\n';
            return;
        }
        dfs(p, prefix);
        if (word.size() == 0)
            cout << "NOT FOUND!!!";
        else
        {
            for (string s : word)
                cout << term[s] << '\n';
        }
        if (word.size() == 1)
        {
            string W = word[0];
            cout << "=====DEFINITION_ENG=====\n";
            cout << dict[W].definition << '\n';
            cout << "=====DEFINITION_VIE=====\n";
            cout << dict[W].definition_vi << '\n';
        }
    }
    SEARCH_DICTIONARY()
    {
        root = newNode();
    }
};

SEARCH_DICTIONARY search_dictionary;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    cin.tie(0)->sync_with_stdio(false);

    // Load dictionary:
    search_dictionary.load_dictionary("C:\\Users\\lenovo\\.vscode\\NPToan\\InClass\\DSA's Project\\Main\\DSA-Project\\Dataset\\Dataset_JSONfiles\\dictionary_Eng_Vie.json");
    // Search:
    string prefix;
    cin >> prefix;
    search_dictionary.Search(prefix);
}

/* SUGGESTED TABLE OF FEATURES:
1. ADDING NEW TERMS
2. FIND DEF ENG - VIE
3. ROADMAP:
    3.1. FULL MAP --> TOPO SORT.
    3.2. SHORT MAP --> BFD/DFS.


*/