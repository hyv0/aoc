#pragma once

#include <string>
#include <vector>
#include <cstdint>


typedef struct {
    uint64_t startNr;
    uint64_t endNr;
}Range;

class FileParser{
    public:
    FileParser(std::string fileName);
    std::string getFileName();
    int readFile();
    const std::vector<Range>& getRanges() const;

    private:
    std::string fileName;
    std::vector<Range> parsed;

};
