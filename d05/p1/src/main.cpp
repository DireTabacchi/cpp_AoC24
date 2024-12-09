#include <chrono>
#include <fstream>
#include <iostream>

const char OBSTACLE = '#';
const char GUARD = '^';

enum Direction {
    north,
    east,
    south,
    west
};

struct Guard {
    int x;
    int y;
    Direction dir;
};

int idx_from_coords(int pos_y, int pos_x, int width)
{
    return (width + 1) * pos_y + pos_x;
}

char char_at(std::string buf, int width, int pos_y, int pos_x)
{
    int idx = (width + 1) * pos_y + pos_x;
    return buf[idx];
}

int x_from_index(int idx, int width)
{
    return idx % (width + 1);
}

int y_from_index(int idx, int width)
{
    return idx / (width + 1);
}

int main(int argc, char** argv)
{
    // Read the file
    if (argc < 2) {
        std::cerr << "Enter an input filename.\n";
        return 1;
    } else if (argc > 2) {
        std::cerr << "Too many arguments.\n";
    }

    std::string filename = argv[1];
    std::ifstream input_file {filename};

    auto start {std::chrono::steady_clock::now()};
    std::string r_str {}, map {};
    int map_width = 0, map_height = 0;
    std::cout << "Making map...\n";
    while (getline(input_file, r_str, '\n')) {
        map += r_str + "\n";
        map_width = r_str.size();
        map_height += 1;
    }
    std::string patrol = map;
    Guard g { 0, 0, Direction::north };

    if (map_width <= 20 && map_height <= 20)
        std::cout << "Map:\n" << map << "\n";

    for (int i = 0; i < patrol.size(); i++) {
        if (patrol[i] != GUARD)
            continue;
        g.x = x_from_index(i, map_width);
        g.y = y_from_index(i, map_width);
        g.dir = Direction::north;
    }

    std::cout << "Watching guard...\n";
    bool watching = true;
    while (watching) {
        switch (g.dir) {
            case north:
                if (g.y == 0) {
                    watching = false;
                    patrol[idx_from_coords(g.y, g.x, map_width)] = 'X';
                    break;
                }
                if (char_at(map, map_width, g.y - 1, g.x) == OBSTACLE) {
                    patrol[idx_from_coords(g.y, g.x, map_width)] = 'X';
                    g.dir = Direction::east;
                } else {
                    patrol[idx_from_coords(g.y, g.x, map_width)] = 'X';
                    g.y -= 1;
                }
                break;
            case east:
                if (g.x == map_width - 1) {
                    watching = false;
                    patrol[idx_from_coords(g.y, g.x, map_width)] = 'X';
                    break;
                }
                if (char_at(map, map_width, g.y, g.x + 1) == OBSTACLE) {
                    patrol[idx_from_coords(g.y, g.x, map_width)] = 'X';
                    g.dir = Direction::south;
                } else {
                    patrol[idx_from_coords(g.y, g.x, map_width)] = 'X';
                    g.x += 1;
                }
                break;
            case south:
                if (g.y == map_height - 1) {
                    watching = false;
                    patrol[idx_from_coords(g.y, g.x, map_width)] = 'X';
                    break;
                }
                if (char_at(map, map_width, g.y + 1, g.x) == OBSTACLE) {
                    patrol[idx_from_coords(g.y, g.x, map_width)] = 'X';
                    g.dir = Direction::west;
                } else {
                    patrol[idx_from_coords(g.y, g.x, map_width)] = 'X';
                    g.y += 1;
                }
                break;
            case west:
                if (g.x == 0) {
                    watching = false;
                    patrol[idx_from_coords(g.y, g.x, map_width)] = 'X';
                    break;
                }
                if (char_at(map, map_width, g.y, g.x - 1) == OBSTACLE) {
                    patrol[idx_from_coords(g.y, g.x, map_width)] = 'X';
                    g.dir = Direction::north;
                } else {
                    patrol[idx_from_coords(g.y, g.x, map_width)] = 'X';
                    g.x -= 1;
                }
                break;

        }
    }

    int pos_count = 0;
    for (int i = 0; i < patrol.size(); i++) {
        if (patrol[i] == 'X') pos_count++;
    }

    auto end { std::chrono::steady_clock::now() };
    std::chrono::duration<double> elapsed = { end - start };

    if (map_width <= 20 && map_height <= 20)
        std::cout << "Patrol map:\n" << patrol << "\n";

    std::cout << "Guard visited " << pos_count << " distinct positions.\n";
    std::cout << "Analysis time: " << elapsed.count() << "\n";
}
