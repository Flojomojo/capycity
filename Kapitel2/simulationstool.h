#ifndef SIMULATIONTOOL_H
#define SIMULATIONTOOL_H

// MATERIALS

class Material
{
protected:
    double price;

public:
    double getPrice() { return this->price; }
};

class Wood : public Material
{
public:
    Wood()
    {
        this->price = 1;
    }
};

class Metal : public Material
{
public:
    Metal()
    {
        this->price = 2;
    }
};

class Plastic : public Material
{
public:
    Plastic()
    {
        this->price = 3;
    }
};

// BUILDINGS

class Building
{
protected:
    double basePrice;
    std::string label;
    std::string fullLabel;
    std::vector<Material> necessaryMaterials;

public:
    double getBasePrice() { return this->basePrice; }
    double getTotalPrice()
    {
        // Calculate the total price by taking the base price and adding all the material prices on top
        double totalPrice = this->basePrice;
        for (Material &value : this->getNecessaryMaterials())
        {
            totalPrice += value.getPrice();
        }
        return totalPrice;
    }
    std::string getLabel() { return this->label; }
    std::string getFullLabel() { return this->fullLabel; }
    std::vector<Material> &getNecessaryMaterials() { return this->necessaryMaterials; }
};

class EmptyBuilding : public Building
{
public:
    EmptyBuilding()
    {
        // Just place holder values basically
        this->basePrice = 0;
        this->label = "0";
    }
};

// This is just a building for better error handling
class ErrorBuilding : public Building
{
public:
    ErrorBuilding()
    {
        // Just place holder values basically
        this->basePrice = -1;
        this->label = "";
    }
};

class SolarPanelBuilding : public Building
{
public:
    SolarPanelBuilding()
    {
        // Added some random values, because no specifications were given
        this->basePrice = 1;
        this->label = "S";
        this->fullLabel = "Solar Panel";

        this->necessaryMaterials.push_back(Metal());
        this->necessaryMaterials.push_back(Wood());
        this->necessaryMaterials.push_back(Wood());
    }
};

class WindPowerPlantBuilding : public Building
{
public:
    WindPowerPlantBuilding()
    {
        this->basePrice = 2;
        this->label = "W";
        this->fullLabel = "Wind Power Plant";

        this->necessaryMaterials.push_back(Metal());
        this->necessaryMaterials.push_back(Metal());
        this->necessaryMaterials.push_back(Plastic());
    }
};

class HydroelectricPowerPlants : public Building
{
public:
    HydroelectricPowerPlants()
    {
        this->basePrice = 4;
        this->label = "H";
        this->fullLabel = "Hydroelectric Power Plant";

        this->necessaryMaterials.push_back(Metal());
        this->necessaryMaterials.push_back(Metal());

        this->necessaryMaterials.push_back(Plastic());
        this->necessaryMaterials.push_back(Plastic());
        this->necessaryMaterials.push_back(Plastic());
    }
};

class CapycitySim
{
private:
    std::vector<std::vector<Building>> buildings;
    std::vector<Building> buildingTypes{SolarPanelBuilding(), WindPowerPlantBuilding(), HydroelectricPowerPlants()};

    bool inBounds(int x, int y)
    {
        // Check if x or y are out of bounds
        return (x <= this->buildings.size() - 1 && x >= 0 && y <= this->buildings[0].size() - 1 && y >= 0);
    };

    // Just a helper function to print the buildings
    void printLine(std::ostream &ostream, std::string postfix = "")
    {
        // +-----------------------+
        ostream << "+";
        for (int i = 0; i < this->buildings[0].size() - 1; i++)
        {
            // If the number is bigger than 9, we need more space
            if (i >= 10)
            {
                ostream << "-----";
            }
            else
            {
                ostream << "----";
            }
        }
        // Again if the number is double digits we need more space
        std::string finalPart = "---+";
        // If its ten we need one less -
        if (this->buildings[0].size() == 10)
        {
            finalPart = "----+";
        }
        else if (this->buildings[0].size() > 10)
        {
            finalPart = "-----+";
        }

        ostream << finalPart << postfix << std::endl;
    }

