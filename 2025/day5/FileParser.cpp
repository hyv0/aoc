#include <fstream>
#include <iostream>
#include <sstream>
#include "FileParser.h"

FileParser::FileParser(std::string fileName){
    this->fileName = fileName;
}

std::string FileParser::getFileName(){
    return fileName;
}

int FileParser::readFile(){
    std::ifstream stream(fileName);

    if(!stream){
        std::cerr << "File could not be opened " << fileName << std::endl; 
        return -1;
    }
    
    ranges.clear();
    ingredientIDs.clear();


    std::string lineString;
    while (std::getline(stream, lineString))
    {
        if(lineString.length() <= 1){
            continue;
        }

        const size_t position = lineString.find("-");

        if(position != std::string::npos){

            Range range;
            std::istringstream numberStream1(lineString.substr(0,position));
            std::istringstream numberStream2(lineString.substr(position+1));

            if (!(numberStream1 >> range.startNr)) {
                std::cerr << "Could not parse first part of " << lineString << std::endl;
                return 1;
            }
            if (!(numberStream2 >> range.endNr)) {
                std::cerr << "Could not parse second part of " << lineString << std::endl;
                return 1;
            }
            ranges.push_back(range);
        }else{
            std::istringstream numberStream(lineString);
            int64_t number;
            if (!(numberStream >> number)) {
                std::cerr << "Could not parse " << lineString << std::endl;
                return 1;
            }
            ingredientIDs.push_back(number);

        }

    }

    return 0;
}


const std::vector<Range>& FileParser::getRanges() const{
    return ranges;
}
const std::vector<int64_t>& FileParser::getIDs() const{
    return ingredientIDs;
}

