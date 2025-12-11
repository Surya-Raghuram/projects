#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>

using namespace std;

// Custom file format with .filey extension
struct fileHeader {
    char signature[6];  // "FILEY0" signature
    uint32_t data_size;
};
// Metadata for the graph
struct graphHeader {
    int32_t num_nodes;
    int32_t num_edges;
};

// Data stored in the .filey file
// structure to store the text
struct graphNode {
    int32_t id;
    float x,y;
    char label[250];
};
// Structure to store the edges that connects the texts
struct graphEdge {
    int32_t from_id;
    int32_t to_id;
};


void writeCustomFile() {
    cout << "---- Writing .filey file ----" << endl;
    
    // Open file with custom extension
    std::ofstream file("map.filey", std::ios::binary);
    
    // Make custom data to write into the file
    graphNode nodes[4] = {
        {1, 0.0f, 0.0f, "start"},
        {2, 1.0f, 1.0f, "top"},
        {3, 1.0f, -1.0f, "bottom"},
        {4, 1.0f, 0.0f, "middle"}
    };
    graphEdge edges[3] = {
        {1, 2},
        {1, 3},
        {1, 4}
    };

    // Custom header
    graphHeader gHeader;
    gHeader.num_nodes = 4;
    gHeader.num_edges = 3;
    // Write to file header
    fileHeader header;
    std::memcpy(header.signature, "FILEY1", 6); // The signature is "FILEY0"
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
