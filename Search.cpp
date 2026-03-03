#include <bits/stdc++.h>
#include <Windows.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

#define fi first
#define se second
#define ll long long
#define pii pair<int, int>
#define MASK(x) (1 << (x))
#define BIT(x, i) (((x) >> (i)) & 1)
#define all(x) (x).begin(),(x).end()
#define meme(a, n) memset((a), (n), sizeof (a))

template <typename T1, typename T2> bool minimize(T1 &a, T2 b) { if (a > b) { a = b; return true; } return false; }
template <typename T1, typename T2> bool maximize(T1 &a, T2 b) { if (a < b) { a = b; return true; } return false; }

const int maxn = 2e4 + 7;
const int inf = 1e9 + 7;
const int logn = 20;

int n;
vector<string> word;
json dt;

struct Node {
    Node *child[256];
    int cnt;
    int exist;
    Node () {
        for (int i = 0; i < 256; i++) child[i] = nullptr;
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

// Trie
void add(string s) {
    Node *p = root;
    for (char c: s) {
        if (p->child[c] == nullptr) p->child[c] = newNode();
        p = p->child[c];
        p->cnt++;
    }
    p->exist++;
}

string lower(string s) {
    for (char &c: s) {
        if ('A' <= c && c <= 'Z') c = c - 'A' + 'a';
    }
    return s;
}

void dfs(Node *p, string cur) {
    if (p->exist) {
        word.push_back(cur);
    }
    for (int i = 0; i < 256; i++) {
        if (p->child[i]) dfs(p->child[i], cur + char(i));
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
        for (auto &item: dt) if (W == lower(item["id"].get<string>())) {
            cout << item["definition"].get<string>() << '\n';
        }
        dfs(p, prefix);
        if (word.size() == 1) {
            cout << "====================\n\n";
            return;
        }
        cout << "=====OTHER WORDS=====\n";
        for (string s: word) if (s != W) cout << s << '\n';
        cout << "=====================\n\n";
        return;
    }

    dfs(p, prefix);

    if (word.size() == 0) cout << "NOT FOUND!!!";
    else {
        cout << "====================\n";
        for (string s: word) cout << s << '\n';
        cout << "====================\n\n";
    }
    if (word.size() == 1) {
        cout << "=====DEFINITION=====\n";
        string W = word[0];
        for (auto &item: dt) if (W == lower(item["id"].get<string>())) {
            cout << item["definition"].get<string>() << '\n';
        }
        cout << "====================\n\n";
    }
}



void solve() {
    root = newNode();
    ifstream f("D:\\PL\\C++\\HCMUS\\DSA\\PROJECT_DSA\\Dataset_fileJSON\\cp-dictionary.json");
    f >> dt;
    // INPUT
    for (auto &item: dt) add(lower(item["id"].get<string>()));

    // Seach
    string prefix;
    while (true) {
        cout << "Search: ";
        cin >> prefix;
        prefix = lower(prefix);
        if (prefix == "end") break;
        Search(prefix);
    }
    
    system("pause");
}

int main()
{
    //freopen("dataset.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    //cin.tie(0)->sync_with_stdio(false);
    solve();
}
//Taidebug