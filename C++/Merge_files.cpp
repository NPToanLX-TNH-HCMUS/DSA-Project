/* CSC1004 - DSA PROJECT - TEAM 7 - DICTIONARY FOR DEVELOPERS
    NGÔ PHƯỚC TOÀN
    NGUYÊN THÀNH TÀY
    BÙI ĐĂNG KHÔI
*/
// --------------------------------------------------------------- LIBRARY/NAMESPACE DECLARATION -------------------------------------------------------
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <windows.h>
#include "json.hpp"
#include <algorithm>
#include <unordered_map>
#define maxn 200007
using namespace std;
using json = nlohmann::json;

// --------------------------------------------------------------- CONST/STRUCT DECLARATION ------------------------------------------------------------
// COLORS:
const string RESET = "\033[0m";
const string GREEN = "\033[1;32m"; // Highlight selected node
const string RED  = "\033[1;31m";  
const string PINK = "\033[1;35m"; 
// COLORS
json dt;

struct WordData
{
    string definition;
    string definition_vi;
    bool in_roadmap;
    vector<string> roadmap;
    string term;
};

struct Node
{
    string id;
    string term;
    vector<string> next_nodes;
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

string upper(string s)
{
    for (char &c : s)
    {
        if ('a' <= c && c <= 'z')
            c = c - 'a' + 'A';
    }
    return s;
}

string slugify(string s)
{
    int i = 0;
    for (char &c : s)
    {
        if ('A' <= c && c <= 'Z')
        {
            c = c - 'A' + 'a';
            s[i++] = c;
        }
        else if (('a' <= c && c <= 'z') || ('0' <= c && c <= '9'))
        {
            s[i++] = c;
        }
        else
        {
            continue;
        }
    }
    s = s.substr(0, i);
    return s;
}

// ---------------------------------------------------------- SECOND STAGE: ROADMAP ~~ DANG KHOI ~~ PHUOC TOAN -------------------------------------------
class ROADMAP_CREATOR
{
private:
    map<std::string, vector<std::string>> nodes, parents;
    map<std::string, std::string> terms;
    vector<Node> node_list;
    map<std::string, int> indegree;
    // Build Graph Structure:
    void build_graph(Node new_node)
    {
        for (auto v : new_node.next_nodes)
        {
            nodes[new_node.id].push_back(v);
            parents[v].push_back(new_node.id);
            indegree[v] += 1;
        }
    }

public:
    void load_data(string roadmap)
    {
        // Open file
        string file_input = roadmap;
        ifstream file(file_input);
        if (!file.is_open())
        {
            cout << "Cannot open roadmap files" << "\n";
            abort();
        }
        // Read file
        json j_data;
        file >> j_data;
        // Create graph
        for (auto &item : j_data)
        {
            Node n;
            n.id = item["id"];
            n.term = item["term"];
            n.next_nodes = item["next_nodes"].get<vector<string>>();
            build_graph(n);
            terms[n.id] = n.term;
            node_list.push_back(n);
        }
        file.close();
    }

    // BFS:
    vector<string> RoadMap_from_start_point(string knowledge)
    {
        map<string, bool> check;
        for (Node v : node_list)
        {
            check[v.id] = false;
        }
        vector<string> RoadMap;
        RoadMap.push_back(upper(knowledge));
        queue<string> q;
        q.push(knowledge);
        while (!q.empty())
        {
            string u = q.front();
            q.pop();
            // Go to it parent
            for (string v : parents[u])
            {
                // If this knowledge already in RoadMap -> skip
                if (check[v])
                    continue;
                RoadMap.push_back(upper(terms[v]));
                q.push(v);
                check[v] = true;
            }
        }
        reverse(RoadMap.begin(), RoadMap.end());
        return RoadMap;
    }

