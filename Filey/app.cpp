#include "raylib.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cmath>

#include "filey_data.h"

using namespace std;

// Save graph data to file (Default updated to test1.filey)
void saveGraphToFile(const vector<graphNode>& nodes, const vector<graphEdge>& edges, const char* filename = "test1.filey") {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error: Could not open file for writing!" << endl;
        return;
    }

    // Write file header
    fileHeader header;
    memcpy(header.signature, FILEY_SIGNATURE, 6);
    graphHeader gHeader;
    gHeader.num_nodes = nodes.size();
    gHeader.num_edges = edges.size();
    header.data_size = sizeof(graphHeader) + sizeof(graphNode) * gHeader.num_nodes + sizeof(graphEdge) * gHeader.num_edges;
    
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    file.write(reinterpret_cast<const char*>(&gHeader), sizeof(gHeader));
    file.write(reinterpret_cast<const char*>(nodes.data()), sizeof(graphNode) * nodes.size());
    file.write(reinterpret_cast<const char*>(edges.data()), sizeof(graphEdge) * edges.size());
    
    file.close();
    cout << "Saved " << nodes.size() << " nodes and " << edges.size() << " edges to " << filename << endl;
}

// Load graph data from file (Default updated to test1.filey)
bool loadGraphFromFile(vector<graphNode>& nodes, vector<graphEdge>& edges, const char* filename = "test1.filey") {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "No existing file found (" << filename << "), starting with empty graph." << endl;
        return false;
    }

    fileHeader fHeader;
    file.read(reinterpret_cast<char*>(&fHeader), sizeof(fHeader));
    if (memcmp(fHeader.signature, FILEY_SIGNATURE, 6) != 0) {
        cerr << "Error: Invalid file signature!" << endl;
        return false;
    }

    graphHeader gHeader;
    file.read(reinterpret_cast<char*>(&gHeader), sizeof(gHeader));

    nodes.resize(gHeader.num_nodes);
    file.read(reinterpret_cast<char*>(nodes.data()), sizeof(graphNode) * gHeader.num_nodes);

    edges.resize(gHeader.num_edges);
    file.read(reinterpret_cast<char*>(edges.data()), sizeof(graphEdge) * gHeader.num_edges);
    
    file.close();
    cout << "Loaded " << nodes.size() << " nodes and " << edges.size() << " edges from " << filename << endl;
    return true;
}

