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
    
    parsedLines.clear();
    std::string lineString;
    while (std::getline(stream, lineString))
    {
        if(lineString.length() <= 1){
            break;
        }

        Line lineParsed;
        lineParsed.firstChar = lineString[0];
        std::istringstream numberStream(lineString.substr(1));
        if (!(numberStream >> lineParsed.number)){
            std::cerr << "Could not parse line " << lineString << std::endl;
            return 1;
        }
        parsedLines.push_back(lineParsed);
    }

    return 0;
}


const std::vector<Line>& FileParser::getLines() const{
    return parsedLines;
}
