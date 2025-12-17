// Correct header guard
#ifndef FILEY_DATA_H
#define FILEY_DATA_H

#include <cstdint>

// File signature
const char FILEY_SIGNATURE[6] = {'F', 'I', 'L', 'E', 'Y', '2'};

// Header for the file
struct fileHeader {
    char signature[6];
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
    // int32_t weight;
    int32_t from_id;
    int32_t to_id;
};

#endif