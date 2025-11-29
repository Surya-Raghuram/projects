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

// Data stored in the .filey file
struct fileData {
    int number;
    float value;
    char text[20];
};

void readCustomFile() {
    cout << "\n---- Reading .filey file ----" << endl;
    
    std::ifstream file("test.filey", std::ios::binary);
    
    if (!file) {
        std::cerr << "Error: Could not open test.filey" << endl;
        return;
    }
    
    // Read header and store it in rHeader
    fileHeader rHeader;
    file.read(reinterpret_cast<char*>(&rHeader), sizeof(rHeader));
    
    // Verify signature
    if (std::memcmp(rHeader.signature, "FILEY0", 6) != 0) {
        cerr << "Error: Invalid file signature! Is the file a .filey file?" << endl;
        return;
    }
    
    cout << "Signature: " << std::string(rHeader.signature, 6) << endl;
    cout << "Data size: " << rHeader.data_size << " bytes" << endl;
    
    // Read data from the file and store it in rData
    fileData rData;
    file.read(reinterpret_cast<char*>(&rData), sizeof(rData));
    
    cout << "Number: " << rData.number << endl;
    cout << "Value: " << rData.value << endl;
    cout << "Text: " << rData.text << endl;
    
    file.close();
}

int main() {
    readCustomFile();
    cout << "\n✓ Read test.filey\n";
    return 0;
}
