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


void readCustomFile() {
    cout << "\n---- Reading .filey file ----" << endl;
    
    std::ifstream file("map.filey", std::ios::binary);
    
    if (!file) {
        std::cerr << "Error: Could not open map.filey" << endl;
        return;
    }
    
    // Read header and store it in rHeader
    fileHeader rHeader;
    file.read(reinterpret_cast<char*>(&rHeader), sizeof(rHeader));
    
    // Verify signature
    if (std::memcmp(rHeader.signature, "FILEY1", 6) != 0) {
        cerr << "Error: Invalid file signature! Is the file a .filey file?" << endl;
        return;
    }

    // Read graph metadata
    graphHeader gHeader;
    file.read(reinterpret_cast<char*>(&gHeader), sizeof(gHeader));

    cout << "Signature: " << std::string(rHeader.signature, 6) << endl;
    cout << "Data size: " << rHeader.data_size << " bytes" << endl;
    cout << "Number of nodes: " << gHeader.num_nodes << endl;
    cout << "Number of edges: " << gHeader.num_edges << endl;
    
    // Read data from the file and store it in graph wise
    graphNode* nodes = new graphNode[gHeader.num_nodes];
    file.read(reinterpret_cast<char*>(nodes), sizeof(graphNode) * gHeader.num_nodes);
    graphEdge* edges = new graphEdge[gHeader.num_edges];
    file.read(reinterpret_cast<char*>(edges), sizeof(graphEdge) * gHeader.num_edges);

    file.close(); // Close the file after reading
    
    // Display read data
    cout << "\n--- Nodes ---" << endl;
    for(int i=0; i<gHeader.num_nodes; i++){
        graphNode node;
        node = nodes[i];
        cout << "ID: " << node.id << "[" << node.x <<", " << node.y << "]" << "\nText: " << node.label << endl;
    }

    cout << "\n--- Connections ---" << endl;
    for(int i=0; i<gHeader.num_edges; i++){
        graphEdge edge;
        edge = edges[i];
        cout<< "EDGE:Node" << edge.from_id << "->Node" << edge.to_id << endl;
    }

    delete[] nodes;
    delete[] edges;
}

int main() {
    readCustomFile();
    cout << "\n✓ Read test.filey\n";
    return 0;
}
