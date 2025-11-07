#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include "MarineMachine.h"

using namespace sf;

// Node structure for A* algorithm
struct Node {
    int x, y;           // Grid position
    float g;            // Cost from start to this node
    float h;            // Heuristic cost to goal
    float f;            // Total cost (g + h)
    Node* parent;       // Parent node for path reconstruction

    Node(int x, int y) : x(x), y(y), g(0), h(0), f(0), parent(nullptr) {}

    // For priority queue comparison (lower f value = higher priority)
    bool operator>(const Node& other) const {
        return f > other.f;
    }
};

class Pathfinding {
private:
    int** m_LevelArray;      // Reference to level grid
    Vector2i m_LevelSize;    // Size of the level grid
    float m_TileSize;        // Size of each tile

    // Calculate heuristic (Manhattan distance)
    float calculateHeuristic(int x1, int y1, int x2, int y2);

    // Check if a grid position is walkable
    bool isWalkable(int x, int y);

    // Get neighbors of a node (4-directional or 8-directional)
    std::vector<Node*> getNeighbors(Node* node, bool diagonal = true);

    // Reconstruct path from goal to start
    std::vector<Vector2f> reconstructPath(Node* goalNode);

public:
    Pathfinding();

    // Initialize with level data
    void setLevelData(int** levelArray, Vector2i levelSize, float tileSize);

    // Find path from start position to goal position (world coordinates)
    std::vector<Vector2f> findPath(Vector2f startPos, Vector2f goalPos);

    // Convert world position to grid coordinates
    Vector2i worldToGrid(Vector2f worldPos);

    // Convert grid coordinates to world position (center of tile)
    Vector2f gridToWorld(int gridX, int gridY);
}; 
