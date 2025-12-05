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
        for(unsigned int i = 0; i < lineString.length(); i++) {
            lineParsed.digits[i] = lineString[i];
        }
        lineParsed.digits[100] = '\0';

        parsedLines.push_back(lineParsed);
    }

    return 0;
}


const std::vector<Line>& FileParser::getLines() const{
    return parsedLines;
}
