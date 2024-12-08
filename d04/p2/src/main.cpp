#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

const std::string F_MAS = "MAS";
const std::string R_MAS = "SAM";

char char_at(std::string buf, int width, int pos_y, int pos_x)
{
    int pos = (width + 1) * pos_y + pos_x;
    return buf[pos];
}

bool match_mas(std::string tok) {
    return (tok == F_MAS || tok == R_MAS);
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
    std::string r_str;
    std::stringstream input;
    int ws_width = 0, ws_height = 0;
    while (getline(input_file, r_str, '\n')) {
        input << r_str + "\n";
        ws_width = r_str.size();
        ws_height += 1;
    }

    std::cout << "Searching wordsearch puzzle for X-MAS...\n\n";

    if (ws_height <= 20 && ws_width <= 20) {
        std::cout << input.str() << "\n";
    }

    int x_mas_count = 0;

    // Search for X-MASes
    for (int pos_x = 0; pos_x < ws_width - 2; pos_x++) {
        for (int pos_y = 0; pos_y < ws_width - 2; pos_y++) {
            std::string tok1 {};
            std::string tok2 {};
            for (
                int inner_y = pos_y, inner_x = pos_x;
                inner_y <= pos_y + 2 && inner_x <= pos_x + 2;
                inner_y++, inner_x++
            ) {
                tok1 += char_at(input.str(), ws_width, inner_y, inner_x);
            }
            if (!match_mas(tok1)) {
                continue;
            }

            for (
                int inner_y = pos_y, inner_x = pos_x + 2;
                inner_y <= pos_y + 2 && inner_x >= pos_x;
                inner_y++, inner_x--
            ) {
                tok2 += char_at(input.str(), ws_width, inner_y, inner_x);
            }
            if (match_mas(tok2)) {
                x_mas_count++;
            }
        }
    }

    auto end {std::chrono::steady_clock::now()};
    std::chrono::duration<double> elapsed {end - start};
    // Print results.
    std::cout << "Found " << x_mas_count << " instances of X-MAS.\n";
    std::cout << "Search time: " << std::fixed << elapsed.count() << "\n";
}

