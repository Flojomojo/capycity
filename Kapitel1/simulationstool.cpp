#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <tuple>
#include "simulationstool.h"
using namespace std;

// Global variable buildings
vector<vector<BUILDING>> buildings;

// https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool is_number(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

void printArray()
{
    for (int i = 0; i < buildings.size(); i++)
    {
        for (int j = 0; j < buildings[i].size(); j++)
        {
            cout << buildings[i][j] << " ";
        }
        cout << endl;
    }
}

void placeBuilding(int x, int y, BUILDING building)
{
    // Check if x or y are out of bounds
    if (x > buildings.size() || x < 0 || y > buildings[0].size() || y < 0)
    {
        cout << "Invalid x or y" << endl;
        return;
    }

    // Check if the building is already at this location
    if (buildings[x][y] == building)
    {
        cout << "The building " << building << " is already at " << x << " " << y << endl;
        return;
    }

    // Check if there is already a building
    if (buildings[x][y] != EMPTY)
    {
        cout << "There is already a building at " << x << " " << y << endl;
        return;
    }

    buildings[x][y] = building;
    cout << "Successfully placed building " << buildingTypes[building];
    return;
}

void printPossibleBuildings()
{
    // Print all the possible buildings (EMPTY excluded)
    cout << "Possible Buildings:" << endl;
    for (int i = SOLARPANEL; i <= HYDROELECTRICPOWERPLANTS; i++)
    {
        cout << " " << i << ": " << buildingTypes[i] << endl;
    }
}

tuple<int, int> getCoordinateFromUser()
{
    // Init some variables we need for getting x and y of the "map"
    vector<int> dimArray;
    string parts;
    char delim = 'x';

    // Prompt the user for the coordinates of the "map"
    cout << "Enter x and y in the format XxY: ";

    // Read the coordinates from the console
    getline(cin, parts);
    stringstream partsStream = stringstream(parts);
    string part;
    // Split the coordinates into an array
    while (getline(partsStream, part, delim))
    {
        if (is_number(part))
        {
            dimArray.push_back(stoi(part));
        }
    }
    // If the array is not exactly of size 2 return a invalid tuple
    if (dimArray.size() != 2)
    {
        cout << "Invalid input" << endl;
        return make_tuple(-1, -1);
    }

    // Extract the x and y from the array
    // Substract one because normal humans don't start to count from 0
    int y = dimArray[0] - 1;
    int x = dimArray[1] - 1;
    return make_tuple(x, y);
}

void deleteBuilding()
{
    // Get the x and y and place a "EMPTY" building at that location
    int x, y;
    tie(x, y) = getCoordinateFromUser();
    placeBuilding(x, y, EMPTY);
}

void pickPlacement()
{
    int x, y;
    tie(x, y) = getCoordinateFromUser();
    cout << "Choose the building you want to place" << endl;

    printPossibleBuildings();

    // Ask the user for the building type
    int buildingType = -1;
    string buildingString;
    getline(cin, buildingString);
    // If it is just a number we already have the building type
    if (is_number(buildingString))
    {
        buildingType = stof(buildingString);
    }
    // Otherwise we need to convert the string the user supplied to a enum
    else
    {
        // Convert the input to uppercase and remove the spaces so we can make a comparison to the enum more easily
        string uppercaseBuildingString;
        for (auto &c : buildingString)
        {
            c = toupper(c);
            if (c != ' ')
                uppercaseBuildingString += c;
        }

        // Iterate over all the possible enum values and check if they match the user input
        for (int i = SOLARPANEL; i <= HYDROELECTRICPOWERPLANTS; i++)
        {
            string currentBuildingString = buildingTypes[i];
            string uppercaseCurrentBuildingString;
            for (auto &c : currentBuildingString)
            {
                c = toupper(c);
                if (c != ' ')
                    uppercaseCurrentBuildingString += c;
            }
            if (uppercaseCurrentBuildingString == uppercaseBuildingString)
            {
                int buildingType = i;
                break;
            }
        }
    }
    // If the supplied int is not a valid enum, return
    if (buildingType > HYDROELECTRICPOWERPLANTS || buildingType < SOLARPANEL)
    {
        cout << "Not a valid building type" << endl;
        return;
    }

    placeBuilding(x, y, static_cast<BUILDING>(buildingType));
}

void showMenu()
{

    enum MENU
    {
        EXIT = 0,
        PLACE = 1,
        DELETE = 2,
        PRINT = 3
    };

    const char *menuItems[] = {
        "Exit",
        "Place",
        "Delete",
        "Print",
    };
    // Loop over all the menu options and print them
    cout << "Menu:" << endl;
    for (int i = EXIT; i <= PRINT; i++)
    {
        cout << " " << i << ": " << menuItems[i] << endl;
    }

    // Prompt the user for the menu option
    cout << "Enter your choice: ";
    string choice;
    getline(cin, choice);
    if (!is_number(choice))
    {
        cout << "Invalid choice" << endl;
        showMenu();
        return;
    }
    int choiceInt = stoi(choice);
    if (choiceInt < EXIT || choiceInt > PRINT)
    {
        cout << "Invalid choice" << endl;
        showMenu();
        return;
    }
    // Iterate over all the enum options and choose the correct one
    switch (choiceInt)
    {
    case EXIT:
        cout << "Bye!" << endl;
        exit(0);
    case PLACE:
        pickPlacement();
        break;
    case DELETE:
        deleteBuilding();
        break;
    case PRINT:
        cout << "Current building space" << endl;
        printArray();
        break;
    }
}

int main()
{
    // Init some variables we need for getting the height and width of the "map"
    vector<int> dimArray;
    string parts;
    char delim = 'x';

    // Prompt the user for the dimensions of the "map"
    cout << "Enter height and width  in the format HxW: ";

    // Read the dimensions from the console
    getline(cin, parts);
    stringstream partsStream = stringstream(parts);
    string part;
    // Split the dimensions into an array
    while (getline(partsStream, part, delim))
    {
        if (is_number(part))
        {
            dimArray.push_back(stoi(part));
        }
    }
    // If the don't have exactly two dimensions, exit the program
    if (dimArray.size() != 2)
    {
        cout << "Invalid input" << endl;
        return -1;
    }

    // Extract the height and width from the array
    int h = dimArray[0];
    int w = dimArray[1];

    // Create a 2D array with the given h/w
    // vector<vector<BUILDING>> buildings;
    buildings.resize(h);

    for (int i = 0; i < h; i++)
    {
        buildings[i].resize(w);
    }

    // Fill the array with the default value
    for (int i = 0; i < buildings.size(); i++)
    {
        for (int j = 0; j < buildings[0].size(); j++)
        {
            buildings[i][j] = BUILDING::EMPTY;
        }
    }

    // Just show the menu forever
    while (true)
    {
        showMenu();
    };

    return 0;
}