    void getBoardInfo(std::ostream &ostream)
    {
        /*
        Print all the info in this format
        In this function we only print the building space info, the other info boxes will be injected in printInfo()
        +-----------------------------------+ x
        | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |   y
        +-----------------------------------+---+     +-----------------+
        | 0   0   0   0   0   0   0   0   0 | 1 |     | Gebäude:        |
        |                                   |---|     |                 |
        | 0   0   0   0   0   0   0   0   0 | 2 |     |   5x H  6x S    |   +-------------------+
        |                                   |---|     |                 |	| Preise:           |
        | 0   0   0   0   0   0   0   0   0 | 3 |     |                 |   |                   |
        |                                   |---|     +-----------------+   |  5x H    5$ | 25$ |
        | 0   0   0   0   0   0   0   0   0 | 4 |                           |  6x S    8$ | 48$ |
        |                                   |---|                           |  0x W   10$ |     |
        | 0   0   0   0   0   0   0   0   0 | 5 |     +-----------------+   |             |     |
        |                                   |---|	  | Materialien:    |   |-------------+-----|
        | 0   0   0   0   0   0   0   0   0 | 6 |     |                 |   |             |	 73$|
        |                                   |---|     |   20x Metal     |   +-------------+-----+
        | 0   0   0   0   0   0   0   0   0 | 7 |     |   10x Holz      |
        |                                   |---|     |                 |
        | 0   0   0   0   0   0   0   0   0 | 8 |     |                 |
        |                                   |---|     +-----------------+
        | 0   0   0   0   0   0   0   0   0 | 9 |
        +-----------------------------------+---+
        */

        // +-----------------------+ x
        this->printLine(ostream, " x");

        // | 1 | 2 | 3 | 4 | 5 | 6 |   y
        ostream << "|";
        for (int i = 0; i < this->buildings[0].size(); i++)
        {
            ostream << " " << (i + 1) << " |";
        }
        ostream << "   y" << std::endl;

        // +-----------------------------------+---+
        this->printLine(ostream, "---+");
        /*
        | 0   0   0   0   0   0 | 1 |
        |                       |---|
        | 0   0   0   0   0   0 | 2 |
        |                       |---|
        | 0   0   0   0   0   0 | 3 |
        |                       |---|
        | 0   0   0   0   0   0 | 4 |
        |                       |---|
        | 0   0   0   0   0   0 | 5 |
        |                       |---|
        | 0   0   0   0   0   0 | 6 |
        |                       |---|
        | 0   0   0   0   0   0 | 7 |
        |                       |---|
        | 0   0   0   0   0   0 | 8 |
        */
        for (int i = 0; i < this->buildings.size(); i++)
        {
            ostream << "| ";
            for (int j = 0; j < this->buildings[i].size() - 1; j++)
            {
                // Need to increase spaces so we can fit the doubledigit numbers
                int neededSpaces = (j > 7) ? 4 : 3;
                std::string spacesString(neededSpaces, ' ');
                ostream << this->buildings[i][j].getLabel() << spacesString;
            }
            /* Print the last element without the extra space
            | 0   0   0   0   0   0 | 1 |
                                   ^
                                   no extra space here
            */
            ostream << this->buildings[i][buildings[i].size() - 1].getLabel() << "";

            // If i is 10 or over, the formattng is broken, so we just "fix" it this way and
            // hope noone wants a buildingspace > 99
            std::string delim = i + 1 >= 10 ? "|" : " |";
            ostream << " | " << (i + 1) << delim << std::endl;

            /* print the line between each row only if its not the last row
            | 0   0   0   0   0   0 | 8 |
            |                       |---| < this one
            | 0   0   0   0   0   0 | 9 |
            We need (3 * i_singledigit + 4 * i_doubledigit) + (i - 1) spaces
            */
            int totalLength = 0;
            if (this->buildings[i].size() > 9)
            {
                // If there are double digit numbers, there will always be 9 single digits one and n - 9 double digit ones
                int amountDoubleDigits = this->buildings[i].size() - 9;
                totalLength = (3 * 9 + 4 * amountDoubleDigits) + (this->buildings[i].size() - 1);
            }
            else
            {
                totalLength = 3 * this->buildings[i].size() + (this->buildings[i].size() - 1);
            }
            std::string spaces(totalLength, ' ');
            std::string spaceString = "|" + spaces + "|---|";
            if (i != this->buildings.size() - 1)
                ostream << spaceString << std::endl;
        }

        //+-----------------------+
        this->printLine(ostream, "---+");
    }

