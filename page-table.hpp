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
    PageTable(const std::string& filename);

    int translate(unsigned int virtualAddress, bool& pageFault, bool& segFault, bool& onDisk);
    void printEntry(size_t index) const;

private:
    std::vector<PageTableEntry> entries;
    int numVirtualBits;
    int numPhysicalBits;
    int pageSize;
    int pageOffsetBits;
    unsigned int pageMask;

    void loadPageTable(const std::string& filename);
};

#endif
