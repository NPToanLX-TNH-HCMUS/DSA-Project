#include "header.h"
#include "Search.cpp"
#include "Roadmap.cpp"
#include "json.hpp"

using json = nlohmann::json;

json dt_Search;
SEARCH_DICTIONARY search_dictionary;

json dt_Roadmap;
ROADMAP_CREATOR roadmap_creator;

int main(int argc, char *argv[]) {

  if (argc < 2) return 0;

  string mode = "search";
  string query;

  if (argc == 2) {
    query = argv[1]; 
  } else {
    mode = argv[1];
    query = argv[2];
  }

  string path_to_json = "../UI/dictionary.json";

  try {
    search_dictionary.load_dictionary(dt_Search, path_to_json);
  } catch (...) {
    cout << "ERROR: Could not load dictionary file";
    return 1;
  }

  if (mode == "search") {
    search_dictionary.Search(query);
    return 0;
  }

  if (mode == "roadmap") {

    string roadmap_path = "../UI/roadmap.json";

    try {
      roadmap_creator.load_roadmap(dt_Roadmap, roadmap_path, query);
    } catch (...) {
      cout << "[]";
      return 0;
    }

    vector<string> roadmap;

    if (roadmap_creator.check_label)
      roadmap = roadmap_creator.get_RoadMap_Label(query);
    else
      roadmap = roadmap_creator.get_RoadMap_Knowledge(query);

    json out = roadmap;
    cout << out.dump();

    return 0;
  }

  return 0;
}