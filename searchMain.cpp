#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>

using namespace std;

struct Node
{
    int x;
    int y;
    bool walkable;
};

vector<Node*> getNeighbors(vector<vector<Node>>& grid, Node* current)
{
    vector<Node*> neighbors;

    int x = current->x;
    int y = current->y;

    // Up
    if (y > 0 && grid[y - 1][x].walkable)
    {
        neighbors.push_back(&grid[y - 1][x]);
    }

    // Down
    if (y < grid.size() - 1 && grid[y + 1][x].walkable)
    {
        neighbors.push_back(&grid[y + 1][x]);
    }

    // Left
    if (x > 0 && grid[y][x - 1].walkable)
    {
        neighbors.push_back(&grid[y][x - 1]);
    }

    // Right
    if (x < grid[y].size() - 1 && grid[y][x + 1].walkable)
    {
        neighbors.push_back(&grid[y][x + 1]);
    }

    return neighbors;
}

int main()
{
    const int WIDTH = 10;
    const int HEIGHT = 10;

    vector<vector<Node>> grid;

    // Create the grid
    for (int y = 0; y < HEIGHT; y++)
    {
        vector<Node> row;

        for (int x = 0; x < WIDTH; x++)
        {
            Node node;

            node.x = x;
            node.y = y;
            node.walkable = true;

            row.push_back(node);
        }

        grid.push_back(row);
    }

    // Random number generator
    random_device randomDevice;
    mt19937 randomGenerator(randomDevice());

    // Random number distributions
    uniform_int_distribution<int> xDistribution(0, WIDTH - 1);
    uniform_int_distribution<int> yDistribution(0, HEIGHT - 1);

    // Choose random start position on the exterior
    int startX;
    int startY;

    int startSide = uniform_int_distribution<int>(0, 3)(randomGenerator);

    if (startSide == 0)
    {
        // Top
        startX = xDistribution(randomGenerator);
        startY = 0;
    }
    else if (startSide == 1)
    {
        // Bottom
        startX = xDistribution(randomGenerator);
        startY = HEIGHT - 1;
    }
    else if (startSide == 2)
    {
        // Left
        startX = 0;
        startY = yDistribution(randomGenerator);
    }
    else
    {
        // Right
        startX = WIDTH - 1;
        startY = yDistribution(randomGenerator);
    }

    // Choose random goal position on the exterior
    int goalX;
    int goalY;

    do
    {
        int goalSide = uniform_int_distribution<int>(0, 3)(randomGenerator);

        if (goalSide == 0)
        {
            // Top
            goalX = xDistribution(randomGenerator);
            goalY = 0;
        }
        else if (goalSide == 1)
        {
            // Bottom
            goalX = xDistribution(randomGenerator);
            goalY = HEIGHT - 1;
        }
        else if (goalSide == 2)
        {
            // Left
            goalX = 0;
            goalY = yDistribution(randomGenerator);
        }
        else
        {
            // Right
            goalX = WIDTH - 1;
            goalY = yDistribution(randomGenerator);
        }

    } while (goalX == startX && goalY == startY);

    // Create random walls
    uniform_int_distribution<int> wallDistribution(1, 100);

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            // Don't put walls on start or goal
            if ((x == startX && y == startY) ||
                (x == goalX && y == goalY))
            {
                grid[y][x].walkable = true;
            }
            else
            {
                int randomNumber = wallDistribution(randomGenerator);

                // 25% chance of being a wall
                if (randomNumber <= 25)
                {
                    grid[y][x].walkable = false;
                }
            }
        }
    }

    // Start and goal nodes
    Node* start = &grid[startY][startX];
    Node* goal = &grid[goalY][goalX];

    // BFS queue
    queue<Node*> frontier;

    frontier.push(start);

    // Keep track of visited nodes
    vector<vector<bool>> visited(
        HEIGHT,
        vector<bool>(WIDTH, false)
    );

    visited[startY][startX] = true;

    // Keep track of where each node came from
    vector<vector<Node*>> parent(
        HEIGHT,
        vector<Node*>(WIDTH, nullptr)
    );

    // BFS
    while (!frontier.empty())
    {
        Node* current = frontier.front();

        frontier.pop();

        cout << "Exploring: ("
            << current->x
            << ", "
            << current->y
            << ")"
            << endl;

        // Did we reach the goal?
        if (current == goal)
        {
            break;
        }

        // Get walkable neighbors
        vector<Node*> neighbors = getNeighbors(grid, current);

        for (Node* neighbor : neighbors)
        {
            if (!visited[neighbor->y][neighbor->x])
            {
                visited[neighbor->y][neighbor->x] = true;

                // Remember where this node came from
                parent[neighbor->y][neighbor->x] = current;

                // Add neighbor to the queue
                frontier.push(neighbor);
            }
        }
    }

    // Reconstruct the path
    vector<Node*> path;

    Node* current = goal;

    while (current != nullptr)
    {
        path.push_back(current);

        if (current == start)
        {
            break;
        }

        current = parent[current->y][current->x];
    }

    // Reverse the path
    reverse(path.begin(), path.end());

    // Print the path
    cout << endl;
    cout << "Path:" << endl;

    for (Node* node : path)
    {
        cout << "("
            << node->x
            << ", "
            << node->y
            << ")"
            << endl;
    }

    // Print the grid
    cout << endl;
    cout << "Grid:" << endl;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (x == startX && y == startY)
            {
                cout << "S ";
            }
            else if (x == goalX && y == goalY)
            {
                cout << "G ";
            }
            else if (!grid[y][x].walkable)
            {
                cout << "# ";
            }
            else
            {
                bool isPath = false;

                for (Node* node : path)
                {
                    if (node->x == x && node->y == y)
                    {
                        isPath = true;
                        break;
                    }
                }

                if (isPath)
                {
                    cout << "* ";
                }
                else
                {
                    cout << ". ";
                }
            }
        }

        cout << endl;
    }

    return 0;
}
