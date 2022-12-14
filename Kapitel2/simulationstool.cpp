#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <tuple>
#include "simulationstool.h"
using namespace std;

CapycitySim *simulation;

// https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool is_number(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

tuple<int, int> getCoordinateFromUser(std::string prompt = "Where do you want to place the building? (Format XxY)")
{
    // Init some variables we need for getting x and y of the "map"
    vector<int> dimArray;
    string parts;
    char delim = 'x';

    // Prompt the user for the coordinates of the "map"
    cout << "[?] " << prompt << endl;
    cout << "> ";
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
    tie(x, y) = getCoordinateFromUser("Which building do you want to delete? (Format XxY)");
    if (x == -1 || y == -1)
    {
        cout << "[!] Invalid coordinates" << endl;
        return;
    }
    simulation->setBuilding(x, y, EmptyBuilding());
}

void pickPlacement()
{
    int x, y;
    tie(x, y) = getCoordinateFromUser();
    if (x == -1 || y == -1)
    {
        cout << "[!] Invalid coordinates" << endl;
        return;
    }
    cout << "[?] Choose the building you want to place" << endl;

    vector<Building> buildingTypes = simulation->getBuildingTypes();

    simulation->printAllBuildingTypes();
    cout << "> ";
    // Ask the user for the building type
    int buildingType = -1;
    string buildingString;
    getline(cin, buildingString);
    // If it is just a number we already have the building type
    if (is_number(buildingString))
    {
        buildingType = stof(buildingString);
    }
    else
    {
        cout << "[!] Not a valid building type" << endl;
        return;
    }
    // If the supplied building is not a valid building return
    if (buildingType >= buildingTypes.size())
    {
        cout << "[!] Not a valid building type" << endl;
        return;
    }
    simulation->setBuilding(x, y, static_cast<Building>(buildingTypes[buildingType]));
}

void showMenu()
{
    // Loop over all the menu options and print them
    cout << "[*] Menu:" << endl;
    for (int i = EXIT; i <= PRINT; i++)
    {
        cout << " " << i << ": " << menuItems[i] << endl;
    }

    // Prompt the user for the menu option
    cout << "[?] Enter your choice: " << endl;
    cout << "> ";
    string choice;
    getline(cin, choice);
    if (!is_number(choice))
    {
        cout << "[!] Invalid choice" << endl;
        showMenu();
        return;
    }
    int choiceInt = stoi(choice);
    if (choiceInt < EXIT || choiceInt > PRINT)
    {
        cout << "[!] Invalid choice" << endl;
        showMenu();
        return;
    }
    // Iterate over all the enum options and choose the correct one
    switch (choiceInt)
    {
    case EXIT:
        cout << "[*] Bye!" << endl;
        exit(0);
    case PLACE:
        pickPlacement();
        break;
    case DEL:
        deleteBuilding();
        break;
    case PRINT:
        cout << "[*] Current building space" << endl;
        simulation->printInfo();
        break;
    }
}

int main()
{
    cout << "[!] Please maximize the terminal window for the best experience" << endl;
    // Init some variables we need for getting the height and width of the "map"
    vector<int> dimArray;
    string parts;
    char delim = 'x';

    // Prompt the user for the dimensions of the "map"
    cout << "[?] How big should the building space be? (Format HxW)" << endl;
    cout << "> ";
    // Read the dimensions from the console
    getline(cin, parts);
    stringstream partsStream = stringstream(parts);
    string part;
    // Split the dimensions into an array
    while (getline(partsStream, part, delim))
    {
        if (is_number(part))
        {
            // If the dimension is larger than 99, dont handle it, because that will print very ugly
            if (part.length() > 2)
            {
                cout << "[!] The dimensions are too big!";
                return 0;
            }
            dimArray.push_back(stoi(part));
        }
    }
    // If the don't have exactly two dimensions, exit the program
    if (dimArray.size() != 2 || dimArray[0] < 1 || dimArray[1] < 1)
    {
        cout << "[!] Invalid input" << endl;
        return -1;
    }

    // Extract the height and width from the array
    int h = dimArray[0];
    int w = dimArray[1];

    // Create the simulation
    simulation = new CapycitySim(h, w);

    // Just show the menu forever
    while (true)
    {
        showMenu();
    };

    // Delete the pointer
    delete simulation;
    return 0;
}