    // Topological Sort:
    vector<string> Roadmap_fullcourse(string knowledge)
    {
        queue<string> listSource;
        vector<string> res;
        map<string, int> temp_indegree = indegree;
        for (auto node : node_list)
        {
            if (!temp_indegree[node.id])
                listSource.push(node.id);
        }
        while (!listSource.empty())
        {
            string u = listSource.front();
            listSource.pop();
            res.push_back(upper(terms[u]));
            for (auto child : nodes[u])
            {
                temp_indegree[child]--;
                if (!temp_indegree[child])
                    listSource.push(child);
            }
        }
        return res;
    }
};

// ------------------------------------------------FIRST STAGE: SEARCH/HASHING/AUTO-COMPLETE ~~ THANH TAY ------------------------------------------------

class SEARCH_DICTIONARY
{
private:
    vector<string> word;
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
    unordered_map<string, WordData> dict;
    void load_dictionary(string filename)
    {
        ifstream f(filename);
        if (!f.is_open())
        {
            cerr << "Cannot open the dictionary file" << "\n";
            abort();
        }
        f >> dt;
        // Dictionary Data Reparation:
        for (auto &item : dt)
        {
            string id = lower(item["id"].get<string>());
            string def = item["definition"].get<string>();
            string def_vie = item["definition_vi"].get<string>();
            bool in_rmap = item["in_roadmap"].get<bool>();
            if (in_rmap && item["roadmap"].is_array())
            {
                vector<string> rmap = item["roadmap"].get<vector<string>>();
                dict[id].roadmap = rmap;
            }
            string term_val = item["term"].get<string>();
            dict[id].definition = def;
            dict[id].definition_vi = def_vie;
            dict[id].in_roadmap = in_rmap;
            dict[id].term = term_val;
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
                    cout << dict[slugify(s)].term << '\n';
            return;
        }
        dfs(p, prefix);
        if (word.size() == 0)
            cout << "NOT FOUND!!!";
        else
        {
            for (string s : word)
                cout << dict[slugify(s)].term << '\n';
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

// --------------------------------------------------------------- MAIN FUNCTION ---------------------------------------------------------------------
SEARCH_DICTIONARY search_dictionary;

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    // cin.tie(0)->sync_with_stdio(false);
    // Load dictionary
    search_dictionary.load_dictionary("../Dataset/Dataset_JSONfiles/dictionary.json");
    while (true)
    {
        cout << "=========================================================" << "\n";
        cout << RED << "WELCOME TO TEAM 7 PROJECT - DICTIONARY FOR DEVELOPERS" << RESET << "\n";
        cout << RED << "What do you want to search ? " << RESET << "\n";
        string prefix;
        getline(cin, prefix);
        cout << "=========================================================" << "\n";

        if (slugify(prefix) == "end")
        {
            cout << PINK << "====== THANKS FOR USING OUR SERVICE - SEE YOU AGAIN - LOVE <3 <3 =============" << RESET << "\n";
            break;
        }
        search_dictionary.Search(prefix);
        string search_key = slugify(prefix);
        if (search_dictionary.dict.count(search_key) > 0 && search_dictionary.dict[search_key].in_roadmap)
        {
            cout << "\n"
                 << GREEN << "This word has a roadmap. Do you want to print it? [Y/N]: " << RESET;
            string choice;
            do
            {
                cin >> choice;
                choice = lower(choice);
                if (choice != "y" && choice != "n")
                {
                    cout << "Invalid input. Please enter 'Y/y' or 'N/n': ";
                }
            } while (choice != "Y" && choice != "N" && choice != "n" && choice != "y");
            if (choice == "y" || choice == "Y")
            {
                int rm_choice;
                while (true)
                {
                    cout << "Would you like a short or a long roadmap ? [1 for short || 2 for long]: ";
                    if (cin >> rm_choice && (rm_choice == 1 || rm_choice == 2))
                    {
                        break;
                    }
                    cout << "Invalid input! Please enter 1 or 2.\n";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }
                cout << "\nPrinting roadmap(s)...\n";
                cout << "---------------------------------------------------------\n";
                for (const string &roadmap_file : search_dictionary.dict[search_key].roadmap)
                {
                    cout << ">>> " << upper(roadmap_file) << " ROADMAP <<<\n";
                    ROADMAP_CREATOR rc;
                    string file_path = "../Dataset/Dataset_JSONfiles/Roadmap/" + roadmap_file + ".json";
                    rc.load_data(file_path);
                    vector<string> roadmap_print;
                    if (rm_choice == 1)
                    {
                        roadmap_print = rc.RoadMap_from_start_point(search_key);
                    }
                    else
                    {
                        roadmap_print = rc.Roadmap_fullcourse(search_key);
                    }
                    for (size_t i = 0; i < roadmap_print.size(); i++)
                    {
                        string v = roadmap_print[i];
                        if (slugify(v) == slugify(search_dictionary.dict[search_key].term))
                        {
                            cout << GREEN << "[*" << v << "]" << RESET;
                        }
                        else
                        {
                            cout << v;
                        }
                        if (i != roadmap_print.size() - 1)
                        {
                            cout << " -> ";
                        }
                        else
                        {
                            cout << "\n\n";
                        }
                    }
                }
                cout << "---------------------------------------------------------\n";
            }
            cin.ignore(1000, '\n');
        }
    }
    return 0;
}

/* SUGGESTED TABLE OF FEATURES:
1. ADDING NEW TERMS
2. FIND DEF ENG - VIE
3. ROADMAP:
    3.1. FULL MAP --> TOPO SORT.
    3.2. SHORT MAP --> BFD/DFS.


*/