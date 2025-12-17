#include "raylib.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>
#include <unordered_map>

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

    // Build an ID -> index map so edges can reference nodes by id (not index)
    std::unordered_map<int32_t, size_t> idToIndex;
    idToIndex.reserve(nodes.size());
    for (size_t i = 0; i < nodes.size(); ++i) {
        idToIndex[nodes[i].id] = i;
    }

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(GRAY);

        const float SCALE = 150.0f;
        const float OFFSET_X = 400.0f;
        const float OFFSET_Y = 300.0f;

        // 1) Draw circles first so they sit behind text
        for(const auto& node: nodes){
            float transparency = 0.5f;
            Color transRed = Fade(RED, transparency);
            DrawCircle(node.x * SCALE + OFFSET_X, OFFSET_Y - node.y * SCALE, 50, transRed);
        }

        // 2) Draw edges using id->index mapping so all edges show up
        for(const auto& edge: edges){
            auto itFrom = idToIndex.find(edge.from_id);
            auto itTo   = idToIndex.find(edge.to_id);
            if (itFrom == idToIndex.end() || itTo == idToIndex.end()) continue;

            const auto& n1 = nodes[itFrom->second];
            const auto& n2 = nodes[itTo->second];

            float x1 = n1.x * SCALE + OFFSET_X;
            float y1 = OFFSET_Y - (n1.y * SCALE);
            float x2 = n2.x * SCALE + OFFSET_X;
            float y2 = OFFSET_Y - (n2.y * SCALE);

            DrawLineEx((Vector2){x1,y1}, (Vector2){x2,y2}, 2.0f, WHITE);
        }

        // 3) Draw text last with a subtle shadow/box for contrast
        for(const auto& node: nodes){
            int fontSize = 50;
            const char* text = node.label;
            int textW = MeasureText(text, fontSize);
            int textH = fontSize;

            float tx = node.x * SCALE + OFFSET_X;
            float ty = OFFSET_Y - node.y * SCALE;

            // Background box with transparency for contrast
            Color bg = Fade(BLACK, 0.5f);
            DrawRectangle(tx - 4, ty - 4, textW + 8, textH + 8, bg);

            // Soft shadow
            DrawText(text, (int)(tx + 2), (int)(ty + 2), fontSize, BLACK);
            // Foreground text
            DrawText(text, (int)tx, (int)ty, fontSize, WHITE);
        }

        EndDrawing();
    }
    return 0;
}