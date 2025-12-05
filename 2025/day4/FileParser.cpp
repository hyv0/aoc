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
    std::ifstream file(fileName);
    

    if(!file){
        std::cerr << "File could not be opened " << fileName << std::endl; 
        return -1;
    }


    char c;
    int i = 0;
    int j = 0;
    int highestJ = 0;
    map.height = 0;
    while(file.get(c)) {
        if(c=='\n'){
            if(highestJ == 0){
                highestJ = j;
            }else{
                if(j != highestJ) {
                    std::cerr << "Line " << i << " has a different length!" << std::endl;
                }
            }
            i++;
            j = 0;
        }else{
            map.arr[i][j] = c;
            if(j == 0) {
                map.height ++;
            }
            j++;
        }
    }
    map.width = highestJ;
    std::cout << "width = " << map.width << " height = " << map.height << std::endl;
    return 0;
}


const Map& FileParser::getMap() const {
    return map;
}
