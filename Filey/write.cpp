#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>

#include "filey_data.h"

using namespace std;

void writeCustomFile() {
    cout << "--- Writing .filey file ---" << endl;
    
    // Open file with custom extension
    std::ofstream file("map.filey", std::ios::binary);
    
    // Make custom data to write into the file
    graphNode nodes[4] = {
        {0, 0.0f, 0.0f, "start"},
        {1, 1.0f, 1.0f, "top"},
        {2, 1.0f, -1.0f, "bottom"},
        {3, 1.0f, 0.0f, "middle"}
    };
    graphEdge edges[3] = {
        {0, 1},
        {0, 2},
        {0, 3}
    };

    // Custom header
    graphHeader gHeader;
    gHeader.num_nodes = 4;
    gHeader.num_edges = 3;
    // Write to file header
    fileHeader header;
    std::memcpy(header.signature, FILEY_SIGNATURE, 6); // Write signature from shared data
    header.data_size = sizeof(graphHeader) + sizeof(graphNode) * gHeader.num_nodes + sizeof(graphEdge) * gHeader.num_edges; // The size of the file is copied to the header

    file.write(reinterpret_cast<char*>(&header), sizeof(header)); // Write file header
    file.write(reinterpret_cast<char*>(&gHeader), sizeof(gHeader)); // Write graph header
    file.write(reinterpret_cast<char*>(nodes), sizeof(nodes)); // Write nodes
    file.write(reinterpret_cast<char*>(edges), sizeof(edges)); // Write edges

    
    file.close();
    cout << "Written map.filey successfully!" << endl;
}

int main() {
    writeCustomFile();
    return 0;
}