int main(){
    vector<graphNode> nodes;
    vector<graphEdge> edges;

    // Try to load existing file, if it doesn't exist start with empty graph
    loadGraphFromFile(nodes, edges);

    // Interactive graph editor
    InitWindow(800, 600, "Filey - Graph Viewer");
    SetTargetFPS(60);

    int nextNodeId = 0;
    // Find the max existing ID to ensure we don't duplicate IDs
    for (const auto& node : nodes) {
        if (node.id >= nextNodeId) nextNodeId = node.id + 1;
    }

    int selectedNodeIndex = -1;  // For creating edges
    int hoveredNodeIndex = -1;
    
    // Dialog state
    bool showSaveDialog = false;
    char filenameInput[256] = "";
    int letterCount = 0;

    while(!WindowShouldClose()){
        // Index map so edges can reference nodes by id (not index)
        std::unordered_map<int32_t, size_t> idToIndex;
        idToIndex.reserve(nodes.size());
        for (size_t i = 0; i < nodes.size(); ++i) {
            idToIndex[nodes[i].id] = i;
        }

        const float SCALE = 150.0f;
        const float OFFSET_X = 400.0f;
        const float OFFSET_Y = 300.0f;
        const float NODE_RADIUS = 50.0f;

        // Handle mouse input
        Vector2 mousePos = GetMousePosition();
        
        // If save dialog is shown, handle text input
        if (showSaveDialog) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (letterCount < 250)) {
                    filenameInput[letterCount] = (char)key;
                    letterCount++;
                }
                key = GetCharPressed();
            }
            
            if (IsKeyPressed(KEY_BACKSPACE)) {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                filenameInput[letterCount] = '\0';
            }
            
            if (IsKeyPressed(KEY_ENTER)) {
                if (letterCount > 0) {
                    char fullFilename[300];
                    snprintf(fullFilename, sizeof(fullFilename), "%s.filey", filenameInput);
                    saveGraphToFile(nodes, edges, fullFilename);
                }
                showSaveDialog = false;
            }
            
            if (IsKeyPressed(KEY_ESCAPE)) {
                showSaveDialog = false;
            }
        } else {
            // Normal mouse handling when dialog is not shown
            
        // Handle keyboard input (controls: S = Save, C = Clear) - only when dialog is not shown
        if (IsKeyPressed(KEY_S)) {
            showSaveDialog = true;
            letterCount = 0;
            memset(filenameInput, 0, sizeof(filenameInput));
            // Clear any pending character input to prevent 's' from being captured
            while (GetCharPressed() > 0);
        }
        if (IsKeyPressed(KEY_C)) {
            nodes.clear();
            edges.clear();
            selectedNodeIndex = -1;
            nextNodeId = 0;
        }
        
        // Check hover over nodes
        hoveredNodeIndex = -1;
        for (size_t i = 0; i < nodes.size(); ++i) {
            float nx = nodes[i].x * SCALE + OFFSET_X;
            float ny = OFFSET_Y - nodes[i].y * SCALE;
            float dist = sqrt((mousePos.x - nx) * (mousePos.x - nx) + (mousePos.y - ny) * (mousePos.y - ny));
            if (dist <= NODE_RADIUS) {
                hoveredNodeIndex = i;
                break;
            }
        }

        // Left click: Add node or select for edge creation
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (hoveredNodeIndex >= 0) {
                // Clicked on existing node - select for edge creation
                if (selectedNodeIndex == -1) {
                    selectedNodeIndex = hoveredNodeIndex;
                } else if (selectedNodeIndex != hoveredNodeIndex) {
                    // Create edge between selected and clicked node
                    graphEdge newEdge;
                    newEdge.from_id = nodes[selectedNodeIndex].id;
                    newEdge.to_id = nodes[hoveredNodeIndex].id;
                    
                    // Check if edge already exists to prevent duplicates
                    bool exists = false;
                    for (const auto& e : edges) {
                        if ((e.from_id == newEdge.from_id && e.to_id == newEdge.to_id) ||
                            (e.from_id == newEdge.to_id && e.to_id == newEdge.from_id)) {
                            exists = true;
                            break;
                        }
                    }
                    
                    if (!exists) {
                        edges.push_back(newEdge);
                    }
                    selectedNodeIndex = -1;
                } else {
                    selectedNodeIndex = -1; // Diselect if clicked same node twice
                }
            } else {
                // Clicked on empty space - create new node
                graphNode newNode;
                newNode.id = nextNodeId++;
                newNode.x = (mousePos.x - OFFSET_X) / SCALE;
                newNode.y = (OFFSET_Y - mousePos.y) / SCALE;
                snprintf(newNode.label, sizeof(newNode.label), "node%d", newNode.id);
                nodes.push_back(newNode);
                selectedNodeIndex = -1;
            }
        }

        // Right click: Delete node or edge
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            if (hoveredNodeIndex >= 0) {
                int32_t deletedId = nodes[hoveredNodeIndex].id;
                nodes.erase(nodes.begin() + hoveredNodeIndex);
                
                // Remove all edges connected to this node
                edges.erase(
                    std::remove_if(edges.begin(), edges.end(),
                        [deletedId](const graphEdge& e) {
                            return e.from_id == deletedId || e.to_id == deletedId;
                        }),
                    edges.end()
                );
                
                selectedNodeIndex = -1;
            }
        }
        } // End of normal input handling

        BeginDrawing();
        ClearBackground(GRAY);

        // 1) Draw circles first so they sit behind text
        for(size_t i = 0; i < nodes.size(); ++i){
            const auto& node = nodes[i];
            float transparency = 0.5f;
            Color nodeColor = Fade(RED, transparency);
            
            // Highlight hovered node
            if ((int)i == hoveredNodeIndex) {
                nodeColor = Fade(ORANGE, 0.7f);
            }
            // Highlight selected node for edge creation
            if ((int)i == selectedNodeIndex) {
                nodeColor = Fade(GREEN, 0.7f);
            }
            
            DrawCircle(node.x * SCALE + OFFSET_X, OFFSET_Y - node.y * SCALE, NODE_RADIUS, nodeColor);
        }

        // 2) Draw edges using id->index mapping
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

        // 3) Draw text last
        for(const auto& node: nodes){
            int fontSize = 20;
            const char* text = node.label;
            int textW = MeasureText(text, fontSize);
            int textH = fontSize;

            float tx = node.x * SCALE + OFFSET_X - textW / 2;
            float ty = OFFSET_Y - node.y * SCALE - textH / 2;

            // Background box
            Color bg = Fade(BLACK, 0.6f);
            DrawRectangle(tx - 4, ty - 4, textW + 8, textH + 8, bg);

            // Shadow and Foreground text
            DrawText(text, (int)(tx + 1), (int)(ty + 1), fontSize, BLACK);
            DrawText(text, (int)tx, (int)ty, fontSize, WHITE);
        }

        // Draw simple status at bottom left (minimal info)
        DrawText(TextFormat("Nodes: %d  Edges: %d", nodes.size(), edges.size()), 10, 570, 20, LIGHTGRAY);

        // Draw save dialog if active
        if (showSaveDialog) {
            // Minimal semi-transparent overlay
            DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.7f));
            
            // Dialog box
            int dialogW = 400;
            int dialogH = 100;
            int dialogX = (800 - dialogW) / 2;
            int dialogY = (600 - dialogH) / 2;
            
            DrawRectangle(dialogX, dialogY, dialogW, dialogH, DARKGRAY);
            DrawRectangleLines(dialogX, dialogY, dialogW, dialogH, WHITE);
            
            // Prompt text
            DrawText("Enter filename:", dialogX + 10, dialogY + 10, 20, WHITE);
            
            // Input box
            int inputX = dialogX + 10;
            int inputY = dialogY + 40;
            int inputW = dialogW - 20;
            int inputH = 30;
            
            DrawRectangle(inputX, inputY, inputW, inputH, LIGHTGRAY);
            DrawRectangleLines(inputX, inputY, inputW, inputH, WHITE);
            
            // Show input text
            DrawText(filenameInput, inputX + 5, inputY + 5, 20, BLACK);
            DrawText(".filey", inputX + 5 + MeasureText(filenameInput, 20), inputY + 5, 20, GRAY);
            
            // Hint text
            DrawText("Press ENTER to save, ESC to cancel", dialogX + 10, dialogY + 80, 14, LIGHTGRAY);
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}