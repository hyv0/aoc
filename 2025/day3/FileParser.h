#pragma once

#include <string>
#include <vector>


typedef struct {
    char digits[101];

}Line;

class FileParser{
    public:
    FileParser(std::string fileName);
    std::string getFileName();
    int readFile();
    const std::vector<Line>& getLines() const;

    private:
    std::string fileName;
    std::vector<Line> parsedLines;

};
