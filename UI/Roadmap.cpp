#include "header.h"
#include "normalize.h"

class ROADMAP_CREATOR {
private:
  // Node for knowledge and label
  struct Node {
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
  void build_graph(Node new_node) {
    for (auto v : new_node.parent_nodes) {
      nodes[v].push_back(new_node.id);
      parents[new_node.id].push_back(v);
    }
  }
  bool check_advanced_label(string s) {
    if (s == "dynamicprogramming" || s == "advanceddatastructure" ||
        s == "graphtheory" || s == "stringhandle" || s == "treedatastructure" ||
        s == "mathforcp") {
      return true;
    }
    return false;
  }

public:
  bool check_label = false;
  void load_roadmap(json &dt, string roadmap, string knowledge) {
    // Open file
    string file_input = roadmap;
    ifstream file(file_input);
    if (!file.is_open()) {
      return;
    }
    // Read file
    file >> dt;
    // Create graph
    for (auto &item : dt) {
      Node n;
      n.id = item["id"];
      n.term = item["term"];
      n.classify = item["classify"];
      if (n.classify == "Label") {
        // cout << n.term << " " << knowledge << endl;
        if (knowledge == n.term) {
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
  // Create a Roadmap when user enter a knowledge
  vector<string> get_RoadMap_Knowledge(string knowledge) {
    // Declaration
    knowledge = slugify(knowledge);
    map<string, bool> visited;
    map<string, bool> subgraph;
    map<string, int> inDegree;
    vector<string> prerequisite_list;
    vector<string> RoadMap;
    for (Node v : node_list) {
      visited[v.id] = false;
      subgraph[v.id] = false;
      inDegree[v.id] = 0;
    }
    queue<string> q;
    q.push(knowledge);
    // Create prerequisite list
    while (!q.empty()) {
      string u = q.front();
      q.pop();
      prerequisite_list.push_back(u);
      subgraph[u] = true;
      for (string v : parents[u]) {
        if (visited[v])
          continue;
        q.push(v);
        visited[v] = true;
      }
    }
    // Topo Sort
    for (string u : prerequisite_list) {
      for (string v : parents[u]) {
        inDegree[u]++;
      }
    }
    queue<string> topo_list;
    for (string u : prerequisite_list) {
      if (!inDegree[u]) {
        topo_list.push(u);
      }
    }
    while (!topo_list.empty()) {
      string u = topo_list.front();
      topo_list.pop();
      if (!check_advanced_label(u))
        RoadMap.push_back(terms[u]);
      for (string v : nodes[u]) {
        if (subgraph[v]) {
          inDegree[v]--;
          if (!inDegree[v])
            topo_list.push(v);
        }
      }
    }
    return RoadMap;
  }

  // Create a Roadmap when user enter a label
  vector<string> get_RoadMap_Label(string Label) {
    // Declaration
    map<string, bool> subgraph;
    map<string, bool> visited;
    map<string, int> inDegree;
    vector<string> prerequisite_list;
    vector<string> RoadMap;
    queue<string> q;
    for (Node i : node_list) {
      visited[i.id] = false;
      subgraph[i.id] = false;
    }
    for (string i : Label_list) {
      q.push(i);
      subgraph[i] = true;
      visited[i] = true;
      inDegree[i] = 0;
    }
    // Create prerequisite list
    while (!q.empty()) {
      string u = q.front();
      q.pop();
      prerequisite_list.push_back(u);
      subgraph[u] = true;
      for (string v : parents[u]) {
        if (visited[v]) {
          continue;
        }
        visited[v] = true;
        inDegree[u] = 0;
        q.push(v);
      }
    }
    for (string u : prerequisite_list) {
      for (string v : parents[u]) {
        inDegree[u]++;
      }
    }
    // Topo Sort
    queue<string> topo_list;
    for (string u : prerequisite_list) {
      if (!inDegree[u])
        topo_list.push(u);
    }
    while (!topo_list.empty()) {
      string u = topo_list.front();
      topo_list.pop();
      if (!check_advanced_label(u))
        RoadMap.push_back(terms[u]);
      for (string v : nodes[u]) {
        if (subgraph[v]) {
          inDegree[v]--;
          if (!inDegree[v])
            topo_list.push(v);
        }
      }
    }
    return RoadMap;
  }
  json exportRoadmapJSON(string input) {
    json result;

    vector<string> roadmap;

    // dùng lại logic cũ
    if (check_label)
      roadmap = get_RoadMap_Label(input);
    else
      roadmap = get_RoadMap_Knowledge(input);

    // convert sang JSON array
    for (auto &item : roadmap) {
      result.push_back(item);
    }

    return result;
  }
};