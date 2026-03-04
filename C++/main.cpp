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

void add_node(Node new_node)
{
    for (auto v : new_node.next_nodes)
    {
        nodes[new_node.id].push_back(v);
        parents[v].push_back(new_node.id);
    }
}

void load_data()
{
    // open file
    ifstream file("frontend.json");
    if (!file.is_open())
    {
        cout << "Khong mo duoc file!";
        cout << endl;
        return;
    }

    // read file
    json j_data;
    file >> j_data;

    // create graph
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

    // debug
    /* for (const auto &n : node_list)
    {
        std::cout << "ID: " << n.id << " - Term: " << n.term << " - Next: ";
        for (auto next : n.next_nodes)
            std::cout << next << " ";
        std::cout << std::endl;
    } */

    file.close();
}

// bfs to retrieval
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
    load_data();
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
