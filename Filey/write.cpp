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

void writeCustomFile() {
    cout << "---- Writing .filey file ----" << endl;
    
    // Open file with custom extension
    std::ofstream file("test.filey", std::ios::binary);
    
    // Write header
    fileHeader header;
    std::memcpy(header.signature, "FILEY0", 6); // The signature is "FILEY0"
    header.data_size = sizeof(fileData); // The size of the file is copied to the header
    file.write(reinterpret_cast<char*>(&header), sizeof(header));
    
    // Write data to the file
    fileData data;

    // These values are arbitrary for demonstration
    data.number = 42;
    data.value = 3.14159f;
    std::strcpy(data.text, "Hello World");

    file.write(reinterpret_cast<char*>(&data), sizeof(data));
    
    file.close();
    cout << "Written test.filey successfully!" << endl;
}

int main() {
    writeCustomFile();
    return 0;
}
