#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

struct Node
{
    string id;
    string term;
    vector<string> next_nodes;
};

map<string, vector<string>> nodes;
map<string, string> ids;
map<string, string> terms;
map<string, vector<string>> parents;
vector<Node> node_list;

string lower(string s)
{
    for (char &c : s)
    {
        if ('A' <= c && c <= 'Z')
            c = c - 'A' + 'a';
    }
    return s;
}

void add_node(Node new_node)
{
    for (auto v : new_node.next_nodes)
    {
        nodes[new_node.id].push_back(v);
        parents[v].push_back(new_node.id);
    }
}

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
        add_node(n);
        ids[n.term] = n.id;
        terms[n.id] = n.term;
        node_list.push_back(n);
    }
    file.close();
}

// BFS: 
vector<string> get_RoadMap(string knowledge)
{
    map<string, bool> check;
    for (Node v : node_list)
    {
        check[v.id] = false;
    }
    vector<string> RoadMap;
    RoadMap.push_back(knowledge);
    queue<string> q;
    q.push(knowledge);
    while (!q.empty())
    {
        string u = q.front();
        q.pop();
        // go to it parent
        for (string v : parents[u])
        {
            // if this knowledge already in RoadMap -> skip
            if (check[v])
                continue;
            RoadMap.push_back(terms[v]);
            q.push(v);
            check[v] = true;
        }
    }
    reverse(RoadMap.begin(), RoadMap.end());
    return RoadMap;
}

int main()
{
    string user_input; cin >> user_input;
    user_input = "../Dataset/Dataset_JSONfiles/Roadmap/" + user_input + ".json";
    load_data(user_input);
    string knowledge;
    cout << "Enter the knowledge you want to learn: ";
    cin >> knowledge;
    cout << "Suggested learning path: \n";
    cout << "---------------\n\n";
    vector<string> Roadmap = get_RoadMap(ids[knowledge]);
    for (auto v : Roadmap)
    {
        cout << v;
        if (v != Roadmap.back())
        {
            cout << " -> ";
        }
    }
    cout << "\n\n";
    cout << "---------------";
    return 0;
}