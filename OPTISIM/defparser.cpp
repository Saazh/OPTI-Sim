#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <stdexcept>

struct CellStruct
{
    std::string CellID;
    std::string CellType;
    float X_Loc;
    float Y_Loc;
    std::string CellOrientation;
};
struct DieStruct
{
    std::string DesignName;
    float DieXSize;
    float DieYSize;
    float DesignUnit;
} DieDetail;
CellStruct Vector2Struct(std::vector<std::string> EachLine)
{
    CellStruct cells;
    int offset = 0;
    if (EachLine.size() == 13)
    {
        offset = 3;
    } // This if statment is for the logic cells
    else if (EachLine.size() == 10)
    {
        offset = 0;
    } // This if statement is for the fillers/endcaps/tapcells
    else
    {
        std::cout << "provide the correct length of word for the cell definition of *.DEF file";
        throw std::invalid_argument("check the length of each cell line definition in the *.DEF file; it is not logic cell or filler/endcap/tapcell cell");
    }

    cells.CellID = EachLine[1];
    cells.CellType = EachLine[2];
    cells.X_Loc = std::stof(EachLine[6 + offset]);
    cells.Y_Loc = std::stof(EachLine[7 + offset]);
    cells.CellOrientation = EachLine[9 + offset];

    return cells;
};

int main()
{
    std::ifstream file("../asset/design.def");

    if (!file)
    {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::string line;
    int componnents_flag = 0;
    int design_name_flag = 0;
    // std::map<std::string, std::vector<std::string>> map_gate;
    // std::map<std::string, std::vector<std::string>> map_misc;

    std::map<std::string, CellStruct> MapCells;
    std::map<std::string, DieStruct> DieInfo;

    while (std::getline(file, line))
    {

        if (!line.empty())
        {
            std::stringstream lineSstrm(line);
            std::vector<std::string> lineVect;
            std::string word;
            while (lineSstrm >> word)
            {
                lineVect.push_back(word);
            }
            // extract the name
            if (lineVect[0] == "DESIGN" && lineVect.size() > 0 && design_name_flag == 0)
            {
                DieDetail.DesignName = lineVect[1];
                design_name_flag = 1;
            }
            // end of extracting the name

            // retriving the DieArea size
            if (lineVect[0] == "DIEAREA" && lineVect.size() > 0)
            {
                DieDetail.DieXSize = std::stof(lineVect[6]);
                DieDetail.DieYSize = std::stof(lineVect[7]);
            }
            // retrieving the Die size Unit
            if (lineVect[0] == "UNITS" && lineVect.size() > 0)
            {
                DieDetail.DesignUnit = std::stof(lineVect[3]);
            }

            // extract the name of the gates, and location
            if (lineVect[0] == "COMPONENTS" && lineVect.size() > 1)
                componnents_flag = 1;
            if (lineVect[0] == "END" && lineVect[1] == "COMPONENTS")
                componnents_flag = 0;
            if (componnents_flag == 1 && lineVect[0] != "COMPONENTS")
            {

                if (lineVect.size() == 10)
                {
                    // push the gate related details to the MapCells
                    MapCells[lineVect[1]] = Vector2Struct(lineVect);
                }

                if (lineVect.size() == 13)
                {
                    // push the miscellanious cells (fillers, endcaps, tapwells) to the the MapCells
                    MapCells[lineVect[1]] = Vector2Struct(lineVect);
                }
            }
        }
    }
    DieInfo["INFO"] = DieDetail;

    file.close(); // Close the file                                                                                                                                                                                                                                                                                                                // Uncomment for the testing


    return 0;
}