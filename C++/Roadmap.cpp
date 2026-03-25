#include "header.h"
#include "normalize.h"

class ROADMAP_CREATOR
{
private:
    // Node for knowledge and label
    struct Node
    {
        string id;
        string term;
        string classify;
        vector<string> parent_nodes;
    };

    // Declaration
    map<string, vector<string>> nodes;
    map<string, string> ids;
    map<string, string> terms;
    map<string, vector<string>> parents;
    vector<Node> node_list;
    vector<string> Label_list;

    // Create graph
    void build_graph(Node new_node)
    {
        for (auto v : new_node.parent_nodes)
        {
            nodes[v].push_back(new_node.id);
            parents[new_node.id].push_back(v);
        }
    }
    bool check_advanced_label(string s)
    {
        if (s == "dynamicprogramming" || s == "advanceddatastructure" || s == "graphtheory" || s == "stringhandle" || s == "treedatastructure" || s == "mathforcp")
        {
            return true;
        }
        return false;
    }

public:
    bool check_label = false;
    void load_roadmap(json &dt, string roadmap, string knowledge)
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
        file >> dt;
        // Create graph
        for (auto &item : dt)
        {
            Node n;
            n.id = item["id"];
            n.term = item["term"];
            n.classify = item["classify"];
            if (n.classify == "Label")
            {
                // cout << n.term << " " << knowledge << endl;
                if (knowledge == n.term)
                {
                    check_label = true;
                }
            }
            n.parent_nodes = item["link"].get<vector<string>>();
            build_graph(n);
            ids[n.term] = n.id;
            terms[n.id] = n.term;
            node_list.push_back(n);
            if (n.classify == ids[knowledge])
                Label_list.push_back(n.id);
        }
        file.close();
    }
    // Create Roadmap when user enter a knowledge
    vector<string> get_RoadMap_Knowledge(string knowledge)
    {
        knowledge = slugify(knowledge);
        map<string, bool> visited;
        for (Node v : node_list)
        {
            visited[v.id] = false;
        }
        vector<string> RoadMap;
        RoadMap.push_back(terms[knowledge]);
        queue<string> q;
        q.push(knowledge);
        while (!q.empty())
        {
            string u = q.front();
            q.pop();
            // go to it parent
            for (string v : parents[u])
            {
                //  if this knowledge already in RoadMap -> skip
                if (visited[v])
                    continue;
                if (!check_advanced_label(v))
                    RoadMap.push_back(terms[v]);
                q.push(v);
                visited[v] = true;
            }
        }
        reverse(RoadMap.begin(), RoadMap.end());
        return RoadMap;
    }

    vector<string> get_RoadMap_Label(string Label)
    {
        Label = slugify(Label);
        map<string, bool> subgraph;
        map<string, bool> visited;
        queue<string> q;
        map<string, int> inDegree;
        vector<string> topo_list;
        for (Node i : node_list)
        {
            visited[i.id] = false;
            subgraph[i.id] = false;
        }
        for (string i : Label_list)
        {
            q.push(i);
            subgraph[i] = true;
            visited[i] = true;
            inDegree[i] = 0;
        }
        while (!q.empty())
        {
            string u = q.front();
            q.pop();
            topo_list.push_back(u);
            subgraph[u] = true;
            for (string v : parents[u])
            {
                if (visited[v])
                {
                    continue;
                }
                visited[v] = true;
                inDegree[u] = 0;
                q.push(v);
            }
        }
        for (string u : topo_list)
        {
            for (string v : parents[u])
            {
                inDegree[u]++;
            }
        }
        queue<string> listSource;
        vector<string> res;
        map<string, int> temp_indegree = inDegree;
        for (auto node : topo_list)
        {
            if (!temp_indegree[node])
                listSource.push(node);
        }
        while (!listSource.empty())
        {
            string u = listSource.front();
            listSource.pop();
            if (!check_advanced_label(u))
                res.push_back(terms[u]);
            for (auto child : nodes[u])
            {
                if (subgraph[child])
                {
                    temp_indegree[child]--;
                    if (!temp_indegree[child])
                        listSource.push(child);
                }
            }
        }
        return res;
    }
};