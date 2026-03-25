/* CSC1004 - DSA PROJECT - TEAM 7 - DICTIONARY FOR DEVELOPERS
    NGÔ PHƯỚC TOÀN
    NGUYỄN THÀNH TÀY
    BÙI ĐĂNG KHÔI
*/
// --------------------------------------------------------------- LIBRARY/NAMESPACE DECLARATION ------------------------------------------------------
#include "header.h"
#include "Roadmap.cpp"
#include "Search.cpp"

// COLORS:
const string RESET = "\033[0m";
const string GREEN = "\033[1;32m"; // Highlight selected node
const string RED = "\033[1;31m";
const string PINK = "\033[1;35m";
// COLORS

json dt_Search, dt_Roadmap;
SEARCH_DICTIONARY search_dictionary;
int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    // cin.tie(0)->sync_with_stdio(false);
    // Load dictionary:
    search_dictionary.load_dictionary(dt_Search, "../Dataset/Dataset_JSONfiles/dictionary.json");
    // Openning question:
    cout << "=========================================================" << "\n";
    cout << RED << "WELCOME TO TEAM 7 PROJECT - DICTIONARY FOR DEVELOPERS" << RESET << "\n";
    cout << "WHAT DO YOU WANT TODAY ?" << "\n";
    cout << "1. Search Vocabulary ?" << "\n";
    cout << "2. Search Roadmap for your carrer ?" << "\n";
    // Opening choice --> User_Prompting:
    int user_fchoice;
    while (true)
    {
        if (cin >> user_fchoice)
        {
            cin.ignore(1000, '\n');
            if (user_fchoice == 1 || user_fchoice == 2)
                break;
            cout << RED << "Please choose 1 or 2: " << RESET << "\n";
        }
        else
        {
            cout << RED << "Invalid input! Please enter a number (1 or 2): " << RESET << "\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    if (user_fchoice == 1)
    {
        while (true)
        {
            // Searching  --> User_Prompting:
            cout << RED << "What do you want to search ? " << RESET << "\n";
            string prefix;
            getline(cin, prefix);
            cout << "=========================================================" << "\n";
            // End:
            if (slugify(prefix) == "end")
            {
                cout << PINK << "============ THANKS FOR USING OUR SERVICE - SEE YOU AGAIN - LOVE <3 <3 =============" << RESET << "\n";
                break;
            }
            // Search:
            search_dictionary.Search(prefix);
            string search_key = slugify(prefix);
            if (search_dictionary.dict[search_key].in_roadmap)
            {
                // Roadmap Creating --> User_Prompting:
                cout << "\n"
                     << GREEN << "This is actually a nodes in a roadmap. Are you curious about it? [Y/N]: " << RESET;
                string choice;
                do
                {
                    getline(cin, choice);
                    choice = lower(choice);
                    if (choice != "y" && choice != "n")
                    {
                        cout << "Invalid input. Please enter 'Y/y' or 'N/n': ";
                    }
                } while (choice != "Y" && choice != "N" && choice != "n" && choice != "y");
                if (choice == "y" || choice == "Y")
                {
                    break;
                }
                cin.ignore(1000, '\n');
            }
        }
    }
    else
    {
        ROADMAP_CREATOR rc;
        string knowledge;
        cout << "What do you want to learn ?" << "\n";
        getline(cin, knowledge);
        rc.load_roadmap(dt_Roadmap, "../Dataset/Dataset_JSONfiles/roadmap.json", knowledge);
        if (rc.check_label)
        {
            // cout << "1 \n" ;
            vector<string> roadmap = rc.get_RoadMap_Label(knowledge);
            for (auto node : roadmap)
            {
                cout << node << " ";
            }
            cout << "\n";
        }
        else
        {
            // cout << "2 \n" ;
            vector<string> roadmap = rc.get_RoadMap_Knowledge(knowledge);
            for (auto node : roadmap)
            {
                cout << node << " ";
            }
            cout << "\n";
        }
    }
    return 0;
}
