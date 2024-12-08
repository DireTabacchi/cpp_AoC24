#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

const std::string F_XMAS = "XMAS";
const std::string R_XMAS = "SAMX";

char char_at(std::string buf, int width, int pos_y, int pos_x)
{
    int pos = (width + 1) * pos_y + pos_x;
    return buf[pos];
}

bool match_xmas(std::string tok) {
    return (tok == F_XMAS || tok == R_XMAS);
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

    std::cout << "Searching wordsearch puzzle for XMAS...\n\n" << input.str() << "\n";

    int xmas_count = 0;
    std::string line;
    // Search all lines for forward (or backward) XMAS
    while (getline(input, line)) {
        int pos = 0;
        while (pos + 4 <= line.size()) {
            std::string tok = line.substr(pos, 4);

            if (match_xmas(tok)) {
                xmas_count++;
            }
            pos++;
        }
    }

    // Search all columns for forward (or backward) XMAS
    for (int pos_x = 0; pos_x < ws_width; pos_x++) {
        for (int pos_y = 0; pos_y < ws_height - 3; pos_y++) {
            std::string tok {};
            for (int inner_y = pos_y; inner_y <= pos_y + 3; inner_y++) {
                tok += char_at(input.str(), ws_width, inner_y, pos_x);
            }
            if (match_xmas(tok)) {
                xmas_count++;
            }
        }
    }

    // Search backward diagonals (\) for forward (or backward) XMAS
    for (int pos_x = 0; pos_x < ws_width - 3; pos_x++) {
        for (int pos_y = 0; pos_y < ws_width - 3; pos_y++) {
            std::string tok {};
            for (
                int inner_y = pos_y, inner_x = pos_x;
                inner_y <= pos_y + 3 && inner_x <= pos_x + 3;
                inner_y++, inner_x++
            ) {
                tok += char_at(input.str(), ws_width, inner_y, inner_x);
            }
            if (match_xmas(tok)) {
                xmas_count++;
            }
        }
    }

    // Search forward diagonals (/) for forward (or backward) XMAS
    for (int pos_x = ws_width - 1; pos_x > 2; pos_x--) {
        for (int pos_y = 0; pos_y < ws_width - 3; pos_y++) {
            std::string tok {};
            for (
                int inner_y = pos_y, inner_x = pos_x;
                inner_y <= pos_y + 3 && inner_x >= pos_x - 3;
                inner_y++, inner_x--
            ) {
                tok += char_at(input.str(), ws_width, inner_y, inner_x);
            }
            if (match_xmas(tok)) {
                xmas_count++;
            }
        }
    }

    auto end {std::chrono::steady_clock::now()};
    std::chrono::duration<double> elapsed {end - start};
    // Print results.
    std::cout << "Found " << xmas_count << " instances of XMAS.\n";
    std::cout << "Search time: " << elapsed.count() << "\n";
}

