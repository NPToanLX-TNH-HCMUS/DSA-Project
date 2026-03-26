#include "header.h"
#include "normalize.h"

struct WordData
{
    string definition;
    string definition_vi;
    bool in_roadmap;
    vector<string> roadmap;
    string term;
    string classify;
};

class SEARCH_DICTIONARY
{
private:
    unordered_map<string, WordData> word;
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

    void dfs(Node *p, string &cur, vector<WordData> &keyword)
    {
        if (p->exist)
        {
            keyword.push_back(word[cur]);
        }
        for (int i = 0; i < 256; i++)
        {
            if (p->child[i])
            {
                cur.push_back(char(i));
                dfs(p->child[i], cur, keyword);
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
    void load_dictionary(json &dt, string filename)
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
            // Temp vars
            string id = slugify(item["id"].get<string>());
            string def = item["definition"].get<string>();
            string def_vie = item["definition_vi"].get<string>();
            bool in_rmap = item["in_roadmap"].get<bool>();
            string term_val = item["term"].get<string>();
            string classify_val = item["classify"].get<string>();
            // Save to word:
            if (in_rmap && item["roadmap"].is_array())
            {
                vector<string> rmap = item["roadmap"].get<vector<string>>();
                word[id].roadmap = rmap;
            }
            word[id].definition = def;
            word[id].definition_vi = def_vie;
            word[id].in_roadmap = in_rmap;
            word[id].term = term_val;
            word[id].classify = classify_val;
            add(id);
        }
    }

    vector<WordData> Search(string prefix)
    {
        vector<WordData> keyword;
        prefix = slugify(prefix);
        Node *p = root;
        for (char c : prefix)
        {
            if (p->child[c] == nullptr)
                return keyword;
            p = p->child[c];
        }
        dfs(p, prefix, keyword);
        return keyword;
    }
    SEARCH_DICTIONARY()
    {
        root = newNode();
    }
};
// Taidebug