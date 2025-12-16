#include "raylib.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>

#include "filey_data.h"

using namespace std;

int main(){

    // Load the data from map.filey
    ifstream file("map.filey", std::ios::binary);
    if(!file){
        cerr << "Could not open file, File doesn't exist";
        return -1;
    }

    // Get file header from file
    fileHeader fHeader;
    file.read(reinterpret_cast<char*>(&fHeader), sizeof(fHeader));
    // Check signature of file Header
    if(memcmp(fHeader.signature, FILEY_SIGNATURE, 6) != 0){
        cerr << "File has invalid signature!";
        return 1;
    }

    // Get graph header from file
    graphHeader gHeader;
    file.read(reinterpret_cast<char*>(&gHeader), sizeof(gHeader));

    // Get file data into vectors
    vector<graphNode> nodes(gHeader.num_nodes);
    file.read(reinterpret_cast<char*>(nodes.data()), sizeof(graphNode) * gHeader.num_nodes);

    vector<graphEdge> edges(gHeader.num_edges);
    file.read(reinterpret_cast<char*>(edges.data()), sizeof(graphEdge) * gHeader.num_edges);
    file.close();

    // Visualize the file data
    InitWindow(800, 600, "map.Filey");
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(GRAY);

        const float SCALE = 150.0f;
        const float OFFSET_X = 400.0f;
        const float OFFSET_Y = 300.0f;

        for(const auto& edge: edges){
            if(edge.from_id >= nodes.size() || edge.to_id >= nodes.size())
                continue;

            float x1 = nodes[edge.from_id].x * SCALE + OFFSET_X;
            float y1 = OFFSET_Y - (nodes[edge.from_id].y * SCALE);
            float x2 = nodes[edge.to_id].x * SCALE + OFFSET_X;
            float y2 = OFFSET_Y - (nodes[edge.to_id].y * SCALE);

            DrawLineEx((Vector2){x1,y1}, (Vector2){x2,y2}, 2.0f, WHITE);
        }
        for(const auto& node: nodes){
            float transparency = 0.5f;
            Color transRed = Fade(RED, transparency);
            DrawCircle(node.x* SCALE + OFFSET_X, OFFSET_Y - node.y*SCALE, 150, transRed);
            DrawText(node.label, node.x*SCALE + OFFSET_X, OFFSET_Y - node.y * SCALE, 50, WHITE);
        }

        EndDrawing();
    }
    return 0;
}