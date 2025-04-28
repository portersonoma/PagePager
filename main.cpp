#include "page-table.hpp"
#include <iostream>
#include <sstream>

unsigned int parseAddress(const std::string& input) {
    unsigned int addr;
    if (input.find("0x") == 0 || input.find("0X") == 0) {
        std::stringstream ss;
        ss << std::hex << input;
        ss >> addr;
    } else {
        addr = std::stoul(input);
    }
    return addr;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <page_table_file>" << std::endl;
        return 1;
    }

    PageTable pt(argv[1]);

    std::string input;
    while (std::getline(std::cin, input)) {
        if (input.empty()) continue;

        unsigned int virtualAddress = parseAddress(input);

        bool pageFault = false, segFault = false, onDisk = false;
        int physAddr = pt.translate(virtualAddress, pageFault, segFault, onDisk);

        if (segFault) {
            std::cout << "SEGFAULT" << std::endl;
        } else if (onDisk) {
            std::cout << "DISK" << std::endl;
        } else {
            std::cout << std::hex << "0x" << physAddr << std::dec << std::endl;
        }
    }

    return 0;
}
