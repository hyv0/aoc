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
    
    parsed.clear();
    std::string lineString;
    while (std::getline(stream, lineString))
    {
        if(lineString.length() <= 1){
            break;
        }

        std::string rangeString;
        std::stringstream stream (lineString);

        while (getline(stream, rangeString, ',')){

            Range range;
            int position = rangeString.find("-");

            std::istringstream numberStream1(rangeString.substr(0,position));
            std::istringstream numberStream2(rangeString.substr(position+1));

            if (!(numberStream1 >> range.startNr)) {
                std::cerr << "Could not parse first part of " << rangeString << std::endl;
                return 1;
            }
            if (!(numberStream2 >> range.endNr)) {
                std::cerr << "Could not parse second part of " << rangeString << std::endl;
                return 1;
            }

            parsed.push_back(range);

        }
    }

    return 0;
}


const std::vector<Range>& FileParser::getRanges() const{
    return parsed;
}
