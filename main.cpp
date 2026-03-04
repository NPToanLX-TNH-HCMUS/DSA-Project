#include <iostream>
#include <string>
#include <fstream>
#include <vector>
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

void add_node(Node new_node)
{
    for (auto v : new_node.next_nodes)
    {
        nodes[new_node.id].push_back(v);
    }
}

int main()
{
    // open file
    ifstream file("frontend.json");
    if (!file.is_open())
    {
        cout << "Khong mo duoc file!";
        cout << endl;
        return 1;
    }

    // read file
    json j_data;
    file >> j_data;

    // create graph
    vector<Node> node_list;
    for (auto &item : j_data)
    {
        Node n;
        n.id = item["id"];
        n.term = item["term"];
        n.next_nodes = item["next_nodes"].get<vector<string>>();
        add_node(n);
        node_list.push_back(n);
    }

    // debug
    for (const auto &n : node_list)
    {
        std::cout << "ID: " << n.id << " - Term: " << n.term << " - Next: ";
        for (auto next : n.next_nodes)
            std::cout << next << " ";
        std::cout << std::endl;
    }

    file.close();
    return 0;
}
