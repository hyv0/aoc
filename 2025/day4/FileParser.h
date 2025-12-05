#pragma once

#include <string>


typedef struct {
    int width;
    int height;
    char arr[150][150];
}Map;

class FileParser{
    public:
    FileParser(std::string fileName);
    std::string getFileName();
    int readFile();
    const Map& getMap() const;

    private:
    std::string fileName;
    Map map;

};