    std::string getCurrentInjectString(int i)
    {
        const char *injectionText =
            R"""(
+-----------------+
| Gebäude:        |
|                 |
|   5x H  6x S    |   +-------------------+
|                 |	 | Preise:           |
|                 |   |                   |
+-----------------+   |  5x H    5$ | 25$ |
                      |  6x S    8$ | 48$ |
                      |  0x W   10$ |     |
+-----------------+   |             |     |
| Materialien:    |   |-------------+-----|
|                 |   |             |  73$|
|   20x Metal     |   +-------------+-----+
|   10x Holz      |
|                 |
|                 |
+-----------------+
        )""";
        // Split the string by \n
        auto result = std::vector<std::string>{};
        auto ss = std::stringstream{injectionText};

        for (std::string line; std::getline(ss, line, '\n');)
            result.push_back(line);

        return result.at(i);
    }

public:
    CapycitySim(int h, int w)
    {
        // Resize the buildings vector to the given h/w
        this->buildings.resize(h);

        for (int i = 0; i < h; i++)
        {
            this->buildings[i].resize(w);
        }

        // Fill the array with the default value
        for (int i = 0; i < this->buildings.size(); i++)
        {
            for (int j = 0; j < this->buildings[0].size(); j++)
            {
                this->buildings[i][j] = EmptyBuilding();
            }
        }
    }

    std::vector<Building> &getBuildingTypes() { return this->buildingTypes; }

    Building getBuilding(int x, int y)
    {
        // Check if x or y are out of bounds
        if (!this->inBounds(x, y))
        {
            std::cout << "[!] Invalid x or y" << std::endl;
            return ErrorBuilding();
        }
        return this->buildings[x][y];
    }

    void setBuilding(int x, int y, Building building)
    {
        // Check if the x or y are out of bounds
        if (!this->inBounds(x, y))
        {
            std::cout << "[!] Invalid x or y" << std::endl;
            return;
        }
        // Check if the building is already at this location
        if (this->buildings[x][y].getFullLabel() == building.getFullLabel())
        {
            std::cout << "[!] The building " << building.getFullLabel() << " is already at " << x << " " << y << std::endl;
            return;
        }

        // Check if there is already a building
        // We need to check if the building is empty first, because if we pass a EMPTY building as
        // the parameter, we want to delete that building so we dont care about if there is a building or not
        if (building.getFullLabel() != EmptyBuilding().getFullLabel() && this->buildings[x][y].getFullLabel() != EmptyBuilding().getFullLabel())
        {
            std::cout << "[!] There is already a building at " << x << " " << y << std::endl;
            return;
        }

        this->buildings[x][y] = building;

        // Add 1 back to the x and y because we want to print the coordinates as normal humans would
        // If the building is a "empty" building, we are not placing a building we are deleting one
        if (building.getLabel() != "0")
        {
            std::cout << "[*] Placed building " << building.getFullLabel() << " at " << (y + 1) << "x" << (x + 1) << std::endl;
        }
        else
        {
            std::cout << "[*] Removed building from " << (y + 1) << "x" << (x + 1) << std::endl;
        }
    }

    void printAllBuildingTypes()
    {
        // Print all the possible buildings (EMPTY excluded)
        std::cout << "[*] Possible Buildings:" << std::endl;
        for (int i = 0; i < this->buildingTypes.size(); i++)
        {
            std::cout << " " << i << ": " << buildingTypes[i].getFullLabel() << std::endl;
        }
    }

    void printInfo()
    {

        std::stringstream stringStream;

        this->getBoardInfo(stringStream);

        // If this string is found in the currentString we will start injection the info boxes
        int currentInjectCounter = 0;
        bool injected = false;
        std::string entryPoint = "+---+";
        int spaceAmount = 5;

        std::string currentString;
        while (std::getline(stringStream, currentString))
        {
            // If we have already started injecting, don't try again
            if (injected || currentString.find(entryPoint) != std::string::npos)
            {
                injected = true;
                std::string injectString(spaceAmount, ' ');
                currentString += injectString + this->getCurrentInjectString(currentInjectCounter);
                currentInjectCounter++;
            }
            std::cout << currentString << std::endl;
        }
    }
};

#endif
