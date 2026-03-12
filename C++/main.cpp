#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <windows.h>
#include "json.hpp"
#define maxn 200007
using namespace std;
using json = nlohmann::json;

// COLORS:
const string RESET = "\033[0m";
const string GREEN = "\033[1;32m"; // Highlight selected node
// COLORS

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

class ROADMAP_CREATOR
{
private:
    map<std::string, vector<std::string>> nodes, parents;
    map<std::string, std::string> terms;
    vector<Node> node_list;
    map<std::string, int> indegree;
    // Buiild Graph Structure:
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
            cout << "Cannot open the file" << "\n";
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

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    while (true)
    {
        ROADMAP_CREATOR roadmap;
        string user_input;
        cout << "Enter your categories: ";
        getline(cin, user_input);
        if (slugify(user_input) == "end")
        {
            cout << "Thanks for using our app. LOVE LOVE <3 <3" << "\n";
            break;
        }
        user_input = "../Dataset/Dataset_JSONfiles/Roadmap/" + user_input + ".json";
        roadmap.load_data(user_input);
        string knowledge;
        cout << "Enter the knowledge you want to learn: ";
        getline(cin, knowledge);
        cout << "Would you like a short or a long roadmap ? [1 for short || 2 for long]:";
        int choice;
        cin >> choice;
        cin.ignore();
        if (choice == 1)
        {
            vector<string> roadmap_print = roadmap.RoadMap_from_start_point(slugify(knowledge));
            for (auto v : roadmap_print)
            {
                if (v != roadmap_print.back())
                {
                    cout << v << " -> ";
                }
                else
                {
                    cout << GREEN << "[*" << v << "]" << RESET << "\n";
                }
            }
        }
        else if (choice == 2)
        {
            vector<string> roadmap_print = roadmap.Roadmap_fullcourse(slugify(knowledge));
            for (size_t i = 0; i < roadmap_print.size(); i++)
            {
                string v = roadmap_print[i];
                if (slugify(v) == slugify(knowledge))
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
                    cout << "\n";
                }
            }
        }
        else
        {
            cout << "I HATE YOU" << endl;
            abort();
        }
    }
    return 0;
}