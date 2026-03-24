#include "header.h"

class ROADMAP_CREATOR
{
private:
    struct Node
    {
        string id;
        string term;
        string classify;
        vector<string> parent_nodes;
    };

    map<string, vector<string>> nodes;
    map<string, string> ids;
    map<string, string> terms;
    map<string, vector<string>> parents;
    vector<Node> node_list;
    bool check_label = false;
    vector<string> Label_list;
    map<std::string, int> indegree;

    void build_graph(Node new_node)
    {
        for (auto v : new_node.parent_nodes)
        {
            parents[new_node.id].push_back(v);
            indegree[v] += 1;
        }
    }
    void load_data(string roadmap, string knowledge)
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

    // add node to graph
    void add_node(Node new_node)
    {
        for (auto v : new_node.parent_nodes)
        {
            parents[new_node.id].push_back(v);
        }
    }

public:
    vector<string> get_RoadMap_Knowledge(string knowledge)
    {
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
                RoadMap.push_back(terms[v]);
                q.push(v);
                visited[v] = true;
            }
        }
        RoadMap.pop_back();
        RoadMap.push_back("Competitive Programming");
        reverse(RoadMap.begin(), RoadMap.end());
        return RoadMap;
    }

    vector<string> get_RoadMap_Label(string Label)
    {
        map<string, bool> visited;
        queue<string> q;
        map<string, int> inDegree;
        vector<string> topo_list;
        for (Node i : node_list)
        {
            visited[i.id] = false;
        }
        for (string i : Label_list)
        {
            q.push(i);
            inDegree[i] = 0;
        }
        while (!q.empty())
        {
            string u = q.front();
            q.pop();
            topo_list.push_back(u);
            for (string v : parents[u])
            {
                if (visited[v])
                {
                    continue;
                }
                visited[v] = true;
                inDegree[v] = 0;
                q.push(v);
            }
        }
        for (string u : topo_list)
        {
            for (string v : parents[u])
            {
                inDegree[v]++;
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
            res.push_back(terms[u]);
            for (auto child : parents[u])
            {
                temp_indegree[child]--;
                if (!temp_indegree[child])
                    listSource.push(child);
            }
        }
        res.pop_back();
        res.push_back("Competitive Programming");
        reverse(res.begin(), res.end());
        return res;
    }
};