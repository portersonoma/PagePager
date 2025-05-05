#include "page-table.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

PageTable::PageTable(const std::string& filename, bool clockMode = false) {
    useClockAlgorithm = clockMode;
    loadPageTable(filename);
}

int PageTable::runClockAlgorithm() {
    size_t n = entries.size();

    while (true) {
        for (size_t i = 0; i < n; ++i) {
            size_t index = (clockHand + i) % n;
            PageTableEntry& e = entries[index];

            if (e.valid) {
                if (!e.used) {
                    // Choose this one to evict
                    clockHand = (index + 1) % n;
                    return e.frameNumber;
                } else {
                    // Give second chance
                    e.used = false;
                }
            }
        }
    }
}


void PageTable::loadPageTable(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    std::string line;

    // FIRST: read system description line
    while (std::getline(infile, line)) {
        if (line.empty()) continue; // skip blank lines
        std::istringstream iss(line);
        iss >> numVirtualBits >> numPhysicalBits >> pageSize;
        pageOffsetBits = static_cast<int>(std::log2(pageSize));
        pageMask = (1 << pageOffsetBits) - 1;
        break; // done reading system info
    }

    // THEN: read actual page table entries
    while (std::getline(infile, line)) {
        if (line.empty()) continue; // skip blank lines

        std::istringstream iss(line);
        char v, u;
        int p, f;
        iss >> v >> p >> f >> u;

        PageTableEntry entry;
        entry.valid = (v == '1');
        entry.permissions = p;
        entry.frameNumber = f;
        entry.used = (u == '1');
        entries.push_back(entry);
    }
}


int PageTable::translate(unsigned int virtualAddress, bool& pageFault, bool& segFault, bool& onDisk) {
    unsigned int pageNumber = virtualAddress >> pageOffsetBits;
    unsigned int offset = virtualAddress & pageMask;

    if (pageNumber >= entries.size()) {
        segFault = true;
        return -1;
    }

    PageTableEntry& entry = entries[pageNumber];

    if (!entry.valid) {
        if (entry.permissions == 0) {
            segFault = true;
        } else {
            if (useClockAlgorithm) {
                pageFault = true;
                // Do page replacement using Clock
                int frameToUse = runClockAlgorithm();
    
                // Find the virtual page currently in that frame and mark it invalid
                for (auto& e : entries) {
                    if (e.valid && e.frameNumber == frameToUse) {
                        e.valid = false;
                        e.used = false;
                        break;
                    }
                }
    
                // Load the new page into the frame
                entry.valid = true;
                entry.frameNumber = frameToUse;
                entry.used = true;
    
                // Now translation can continue!
                segFault = false;
                onDisk = false;
    
                return (entry.frameNumber << pageOffsetBits) | offset;
            } else {
                onDisk = true;
            }
        }
        return -1;
    }
    

    pageFault = false;
    segFault = false;
    onDisk = false;
    return (entry.frameNumber << pageOffsetBits) | offset;
}
