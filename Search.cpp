#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

#define maxn 200007
json dt;

class SEARCH_DICTIONARY {
private:
    vector<string> word;
    unordered_map<string, string> dict;
    struct Node {
        Node *child[26];
        int cnt;
        int exist;
        Node () {
            for (int i = 0; i < 26; i++) child[i] = nullptr;
            cnt = 0;
            exist = 0;
        }
    };

    Node node[maxn];
    Node *root;
    int cur = 0;

    Node *newNode() {
        return &node[cur++];
    }

    string lower(string s) {
        for (char &c: s) {
            if ('A' <= c && c <= 'Z') c = c - 'A' + 'a';
        }
        return s;
    }

    void add(string s) {
        Node *p = root;
        for (char c: s) {
            if (p->child[c - 'a'] == nullptr) p->child[c - 'a'] = newNode();
            p = p->child[c - 'a'];
            p->cnt++;
        }
        p->exist++;
    }

    void dfs(Node *p, string &cur) {
        if (p->exist) {
            word.push_back(cur);
        }
        for (int i = 0; i < 26; i++) {
            if (p->child[i]) {
                cur.push_back(char(i + 'a'));
                dfs(p->child[i], cur);
                cur.pop_back();
            }
        }
    }
    
public:

    void load_dictionary(string filename) {
        ifstream f(filename);
        f >> dt;

        
        // input data to search dictionary
        for (auto &item: dt) {
            string id = lower(item["id"].get<string>());
            string def = item["definition"].get<string>();
            dict[id] = def;
            add(id);
        }   
    }

    void Search(string prefix) {
        word.clear();
        prefix = lower(prefix);
        Node *p = root;
        for (char c: prefix) {
            if (p->child[c] == nullptr) return;
            p = p->child[c];
        }

        if (p->exist) {
            cout << "=====DEFINITION=====\n";
            string W = prefix;
            cout << dict[W] << '\n';
            dfs(p, prefix);
            if (word.size() == 1) return;
            cout << "=====OTHER WORDS=====\n";
            for (string s: word) if (s != W) cout << s << '\n';
            return;
        }

        dfs(p, prefix);

        if (word.size() == 0) cout << "NOT FOUND!!!";
        else {
            for (string s: word) cout << s << '\n';
        }
        if (word.size() == 1) {
            cout << "=====DEFINITION=====\n";
            string W = word[0];
            cout << dict[W] << '\n';
        }
    }

    SEARCH_DICTIONARY() {
        root = newNode();
    }
};

SEARCH_DICTIONARY search_dictionary;

int main()
{
    cin.tie(0)->sync_with_stdio(false);

    // Load dictionary
    search_dictionary.load_dictionary("Dataset_fileJSON\\dictionary.json");
    // Search
    string prefix;
    cin >> prefix;
    search_dictionary.Search(prefix);
}
//Taidebug