enum BUILDING
{
    ERROR = -1,
    EMPTY = 0,
    SOLARPANEL = 1,
    WINDPOWERPLANT = 2,
    HYDROELECTRICPOWERPLANTS = 3
};

const char *buildingTypes[] = {
    "Empty",
    "Solar Panel",
    "Wind Power Plant",
    "Hydroelectric Power Plant",
};

#ifndef SIMULATIONTOOL_H
#define SIMULATIONTOOL_H

class CapycitySim
{
private:
    std::vector<std::vector<BUILDING>> buildings;

    bool inBounds(int x, int y)
    {
        // Check if x or y are out of bounds
        return (x <= this->buildings.size() - 1 && x >= 0 && y <= this->buildings[0].size() - 1 && y >= 0);
    };

    // Just a helper function to print the buildings
    void printLine(std::string postfix = "")
    {
        // +-----------------------+
        std::cout << "+";
        for (int i = 0; i < this->buildings[0].size() - 1; i++)
        {
            std::cout << "----";
        }
        std::cout << "---+" << postfix << std::endl;
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
                this->buildings[i][j] = BUILDING::EMPTY;
            }
        }
    }

    BUILDING getBuilding(int x, int y)
    {
        // Check if x or y are out of bounds
        if (!this->inBounds(x, y))
        {
            std::cout << "[!] Invalid x or y" << std::endl;
            return ERROR;
        }
        return this->buildings[x][y];
    }

    void setBuilding(int x, int y, BUILDING building)
    {
        // Check if the x or y are out of bounds
        if (!this->inBounds(x, y))
        {
            std::cout << "[!] Invalid x or y" << std::endl;
            return;
        }

        // Check if the building is already at this location
        if (this->buildings[x][y] == building)
        {
            std::cout << "[!] The building " << building << " is already at " << x << " " << y << std::endl;
            return;
        }

        // Check if there is already a building
        // We need to check if the building is empty first, because if we pass a EMPTY building as
        // the parameter, we want to delete that building so we dont care about if there is a building or not
        if (building != EMPTY && this->buildings[x][y] != EMPTY)
        {
            std::cout << "[!] There is already a building at " << x << " " << y << std::endl;
            return;
        }

        this->buildings[x][y] = building;

        // Add 1 back to the x and y because we want to print the coordinates as normal humans would
        std::cout << "[*] Placed building " << building << " at " << (x + 1) << " " << (y + 1) << std::endl;
    }

    void printAllBuildingTypes()
    {
        // Print all the possible buildings (EMPTY excluded)
        std::cout << "[*] Possible Buildings:" << std::endl;
        for (int i = SOLARPANEL; i <= HYDROELECTRICPOWERPLANTS; i++)
        {
            std::cout << " " << i << ": " << buildingTypes[i] << std::endl;
        }
    }

    void printBuildings()
    {
        /*
        Print the buildings in this format
        +-----------------------+
        | 1 | 2 | 3 | 4 | 5 | 6 |
        +-----------------------+
        | 0   0   0   0   0   0 |
        | 0   0   0   0   0   0 |
        | 0   0   0   0   0   0 |
        | 0   0   0   0   0   0 |
        | 0   0   0   0   0   0 |
        | 0   0   0   0   0   0 |
        | 0   0   0   0   0   0 |
        | 0   0   0   0   0   0 |
        +-----------------------+

        +-----------------------+
        | 1 | 2 | 3 | 4 | 5 | 6 |
        +-----------------------+---+
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
        +-----------------------+---+
        */
        // +-----------------------+
        this->printLine();

        // | 1 | 2 | 3 | 4 | 5 | 6 |
        std::cout << "|";
        for (int i = 0; i < this->buildings[0].size(); i++)
        {
            std::cout << " " << (i + 1) << " |";
        }
        std::cout << std::endl;

        // +-----------------------+---+
        this->printLine("---+");

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
            std::cout << "| ";
            for (int j = 0; j < this->buildings[i].size() - 1; j++)
            {
                std::cout << this->buildings[i][j] << "   ";
            }
            /* Print the last element without the extra space
            | 0   0   0   0   0   0 |
                                   ^
                                   no extra space here
            */
            std::cout << this->buildings[i][buildings[i].size() - 1] << "";
            std::cout << " | " << i << " |" << std::endl;

            /* print the line between each row only if its not the last row
            | 0   0   0   0   0   0 |
            |                       | < this one
            | 0   0   0   0   0   0 |
            We need 3 * i + (i - 1) spaces
            */
            int totalLength = 3 * this->buildings[i].size() + (this->buildings[i].size() - 1);
            std::string spaces(totalLength, ' ');
            std::string spaceString = "|" + spaces + "|---|";
            if (i != this->buildings.size() - 1)
                std::cout << spaceString << std::endl;
        }

        //+-----------------------+
        this->printLine("---+");
    }
};

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
    std::vector<Material> necessaryMaterials;

public:
    double getBasePrice() { return this->basePrice; }
    double getTotalPrice()
    {
        double totalPrice = this->basePrice;
        for (Material &value : this->getNecessaryMaterials())
        {
            totalPrice += value.getPrice();
        }
        return totalPrice;
    }
    std::string getLabel() { return this->label; }
    std::vector<Material> &getNecessaryMaterials() { return this->necessaryMaterials; }
};

class EmptyBuilding : public Building
{
public:
    EmptyBuilding()
    {
        this->basePrice = -1;
        this->label = "[]";
    }
};

class SolarPanelBuilding : public Building
{
public:
    SolarPanelBuilding()
    {
        this->basePrice = 1;
        this->label = "S";

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

        this->necessaryMaterials.push_back(Metal());
        this->necessaryMaterials.push_back(Metal());
        this->necessaryMaterials.push_back(Plastic());
    }
};

class HydroElectricPowerPlants : public Building
{
public:
    HydroElectricPowerPlants()
    {
        this->basePrice = 4;
        this->label = "H";

        this->necessaryMaterials.push_back(Metal());
        this->necessaryMaterials.push_back(Metal());

        this->necessaryMaterials.push_back(Plastic());
        this->necessaryMaterials.push_back(Plastic());
        this->necessaryMaterials.push_back(Plastic());
    }
};

#endif
