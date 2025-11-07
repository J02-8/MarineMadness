#include "Pathfinding.h"
#include <iostream>
#include <unordered_set>

using namespace std;

Pathfinding::Pathfinding() : m_LevelArray(nullptr), m_TileSize(50.0f) {}

void Pathfinding::setLevelData(int** levelArray, Vector2i levelSize, float tileSize) {
    m_LevelArray = levelArray;
    m_LevelSize = levelSize;
    m_TileSize = tileSize;
}

float Pathfinding::calculateHeuristic(int x1, int y1, int x2, int y2) {
    // Manhattan distance
    return static_cast<float>(abs(x1 - x2) + abs(y1 - y2));
}

bool Pathfinding::isWalkable(int x, int y) {
    // Check bounds
    if (x < 0 || x >= m_LevelSize.x || y < 0 || y >= m_LevelSize.y) {
        return false;
    }

    // Check if tile is not a wall 
    // Adjust this based on your tile system
    return m_LevelArray[y][x] == 1;
}

std::vector<Node*> Pathfinding::getNeighbors(Node* node, bool diagonal) {
    std::vector<Node*> neighbors;

    // 4-directional movement
    int dx[] = { 0, 1, 0, -1 };
    int dy[] = { -1, 0, 1, 0 };
    int numDirections = 4;

    // 8-directional movement (includes diagonals)
    if (diagonal) {
        int dxDiag[] = { 0, 1, 0, -1, 1, 1, -1, -1 };
        int dyDiag[] = { -1, 0, 1, 0, -1, 1, 1, -1 };
        numDirections = 8;

        for (int i = 0; i < numDirections; i++) {
            int newX = node->x + dxDiag[i];
            int newY = node->y + dyDiag[i];

            if (isWalkable(newX, newY)) {
                // For diagonal movement, check if adjacent tiles are walkable
                if (i >= 4) {
                    int checkX1 = node->x + dxDiag[i];
                    int checkY1 = node->y;
                    int checkX2 = node->x;
                    int checkY2 = node->y + dyDiag[i];

                    if (isWalkable(checkX1, checkY1) && isWalkable(checkX2, checkY2)) {
                        neighbors.push_back(new Node(newX, newY));
                    }
                }
                else {
                    neighbors.push_back(new Node(newX, newY));
                }
            }
        }
    }
    else {
        for (int i = 0; i < numDirections; i++) {
            int newX = node->x + dx[i];
            int newY = node->y + dy[i];

            if (isWalkable(newX, newY)) {
                neighbors.push_back(new Node(newX, newY));
            }
        }
    }

    return neighbors;
}

std::vector<Vector2f> Pathfinding::reconstructPath(Node* goalNode) {
    std::vector<Vector2f> path;
    Node* current = goalNode;

    while (current != nullptr) {
        path.push_back(gridToWorld(current->x, current->y));
        current = current->parent;
    }

    // Reverse path so it goes from start to goal
    std::reverse(path.begin(), path.end());

    return path;
}

std::vector<Vector2f> Pathfinding::findPath(Vector2f startPos, Vector2f goalPos) {
    if (m_LevelArray == nullptr) {
        return std::vector<Vector2f>(); // Empty path
    }

    Vector2i startGrid = worldToGrid(startPos);
    Vector2i goalGrid = worldToGrid(goalPos);

    // Check if start or goal is not walkable
    if (!isWalkable(startGrid.x, startGrid.y) || !isWalkable(goalGrid.x, goalGrid.y)) {
        return std::vector<Vector2f>();
    }

    // Priority queue for open set (nodes to explore)
    auto compare = [](Node* a, Node* b) { return a->f > b->f; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> openSet(compare);

    // Closed set to track visited nodes
    map<int, bool> closedSet;

    // Create start node
    Node* startNode = new Node(startGrid.x, startGrid.y);
    startNode->g = 0;
    startNode->h = calculateHeuristic(startGrid.x, startGrid.y, goalGrid.x, goalGrid.y);
    startNode->f = startNode->g + startNode->h;

    openSet.push(startNode);

    // Store all allocated nodes for cleanup
    std::vector<Node*> allNodes;
    allNodes.push_back(startNode);

    Node* goalNode = nullptr;

    while (!openSet.empty()) {
        // Get node with lowest f value
        Node* current = openSet.top();
        openSet.pop();

        // Check if we reached the goal
        if (current->x == goalGrid.x && current->y == goalGrid.y) {
            goalNode = current;
            break;
        }

        // Mark as visited
        int currentKey = current->y * m_LevelSize.x + current->x;
        if (closedSet.find(currentKey) != closedSet.end()) {
            continue; // Already visited
        }
        closedSet[currentKey] = true;

        // Explore neighbors
        std::vector<Node*> neighbors = getNeighbors(current, true);

        for (Node* neighbor : neighbors) {
            int neighborKey = neighbor->y * m_LevelSize.x + neighbor->x;

            if (closedSet.find(neighborKey) != closedSet.end()) {
                delete neighbor;
                continue; // Already visited
            }

            // Calculate costs
            float moveCost = (neighbor->x != current->x && neighbor->y != current->y) ? 1.414f : 1.0f;
            float tentativeG = current->g + moveCost;

            neighbor->g = tentativeG;
            neighbor->h = calculateHeuristic(neighbor->x, neighbor->y, goalGrid.x, goalGrid.y);
            neighbor->f = neighbor->g + neighbor->h;
            neighbor->parent = current;

            openSet.push(neighbor);
            allNodes.push_back(neighbor);
        }
    }

    // Reconstruct path
    std::vector<Vector2f> path;
    if (goalNode != nullptr) {
        path = reconstructPath(goalNode);
    }

    // Clean up all allocated nodes
    for (Node* node : allNodes) {
        delete node;
    }

    return path;
}

Vector2i Pathfinding::worldToGrid(Vector2f worldPos) {
    return Vector2i(
        static_cast<int>(worldPos.x / m_TileSize),
        static_cast<int>(worldPos.y / m_TileSize)
    );
}

Vector2f Pathfinding::gridToWorld(int gridX, int gridY) {
    // Return center of tile
    return Vector2f(
        gridX * m_TileSize + m_TileSize / 2.0f,
        gridY * m_TileSize + m_TileSize / 2.0f
    );
}