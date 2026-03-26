/* CSC1004 - DSA PROJECT - TEAM 7 - DICTIONARY FOR DEVELOPERS
    NGÔ PHƯỚC TOÀN
    NGUYỄN THÀNH TÀY
    BÙI ĐĂNG KHÔI
*/
// ---------------------------------------------------------------
// LIBRARY/NAMESPACE DECLARATION
// ------------------------------------------------------
#include "Roadmap.cpp"
#include "Search.cpp"
#include "header.h"

// COLORS:
const string RESET = "\033[0m";
const string GREEN = "\033[1;32m";
const string RED = "\033[1;31m";
const string PINK = "\033[1;35m";
const string YELLOW = "\033[0;33m";
const string BLUE = "\033[0;34m";
const string MAGENTA = "\033[0;35m"; 
const string CYAN = "\033[0;36m";
// COLORS

json dt_Search, dt_Roadmap;
SEARCH_DICTIONARY search_dictionary;
int main() {
#ifdef _WIN32
  SetConsoleOutputCP(65001);
#endif
  // cin.tie(0)->sync_with_stdio(false);
  // Load dictionary:
  search_dictionary.load_dictionary(
      dt_Search, "../Dataset/Dataset_JSONfiles/dictionary.json");
  // Openning question:
  cout << "=========================================================" << "\n";
  cout << RED << "WELCOME TO TEAM 7 PROJECT - DICTIONARY FOR DEVELOPERS"
       << RESET << "\n";
  cout << "WHAT DO YOU WANT TODAY ?" << "\n";
  cout << "1. Search Vocabulary ?" << "\n";
  cout << "2. Search Roadmap for your carrer ?" << "\n";
  // Opening choice --> User_Prompting:
  int user_fchoice;
  while (true) {
    if (cin >> user_fchoice) {
      cin.ignore(1000, '\n');
      if (user_fchoice == 1 || user_fchoice == 2)
        break;
      cout << RED << "Please choose 1 or 2: " << RESET << "\n";
    } else {
      cout << RED << "Invalid input! Please enter a number (1 or 2): " << RESET
           << "\n";
      cin.clear();
      cin.ignore(1000, '\n');
    }
  }
  if (user_fchoice == 1) {
    while (true) {
      // Searching  --> User_Prompting:
      cout << GREEN << "What do you want to search ? " << RESET << "\n";
      string prefix;
      getline(cin, prefix);
      cout << "============================================================="
           << "\n";
      // End:
      if (slugify(prefix) == "end") {
        cout << PINK
             << "============ THANKS FOR USING OUR SERVICE - SEE YOU AGAIN - "
                "LOVE <3 <3 ============="
             << RESET << "\n";
        break;
      }
      // Search:
      vector<WordData> result = search_dictionary.Search(prefix);
      // cout << result.size() << "\n";
      if (result.size() == 0) {
        cout << RED << "NO WORD FOUND" << RESET << "\n";
      } else if (result.size()) {
        cout << YELLOW
             << "----------------------- DEFINITION_ENG ---------------------"
             << RESET << "\n";
        cout << CYAN << result[0].definition << RESET << "\n";
        cout << YELLOW
             << "------------------------------------------------------------"
             << RESET << "\n";
        cout << YELLOW
             << "----------------------- DEFINITION_VIE ---------------------"
             << RESET << "\n";
        cout << CYAN << result[0].definition_vi << RESET << "\n";
        cout << YELLOW
             << "------------------------------------------------------------"
             << RESET << "\n";
        cout << "============================================================="
             << "\n";
      }
    }
  } else {
    while (true) {
      ROADMAP_CREATOR rc;
      string knowledge;
      cout << GREEN << "What do you want to learn ?" << RESET << "\n"; 
      getline(cin, knowledge);
      if (lower(knowledge) == "end") {
        cout << PINK
             << "============ THANKS FOR USING OUR SERVICE - SEE YOU AGAIN - "
                "LOVE <3 <3 ============="
             << RESET << "\n";
        break;
      }
      // Auto-Complete:
      vector<WordData> result = search_dictionary.Search(knowledge);

      rc.load_roadmap(dt_Roadmap, "../Dataset/Dataset_JSONfiles/roadmap.json",
                      knowledge);
      if (rc.check_label) {
        vector<string> roadmap = rc.get_RoadMap_Label(knowledge);
        for (auto node : roadmap) {
          if (search_dictionary.word[node].classify == "Label") {
            cout << BLUE << node << RESET;
          } else if (search_dictionary.word[node].classify == "header") {
            cout << MAGENTA << node << RESET;
          } else {
            cout << node;
          }
          if (node != roadmap.back()) {
            cout << " --> ";
          }
        }
        cout << "\n";
      } else {
        vector<string> roadmap = rc.get_RoadMap_Knowledge(knowledge);
        for (auto node : roadmap) {
          if (search_dictionary.word[node].classify == "Label") {
            cout << BLUE << node << RESET;
          } else if (search_dictionary.word[node].classify == "header") {
            cout << MAGENTA << node << RESET;
          } else {
            cout << node;
          }
          if (node != roadmap.back()) {
            cout << " --> ";
          }
        }
        cout << "\n";
      }
    }
  }
  return 0;
}
