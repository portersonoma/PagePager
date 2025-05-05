#ifndef PAGE_TABLE_HPP
#define PAGE_TABLE_HPP

#include <vector>
#include <string>

struct PageTableEntry {
    bool valid;
    int permissions;
    int frameNumber;
    bool used;
};

class PageTable {
public:
    PageTable(const std::string& filename, bool clockMode);

    int translate(unsigned int virtualAddress, bool& pageFault, bool& segFault, bool& onDisk);
    void printEntry(size_t index) const;

private:
    std::vector<PageTableEntry> entries;
    int numVirtualBits;
    int numPhysicalBits;
    int pageSize;
    int pageOffsetBits;
    unsigned int pageMask;

    int runClockAlgorithm();

    bool useClockAlgorithm = false; // default = false
    size_t clockHand = 0;           // clock pointer

    void loadPageTable(const std::string &filename);
};

#endif
