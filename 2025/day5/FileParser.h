#pragma once

#include <string>
#include <vector>
#include <cstdint>


typedef struct {
    int64_t startNr;
    int64_t endNr;
}Range;

class FileParser{
    public:
    FileParser(std::string fileName);
    std::string getFileName();
    int readFile();
    const std::vector<Range>& getRanges() const;
    const std::vector<int64_t>& getIDs() const;


    private:
    std::string fileName;
    std::vector<Range> ranges;
    std::vector<int64_t> ingredientIDs;

};
