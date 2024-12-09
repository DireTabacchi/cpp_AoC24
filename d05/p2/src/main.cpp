#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

const char OBSTACLE = '#';
const char GUARD = '^';

unsigned long steps = 0;

enum Direction {
    north,
    east,
    south,
    west
};

enum Move {
    forward,
    turn,
    start,
    exit_map
};

struct Position {
    int x;
    int y;
    Direction entrance;
    Move move;
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

// Return true if there is a loop
bool check_loop(Position pos, Guard g, std::string map, int width, int height)
{
    std::vector<Position> visited {};
    while (true) {
        steps++;
        switch (g.dir) {
            case north:
                if (g.y == 0) {
                    return false;
                    break;
                }
                if (char_at(map, width, g.y - 1, g.x) == OBSTACLE) {
                    if (visited.size() == 0) {
                        visited.push_back(Position{g.x, g.y, g.dir, Move::turn});
                    } else {
                        for (Position& pos_info : visited) {
                            if (pos_info.x == g.x && pos_info.y == g.y &&
                                pos_info.entrance == g.dir) {
                                return true;
                                break;
                            } else {
                                continue;
                            }
                        }
                        visited.push_back(Position{g.x, g.y, g.dir, Move::turn});
                            
                    }
                    g.dir = Direction::east;
                } else {
                    while (map[idx_from_coords(g.y - 1, g.x, width)] != OBSTACLE && g.y > 0) {
                        g.y -= 1;
                    }
                    if (g.y == 0) return false;
                }
                break;
            case east:
                if (g.x == width - 1) {
                    return false;
                    break;
                }
                if (char_at(map, width, g.y, g.x + 1) == OBSTACLE) {
                    if (visited.size() == 0) {
                        visited.push_back( Position{g.x, g.y, g.dir, Move::turn} );
                    } else {
                        for (Position& pos_info : visited) {
                            if (pos_info.x == g.x && pos_info.y == g.y &&
                                pos_info.entrance == g.dir) {
                                return true;
                                break;
                            } else {
                                continue;
                            }
                        }
                        visited.push_back(Position{g.x, g.y, g.dir, Move::turn});
                    }
                    g.dir = Direction::south;
                } else {
                    while (map[idx_from_coords(g.y, g.x + 1, width)] != OBSTACLE && g.x < width - 1) {
                        g.x += 1;
                    }
                    if (g.x == width - 1) return false;
                }
                break;
            case south:
                if (g.y == height - 1) {
                    return false;
                    break;
                }
                if (char_at(map, width, g.y + 1, g.x) == OBSTACLE) {
                    if (visited.size() == 0) {
                        visited.push_back(Position{g.x, g.y, g.dir, Move::turn});
                    } else {
                        for (Position& pos_info : visited) {
                            if (pos_info.x == g.x && pos_info.y == g.y &&
                                pos_info.entrance == g.dir) {
                                return true;
                                break;
                            } else {
                                continue;
                            }
                        }
                        visited.push_back(Position{g.x, g.y, g.dir, Move::turn});
                    }
                    g.dir = Direction::west;
                } else {
                    while (map[idx_from_coords(g.y + 1, g.x, width)] != OBSTACLE && g.y < height - 1) {
                        g.y += 1;
                    }
                    if (g.y == height - 1) return false;
                }
                break;
            case west:
                if (g.x == 0) {
                    return false;
                    break;
                }
                if (char_at(map, width, g.y, g.x - 1) == OBSTACLE) {
                    if (visited.size() == 0) {
                        visited.push_back(Position{g.x, g.y, g.dir, Move::turn});
                    } else {
                        for (Position& pos_info : visited) {
                            if (pos_info.x == g.x && pos_info.y == g.y &&
                                pos_info.entrance == g.dir) {
                                return true;
                                break;
                            } else {
                                continue;
                            }
                        }
                        visited.push_back(Position{g.x, g.y, g.dir, Move::turn});
                    }
                    g.dir = Direction::north;
                } else {
                    while (map[idx_from_coords(g.y, g.x - 1, width)] != OBSTACLE && g.x > 0) {
                        g.x -= 1;
                    }
                    if (g.x == 0) return false;
                }
                break;
        }
    }
    return false;
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
    Guard g { 0, 0, Direction::north };

    if (map_width <= 20 && map_height <= 20)
        std::cout << "Map:\n" << map << "\n";

    for (int i = 0; i < map.size(); i++) {
        if (map[i] != GUARD)
            continue;
        g.x = x_from_index(i, map_width);
        g.y = y_from_index(i, map_width);
        g.dir = Direction::north;
    }

    std::vector<Position> positions {};

    std::cout << "Watching guard...\n";
    bool watching = true;
    while (watching) {
        steps++;
        switch (g.dir) {
            case north:
                if (g.y == 0) {
                    watching = false;
                    positions.push_back(Position{g.x, g.y, g.dir, Move::exit_map});
                    break;
                }
                if (char_at(map, map_width, g.y - 1, g.x) == OBSTACLE) {
                    positions.push_back(Position{g.x, g.y, g.dir, Move::turn});
                    g.dir = Direction::east;
                } else {
                    if (positions.size() < 1)
                        positions.push_back(Position{g.x, g.y, g.dir, Move::start});
                    else if (!(positions.back().move == Move::turn &&
                        positions.back().x == g.x && positions.back().y == g.y)
                    ) {
                        positions.push_back(Position{g.x, g.y, g.dir, Move::forward});
                    }
                    g.y -= 1;
                }
                break;
            case east:
                if (g.x == map_width - 1) {
                    watching = false;
                    positions.push_back(Position{g.x, g.y, g.dir, Move::exit_map});
                    break;
                }
                if (char_at(map, map_width, g.y, g.x + 1) == OBSTACLE) {
                    positions.push_back(Position{g.x, g.y, g.dir, Move::turn});
                    g.dir = Direction::south;
                } else {
                    if (!(positions.back().move == Move::turn &&
                        positions.back().x == g.x && positions.back().y == g.y)
                    ) {
                        positions.push_back(Position{g.x, g.y, g.dir, Move::forward});
                    }
                    g.x += 1;
                }
                break;
            case south:
                if (g.y == map_height - 1) {
                    watching = false;
                    positions.push_back(Position{g.x, g.y, g.dir, Move::exit_map});
                    break;
                }
                if (char_at(map, map_width, g.y + 1, g.x) == OBSTACLE) {
                    positions.push_back(Position{g.x, g.y, g.dir, Move::turn});
                    g.dir = Direction::west;
                } else {
                    if (!(positions.back().move == Move::turn &&
                        positions.back().x == g.x && positions.back().y == g.y)
                    ) {
                        positions.push_back(Position{g.x, g.y, g.dir, Move::forward});
                    }
                    g.y += 1;
                }
                break;
            case west:
                if (g.x == 0) {
                    watching = false;
                    positions.push_back(Position{g.x, g.y, g.dir, Move::exit_map});
                    break;
                }
                if (char_at(map, map_width, g.y, g.x - 1) == OBSTACLE) {
                    positions.push_back(Position{g.x, g.y, g.dir, Move::turn});
                    g.dir = Direction::north;
                } else {
                    if (!(positions.back().move == Move::turn &&
                        positions.back().x == g.x && positions.back().y == g.y)
                    ) {
                        positions.push_back(Position{g.x, g.y, g.dir, Move::forward});
                    }
                    g.x -= 1;
                }
                break;

        }
    }

    // filter out duplicate positions, take earliest
    std::vector<Position> obs_positions {};
    for (Position pos : positions) {
        bool duplicate = false;
        if (obs_positions.size() == 0) {
            obs_positions.push_back(pos);
            continue;
        }
        for (Position obs_pos : obs_positions) {
            if (obs_pos.x == pos.x && obs_pos.y == pos.y) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            obs_positions.push_back(pos);
        }
    }

    std::cout << "Planning obstructions... " << obs_positions.size() << " positions to check...\n";
    Position start_pos {positions[0]};
    int loop_count = 0;
    std::string obs_map = map;
    for (Position pos : obs_positions) {
        if (pos.move == Move::start || (pos.x == start_pos.x && pos.y == start_pos.y))
            continue;
        std::string loop_map = map;
        loop_map[idx_from_coords(pos.y, pos.x, map_width)] = OBSTACLE;
        bool loops = check_loop(pos, Guard{ start_pos.x, start_pos.y, Direction::north }, loop_map, map_width, map_height);
        if (loops) {
            loop_count++;
            obs_map[idx_from_coords(pos.y, pos.x, map_width)] = 'O';
        }
    }

    auto end { std::chrono::steady_clock::now() };
    std::chrono::duration<double> elapsed = { end - start };

    std::cout << "Options for obstructions:\n" << obs_map << "\n";

    std::cout << "Found " << loop_count << " possible obstruction positions. (Shown above)\n";
    std::cout << "Analysis time: " << elapsed.count() << "\n";
    std::cout << "And took " << steps << " steps to complete.\n";
}
